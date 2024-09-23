#include "Map.hpp"

Map::Map(vec3 mapSize, int seed)
    : mapSize(mapSize), _map(make_shared<vector<vector<vector<shared_ptr<Mesh>>>>>()), seed(seed),
      noiseLayer1(1.0, 1.5, 2.0, 3), // 低频高振幅
      noiseLayer2(2.0, 0.8, 2.0, 3), // 中频中振幅
      noiseLayer3(4.0, 0.3, 2.0, 3)  // 高频低振幅
{
}

void Map::InitMap() {
    resizeMap();

    generateMap();

    setExposed();
}

void Map::SaveMap(const std::string &worldName, const std::string &saveRoot) const {
    int parallerNum = getCPUCores();
    int sliceNum = _map -> size() / parallerNum; // 向下取整
    std::vector<std::future<void>> futures(parallerNum);

    for (size_t i = 0; i < parallerNum; i++)
    {
        futures[i] = std::async(std::launch::async, [this, i, parallerNum, sliceNum, worldName, saveRoot]()
                                {
                                    int start = i * sliceNum;
                                    int end = (i == parallerNum - 1) ? _map->size() : (i + 1) * sliceNum;
                                    int mapY = _map->at(0).size();
                                    int mapZ = _map->at(0).at(0).size();
                                    std::vector<MeshIOStruct> meshIOStructs((end - start) * mapY * mapZ);

                                    // 将 Map 转换为 MeshIOStruct
                                    for (int x = start; x < end; x++)
                                    {
                                        for (int y = 0; y < _map->at(x).size(); y++)
                                        {
                                            for (int z = 0; z < _map->at(x).at(y).size(); z++)
                                            {
                                                auto &mesh = _map->at(x).at(y).at(z);
                                                meshIOStructs[(x - start) * mapY * mapZ + y * mapZ + z] = MeshIOStruct{mesh->ID(), MeshType(mesh->type())};
                                            }
                                        }
                                    }

                                    // map 转换为 MapIOStruct
                                    MapIOStruct mapIOStruct{seed, mapSize, meshIOStructs};

                                    // TODO: Serialize the mapIOStruct

                                    // TODO: Save the mapIOStruct to the disk
                                });
    }

    // 等待所有线程结束
    for (auto &future : futures)
    {
        future.wait();
    }
}

bool Map::CheckCollisionHelper(const vec3 &blockPos) const {
    // true for permit to pass
    if (blockPos.x < 0 || blockPos.x >= mapSize.x || blockPos.y < 0 || blockPos.y >= mapSize.y || blockPos.z < 0 ||
        blockPos.z >= mapSize.z) {
        return true;
    }
    auto &map = *(this->_map);
    if (map[blockPos.x][blockPos.y][blockPos.z]->Passed())
        return true;
    return false;
}

bool Map::CheckHaveSomething(const vec3 &blockPos) const {
    if (blockPos.x < 0 || blockPos.x >= mapSize.x || blockPos.y < 0 || blockPos.y >= mapSize.y || blockPos.z < 0 ||
        blockPos.z >= mapSize.z) {
        return false;
    }
    auto &map = *(this->_map);
    if (!map[blockPos.x][blockPos.y][blockPos.z]->RayPassed())
        return true;
    return false;
}

bool Map::ViewRayTrace(const vec3 &position, const vec3 &direction, vec3 &ToDo, vec3 &ToAdd, float dis,
                        float step) const {
    // ensure the direction is normalized
    vector<vec3> path;
    path.push_back(GetBlockCoords(position));
    for (float i = 0; i < dis; i += step) {
        vec3 newPos = position + direction * i;
        auto blockPos = GetBlockCoords(newPos);
        if (blockPos != path.back())
            path.push_back(blockPos);
        if (CheckHaveSomething(blockPos)) {
            ToDo = blockPos;
            if (path.size() < 2)
                ToAdd = path[0];
            else
                ToAdd = path[path.size() - 2];
            return true;
        }
    }
    return false;
}

