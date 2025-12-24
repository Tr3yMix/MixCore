#pragma once

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>


namespace MixCore::Logger {

    template<typename T>
    concept Streamable = requires(std::ostream& os, T t)
    {
        {os << t} -> std::same_as<std::ostream&>;
    };

    enum class LogType {Info, Error, Warn, Debug};

    inline const char* logTypeToString(const LogType logType) {
        switch (logType) {
            case LogType::Info: return "[Info]: ";
            case LogType::Error: return "[Error]: ";
            case LogType::Warn: return "[Warn]: ";
            case LogType::Debug: return "[Debug]: ";
        }
        return "[]: ";
    }

    /*
    inline WORD logColor(const LogType logType) {
        switch (logType) {
            case LogType::Info: return FOREGROUND_GREEN | FOREGROUND_INTENSITY;
            case LogType::Error: return FOREGROUND_RED | FOREGROUND_INTENSITY;
            case LogType::Warn: return FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
            case LogType::Debug: return FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
        }
        return FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
    }
    */

    inline void init() {
        std::ofstream logFile("log.txt", std::ios::trunc);
        logFile.close();
    }

    inline std::ofstream& getLogFile() {
        static std::ofstream logFile("log.txt", std::ios::app);
        return logFile;
    }

    inline void outputToFile(const LogType type, const std::string& str) {
        auto& file = getLogFile();
        file << logTypeToString(type) << str << std::endl;
        file.flush();
    }

    template <Streamable... Args>
    void log(const LogType type = LogType::Info, Args&&... args) {
        std::stringstream stream;
        (stream << ... << std::forward<Args>(args));
        // ReSharper disable once CppLocalVariableMayBeConst
        /*
        HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(consoleHandle, logColor(type));
        */
        std::cout << logTypeToString(type) << stream.str() << std::endl;

        outputToFile(type, stream.str());
    }

    template <Streamable... Args>
    void debug(Args&&... args){log(LogType::Debug, std::forward<Args>(args)...);}

    template <Streamable... Args>
    void error(Args&&... args){log(LogType::Error, std::forward<Args>(args)...);}

    template <Streamable... Args>
    void warn(Args&&... args){log(LogType::Warn, std::forward<Args>(args)...);}

    template <Streamable... Args>
    void info(Args&&... args){log(LogType::Info, std::forward<Args>(args)...);}

    /*

    template <typename... Args>
    void logf(const LogType type, const std::string_view fmt, Args&&... args) {
        // ReSharper disable once CppLocalVariableMayBeConst
        /*
        HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(consoleHandle, logColor(type));

        std::cout << logTypeToString(type) << std::vformat(fmt, std::make_format_args(args...)) << std::endl;

        outputToFile(type, std::vformat(fmt, std::make_format_args(args...)));
    }

    template <typename... Args>
    void debugf(std::string_view fmt, Args&&... args) {
        logf(LogType::Debug, fmt, std::forward<Args>(args)...);
    }

    template <typename... Args>
    void errorf(std::string_view fmt, Args&&... args) {
        logf(LogType::Error, fmt, std::forward<Args>(args)...);
    }

    template <typename... Args>
    void warnf(std::string_view fmt, Args&&... args) {
        logf(LogType::Warn, fmt, std::forward<Args>(args)...);
    }

    template <typename... Args>
    void infof(std::string_view fmt, Args&&... args) {
        logf(LogType::Info, fmt, std::forward<Args>(args)...);
    }

    */

}



