#include "log.h"

#include <cstdio>
#include <cstdlib>
#include <cstdarg>
#include <ctime>
#include <errno.h>
#include <sys/stat.h>
#include <dirent.h>

namespace bm {
namespace log {

Logger::~Logger() {
    pthread_mutex_destroy(&_flog_lock);
    if (_flog != stdout && _flog != NULL) {
        fclose(_flog);
    }

    pthread_mutex_destroy(&_ffatal_lock);
    if (_ffatal != stderr && _ffatal != NULL) {
        fclose(_ffatal);
    }
}

void Logger::init(int level, const std::string &dir, const std::string &fname) {
    if (_is_inited) {
        return;
    }
    _log_level = BMLogLevel(level);
    std::string path = dir + "/" + fname;

    int ret = mkdir(dir.c_str(), 0775);
    if (ret != 0  && !(ret == -1 && errno == EEXIST && opendir(dir.c_str()) != NULL)) {
        fprintf(stderr, "Create directory [%s] failed! ERROR [%s]\n",
                         dir.c_str(), strerror(errno));
        return;
    }

    pthread_mutex_lock(&_init_lock); // Access init lock
    if (!_is_inited) {
        _flog = fopen(path.c_str(), "a");
        if (_flog == NULL) {
            fprintf(stderr, "Opening file [%s] failed! ERROR [%s]\n",
                             path.c_str(), strerror(errno));
            exit(1);
        }

        _ffatal = fopen((path + ".wf").c_str(), "a");
        if (_ffatal == NULL) {
            fprintf(stderr, "Opening file [%s.wf] failed! ERROR [%s]\n",
                             path.c_str(), strerror(errno));
            exit(1);
        }
        _is_inited = true;
    }
    pthread_mutex_unlock(&_init_lock);
}

bool Logger::write_log(BMLogLevel level, const char *fmt, ...) {
    if (level > _log_level) {
        return true;
    }

    time_t t;  
    struct tm *tp;
    char time_str[100];
    time(&t);
    tp = localtime(&t);
    strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", tp);

    va_list ap;
    char buffer[4096];

    va_start(ap, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, ap);
    va_end(ap);

    switch (level) {
    case BM_LOG_LEVEL_FATAL:
        pthread_mutex_lock(&_ffatal_lock);
        fprintf(_ffatal, "[%s][FATAL]%s\n", time_str, buffer);
        fflush(_ffatal);
        pthread_mutex_unlock(&_ffatal_lock);
        break;

    case BM_LOG_LEVEL_WARNING:
        pthread_mutex_lock(&_ffatal_lock);
        fprintf(_ffatal, "[%s][WARNING]%s\n", time_str, buffer);
        pthread_mutex_unlock(&_ffatal_lock);
        break;

    case BM_LOG_LEVEL_NOTICE:
        pthread_mutex_lock(&_flog_lock);
        fprintf(_flog, "[%s][NOTICE]%s\n", time_str, buffer);
        pthread_mutex_unlock(&_flog_lock);
        break;

    case BM_LOG_LEVEL_TRACE:
        pthread_mutex_lock(&_flog_lock);
        fprintf(_flog, "[%s][TRACE]%s\n", time_str, buffer);
        pthread_mutex_unlock(&_flog_lock);
        break;

    case BM_LOG_LEVEL_DEBUG:
        pthread_mutex_lock(&_flog_lock);
        fprintf(_flog, "[%s][DEBUG]%s\n", time_str, buffer);
        pthread_mutex_unlock(&_flog_lock);
        break;

    default:
        fprintf(stderr, "Unknown log level [%d]\n", level);
        return false;
    }
    return true;
}

} // END namespace log
} // END namespace bm
