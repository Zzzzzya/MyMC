#include <IO/IOManager.hpp>

bool IOManager::Read(const std::string& filename, IOStruct& data, std::ios::openmode mode) 
{
    std::shared_ptr<spdlog::logger> logger = Loggers::getLogger("IORead");

    std::ifstream file(filename, mode);
    if (!file.good())
    {
        LOG_ERROR(logger, "Failed to open " + filename);
        return false;
    }

    // 读取文件到 vector 中
    std::vector<char> buffer((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    data.Deserialize(buffer);

    return true;
}

bool IOManager::Write(const std::string &filename, IOStruct& data, std::ios::openmode mode)
{
    std::shared_ptr<spdlog::logger> logger = Loggers::getLogger("IOWrite");

    std::ofstream file(filename, mode);
    if (!file.good())
    {
        LOG_ERROR(logger, "Failed to open " + filename);
        return false;
    }

    auto buffer = data.Serialize();
    file.write(buffer.data(), buffer.size());

    return true;
}
