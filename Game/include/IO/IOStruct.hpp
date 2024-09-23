#pragma once

#ifndef IO_STRUCT_HPP
#define IO_STRUCT_HPP

typedef int ID;

enum MeshType {
    CubeType = 0,
    QuadType = 1,
    CrossQuadType = 2
};

struct IOStruct {
    virtual std::vector<char> Serialize() = 0;
    virtual void Deserialize(const std::vector<char>& data) = 0;
};

struct MeshIOStruct : public IOStruct {
    ID id;
    MeshType type;
    MeshIOStruct() {}
    MeshIOStruct(ID _id, MeshType _type) : id(_id), type(_type) {}

    // 序列化
    std::vector<char> Serialize() {
        std::vector<char> serializedData;

        // 写入 serializedData
        serializedData.insert(serializedData.end(), reinterpret_cast<const char*>(&id), reinterpret_cast<const char*>(&id) + sizeof(id));
        serializedData.insert(serializedData.end(), reinterpret_cast<const char*>(&type), reinterpret_cast<const char*>(&type) + sizeof(type));

        return serializedData;
    }

    // 反序列化
    void Deserialize(const std::vector<char>& data) {
        // 读取 data
        id = *reinterpret_cast<const ID*>(data.data());
        type = *reinterpret_cast<const MeshType*>(data.data() + sizeof(id));
    }
};

struct MapIOStruct : public IOStruct {
    int seed;
    vec3 mapSize;
    std::vector<MeshIOStruct> map;
    MapIOStruct() {}
    MapIOStruct(int _seed, vec3 _mapSize, std::vector<MeshIOStruct> _map) : seed(_seed), mapSize(_mapSize), map(_map) {}

    // 序列化
    std::vector<char> Serialize() {
        std::vector<char> serializedData;

        // 写入 serializedData
        serializedData.insert(serializedData.end(), reinterpret_cast<const char*>(&seed), reinterpret_cast<const char*>(&seed) + sizeof(seed));
        serializedData.insert(serializedData.end(), reinterpret_cast<const char*>(&mapSize), reinterpret_cast<const char*>(&mapSize) + sizeof(mapSize));

        for (auto &mesh : map) {
            auto meshSerializedData = mesh.Serialize();
            serializedData.insert(serializedData.end(), meshSerializedData.begin(), meshSerializedData.end());
        }

        return serializedData;
    }

    // 反序列化
    void Deserialize(const std::vector<char>& data) {
        // 读取 data
        seed = *reinterpret_cast<const int*>(data.data());
        mapSize = *reinterpret_cast<const vec3*>(data.data() + sizeof(seed));

        int offset = sizeof(seed) + sizeof(mapSize);
        while (offset < data.size()) {
            MeshIOStruct mesh;
            mesh.Deserialize(std::vector<char>(data.begin() + offset, data.end()));
            map.push_back(mesh);
            offset += sizeof(ID) + sizeof(MeshType);
        }
    }
};

#endif // IO_STRUCT_HPP