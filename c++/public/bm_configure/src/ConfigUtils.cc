#include "ConfigUtils.h"
#include <errno.h>
#include <cstdlib>
#include <fstream>
#include <boost/algorithm/string.hpp>
#include "constants.h"

namespace bm {
namespace configure {

ConfigUtils::DataPosition ConfigUtils::cur_data_pos = ConfigUtils::DataPosition();
const ConfigUtils::DataPosition ConfigUtils::EMPTY_DATA_POS = ConfigUtils::DataPosition();

// \0	空字符(NULL)	00H/0
// \n	换行符(LF)	0AH/10
// \r	回车符(CR)	0DH/13
// \t	水平制表符(HT)	09H/9
// \v	垂直制表(VT)	0B/11
// \a	响铃(BEL) 	07/7
// \b	退格符(BS)	08H/8
// \f	换页符(FF)	0CH/12
// \'	单引号	27H/39
// \"	双引号	22H/34
// \\	反斜杠	5CH/92
// \ddd 	任意字符 	三位八进制
// \xhh 	任意字符 	二位十六进制
static int hex2int(char c) {
    if (c >= 'A' && c <= 'F') return int(c - 'A' + 10);
    if (c >= 'a' && c <= 'f') return int(c - 'a' + 10);
    if (c >= '0' && c <= '9') return int(c - '0');
    return -255;
}

int32_t ConfigUtils::str2str(const std::string& str, std::string* buf) {
    if (str.size() == 0 || str[0] != '\"') {
        *buf = str;
        return 0;
    }

    *buf = "";
    for (size_t i = 1; i < str.size(); ++i) {
        if (str[i] == '\"') {
            return 0;
        } 
        
        int32_t c = -1;
        if (str[i] == '\\') {
            ++i;
            if (i >= str.size()) {
                return CONFIG_ERROR_FORMAT_ERROR;
            }
            switch (str[i]) {
            case '0': buf->push_back('\0'); break;
            case 'n': buf->push_back('\n'); break;
            case 'r': buf->push_back('\r'); break;
            case 't': buf->push_back('\t'); break;
            case 'v': buf->push_back('\v'); break;
            case 'a': buf->push_back('\a'); break;
            case 'b': buf->push_back('\b'); break;
            case 'f': buf->push_back('\f'); break;
            case '\'': buf->push_back('\''); break;
            case '\"': buf->push_back('\"'); break;
            case '\\': buf->push_back('\\'); break;
            case 'x':
                if (i + 2 >= str.size()) {
                    return CONFIG_ERROR_FORMAT_ERROR;
                }
                c = hex2int(str[i + 1]) * 16 + hex2int(str[i + 2]);
                if (c < 0) {
                    return CONFIG_ERROR_FORMAT_ERROR;
                } else {
                    buf->push_back(char(c));
                }
                i += 2;
                break;
            default:
                if (str[i] >= '0' && str[i] < '8' && i + 2 < str.size()) {
                    c = (str[i] - '0') * 64 + (str[i + 1] - '0') * 8 + (str[i + 2] - '0');
                    if (c > 255) {
                        return CONFIG_ERROR_FORMAT_ERROR;
                    } else {
                        buf->push_back(char(c));
                    }
                } else {
                    return CONFIG_ERROR_FORMAT_ERROR;
                }
                i += 2;
                break;
            }
        } else {
            buf->push_back(str[i]);
        }
    }
    return CONFIG_ERROR_FORMAT_ERROR;
}

int32_t ConfigUtils::str2int64(const std::string& str, int64_t* buf) {
    if (str.size() == 0) {
        return CONFIG_ERROR_NULL_VALUE;
    }

    char *endptr = NULL;
    errno = 0;
    *buf = strtoll(str.c_str(), &endptr, 0);
    if (errno == ERANGE) {
        return CONFIG_ERROR_OUT_OF_RANGE;
    }

    if (*endptr != '\0') {
        return CONFIG_ERROR_FORMAT_ERROR;
    }
    return 0;
}

int32_t ConfigUtils::str2uint64(const std::string& str, uint64_t* buf) {
    if (str.size() == 0) {
        return CONFIG_ERROR_NULL_VALUE;
    }

    if (str[0] == '-') {
        return CONFIG_ERROR_FORMAT_ERROR;
    }

    char *endptr = NULL;
    errno = 0;
    *buf = strtoull(str.c_str(), &endptr, 0);
    if (errno == ERANGE) {
        return CONFIG_ERROR_OUT_OF_RANGE;
    }

    if (*endptr != '\0') {
        return CONFIG_ERROR_FORMAT_ERROR;
    }
    return 0;
}

int32_t ConfigUtils::str2double(const std::string& str, double* buf) {
    if (str.size() == 0) {
        return CONFIG_ERROR_NULL_VALUE;
    }

    char *endptr = NULL;
    errno = 0;
    *buf = strtod(str.c_str(), &endptr);
    if (errno == ERANGE) {
        return CONFIG_ERROR_OUT_OF_RANGE;
    }

    if (*endptr != '\0') {
        return CONFIG_ERROR_FORMAT_ERROR;
    }
    return 0;
}

bool ConfigUtils::is_integer(const std::string& str) {
    for (size_t i = 0; i < str.size(); ++i) {
        if (isdigit(str[i]) || (str[i] == '-' && i == 0) || (str[i] == '+' && i == 0)) {
            continue;
        } else {
            return false;
        }
    }
    return true;
}

// Float状态机，检查某个字符串是否为float
// float的组成如下，第二行为状态号: 
//   [+-][digit][.][digit][Ee][+-][digit] '\0'  ->  true false
// 0  1     2    3    4    5   6     7               -1    -2
enum {DIGIT = 0, SIG = 1, DOT = 2, E = 3, END = 4};
static int g_float_table[8][5] = {
    //digit sig dot E end
    {2,  1,  3, -2, -2}, // 0
    {2, -2,  3, -2, -2}, // 1
    {2, -2,  3,  5, -1}, // 2
    {4, -2, -2, -2, -2}, // 3
    {4, -2, -2,  5, -1}, // 4
    {7,  6, -2, -2, -2}, // 5
    {7, -2, -2, -2, -2}, // 6
    {7, -2, -2, -2, -1}  // 7
};

// 举例：以g_float_table[2]为例：
// 当前处于状态2 (digit)，如果读到数字，仍然处于状态2.如果读到正负号，出错(-2)
// 如果读到点，变为状态3，如果读到字母E，变为状态5，如果读到结尾，此时已经满足isFloat为true(-1)
static bool float_auto_machine(int status, const std::string& str, size_t next_pos) {
    if (status == -1) {
        return true;
    }
    if (status == -2) {
        return false;
    }
    char next_char = str.size() > next_pos ? str[next_pos] : '\0';
    int next;
    if (isdigit(next_char)) {
        next = DIGIT;
    } else if (next_char == '+' || next_char == '-') {
        next = SIG;
    } else if (next_char == '.') {
        next = DOT;
    } else if (next_char == 'E' || next_char == 'e') {
        next = E;
    } else if (next_char == '\0') {
        next = END;
    } else {
        return false;
    }
    return float_auto_machine(g_float_table[status][next], str, next_pos + 1);
}

bool ConfigUtils::is_float(const std::string& str) {
    return float_auto_machine(0, str, 0);
}

char ConfigUtils::error_buffer[4096] = {'\0'};

int32_t ConfigUtils::set_error_info(const char* format, ...) {
    va_list va;
    va_start(va, format);
    int ret = vsnprintf(error_buffer, sizeof(error_buffer), format, va);
    va_end(va);
    return ret;
}

const char* ConfigUtils::get_error_info() {
    return error_buffer;
}

int32_t ConfigUtils::line_parser(const std::string& line, std::string& key, std::string& value) {
    size_t colon_idx = line.find(':');
    if (colon_idx == std::string::npos) {
        return CONFIG_ERROR_FORMAT_ERROR;
    }

    std::string t_key = boost::trim_copy(line.substr(0, colon_idx));
    if (t_key.size() == 0) {
        return CONFIG_ERROR_NULL_VALUE;
    }

    for (uint32_t k = 0; k < t_key.size(); ++k) {
        if (isspace(t_key[k])) {
            return CONFIG_ERROR_FORMAT_ERROR;
        }
    }
    key = t_key;
    value = boost::trim_copy(line.substr(colon_idx + 1));
    return 0;
}

int32_t ConfigUtils::section_parser(const std::string& str, std::string& section) {
    size_t right_square_bracket_idx = str.find(']');
    if (right_square_bracket_idx == std::string::npos) {
        return CONFIG_ERROR_FORMAT_ERROR;
    }

    std::string t_section = boost::trim_copy(str.substr(1, right_square_bracket_idx - 1));
    if (t_section.size() == 0) {
        return CONFIG_ERROR_NULL_VALUE;
    }

    for (uint32_t k = 0; k < t_section.size(); ++k) {
        if (isspace(t_section[k])) {
            return CONFIG_ERROR_FORMAT_ERROR;
        }
    }
    std::string remain_str = boost::trim_copy(str.substr(right_square_bracket_idx + 1));
    if (remain_str.size() > 0 && remain_str[0] != '#') {
        return CONFIG_ERROR_FORMAT_ERROR;
    }
    section = t_section;
    return 0;
}

int32_t ConfigUtils::read_file(const std::string& fname, std::vector<std::string>& content) {
    std::ifstream ifs(fname.c_str(), std::ifstream::in);
    if (!ifs.good()) {
        return CONFIG_ERROR_ERROR;
    }
    content.clear();
    std::string line;
    while (getline(ifs, line)) {
        content.push_back(line);
    }
    ifs.close();
    return 0;
}

} // END namespace configure
} // END namespace bm
