#include "Log/Logger.hpp"

spdlog::level::level_enum Loggers::global_level = spdlog::level::info;

spdlog::level::level_enum Loggers::getGlobalLevel()
{
    return global_level;
}

std::vector<spdlog::sink_ptr> Loggers::createSinks(const std::string &log_file_name)
{
    std::vector<spdlog::sink_ptr> sinks;

    auto sink1 = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    sink1->set_level(Loggers::getGlobalLevel());
    sinks.push_back(sink1);

    auto sink2 = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(log_file_name, 1024 * 1024 * 10, 100, false);
    sink2->set_level(spdlog::level::debug);
    sinks.push_back(sink2);
    return sinks;
}

void Loggers::createLogger(const std::string &logger_name)
{
    std::string log_file_name = logger_name + "_log.txt";
    auto sinks = Loggers::createSinks(log_file_name);

    auto logger = std::make_shared<spdlog::logger>(logger_name, begin(sinks), end(sinks));
    logger->set_level(spdlog::level::debug);
    spdlog::register_logger(logger);
}

std::shared_ptr<spdlog::logger> Loggers::getLogger(const std::string &logger_name){
    auto logger = spdlog::get(logger_name);
    if(!logger){//looger指向为空
        createLogger(logger_name);
        logger = spdlog::get(logger_name);
    }
    return logger;
}


void Loggers::init()
{
    auto level = spdlog::level::debug;
    if (std::getenv("STAGE") != NULL)
    {
        std::string stage = std::getenv("STAGE");
        if (stage == "dev")
            level = spdlog::level::debug;
    }
    Loggers::global_level = level;

    spdlog::flush_every(std::chrono::seconds(1));
    spdlog::flush_on(spdlog::level::debug);

    Loggers::createLogger("service");
}