void Map::flushExposedFaces(const vec3 &block, vector<vec3> &chunksToUpdate) {
    auto &map = *(this->_map);
    int mapX = mapSize.x;
    int mapY = mapSize.y;
    int mapZ = mapSize.z;
    // Set the Exposed attribute of each cube
    int dx[] = {1, -1, 0, 0, 0, 0};
    int dy[] = {0, 0, 1, -1, 0, 0};
    int dz[] = {0, 0, 0, 0, -1, 1};
    int ys[] = {1, 0, 3, 2, 5, 4};

    auto i = block.x;
    auto j = block.y;
    auto k = block.z;

    if (map[i][j][k]->ID() == 0) {
        for (int p = 0; p < 6; p++) {
            map[i][j][k]->Exposed[p] = 0;
            int nx = i + dx[p];
            int ny = j + dy[p];
            int nz = k + dz[p];

            // If the cube is on the edge of the map or the cube is exposed
            if (nx < 0 || nx >= mapX || ny < 0 || ny >= mapY || nz < 0 || nz >= mapZ) {
                continue;
            }
            else {
                if (map[nx][ny][nz]->ID() != CB_EMPTY)
                    map[nx][ny][nz]->Exposed[ys[p]] = true;
            }
        }
    }
    else {
        for (int p = 0; p < 6; p++) {
            map[i][j][k]->Exposed[p] = 0;
            int nx = i + dx[p];
            int ny = j + dy[p];
            int nz = k + dz[p];

            // If the cube is on the edge of the map or the cube is exposed
            if (nx < 0 || nx >= mapX || ny < 0 || ny >= mapY || nz < 0 || nz >= mapZ) {
                map[i][j][k]->Exposed[p] = true;
            }
            else {
                if (map[nx][ny][nz]->ID() != CB_EMPTY)
                    map[nx][ny][nz]->Exposed[ys[p]] = false;
                if (map[i][j][k]->ID() == CB_WATER)
                    map[i][j][k]->Exposed[p] = map[nx][ny][nz]->ID() == 0;
                else
                    map[i][j][k]->Exposed[p] = !map[nx][ny][nz]->Occluded();
            }
        }
    }
}

bool Map::CheckCollision(const vec3 &position, vec3 &movVec) const {
    // Check the collision with the map
    // position wants to move as movVec
    vec3 newPos = position + movVec;
    vec3 newPos2 = position - vec3(0.0f, 3.0f, 0.0f) + movVec;
    auto blockPos = GetBlockCoords(newPos);
    auto blockPos2 = GetBlockCoords(newPos2);
    if (!CheckCollisionHelper(blockPos) || !CheckCollisionHelper(blockPos2))
        return false;
    return true;
}

bool Map::CheckCollisionSingle(const vec3 &position) const {
    // true for permit to pass
    vec3 newPos = position;
    auto blockPos = GetBlockCoords(newPos);
    if (!CheckCollisionHelper(blockPos))
        return false;
    return true;
}

// Init Step 1 : Resize the map & fill the map with cubes 0
void Map::resizeMap() {
    auto &map = *(this->_map);
    int mapX = mapSize.x;
    int mapY = mapSize.y;
    int mapZ = mapSize.z;

    map.resize(mapX);
    for (auto &rol : map) {
        rol.resize(mapY);
        for (auto &cubes : rol) {
            cubes.resize(mapZ);
            for (auto &cube : cubes) {
                cube = make_shared<Cube>(); // 默认为Cube
                cube->ID() = 0;
            }
        }
    }

    heightMap.resize(mapX, vector<int>(mapZ, 0));
}

// Init Step 2 : Generate the map With Perlin Noise
void Map::generateMap() {
    // Step1 : 地底铺设
    // 地底铺设固定层数的基岩 + 石头 + 土方块
    GenerateEarth();

    // Step2 : 生成地表地形
    // Generate the High with Perlin Noise 用柏林噪声生成高度图
    // 用高度图更新每一个坐标的高度。高度图的值为0-1之间的值
    GenerateSurface();

    // Step3 : 生成其余地表物体
    // 目前顺序 : 树木 -> 地表草
    GenerateTrees();
    GenerateGrass();

    // Step4 : 生成云朵
    // GenerateClouds();
}

// Init Step 3 : Set the Expose attribute of each cube
void Map::setExposed() {
    auto &map = *(this->_map);
    int mapX = mapSize.x;
    int mapY = mapSize.y;
    int mapZ = mapSize.z;
    // Set the Exposed attribute of each cube
    int dx[] = {1, -1, 0, 0, 0, 0};
    int dy[] = {0, 0, 1, -1, 0, 0};
    int dz[] = {0, 0, 0, 0, -1, 1};

    for (int i = 0; i < mapX; i++)
        for (int j = 0; j < mapY; j++)
            for (int k = 0; k < mapZ; k++) {
                if (map[i][j][k]->ID() == 0)
                    continue;
                for (int p = 0; p < 6; p++) {
                    int nx = i + dx[p];
                    int ny = j + dy[p];
                    int nz = k + dz[p];

                    // If the cube is on the edge of the map or the cube is exposed
                    if (nx < 0 || nx >= mapX || ny < 0 || ny >= mapY || nz < 0 || nz >= mapZ) {
                        map[i][j][k]->Exposed[p] = true;
                    }
                    else {
                        if (map[i][j][k]->ID() == CB_WATER)
                            map[i][j][k]->Exposed[p] = map[nx][ny][nz]->ID() == 0;
                        else
                            map[i][j][k]->Exposed[p] = !map[nx][ny][nz]->Occluded();
                    }
                }
            }
}

