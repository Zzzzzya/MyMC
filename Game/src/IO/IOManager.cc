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

bool IOManager::DirExists(const std::string& dir)
{
    struct stat info;
    if (stat(dir.c_str(), &info) != 0)
    {
        return false;
    }
    else if (info.st_mode & S_IFDIR)
    {
        return true;
    }
    return false;
}

bool IOManager::MakeDir(const std::string& dir)
{
#ifdef _WIN32
    if (_mkdir(dir.c_str()) == -1)
#else
    if (mkdir(dir.c_str(), 0777) == -1)
#endif
    {
        return false;
    }
    return true;
}

bool IOManager::GetFiles(const std::string& dir, std::vector<std::string>& files)
{
    std::shared_ptr<spdlog::logger> logger = Loggers::getLogger("IOGetFiles");
    try {
        // 检查路径是否存在
        if (!fs::exists(dir) || !fs::is_directory(dir)) {
            LOG_ERROR(logger, "Directory does not exist or is not a directory: " + dir);
            return false;
        }

        // 遍历目录
        for (const auto& entry : fs::directory_iterator(dir)) {
            if (fs::is_regular_file(entry)) {
                // 添加文件名到列表
                files.push_back(entry.path().filename().string());
            }
        }
    } catch (const fs::filesystem_error& e) {
        LOG_ERROR(logger, "Filesystem error: {}", e.what());
        return false;
    } catch (const std::exception& e) {
        LOG_ERROR(logger, "General error: {}", e.what());
        return false;
    }

    return true;
}
