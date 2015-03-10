#include "configure.h"
#include "bm_utilities.h"

namespace bm_utilities {

Configure::Configure(const std::string &dir, const std::string &fname, char assign_symbol) : _assign_symbol(assign_symbol)
{
    read_configure(dir + "/" + fname);
}

Configure::Configure(const std::string &path, char assign_symbol) : _assign_symbol(assign_symbol)
{
    read_configure(path);
}

void
Configure::read_configure(const std::string &path)
{
    std::ifstream ifs(path.c_str(), std::ifstream::in);

    if (!ifs.is_open()) {
        std::cerr << "Opening file [" << path << "] failed!" << std::endl;
        exit(1);
    }

    bool succeed = false;
    std::string line, key, value;

    while (getline(ifs, line)) {
        succeed = get_key_value(line, key, value);
        if (!succeed) continue;
        _data_map[key] = value;
    }
    ifs.close();
}

bool
Configure::get_key_value(const std::string &line, std::string &key, std::string &value)
{
    std::string str = bm_utilities::strip(line);
    if (line == "" || line[0] == '#') {
        return false;;
    }

    size_t assign_pos = str.find(_assign_symbol);

    if (assign_pos == std::string::npos || assign_pos != str.rfind(_assign_symbol)) {
        std::cerr << "Wrong data format, ignore this line [" << str << "]!" << std::endl;
        return false;
    }
    key = bm_utilities::strip(str.substr(0, assign_pos));
    value = bm_utilities::strip(str.substr(assign_pos + 1));
    return true;
}

} // END namespace bm_utilities
