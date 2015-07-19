#include <iostream>
#include <string>
#include <vector>

#include "ConfigUtils.h"
#include "ConfigException.h"
#include "constants.h"
#include "Configure.h"

using namespace std;

void test_ConfigUtils() {
    uint64_t val_1;
    bm::configure::ConfigUtils::str2uint64("3123143", &val_1);
    cout << val_1 + 1 << endl;

    int64_t val_2;
    bm::configure::ConfigUtils::str2int64("12345", &val_2);
    cout << val_2 + 1 << endl;

    cout << bm::configure::ConfigUtils::is_float("1242.342") << endl;
    cout << bm::configure::ConfigUtils::is_float("124.2.342") << endl;
    cout << bm::configure::ConfigUtils::is_float("1242.342e1.2") << endl;
    cout << bm::configure::ConfigUtils::is_float("1242.342e12") << endl;
    cout << bm::configure::ConfigUtils::is_float("+1242.342e12") << endl;
    cout << bm::configure::ConfigUtils::is_float("-1242.342e12") << endl;

    bm::configure::ConfigUtils::set_error_info("[ERROR] Hello %s", "zhangjian");
    cout << bm::configure::ConfigUtils::get_error_info() << endl;


    string conf = " user_name : zhangjian09   ";
    string key;
    string value;

    int ret = bm::configure::ConfigUtils::line_parser(conf, key, value);
    cout << "ret = [" << ret << "]" << endl;
    cout << "key = [" << key << "]" << endl;
    cout << "value = [" << value << "]" << endl;

    string section_conf = "[proc_test]   #ahha";
    string section;
    ret = bm::configure::ConfigUtils::section_parser(section_conf, section);
    cout << "ret = [" << ret << "]" << endl;
    cout << "section = [" << section << "]" << endl;

    vector<string> content;
    bm::configure::ConfigUtils::read_file("test.cc", content);   
    for (vector<string>::iterator it = content.begin(); it != content.end(); ++it) {
        //cout << *it << "$" << endl;
    }
    cout << bm::configure::CONFIG_GLOBAL << endl;
    cout << bm::configure::CONFIG_INCLUDE << endl;

}

void test_ConfigException() {
    try {
        bm::configure::ConfigThrower::throwException(bm::configure::CONFIG_ERROR_OK);
    } catch (bm::configure::ConfigException &e) {
        cout << e.what() << endl;
    }

    try {
        bm::configure::ConfigThrower::throwException(bm::configure::CONFIG_ERROR_NO_SUCH_KEY);
    } catch (bm::configure::ConfigException &e) {
        cout << e.what() << endl;
    }

    try {
        bm::configure::ConfigThrower::throwException(bm::configure::CONFIG_ERROR_OUT_OF_RANGE);
    } catch (bm::configure::ConfigException &e) {
        cout << e.what() << endl;
    }

    try {
        bm::configure::ConfigThrower::throwException(bm::configure::CONFIG_ERROR_FORMAT_ERROR);
    } catch (bm::configure::ConfigException &e) {
        cout << e.what() << endl;
    }

    try {
        bm::configure::ConfigThrower::throwException(bm::configure::CONFIG_ERROR_NULL_VALUE);
    } catch (bm::configure::ConfigException &e) {
        cout << e.what() << endl;
    }

    try {
        bm::configure::ConfigThrower::throwException(bm::configure::CONFIG_ERROR_NULL_BUFFER);
    } catch (bm::configure::ConfigException &e) {
        cout << e.what() << endl;
    }

    try {
        bm::configure::ConfigThrower::throwException(bm::configure::CONFIG_ERROR_GROUP_TO_UNIT);
    } catch (bm::configure::ConfigException &e) {
        cout << e.what() << endl;
    }

    try {
        bm::configure::ConfigThrower::throwException(bm::configure::CONFIG_ERROR_UNKNOWN);
    } catch (bm::configure::ConfigException &e) {
        cout << e.what() << endl;
    }
}

void test_Configure() {
    bm::configure::Configure config;
    config.load("../conf", "test.conf", bm::configure::GROUP_DUP_LEVEL_3);

    cout << "name = " << config["name"].to_string() << endl;
    cout << "age = " << config["age"].to_uint32() << endl;
    cout << "family.size() = " << config["info"]["family"].size() << endl;
    for (uint32_t k = 0; k < config["info"]["family"].size(); ++k) {
        cout << config["info"]["family"][k].to_string() << endl;
    }

    cout << config["info"]["money"].size() << endl;
    cout << config["info"]["money"]["tangzhoulin"].to_double() << endl;
    cout << config["info"]["money"]["zhanganyuan"].to_double() << endl;

    cout << "include.file_name = [" << config["file_name"].to_string() << "]" << endl;

    cout << "province.size = " << config["china"]["province"].size() << endl;
    for (uint32_t k = 0; k < config["china"]["province"].size(); ++k) {
        cout << "province[" << k << "] = " << config["china"]["province"][k].to_string() << endl;
    }
    cout << config["book"]["business"].to_string() << endl;

    try {
        cout << config["info"]["money"]["not_exist"].to_string() << endl;
    } catch (bm::configure::ConfigException &e) {
        cout << "Exception: [" << e.what() << "]" << endl;
    }
    cout << config["info"]["money"]["china"]["haha"].to_string() << endl;
    cout << config["info"]["remain"].to_string() << endl;
}

int main() {
    //test_ConfigUtils();
    //test_ConfigException();
    
    test_Configure();

    return 0;
}
