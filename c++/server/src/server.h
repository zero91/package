#ifndef _BM_SERVER_H_
#define _BM_SERVER_H_

#include <string>
#include <vector>
#include <fcntl.h>

#include <event.h>

#include "common.h"
#include "interface.pb.h"
#include "configure.h"

namespace bm_server {

/* 将文件设置为非阻塞的 */
inline int setnonblock(int fd)
{
    int flags;
    
    flags = fcntl(fd, F_GETFL);
    flags |= O_NONBLOCK;
    return fcntl(fd, F_SETFL, flags);
}

enum read_data_type_t {
    READ_HEADER,
    READ_BODY,
    UNSPECIFIED,
};

/* 每个连接状态的保存结构体 */
struct task_info_t {
    read_data_type_t read_type; /* 正在读的数据属于哪种类型的数据 */
    uint32_t offset; /* 正在读取的数据的当前读取偏移量 */
    char *body_buffer;

    std::string res_str; /* 返回结果字符串 */

    nshead_t req_head;
    nshead_t res_head;
    bm_interface::bm_req_t req;
    bm_interface::bm_res_t res;

    struct event *read_event;
    struct event *write_event;
    uint32_t timeout; /* not used */

    task_info_t() : read_type(READ_HEADER),
                    offset(0),
                    body_buffer(NULL),
                    read_event(NULL),
                    write_event(NULL)
    {
    }

    ~task_info_t()
    {
        if (body_buffer != NULL) {
            delete [] body_buffer;
        }
        if (read_event != NULL) {
            delete read_event;
        }
        if (write_event != NULL) {
            delete write_event;
        }
    }
};

/* 连接状态池 */
class task_pool_t {
public:
    task_pool_t(uint32_t task_num);

    task_info_t* pop();

    void push(task_info_t *task);

private:
    task_info_t *create_task_info_t();

private:
    std::vector<task_info_t*> tasks;

    uint32_t max_task_num;
    uint32_t beg_index;
};

class server {
public:
    typedef int (*callback_func_t)(const nshead_t &req_nshead, const bm_interface::bm_req_t &req, nshead_t &res_nshead, bm_interface::bm_res_t &res);

public:
    server(const bm_utilities::Configure &conf);

    void run(callback_func_t func);

private:
    static void on_read(int fd, short event_type, void *arg);

    static void on_write(int fd, short event_type, void *arg);

    static void on_accept(int fd, short event_type, void *arg);

private:
    static struct event_base *base;
    int server_fd; /* server socket */

    static task_pool_t *task_pool;

    static callback_func_t _callback_func; /* server请求回调函数 */
};

} // END namespace bm_server

#endif
