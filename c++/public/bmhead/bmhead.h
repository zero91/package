#ifndef _BM_COMMON_H_
#define _BM_COMMON_H_

#include <stdint.h>

namespace bm_server {

static const unsigned int BMHEAD_MAGICNUM = 0xfb709394;

// 返回错误码 = BMHEAD_RET_SUCCESS成功, < 0 失败
typedef enum _BMHEAD_RET_ERROR_T {
BMHEAD_RET_SUCCESS       =   0,  //<读写OK
BMHEAD_RET_EPARAM        =  -1,  //<参数有问题
BMHEAD_RET_EBODYLEN      =  -2,  //<变长数据长度有问题
BMHEAD_RET_WRITE         =  -3,  //<写的问题
BMHEAD_RET_READ          =  -4,  //<读消息体失败，具体错误看errno
BMHEAD_RET_READHEAD      =  -5,  //<读消息头失败, 具体错误看errno
BMHEAD_RET_WRITEHEAD     =  -6,  //<写消息头失败, 可能是对方将连接关闭了
BMHEAD_RET_PEARCLOSE     =  -7,  //<对端关闭连接
BMHEAD_RET_ETIMEDOUT     =  -8,  //<读写超时
BMHEAD_RET_EMAGICNUM     =  -9,  //<magic_num不匹配
BMHEAD_RET_UNKNOWN       =  -10
} BMHEAD_RET_ERROR_T;

static const char *bmhead_errstr[] = {
	"BMHEAD_RET_SUCCESS",
	"BMHEAD_RET_EPARAM",
	"BMHEAD_RET_EBODYLEN",
	"BMHEAD_RET_WRITE",
	"BMHEAD_RET_READ",
	"BMHEAD_RET_READHEAD",
	"BMHEAD_RET_WRITEHEAD",
	"BMHEAD_RET_PEARCLOSE",
	"BMHEAD_RET_ETIMEDOUT",
	"BMHEAD_RET_EMAGICNUM",
	"BMHEAD_RET_UNKNOWN",
};

// 网络通信数据头
typedef struct _bmhead_t {
    uint16_t id;           // id
    uint16_t version;      // 版本号
    uint32_t log_id;       // 由apache产生的logid，贯穿一次请求的所有网络交互
    char     provider[16]; // 客户端标识，建议命名方式：产品名-模块名，比如"bm_web-recom"
    uint32_t magic_num;    // 特殊标识，标识一个包的起始
    uint32_t reserved;     // 保留
    uint32_t body_len;     // head后请求数据的总长度
} bmhead_t;

// 返回表示错误含义的字符串
const char* bmhead_error(int ret);

} // END namespace bm_server

#endif // END _BM_COMMON_H_
