#include <iostream>
#include <string>

#include "bm_utilities.h"

using namespace std;
using namespace bm_utilities;

void
test_strip(const string &str)
{
    cout << "strip[" << str << "] = [" << strip(str) << "]" << endl;
    cout << "lstrip[" << str << "] = [" << lstrip(str) << "]" << endl;
    cout << "rstrip[" << str << "] = [" << rstrip(str) << "]" << endl;
}

void
test_split(const string &str, const string &sep="", int max_split = -1)
{
    vector<string> res = split(str, sep, max_split);
    cout << "------------------ test_split ------------------" << endl;
    for (size_t i = 0; i < res.size(); ++i) {
        cout << "[" << res[i] << "]" << endl;
    }
    cout << endl;
}

int main(int argc, char *argv[])
{
    string str;
    string sep;
    int max_split;
    //while (getline(cin, str)) {
    while (true) {
        cout << "Input string: \n"; getline(cin, str);
        cout << "Input sep: "; getline(cin, sep);
        cout << "input max_split: "; cin >> max_split;

        cout << "str = [" << str << "] sep = [" << sep << "] max_split = [" << max_split << "]" << endl;
        test_split(str, sep, max_split);

        getline(cin, str); //read "\n"
        //test_strip(str);
    }
    return 0;
}
