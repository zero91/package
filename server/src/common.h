#ifndef _BM_COMMON_H_
#define _BM_COMMON_H_

namespace bm_server {

/* 网络通信数据头 */
struct nshead_t {
    uint32_t id;
    uint32_t body_len;
};

} // END namespace bm_server

#endif // END _BM_COMMON_H_