void Map::GenerateEarth() {
    auto &map = *(this->_map);
    int mapX = mapSize.x;
    int mapY = mapSize.y;
    int mapZ = mapSize.z;

    for (int j = 0; j < mapY; j++)
        for (int i = 0; i < mapX; i++)
            for (int k = 0; k < mapZ; k++) {
                if (j < bedRockHeight) {
                    map[i][j][k]->ID() = CB_BEDROCK;
                }
                else if (j < bedRockHeight + stoneHeight) {
                    map[i][j][k]->ID() = CB_STONE;
                }
                else if (j < bedRockHeight + stoneHeight + dirtHeight) {
                    map[i][j][k]->ID() = CB_DIRT_BLOCK;
                }
            }
}

void Map::GenerateSurface() {
    auto &map = *(this->_map);
    int mapX = mapSize.x;
    int mapY = mapSize.y;
    int mapZ = mapSize.z;
    for (int i = 0; i < mapX; i++)
        for (int k = 0; k < mapZ; k++) {
            double x = (double)i / mapX;
            double z = (double)k / mapZ;
            // double height = perlinNoise.GetValue(x, z);

            double value1 = noiseLayer1.GenerateTerrain(x, z);
            double value2 = noiseLayer2.GenerateTerrain(x, z);
            double value3 = noiseLayer3.GenerateTerrain(x, z);

            // 组合噪声值
            double height = (value1 + value2 + value3) / 3.0;
            int earthLine = bedRockHeight + stoneHeight + dirtHeight;

            bool bCloud = perlinNoise.GenerateTerrain(x, z) > 0.5;

            int y = (int)((height - 0.05) * 40);

            int maxHeight = std::min(mapY, earthLine + y);

            map[i][maxHeight - 1][k]->ID() = CB_GRASS_BLOCK;
            heightMap[i][k] = maxHeight - 1; // 草方格高度
            for (int j = maxHeight - 2; j >= earthLine; j--) {
                map[i][j][k]->ID() = CB_DIRT_BLOCK;
            }

            static int dx[] = {1, -1, 0, 0, 1, -1, 1, -1};
            static int dz[] = {0, 0, 1, -1, -1, 1, 1, -1};

            if (maxHeight <= 37) {
                for (int j = 37; j >= maxHeight; j--) {
                    map[i][j][k]->ID() = CB_WATER;
                }
                map[i][maxHeight - 1][k]->ID() = CB_SAND;
            }

            if (map[i][37][k]->ID() != CB_WATER && map[i][37][k]->ID() != CB_EMPTY) {
                for (int pp = -3; pp <= 3; pp++) {
                    for (int kk = -3; kk <= 3; kk++) {
                        if (i + pp >= 0 && i + pp < mapX && k + kk >= 0 && k + kk < mapZ) {
                            if (map[i + pp][37][k + kk]->ID() == CB_WATER) {
                                for (int hh = 37; hh < 40; hh++) {
                                    if (map[i][hh][k]->ID() == CB_EMPTY)
                                        break;
                                    map[i][hh][k]->ID() = CB_SAND;
                                }
                            }
                        }
                    }
                }
                // map[i][37][k]->ID() = CB_SAND;
            }
            if (bCloud) {
                map[i][88][k]->ID() = CB_CLOUD;
            }
        }
}

void Map::GenerateTrees() {
    auto &map = *(this->_map);
    int mapX = mapSize.x;
    int mapY = mapSize.y;
    int mapZ = mapSize.z;
    for (int i = 0; i < mapX; i += 20)
        for (int k = 0; k < mapZ; k += 20) {
            int treeNum = rand(1, 3);
            for (int t = 0; t < treeNum; t++) {
                int treeHeight = rand(7, 9);
                int x = i + rand(0, 19);
                int z = k + rand(0, 19);
                if (rand(1, 100) < 90) {
                    continue;
                }
                if (x >= mapX || z >= mapZ)
                    continue;
                int y = heightMap[x][z];
                if (CheckIfCanContainsATree(x, y, z, treeHeight)) {
                    auto type = rand(0, 7);
                    GenerateATree(x, y, z, treeHeight, type == 1 ? 1 : 0);
                }
            }
        }
}

