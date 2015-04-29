#ifndef _BM_LOG_H_
#define _BM_LOG_H_

#include <pthread.h>
#include <string>
#include <cstdio>

#ifdef DISALLOW_COPY_AND_ASSIGN
#undef DISALLOW_COPY_AND_ASSIGN
#endif
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
            TypeName(const TypeName &);         \
            TypeName &operator=(const TypeName &)

#define bm_log_write(level, _fmt_, args...)                            \
do {                                                                   \
    bm_utilities::Logger::get_instance()->write_log(level,             \
        "[%s:%s:%d] "_fmt_, __FUNCTION__, __FILE__, __LINE__, ##args); \
} while (0);

#define bm_log_init(level, dir, fname) \
do { \
    bm_utilities::Logger::get_instance()->init(level, dir, fname); \
} while (0);

#define bm_log_set_level(level) \
do { \
    bm_utilities::Logger::get_instance()->set_level(level); \
} while (0);

#define BM_LOG_FATAL(fmt, arg...) \
do { \
    bm_utilities::bm_writelog(bm_utilities::BM_LOG_LEVEL_FATAL , fmt, ##arg); \
} while (0)

#define BM_LOG_WARNING(fmt, arg...) \
do { \
    bm_utilities::bm_writelog(bm_utilities::BM_LOG_LEVEL_WARNING, fmt, ##arg); \
} while (0)

#define BM_LOG_NOTICE(fmt, arg...) \
do { \
    bm_utilities::bm_writelog(bm_utilities::BM_LOG_LEVEL_NOTICE, fmt, ##arg); \
} while (0)

#define BM_LOG_TRACE(fmt, arg...) \
do { \
    bm_utilities::bm_writelog(bm_utilities::BM_LOG_LEVEL_TRACE, fmt, ##arg); \
} while (0)

#define BM_LOG_DEBUG(fmt, arg...) \
do { \
    bm_utilities::bm_writelog(bm_utilities::BM_LOG_LEVEL_DEBUG, fmt, ##arg); \
} while (0)

enum LogLevel {
    BM_LOG_LEVEL_FATAL     = 1,
    BM_LOG_LEVEL_WARNING   = 2,
    BM_LOG_LEVEL_NOTICE    = 4,
    BM_LOG_LEVEL_TRACE     = 8,
    BM_LOG_LEVEL_DEBUG     = 16
};

namespace bm_utilities {
class Logger {
public:
    static Logger *get_instance() {
        static Logger log; // single instance
        return &log;
    }

    ~Logger();

    bool is_init() { return _is_inited; }

    void init(int level, const std::string &dir, const std::string &fname);

    void set_level(int level) {
        _log_level = LogLevel(level);
    }

    bool write_log(LogLevel level, const char *fmt, ...);

private:
    DISALLOW_COPY_AND_ASSIGN(Logger);

    Logger() : _is_inited(false) {
        pthread_mutex_init(&_init_lock, NULL);
        pthread_mutex_init(&_flog_lock, NULL);
        pthread_mutex_init(&_ffatal_lock, NULL);

        _flog   = stdout;
        _ffatal = stderr;
        _log_level = BM_LOG_LEVEL_DEBUG;
    }

private:
    FILE *_flog;
    pthread_mutex_t _flog_lock;

    FILE *_ffatal;
    pthread_mutex_t _ffatal_lock;

    bool _is_inited;
    pthread_mutex_t _init_lock;

    LogLevel _log_level;
}; // END class Logger

} // END namespace bm_utilities

#endif // END _BM_LOG_H_
