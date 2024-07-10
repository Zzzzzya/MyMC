#pragma once
#ifndef LOGGER_H
#define LOGGER_H

#include <stdlib.h>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/stdout_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/daily_file_sink.h>
#include <vector>

class Loggers // 日志类
{
public:
    // 初始化日志系统
    static void init();
    // 获取全局日志级别
    static spdlog::level::level_enum getGlobalLevel();
    // 根据日志文件名创建日志sink
    static std::vector<spdlog::sink_ptr> createSinks(const std::string &log_file_name);
    // 根据logger名称创建spdlog的logger实例
    static void createLogger(const std::string &logger_name);
    // 获取或创建logger实例
    static std::shared_ptr<spdlog::logger> getLogger(const std::string &logger_name);

private:
    // 全局日志级别静态成员变量
    static spdlog::level::level_enum global_level;
};

#endif