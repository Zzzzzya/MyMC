#pragma once

#ifndef IO_MANAGER_HPP
#define IO_MANAGER_HPP

#include <fstream>
#include <vector>

#include "Logger.hpp"

struct IOStruct {
    virtual std::vector<char> Serialize() = 0;
    virtual void Deserialize(const std::vector<char>& data) = 0;
};

// IOManager 工具类
class IOManager {
public:
    // 读取文件
    static bool Read(const std::string& filename, IOStruct& data, std::ios::openmode mode = std::ios::binary);
    // 写入文件
    static bool Write(const std::string &filename, IOStruct& data, std::ios::openmode mode = std::ios::binary);

private:
    // 私有化构造函数和析构函数
    IOManager() = default;
    ~IOManager() = default;
};

#endif // IO_MANAGER_HPP
