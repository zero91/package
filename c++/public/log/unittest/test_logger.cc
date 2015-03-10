#include <iostream>

#include "logger.h"

using namespace std;
using namespace bm_utilities;

int main(int argc, char *argv[])
{
    BM_LOG_INIT("../data_out/test_log", "test_logger");
    //BM_LOG_INIT("dir/test_logger");
    
    BM_LOG(BM_LOG_LEVEL_INFO, "Testing BM_LOG_INFO");
    BM_LOG(BM_LOG_LEVEL_WARNING, "Testing BM_LOG_WARNING");
    BM_LOG(BM_LOG_LEVEL_FATAL, "Testing BM_LOG_FATAL");

    return 0;
}