void Map::GenerateGrass() {
    auto &map = *(this->_map);
    int mapX = mapSize.x;
    int mapY = mapSize.y;
    int mapZ = mapSize.z;
    for (int i = 0; i < mapX; i++)
        for (int k = 0; k < mapZ; k++) {
            int y = heightMap[i][k];
            if (rand(0, 100) < 10 && map[i][y][k]->ID() == CB_GRASS_BLOCK && y + 1 < mapY &&
                map[i][y + 1][k]->ID() == QD_EMPTY) {
                map[i][y + 1][k] = make_shared<CrossQuad>();
                map[i][y + 1][k]->ID() = QD_GRASS;
            }
        }
}

void Map::GenerateClouds() {
}

// 周围一格内的坐标
static std::vector<vec2> d1 = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}, {1, 1}, {-1, -1}, {1, -1}, {-1, 1}};

// 周围两格内的坐标
static std::vector<vec2> d2 = {{1, 0},   {-1, 0},  {0, 1},  {0, -1}, {1, 1},  {-1, -1}, {1, -1},
                               {-1, 1},  {2, 0},   {-2, 0}, {0, 2},  {0, -2}, {2, 1},   {2, -1},
                               {-2, 1},  {-2, -1}, {1, 2},  {-1, 2}, {1, -2}, {-1, -2}, {2, 2},
                               {-2, -2}, {2, -2},  {-2, 2}, {1, 1},  {1, -1}, {-1, 1},  {-1, -1}};

// 周围三格内的坐标
static std::vector<vec2> d3 = {
    {1, 0}, {-1, 0},  {0, 1},  {0, -1},  {1, 1}, {-1, -1}, {1, -1}, {-1, 1},  {2, 0}, {-2, 0},  {0, 2},  {0, -2},
    {2, 1}, {2, -1},  {-2, 1}, {-2, -1}, {1, 2}, {-1, 2},  {1, -2}, {-1, -2}, {2, 2}, {-2, -2}, {2, -2}, {-2, 2},
    {1, 1}, {1, -1},  {-1, 1}, {-1, -1}, {3, 0}, {-3, 0},  {0, 3},  {0, -3},  {3, 1}, {3, -1},  {-3, 1}, {-3, -1},
    {1, 3}, {-1, 3},  {1, -3}, {-1, -3}, {2, 3}, {2, -3},  {-2, 3}, {-2, -3}, {3, 2}, {3, -2},  {-3, 2}, {-3, -2},
    {3, 3}, {-3, -3}, {3, -3}, {-3, 3},  {1, 2}, {-1, 2},  {1, -2}, {-1, -2}, {2, 1}, {-2, 1},  {2, -1}, {-2, -1}};

bool Map::CheckIfCanContainsATree(int x, int y, int z, int height) {
    for (int i = 1; i <= height + 2; i++)
        if (y + i >= mapSize.y || (*_map)[x][y + i][z]->ID() != 0)
            return false;

    for (int i = height + 2; i >= height + 1; i--) {
        for (auto &d : d2) {
            int nx = x + d.x;
            int nz = z + d.y;
            if (nx < 0 || nx >= mapSize.x || nz < 0 || nz >= mapSize.z)
                return false;
            if ((*_map)[nx][y + i][nz]->ID() != 0)
                return false;
        }
    }

    for (int i = height; i >= height - 1; i--) {
        for (auto &d : d3) {
            int nx = x + d.x;
            int nz = z + d.y;
            if (nx < 0 || nx >= mapSize.x || nz < 0 || nz >= mapSize.z)
                return false;
            if ((*_map)[nx][y + i][nz]->ID() != 0)
                return false;
        }
    }
    return true;
}

void Map::GenerateATree(int x, int y, int z, int height, int type) {
    (*_map)[x][y][z]->ID() = CB_DIRT_BLOCK;
    for (int i = 1; i <= height; i++)
        (*_map)[x][y + i][z]->ID() = CB_WOOD;

    auto leaveType = !type ? CB_LEAVES : CB_DIAMOND;
    (*_map)[x][y + height + 1][z]->ID() = leaveType;
    (*_map)[x][y + height + 2][z]->ID() = leaveType;
    for (int i = height + 2; i >= height + 1; i--) {
        for (auto &d : d2) {
            int nx = x + d.x;
            int nz = z + d.y;
            (*_map)[nx][y + i][nz]->ID() = leaveType;
        }
    }

    for (int i = height; i >= height - 1; i--) {
        for (auto &d : d3) {
            int nx = x + d.x;
            int nz = z + d.y;
            (*_map)[nx][y + i][nz]->ID() = leaveType;
        }
    }
}
