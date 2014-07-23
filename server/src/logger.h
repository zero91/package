#ifndef _BM_LOGGER_H_
#define _BM_LOGGER_H_

#include <pthread.h>
#include <string>
#include <fstream>
#include <iostream>

#ifndef BM_LOG
#define BM_LOG(level, _fmt_, args...) \
    bm_utilities::Logger::log_instance->write_log(level, \
    "[%s] [%s] [%d] "_fmt_, __FILE__, __FUNCTION__, __LINE__, ##args)
#endif

#ifndef BM_LOG_INIT
#define BM_LOG_INIT(dir,args...)  bm_utilities::Logger::init(dir, ##args)
#endif

namespace bm_utilities {

enum Logger_Level {
    BM_LOG_LEVEL_INFO = 1,  /* 基本信息日志级别 */
    BM_LOG_LEVEL_WARNING = 2, /* warning日志级别 */
    BM_LOG_LEVEL_FATAL = 4, /* fatal日志级别 */
    BM_LOG_LEVEL_DEBUG = 8, /* debug日志级别 */
};

class Logger {
public:
    Logger(const std::string &dir, const std::string &fname);

    explicit Logger(const std::string &path);

    ~Logger();

    bool write_log(enum Logger_Level level, const char *fmt, ...);

    static void init(const std::string &dir, const std::string &fname)
    {
        log_instance = new Logger(dir, fname);
    }

    static void init(const std::string &path)
    {
        bm_utilities::Logger::log_instance = new Logger(path);
    }

public:
    static Logger *log_instance;

private:
    void open_log_file(const std::string &path);

private:
    std::ofstream _flog; /* BM_LOG_INFO */
    std::ofstream _ffatal; /* BM_LOG_WARNING, BM_LOG_FATAL */

    pthread_mutex_t _flog_lock;
    pthread_mutex_t _ffatal_lock;
};

} // END namespace bm_utilities

#endif // END _BM_LOGGER_H_
