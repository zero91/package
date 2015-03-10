#include <iostream>
#include <string>

#include "server.h"
#include "configure.h"

using namespace std;

using namespace bm_server;

int callback(const nshead_t &req_nshead, const bm_interface::bm_req_t &req, nshead_t &res_nshead, bm_interface::bm_res_t &res)
{
    cout << "______________________________________" << endl;
    cout << "Execute callback function" << endl;

    cout << "req_nshead.id = " << req_nshead.id << endl;
    cout << "req_nshead.body_len = " << req_nshead.body_len << endl;

    cout << "req.name = " << req.name() << endl;
    cout << "req.age = " << req.age() << endl;
    cout << "req.num_size() = " << req.num_size() << endl;

    /*
    message bm_req_t {
        required string name = 1;
        required int32 age = 2;
        repeated int32 num = 3;
    }

    message bm_res_t {
        required int32 sum = 1;
    }
    */

    int sum = 0;
    for (uint32_t i = 0; i < req.num_size(); ++i) {
        cout << "req.num(" << i << ") = " << req.num(i) << endl;
        sum += req.num(i);
    }
    res.set_sum(sum);
    cout << "--------------------------------------------------" << endl;
    return 0;
}

int main(int argc, char *argv[])
{
    bm_utilities::Configure conf("../conf/server.conf");

    bm_server::server svr(conf);
    svr.run(callback);

    return 0;
}
