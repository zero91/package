#ifndef _BM_COMMON_H_
#define _BM_COMMON_H_

namespace bm_server {

static const unsigned int NSHEAD_MAGICNUM = 0xfb709394;

// 网络通信数据头
typedef struct _nshead_t {
    uint16_t id;           // id
    uint16_t version;      // 版本号
    uint32_t log_id;       // (M)由apache产生的logid，贯穿一次请求的所有网络交互
    char     provider[16]; // (M)客户端标识，建议命名方式：产品名-模块名，比如"bm_web-recom"
    uint32_t magic_num;    // (M)特殊标识，标识一个包的起始
    uint32_t reserved;     // 保留
    uint32_t body_len;     // (M)head后请求数据的总长度
} nshead_t;
    
// sock     sock描述符
// head    从sock读sizeof(nshead_t)长度的数据填充进head指向的内存
// req       存储body中req数据的内存
// size      req长度
// buf       存储body中具体数据的内存
// buf_size
// timeout  函数读取超时时间
// flags    标志位
// 
// 函数执行流程：
// 调用ul_sreado_ms_ex方法从sock读nshead头，超时时间为timeout。
// 读成功判断
// 若flags设置NSHEAD_CHECK_MAGICNUM位，则判断head->magic_num == NSHEAD_MAGICNUM
// 对 body 的长度进行检查, 过长和过短都不符合要求
// 读取请求
// 读取具体数据
// 结束
int nshead_read(int sock, nshead_t *head, void *req, size_t req_size, void *buf, size_t buf_size, int timeout, unsigned flags);

// 函数执行流程：
// ul_swriteo_ms_ex写head头部
// ul_swriteo_ms_ex写req
// ul_swriteo_ms_ex写buf， 长度为head->body_len - req_size
int nshead_write(int sock, nshead_t *head, void *req, size_t req_size, void *buf, int timeout, unsigned flags);

/// 返回错误码 = NSHEAD_RET_SUCCESS成功, <0失败
typedef enum _NSHEAD_RET_ERROR_T {
NSHEAD_RET_SUCCESS       =   0, ///<读写OK
NSHEAD_RET_EPARAM        =  -1, ///<参数有问题
NSHEAD_RET_EBODYLEN      =  -2, ///<变长数据长度有问题
NSHEAD_RET_WRITE         =  -3, ///<写的问题
NSHEAD_RET_READ          =  -4, ///<读消息体失败，具体错误看errno
NSHEAD_RET_READHEAD      =  -5, ///<读消息头失败, 具体错误看errno
NSHEAD_RET_WRITEHEAD     =  -6, ///<写消息头失败, 可能是对方将连接关闭了
NSHEAD_RET_PEARCLOSE     =  -7, ///<对端关闭连接
NSHEAD_RET_ETIMEDOUT     =  -8, ///<读写超时
NSHEAD_RET_EMAGICNUM     =  -9, ///<magic_num不匹配
NSHEAD_RET_UNKNOWN       =  -10
} NSHEAD_RET_ERROR_T;


} // END namespace bm_server

#endif // END _BM_COMMON_H_
