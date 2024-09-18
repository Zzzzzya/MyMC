#include "Mesh.hpp"
#include "Shader.hpp"
#include "mTexture.hpp"
#include "Map.hpp"

/* Cube */
/* ID映射 CubeID -> {TextureID, ShaderID} */
unordered_map<int, std::pair<int, int>> Cube::CubeIdMap = {{0, {0, 0}}};

int Cube::GetTextureID() const {
    return CubeIdMap[CubeID].first;
}

int Cube::GetShaderID() const {
    return CubeIdMap[CubeID].second;
}

static vector<vec3> normals = {
    {1.0f, 0.0f, 0.0f},  // 右
    {-1.0f, 0.0f, 0.0f}, // 左
    {0.0f, 1.0f, 0.0f},  // 上
    {0.0f, -1.0f, 0.0f}, // 下
    {0.0f, 0.0f, 1.0f},  // 前
    {0.0f, 0.0f, -1.0f}, // 后
};

void Cube::GenerateVertices(vector<Vertex> &vertices, vec3 WorldPos) const {
    for (int p = 0; p < 6; p++) {
        if (Exposed[p]) {
            for (int q = 0; q < 6; q++) {
                Vertex vertex = CubeVertice[p * 6 + q];
                vertex.CubeMapTex = vertex.position - vec3(0.0f);
                vertex.position += WorldPos;
                vertex.cubeID = CubeID;
                vertex.faceID = 0;
                vertex.normal = normals[p];
                vertices.push_back(vertex);
            }
        }
    }
}

