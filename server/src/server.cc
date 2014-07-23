#include "server.h"

#include <unistd.h>
#include <errno.h>
#include <cstdlib>

namespace bm_server {

task_pool_t::task_pool_t(uint32_t task_num) : max_task_num(task_num), beg_index(0)
{
    if (max_task_num <= 0) {
        max_task_num = 4;
    }

    tasks.reserve(max_task_num);
    for (uint32_t i = 0; i < max_task_num; ++i) {
        tasks.push_back(create_task_info_t());
    }
}

/* 非线程安全 */
task_info_t *
task_pool_t::pop()
{
    if (beg_index == tasks.size()) {
        tasks.reserve(max_task_num * 2);
        for (uint32_t i = max_task_num; i < max_task_num * 2; ++i) {
            tasks.push_back(create_task_info_t());
        }
    }
    return tasks[beg_index++];
}

/* 非线程安全 */
void
task_pool_t::push(task_info_t *task)
{
    if (beg_index == 0) {
        fprintf(stderr, "WRONG OPERATION!\n");
        exit(1);
    } else {
        tasks[--beg_index] = task;
    }
}

task_info_t *
task_pool_t::create_task_info_t()
{
    task_info_t *ret = new task_info_t();
    ret->read_event = new event();
    ret->write_event = new event();
    ret->read_type = READ_HEADER;

    return ret;
}

struct event_base * server::base = NULL;
server::callback_func_t server::_callback_func = NULL;
task_pool_t *server::task_pool = NULL;

server::server(const bm_utilities::Configure &conf)
{
    /* 创建server端socket */
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    struct sockaddr_in svr_addr;
    bzero(&svr_addr, sizeof(svr_addr));
    svr_addr.sin_family = AF_INET;
    svr_addr.sin_addr.s_addr = htons(INADDR_ANY);
    svr_addr.sin_port = htons(atoi(conf["port"].c_str()));

    bind(server_fd, (struct sockaddr*)&svr_addr, sizeof(svr_addr));
    listen(server_fd, 60);

    /* 初始化事件base */
    base = event_base_new();

    task_pool = new task_pool_t(4);
}

void
server::run(callback_func_t func)
{
    _callback_func = func;

    struct event ev_listen_event;

    event_set(&ev_listen_event, server_fd, EV_READ | EV_PERSIST, on_accept, NULL);
    event_base_set(base, &ev_listen_event);
    event_add(&ev_listen_event, NULL);
    event_base_dispatch(base);
}

void
server::on_read(int fd, short event_type, void *arg)
{
    task_info_t *task_info = (task_info_t *) arg;

    if (event_type == EV_TIMEOUT) {
        /*
        if(event_add(task_info->read_event, NULL) != 0) {
            close(task_info->read_event->ev_fd);
            task_pool->push(task_info);
        }
        */
        return;
    }
    
    int req_header_len = sizeof(nshead_t);
    int bytes;
    while (task_info->read_type == READ_HEADER) {
        bytes = recv(fd, (char*)&task_info->req_head + task_info->offset, req_header_len - task_info->offset, 0);
        if (bytes < 0) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                /*
                if (event_add(task_info->read_event, NULL) != 0) {
                    close(task_info->read_event->ev_fd);
                    task_pool->push(task_info);
                    return;
                }
                */
            } else {
                close(task_info->read_event->ev_fd);
                task_pool->push(task_info);
                event_del(task_info->read_event);
            }
            return;
        } else if (bytes == 0) {
            close(task_info->read_event->ev_fd);
            task_pool->push(task_info);
            event_del(task_info->read_event);
            return;
        }
    
        if (bytes + task_info->offset < req_header_len) {
            task_info->offset += bytes;
            /*
            if (event_add(task_info->read_event, NULL) != 0) {
                close(task_info->read_event->ev_fd);
                task_pool->push(task_info);
                return;
            }
            */
        } else {
            task_info->read_type = READ_BODY;
            task_info->body_buffer = new char[task_info->req_head.body_len + 1];
            if (task_info->body_buffer == NULL) {
                close(fd);
                task_pool->push(task_info);
                event_del(task_info->read_event);
                return;
            }
            task_info->body_buffer[task_info->req_head.body_len] = '\0';
            task_info->offset = 0;
            break;
        }
    }

    while (task_info->read_type == READ_BODY) {
        bytes = recv(fd, task_info->body_buffer + task_info->offset, task_info->req_head.body_len - task_info->offset, 0);
        if (bytes < 0) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                /*
                if(event_add(task_info->read_event, NULL) != 0) {
                    close(task_info->read_event->ev_fd);
                    task_pool->push(task_info);
                    return;
                }
                */
            } else {
                close(task_info->read_event->ev_fd);
                task_pool->push(task_info);
                event_del(task_info->read_event);
            }
            return;
        } else if (bytes == 0) {
            close(task_info->read_event->ev_fd);
            task_pool->push(task_info);
            event_del(task_info->read_event);
            return;
        }
    
        if (task_info->req_head.body_len - task_info->offset > bytes) {
            task_info->offset += bytes;
            /*
            if (event_add(task_info->read_event, NULL) != 0) {
                close(task_info->read_event->ev_fd);
                task_pool->push(task_info);
                return;
            }
            */
        } else {
            task_info->read_type = UNSPECIFIED;
            break;
        }
    }
    event_del(task_info->read_event);
    task_info->req.ParseFromString(task_info->body_buffer);

    _callback_func(task_info->req_head, task_info->req, task_info->res_head, task_info->res);

    std::string res_data;
    task_info->res.SerializeToString(&res_data);
    task_info->res_head.body_len = res_data.size();
    task_info->res_head.id = task_info->req_head.id; /* 与请求相同 */
    task_info->res_str.assign((char*)&task_info->res_head, sizeof(task_info->res_head));
    task_info->res_str += res_data;
    task_info->offset = 0;

    /* 添加写事件 */
    event_set(task_info->write_event, fd, EV_WRITE | EV_PERSIST, on_write, task_info);
    event_base_set(base, task_info->write_event);
    event_add(task_info->write_event, NULL);
}

void
server::on_write(int fd, short event_type, void *arg)
{
    task_info_t *task_info = (task_info_t *) arg;
    
    if (event_type == EV_TIMEOUT) {
        /*
        if(event_add(task_info->write_event, NULL) != 0) {
            close(task_info->write_event->ev_fd);
            task_pool->push(task_info);
        }
        */
        return;
    }
    
    while (task_info->offset < task_info->res_str.size()) {
        ssize_t write_bytes = send(task_info->write_event->ev_fd,
                        task_info->res_str.c_str() + task_info->offset,
                        task_info->res_str.size() - task_info->offset, 0);
        if (write_bytes < 0) {
            if (errno == EAGAIN) {
                return;
            }
            return;
        }
        task_info->offset += write_bytes;
    }
    event_del(task_info->write_event);
}

void
server::on_accept(int fd, short event_type, void *arg)
{
    int client_fd;
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);

    client_fd = accept(fd, (struct sockaddr*)&client_addr, &client_addr_len);

    task_info_t *task_info = task_pool->pop();

    task_info->read_type = READ_HEADER;

    event_set(task_info->read_event, client_fd, EV_READ | EV_PERSIST, on_read, task_info);
    event_base_set(base, task_info->read_event);
    event_add(task_info->read_event, NULL);
}

} // END namespace bm_server
