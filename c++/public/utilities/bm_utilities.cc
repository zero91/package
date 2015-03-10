#include "bm_utilities.h"

#include <cctype>

namespace bm_utilities {

std::string
strip(const std::string &str) {
    size_t beg = 0, end = str.size();

    while (beg < end && isspace(str[beg])) {
        ++beg;
    }
    while (end > beg && isspace(str[end - 1])) {
        --end;
    }
    return str.substr(beg, end - beg);
}

std::string
lstrip(const std::string &str) {
    size_t beg = 0;
    while (beg < str.size() && isspace(str[beg])) {
        ++beg;
    }
    return str.substr(beg);
}

std::string
rstrip(const std::string &str) {
    size_t size = str.size();
    while (size > 0 && isspace(str[size - 1])) {
        --size;
    }
    return str.substr(0, size);
}

int
psplit(std::vector<std::string> &res, const std::string &str, const std::string &sep, int max_split) {
    res.clear();

    if (sep == "") { // 按空白字符分割
        size_t beg, end;

        beg = end = 0;
        while (true) {
            while (beg < str.size() && isspace(str[beg])) {
                ++beg;
            }
            if (beg == str.size()) {
                break;
            }

            end = beg + 1;
            while (end < str.size() && !isspace(str[end])) {
                ++end;
            }
            res.push_back(str.substr(beg, end - beg));

            if (end == str.size()) {
                break;
            }
            beg = end;
        }
        return res.size();
    }

    if (max_split == -1) {
        max_split = str.size();
    }

    size_t pos = 0, find_pos;
    while (max_split-- > 0) {
        find_pos = str.find(sep, pos);
        if (find_pos == std::string::npos) {
            break;
        }
        res.push_back(str.substr(pos, find_pos - pos));
        pos = find_pos + sep.size();
    }
    res.push_back(str.substr(pos));
    return res.size();
}

}; // END namespace bm_utilities

