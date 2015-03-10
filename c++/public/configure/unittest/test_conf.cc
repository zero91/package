#include <iostream>

#include "configure.h"

using namespace std;

int main(int argc, char *argv[])
{
    bm_utilities::Configure conf("../conf/test_conf.conf", ':');

    cout << conf["name"].c_str() << endl;
    cout << conf["name"] << endl;
    cout << conf["age"] << endl;
    cout << conf["height"] << endl;
    cout << conf["asdf sadf"] << endl;

    return 0;
}
