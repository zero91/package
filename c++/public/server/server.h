#ifndef _BM_SERVER_H_
#define _BM_SERVER_H_

#include <string>
#include <vector>
#include <fcntl.h>

#include <event.h>

#include "bmhead.h"
#include "interface.pb.h"
#include "configure.h"

namespace bm_server {

static const uint32_t DEFAULT_TASK_NUM = 4;
static const int MAX_CONNECT_QUEUE = 60; // 最多请求队列长度

// 将文件设置为非阻塞的
inline int setnonblock(int fd) {
    int flags;
    
    flags = fcntl(fd, F_GETFL);
    flags |= O_NONBLOCK;
    return fcntl(fd, F_SETFL, flags);
}

enum read_data_type_t {
    READ_HEADER, // 正在读取头部数据
    READ_BODY,   // 正在读取内容部分数据
    UNSPECIFIED, // 出错
};

// 每个连接状态的保存结构体
struct task_info_t {
    read_data_type_t read_type; // 正在读取哪部分数据
    uint32_t offset; // 正在读取的数据的当前读取偏移量
    char *body_buffer;

    std::string res_str; // 返回结果字符串

    bmhead_t req_head;
    bmhead_t res_head;
    bm_interface::bm_req_t req;
    bm_interface::bm_res_t res;

    struct event *read_event;
    struct event *write_event;
    uint32_t timeout; // not used

    task_info_t() : read_type(READ_HEADER),
                    offset(0),
                    body_buffer(NULL),
                    read_event(NULL),
                    write_event(NULL) { }

    ~task_info_t() {
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

// 连接状态池
class task_pool_t {
public:
    explicit task_pool_t(uint32_t task_num, bool grow=true);

    ~task_pool_t();

    task_info_t* pop(); // 取一个连接任务

    bool push(task_info_t *task); // 存放一个连接任务

private:
    task_info_t *create_task_info_t(); // 创建新的连接任务结构变量

private:
    uint32_t _max_task_num; // 最多存放的任务量
    uint32_t _beg_index; // 当前可用连接的起始索引
    bool _grow; // 当空间不够用时，是否增长空间以存放更多的连接

    std::vector<task_info_t*> _tasks;
    pthread_mutex_t _task_lock; // 连接池互斥锁
};

class server {
public:
    typedef int (*callback_func_t)(const bmhead_t &req_bmhead, const bm_interface::bm_req_t &req, bmhead_t &res_bmhead, bm_interface::bm_res_t &res, void *user_buf);

public:
    explicit server(const bm_utilities::Configure &conf);

    void run(callback_func_t func);

private:
    static void on_accept(int fd, short event_type, void *arg);

    static void on_read(int fd, short event_type, void *arg);

    static void on_write(int fd, short event_type, void *arg);

private:
    static struct event_base *base;
    int server_fd; // server socket

    static task_pool_t *task_pool;

    static callback_func_t _callback_func; // server请求回调函数
};

} // END namespace bm_server

#endif
