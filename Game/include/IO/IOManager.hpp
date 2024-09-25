#pragma once

#ifndef IO_MANAGER_HPP
#define IO_MANAGER_HPP

#include <fstream>
#include <vector>
#include <filesystem>

#ifdef _WIN32
#include <direct.h>
#else
#include <sys/stat.h>
#include <sys/types.h>
#endif

#include "Logger.hpp"

namespace fs = std::filesystem;

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
    // 检查目录是否存在
    static bool DirExists(const std::string& dir);
    // 创建目录
    static bool MakeDir(const std::string& dir);
    // 获取目录下所有文件
    static bool GetFiles(const std::string& dir, std::vector<std::string>& files);

private:
    // 私有化构造函数和析构函数
    IOManager() = default;
    ~IOManager() = default;
};

#endif // IO_MANAGER_HPP
