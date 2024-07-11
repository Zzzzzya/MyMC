#include "Map.hpp"

Map::Map(vec3 mapSize, int seed)
    : mapSize(mapSize), _map(make_shared<vector<vector<vector<shared_ptr<Mesh>>>>>()), seed(seed) {
}

void Map::InitMap() {
    resizeMap();

    generateMap();

    setExposed();
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
                    if (nx < 0 || nx >= mapX || ny < 0 || ny >= mapY || nz < 0 || nz >= mapZ ||
                        !map[nx][ny][nz]->Occluded()) {
                        map[i][j][k]->Exposed[p] = true;
                    }
                    else {
                        map[i][j][k]->Exposed[p] = false;
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
            double height = perlinNoise.GenerateTerrain(x, z);
            int y = (int)(height * 20);
            int earthLine = bedRockHeight + stoneHeight + dirtHeight;
            int maxHeight = std::min(mapY, earthLine + y);

            map[i][maxHeight - 1][k]->ID() = CB_GRASS_BLOCK;
            heightMap[i][k] = maxHeight - 1; // 草方格高度
            for (int j = maxHeight - 2; j >= earthLine; j--) {
                map[i][j][k]->ID() = CB_DIRT_BLOCK;
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
            if (map[i][y][k]->ID() == CB_GRASS_BLOCK && y + 1 < mapY && map[i][y + 1][k]->ID() == QD_EMPTY) {
                map[i][y + 1][k] = make_shared<Quad>();
                map[i][y + 1][k]->ID() = QD_GRASS;
            }
        }
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
