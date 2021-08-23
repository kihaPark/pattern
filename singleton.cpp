#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <string_view>


// tested x64, c++17

using namespace std;

class Logger final
{
public:
    enum class Level {
        Error,
        Info,
        Debug
    };

    static Logger& instance() {
        static Logger instance;
        return instance;
    }

    Logger(const Logger&) = delete;
    Logger(Logger&&) = delete;

    Logger& operator=(const Logger&) = delete;
    Logger& operator=(Logger&&) = delete;

    void setFileLevel(Level level) {
        fileLevel = level;
    }

    void setConsoleLevel(Level level) {
        consoleLevel = level;
    }

    void log(string_view message, Level logLevel);
    void log(const vector<string>& messages, Level logLevel);

private:
    Logger() {
        fileStream.open(fileName, ios_base::app);
    }

    ~Logger() {
        fileStream.close();
    }

    string_view getLevelString(Level level) const;

    static inline const char* const fileName = "log.txt";

    ofstream fileStream;

    Level fileLevel = Level::Error;
    Level consoleLevel = Level::Error;
};

string_view Logger::getLevelString(Level level) const
{
    switch (level) {
    case Level::Error:
        return "ERROR";
    case Level::Info:
        return "INFO";
    case Level::Debug:
        return "DEBUG";
    default:
        return "UNKNOWN";
    }
}

void Logger::log(string_view msg, Level level)
{
    if (consoleLevel >= level)
        cout << getLevelString(level).data() << ": " << msg << endl;

    if (fileLevel >= level)
        fileStream << getLevelString(level).data() << ": " << msg << endl;
}

void Logger::log(const vector<string>& messages, Level level)
{
    for (const auto& msg : messages)
        log(msg, level);
}


int main()
{
    Logger::instance().setFileLevel(Logger::Level::Info);
    Logger::instance().setConsoleLevel(Logger::Level::Debug);

    Logger::instance().log("test log1", Logger::Level::Error);
    Logger::instance().log("test log2", Logger::Level::Info);
    Logger::instance().log("test log3", Logger::Level::Debug);

    vector<string> strVec = { "vec log4", "vec log5" };
    Logger::instance().log(strVec, Logger::Level::Debug);

    return 0;
}

