#ifndef TANZANITE_LOGGER_H
#define TANZANITE_LOGGER_H

#include <iostream>

#define INFO    1
#define WARNING 2
#define ERROR   3
#define SUCCESS 4
#define FATAL   5

#define PREFIX "Tanzanite Web"

#ifdef LOGGER_USE_256RGB

#define COLOR_INFO    "\033[38;5;120m" // Green
#define COLOR_WARNING "\033[1;33m" // Yellow
#define COLOR_ERROR   "\033[1;31m" // Red
#define COLOR_SUCCESS "\033[1;36m" // Cyan
#define COLOR_FATAL   "\033[1;35m" // Magenta
#define COLOR_RESET   "\033[0m"    // Reset color

#else

#define COLOR_INFO    "\033[1;32m" // Green
#define COLOR_WARNING "\033[1;33m" // Yellow
#define COLOR_ERROR   "\033[1;31m" // Red
#define COLOR_SUCCESS "\033[1;36m" // Cyan
#define COLOR_FATAL   "\033[1;35m" // Magenta
#define COLOR_RESET   "\033[0m"    // Reset color

#endif

void log(int log_severity)
{
    switch (log_severity) {
        case INFO:
            std::cerr << COLOR_SUCCESS << PREFIX << COLOR_RESET << COLOR_INFO << " | INFO >> ";
            break;
        case WARNING:
            std::cerr << COLOR_SUCCESS << PREFIX << COLOR_RESET << COLOR_WARNING << " |WARNING >> ";
            break;
        case ERROR:
            std::cerr << COLOR_SUCCESS << PREFIX << COLOR_RESET << COLOR_ERROR << " | ERROR >> ";
            break;
        case SUCCESS:
            std::cerr << COLOR_SUCCESS << PREFIX << COLOR_RESET << COLOR_SUCCESS << " | SUCCESS >> ";
            break;
        case FATAL:
            std::cerr << COLOR_SUCCESS << PREFIX << COLOR_RESET << COLOR_FATAL << " | FATAL >> ";
            break;
        default:
            std::cerr << COLOR_SUCCESS << PREFIX << " | LOG >> ";
            break;
    }
}

/* Log class to handle the log messages */
class Logger
{
public:
    Logger(int log_severity) : log_severity(log_severity)
    {
        log(log_severity);
    }

    ~Logger()
    {
        std::cerr << COLOR_RESET << std::endl;
    }

    template <typename T>
    Logger& operator<<(const T& data)
    {
        std::cerr << data;
        return *this;
    }

private:
    int log_severity;
};

#define LOG(level) Logger(level)

#endif
