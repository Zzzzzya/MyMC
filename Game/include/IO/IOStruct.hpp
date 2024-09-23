#pragma once

#ifndef IO_STRUCT_HPP
#define IO_STRUCT_HPP

typedef int ID;

enum MeshType {
    CubeType = 0,
    QuadType = 1,
    CrossQuadType = 2
};

struct MeshIOStruct {
    ID id;
    MeshType type;
    MeshIOStruct() {}
    MeshIOStruct(ID _id, MeshType _type) : id(_id), type(_type) {}
};

struct MapIOStruct {
    int seed;
    vec3 mapSize;
    std::vector<MeshIOStruct> map;
    MapIOStruct() {}
    MapIOStruct(int _seed, vec3 _mapSize, std::vector<MeshIOStruct> _map) : seed(_seed), mapSize(_mapSize), map(_map) {}
};

#endif // IO_STRUCT_HPP