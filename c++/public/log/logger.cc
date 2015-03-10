#include "logger.h"

#include <cstdlib>
#include <cstdarg>
#include <ctime>

namespace bm_utilities {

Logger* Logger::log_instance = NULL;

Logger::Logger(const std::string &dir, const std::string &fname) {
    open_log_file(dir + "/" + fname);
}

Logger::Logger(const std::string &path) {
    open_log_file(path);
}

Logger::~Logger() {
    pthread_mutex_destroy(&_flog_lock);
    pthread_mutex_destroy(&_ffatal_lock);
    _flog.close();
    _ffatal.close();
}

void
Logger::open_log_file(const std::string &path) {
    _flog.open(path.c_str(), std::ofstream::out | std::ofstream::app);
    if (!_flog.is_open()) {
        std::cerr << "Opening file [" << path << "] failed!" << std::endl;
        exit(1);
    }

    _ffatal.open((path + ".wf").c_str(), std::ofstream::out | std::ofstream::app);
    if (!_ffatal.is_open()) {
        std::cerr << "Opening file [" << path << ".wf] failed!" << std::endl;
        exit(1);
    }
}

bool
Logger::write_log(enum Logger_Level level, const char *fmt, ...) {
    time_t t;  
    struct tm *tp;
    char time_str[100];
    time(&t);
    tp = localtime(&t);
    strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", tp);

    va_list ap;
    char buffer[4096];

    va_start(ap, fmt);
    vsprintf(buffer, fmt, ap);
    va_end(ap);

    switch (level) {
    case BM_LOG_LEVEL_INFO:
        pthread_mutex_lock(&_flog_lock);
        _flog << "[" << time_str << "]  [INFO] " << buffer << "\n";
        _flog.flush();
        pthread_mutex_unlock(&_flog_lock);
        break;

    case BM_LOG_LEVEL_WARNING:
        pthread_mutex_lock(&_ffatal_lock);
        _ffatal << "[" << time_str << "][WARNING]" << buffer << "\n";
        _ffatal.flush();
        pthread_mutex_unlock(&_ffatal_lock);
        break;

    case BM_LOG_LEVEL_FATAL:
        pthread_mutex_lock(&_ffatal_lock);
        _ffatal << "[" << time_str << "] [FATAL] " << buffer << "\n";
        _ffatal.flush();
        pthread_mutex_unlock(&_ffatal_lock);
        break;

    default:
        std::cerr << "Unknown log level [" << level << "]" << std::endl;
        return false;
    }

    return true;
}

} // END namespace bm_utilities