vector<Vertex> Cube::CubeVertice = {
    // 右面
    {{1.0f, -1.0f, 1.0f}, {0.0f, 0.0f}},
    {{1.0f, -1.0f, -1.0f}, {1.0f, 0.0f}},
    {{1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},
    {{1.0f, -1.0f, -1.0f}, {1.0f, 0.0f}},
    {{1.0f, 1.0f, -1.0f}, {1.0f, 1.0f}},
    {{1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},

    // 左面
    {{-1.0f, -1.0f, -1.0f}, {1.0f, 0.0f}},
    {{-1.0f, -1.0f, 1.0f}, {0.0f, 0.0f}},
    {{-1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},
    {{-1.0f, -1.0f, -1.0f}, {1.0f, 0.0f}},
    {{-1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},
    {{-1.0f, 1.0f, -1.0f}, {1.0f, 1.0f}},
    // 上面
    {{1.0f, 1.0f, -1.0f}, {1.0f, 1.0f}},
    {{-1.0f, 1.0f, -1.0f}, {0.0f, 1.0f}},
    {{1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}},
    {{1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}},
    {{-1.0f, 1.0f, -1.0f}, {0.0f, 1.0f}},
    {{-1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},
    // 下面
    {{-1.0f, -1.0f, -1.0f}, {0.0f, 0.0f}},
    {{1.0f, -1.0f, -1.0f}, {1.0f, 0.0f}},
    {{1.0f, -1.0f, 1.0f}, {1.0f, 1.0f}},
    {{-1.0f, -1.0f, -1.0f}, {0.0f, 0.0f}},
    {{1.0f, -1.0f, 1.0f}, {1.0f, 1.0f}},
    {{-1.0f, -1.0f, 1.0f}, {0.0f, 1.0f}},
    // 前面
    {{-1.0f, -1.0f, 1.0f}, {0.0f, 0.0f}},
    {{1.0f, -1.0f, 1.0f}, {1.0f, 0.0f}},
    {{1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},
    {{-1.0f, -1.0f, 1.0f}, {0.0f, 0.0f}},
    {{1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},
    {{-1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},
    // 后面
    {{1.0f, -1.0f, -1.0f}, {0.0f, 0.0f}},
    {{-1.0f, -1.0f, -1.0f}, {1.0f, 0.0f}},
    {{1.0f, 1.0f, -1.0f}, {0.0f, 1.0f}},
    {{1.0f, 1.0f, -1.0f}, {0.0f, 1.0f}},
    {{-1.0f, -1.0f, -1.0f}, {1.0f, 0.0f}},
    {{-1.0f, 1.0f, -1.0f}, {1.0f, 1.0f}},
};

/* Quad */
vector<Vertex> Quad::QuadVertice = {
    // 前面
    {{-1.0f, -1.0f, 0.0f}, {0.0f, 0.0f}}, {{1.0f, -1.0f, 0.0f}, {1.0f, 0.0f}}, {{1.0f, 1.0f, 0.0f}, {1.0f, 1.0f}},
    {{-1.0f, -1.0f, 0.0f}, {0.0f, 0.0f}}, {{1.0f, 1.0f, 0.0f}, {1.0f, 1.0f}},  {{-1.0f, 1.0f, 0.0f}, {0.0f, 1.0f}},

};
void Quad::GenerateVertices(vector<Vertex> &vertices, vec3 WorldPos) const {
    for (int p = 0; p < 6; p++) {
        if (Exposed[p] == true) {
            for (int q = 0; q < 6; q++) {
                Vertex vertex = QuadVertice[q];
                vertex.CubeMapTex = vertex.position;
                vertex.position += WorldPos;
                vertex.cubeID = QuadID;
                vertex.faceID = 1;
                vertices.push_back(vertex);
            }
            return;
        }
    }
}

/* Cross Quad */
vector<Vertex> CrossQuad::CrossQuadVertice = {
    // 左上右下
    {{-1.0f, -1.0f, 1.0f}, {0.0f, 0.0f}},
    {{1.0f, -1.0f, -1.0f}, {1.0f, 0.0f}},
    {{1.0f, 1.0f, -1.0f}, {1.0f, 1.0f}},
    {{-1.0f, -1.0f, 1.0f}, {0.0f, 0.0f}},
    {{1.0f, 1.0f, -1.0f}, {1.0f, 1.0f}},
    {{-1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},

    {{1.0f, -1.0f, -1.0f}, {1.0f, 0.0f}},
    {{-1.0f, -1.0f, 1.0f}, {0.0f, 0.0f}},
    {{1.0f, 1.0f, -1.0f}, {1.0f, 1.0f}},
    {{1.0f, 1.0f, -1.0f}, {1.0f, 1.0f}},
    {{-1.0f, -1.0f, 1.0f}, {0.0f, 0.0f}},
    {{-1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},
    // 左下右上
    {{1.0f, -1.0f, 1.0f}, {1.0f, 0.0f}},
    {{-1.0f, -1.0f, -1.0f}, {0.0f, 0.0f}},
    {{1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},
    {{1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},
    {{-1.0f, -1.0f, -1.0f}, {0.0f, 0.0f}},
    {{-1.0f, 1.0f, -1.0f}, {0.0f, 1.0f}},

    {{-1.0f, -1.0f, -1.0f}, {0.0f, 0.0f}},
    {{1.0f, -1.0f, 1.0f}, {1.0f, 0.0f}},
    {{1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},
    {{-1.0f, -1.0f, -1.0f}, {0.0f, 0.0f}},
    {{1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},
    {{-1.0f, 1.0f, -1.0f}, {0.0f, 1.0f}},

};
void CrossQuad::GenerateVertices(vector<Vertex> &vertices, vec3 WorldPos) const {
    for (int p = 0; p < 6; p++) {
        if (Exposed[p] == true) {
            for (int q = 0; q < 24; q++) {
                Vertex vertex = CrossQuadVertice[q];
                vertex.CubeMapTex = vertex.position;
                vertex.position += WorldPos;
                vertex.cubeID = CrossQuadID;
                vertex.faceID = 1;
                vertices.push_back(vertex);
            }
            return;
        }
    }
}

/* Chunk */
Chunk::Chunk(const shared_ptr<Map> &Map, vec3 position, vec3 size) : map(Map), pos(position), size(size) {
    init();
}

void Chunk::GenerateMesh() {
    if (vertices.size() != 0) {
        vertices.clear();
        vertices.resize(0);
    }

    auto logger = Loggers::getLogger("Chunk");
    vec3 WorldPos = pos * size * 2.0f;
    WorldPos.z = -WorldPos.z;

    auto mapMap = map->GetMap();
    for (int i = 0; i < size.x; i++)
        for (int j = 0; j < size.y; j++)
            for (int k = 0; k < size.z; k++) {
                auto &mesh = (*mapMap)[i + pos.x * size.x][j + pos.y * size.y][k + pos.z * size.z];
                if (mesh == nullptr || mesh->ID() == 0)
                    continue;
                if (mesh->ID() == CB_CLOUD) {
                    mesh->GenerateVertices(map->cloudChunk.CloudVertices, WorldPos + vec3(i, j, -k) * 2.0f);
                    continue;
                }
                if (mesh->ID() == CB_WATER) {
                    mesh->GenerateVertices(map->waterChunk.WaterVertices, WorldPos + vec3(i, j, -k) * 2.0f);
                    continue;
                }
                mesh->GenerateVertices(vertices, WorldPos + vec3(i, j, -k) * 2.0f);
            }
}

void Chunk::setupBuffer() {
    if (vertices.size() != 0) {
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, texCoords));
        glEnableVertexAttribArray(2);
        glVertexAttribIPointer(2, 1, GL_INT, sizeof(Vertex), (void *)offsetof(Vertex, faceID));
        glEnableVertexAttribArray(3);
        glVertexAttribIPointer(3, 1, GL_INT, sizeof(Vertex), (void *)offsetof(Vertex, cubeID));
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, CubeMapTex));

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        verSize = vertices.size();
        vertices.clear();
        vertices.resize(0);
    }
}

static inline mat4 getModel(int x, int y, int z, float CubeSize) {
    return glm::translate(mat4(1.0f), vec3(x * CubeSize, y * CubeSize, z * CubeSize));
}

void Chunk::Draw(const mat4 &view, const mat4 &projection, float CubeMap) {
    if (int(CubeMap) == 1 && isCulled)
        return;
    if (verSize != 0) {
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, verSize);
        glBindVertexArray(0);
    }
}

void Chunk::init() {

    // VAO VBO 生成
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glGenVertexArrays(1, &CloudVAO);
    glGenBuffers(1, &CloudVBO);

    // Buffer 生成
    GenerateMesh();

    // Buffer 设置
    setupBuffer();
}

ScreenQuad::ScreenQuad() {
}

void ScreenQuad::init() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    GenerateMesh();

    setupBuffer();
}

void ScreenQuad::GenerateMesh() {
    for (auto &ver : Cube::CubeVertice) {
        vertices.push_back(ver);
    }
}

void ScreenQuad::setupBuffer() {
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void ScreenQuad::Draw() {
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, vertices.size());
    glBindVertexArray(0);
}

void CloudChunk::setupBuffer() {
    glGenVertexArrays(1, &CloudVAO);
    glGenBuffers(1, &CloudVBO);
    if (CloudVertices.size() != 0) {
        glBindVertexArray(CloudVAO);
        glBindBuffer(GL_ARRAY_BUFFER, CloudVBO);
        glBufferData(GL_ARRAY_BUFFER, CloudVertices.size() * sizeof(Vertex), &CloudVertices[0], GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, texCoords));
        glEnableVertexAttribArray(2);
        glVertexAttribIPointer(2, 1, GL_INT, sizeof(Vertex), (void *)offsetof(Vertex, faceID));
        glEnableVertexAttribArray(3);
        glVertexAttribIPointer(3, 1, GL_INT, sizeof(Vertex), (void *)offsetof(Vertex, cubeID));
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, CubeMapTex));

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        CloudVersize = CloudVertices.size();
        CloudVertices.clear();
        CloudVertices.resize(0);
    }
}

void CloudChunk::Draw(const mat4 &view, const mat4 &projection, float CubeMap) {
    if (CloudVersize != 0) {
        glBindVertexArray(CloudVAO);
        glDrawArrays(GL_TRIANGLES, 0, CloudVersize);
        glBindVertexArray(0);
    }
}

void SunChunk::init() {
    glGenVertexArrays(1, &SunVAO);
    glGenBuffers(1, &SunVBO);
}

void SunChunk::setupBuffer() {

    glBindVertexArray(SunVAO);
    glBindBuffer(GL_ARRAY_BUFFER, SunVBO);
    glBufferData(GL_ARRAY_BUFFER, Cube::CubeVertice.size() * sizeof(Vertex), &Cube::CubeVertice[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    SunVersize = Cube::CubeVertice.size();
}

void SunChunk::Draw(const mat4 &view, const mat4 &projection, float CubeMap) {
    if (SunVersize != 0) {
        glBindVertexArray(SunVAO);
        glDrawArrays(GL_TRIANGLES, 0, SunVersize);
        glBindVertexArray(0);
    }
}

void WaterChunk::init() {
    glGenVertexArrays(1, &WaterVAO);
    glGenBuffers(1, &WaterVBO);
}

void WaterChunk::setupBuffer() {
    glBindVertexArray(WaterVAO);
    glBindBuffer(GL_ARRAY_BUFFER, WaterVBO);
    glBufferData(GL_ARRAY_BUFFER, WaterVertices.size() * sizeof(Vertex), &WaterVertices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, texCoords));
    glEnableVertexAttribArray(2);
    glVertexAttribIPointer(2, 1, GL_INT, sizeof(Vertex), (void *)offsetof(Vertex, faceID));
    glEnableVertexAttribArray(3);
    glVertexAttribIPointer(3, 1, GL_INT, sizeof(Vertex), (void *)offsetof(Vertex, cubeID));
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, CubeMapTex));
    glEnableVertexAttribArray(5);
    glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, normal));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    WaterVersize = WaterVertices.size();
}

void WaterChunk::Draw(const mat4 &view, const mat4 &projection, float CubeMap) {
    if (WaterVersize != 0) {
        glBindVertexArray(WaterVAO);
        glDrawArrays(GL_TRIANGLES, 0, WaterVersize);
        glBindVertexArray(0);
    }
}

ScreenMesh::ScreenMesh() {
}

void ScreenMesh::init() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    GenerateMesh();

    setupBuffer();
}

void ScreenMesh::Draw() {
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, vertices.size());
    glBindVertexArray(0);
}

void ScreenMesh::GenerateMesh() {
    for (int i = 24; i < 30; i++) {
        auto &ver = Cube::CubeVertice[i];
        auto ver2D = Vertex2D(vec2(ver.position.x, ver.position.y), ver.texCoords);
        vertices.push_back(ver2D);
    }
}

void ScreenMesh::setupBuffer() {
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex2D), &vertices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (void *)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (void *)offsetof(Vertex2D, texCoords));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
