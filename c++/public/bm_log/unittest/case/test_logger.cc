#include <iostream>

#include "bm_log.h"

using namespace std;

int main(int argc, char *argv[]) {
    //bm_log_set_level(4);
    bm_log_init(16, "../log/zhangjian09", "test_logger");

    bm_log_write(BM_LOG_LEVEL_NOTICE, "Testing BM_LOG_LEVEL_NOTICE");
    bm_log_write(BM_LOG_LEVEL_WARNING, "Testing BM_LOG_WARNING");
    bm_log_write(BM_LOG_LEVEL_FATAL, "Testing BM_LOG_FATAL");
    bm_log_write(BM_LOG_LEVEL_DEBUG, "Testing BM_LOG_DEBUG");
    bm_log_write(BM_LOG_LEVEL_TRACE, "Testing BM_LOG_TRACE");

    return 0;
}
