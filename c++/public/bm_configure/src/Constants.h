#ifndef BM_CONFIGURE_CONSTANTS_H
#define BM_CONFIGURE_CONSTANTS_H

namespace bm {
namespace configure {

enum ConfigType {
    CONFIG_TYPE_UNIT  = 0,
    CONFIG_TYPE_GROUP = 1,
    CONFIG_TYPE_ARRAY = 2,
    CONFIG_TYPE_ERROR = 3
};

enum ConfigErrorNo {
    CONFIG_ERROR_OK                 = 0,
    CONFIG_ERROR_NO_SUCH_KEY        = 1,
    CONFIG_ERROR_DUPLICATED_KEY     = 2,
    CONFIG_ERROR_OUT_OF_RANGE       = 3,
    CONFIG_ERROR_GROUP_TO_UNIT      = 4,
    CONFIG_ERROR_FORMAT_ERROR       = 5,
    CONFIG_ERROR_CONSTRAINT_ERROR   = 6,
    CONFIG_ERROR_CONFIG_ERROR       = 7,
    CONFIG_ERROR_NULL_VALUE         = 8,
    CONFIG_ERROR_NULL_BUFFER        = 9,
    CONFIG_ERROR_ERROR              = 10,
    CONFIG_ERROR_UNKNOWN            = 11
};

enum GroupDupLevel {
    GROUP_DUP_LEVEL_0 = 0,   //允许重名，不检测
    GROUP_DUP_LEVEL_1 = 1,   //检测第一级group，即 "GLOBAL"下的group是否重名 
    GROUP_DUP_LEVEL_2 = 2,   //检测所有深度的重名group
    GROUP_DUP_LEVEL_3 = 3,   //报告所有group名
};

static const char* CONFIG_GLOBAL = "GLOBAL";
static const char* CONFIG_INCLUDE = "$include";

const uint32_t MAX_INCLUDE_DEPTH = 1;

}; // END namespace configure
}; // END namespace bm
#endif
