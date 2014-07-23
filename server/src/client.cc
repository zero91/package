#include "interface.pb.h"
#include "server.h"

#include <iostream>
#include <string>

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

#include <arpa/inet.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>

#define EXIT_ERR(m)\
do\
{\
perror(m);\
exit(EXIT_FAILURE);\
}while(0)

using namespace std;

int main(void)
{
    bm_interface::bm_req_t msg;

    int listenfd;
    if ((listenfd = socket(PF_INET, SOCK_STREAM, 0)) < 0)
        EXIT_ERR("socket");
    
    //要连接的对方的地址
    struct sockaddr_in servaddr;
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(9102);
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    
    //连接
    if (connect(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0)
        EXIT_ERR("connect");
    
    char sendbuf[1024] = {0};
    std::string str;
    std::string data;

    bm_server::nshead_t req_head;
    req_head.id = 4321;
    
    do {
        memset(sendbuf, 0, sizeof(sendbuf));

        string name;
        cout << "Input your name: ";
        cin >> name;
        msg.set_name(name);

        int age;
        cout << "Input your age: ";
        cin >> age;
        msg.set_age(age);

        int num_cnt, t;
        cout << "Input how many numbers: ";
        cin >> num_cnt;
        for (int i = 0; i < num_cnt; ++i) {
            cin >> t;
            msg.add_num(t);
        }

        string data;
        msg.SerializeToString(&data);

        strcpy(sendbuf, data.c_str());

        req_head.body_len = data.size();
        cout << "req_head.body_len = " << req_head.body_len << endl;

        send(listenfd, (char*)&req_head, sizeof(req_head), 0);

        sprintf(sendbuf, "%s", data.c_str());
        cout << sendbuf << endl;
        if(send(listenfd, sendbuf, strlen(sendbuf), 0) <= 0) {
            EXIT_ERR("send");
            break;
        }
        recv(listenfd, sendbuf, sizeof(sendbuf), 0);

        bm_server::nshead_t res_head = *(bm_server::nshead_t*)sendbuf;
        bm_interface::bm_res_t res;
        res.ParseFromString(sendbuf + sizeof(res_head));
        cout << "sum = " << res.sum() << endl;
    } while(false);
    close(listenfd);
    return 0;
}
