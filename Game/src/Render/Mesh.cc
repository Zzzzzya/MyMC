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

void Cube::GenerateVertices(vector<Vertex> &vertices, vec3 WorldPos) const {
    for (int p = 0; p < 6; p++) {
        if (Exposed[p]) {
            for (int q = 0; q < 6; q++) {
                Vertex vertex = CubeVertice[p * 6 + q];
                vertex.CubeMapTex = vertex.position - vec3(0.0f);
                vertex.position += WorldPos;
                vertex.cubeID = CubeID;
                vertex.faceID = 0;
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
                mesh->GenerateVertices(vertices, WorldPos + vec3(i, j, -k) * 2.0f);
            }
}

void Chunk::setupBuffer() {
    if (vertices.size() == 0)
        return;
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
}

static inline mat4 getModel(int x, int y, int z, float CubeSize) {
    return glm::translate(mat4(1.0f), vec3(x * CubeSize, y * CubeSize, z * CubeSize));
}

void Chunk::Draw(const mat4 &view, const mat4 &projection, float CubeMap) {
    if (isCulled)
        return;
    if (vertices.size() == 0)
        return;

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, vertices.size());
    glBindVertexArray(0);
}

void Chunk::init() {

    // VAO VBO 生成
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // Buffer 生成
    GenerateMesh();

    // Buffer 设置
    setupBuffer();
}
