#ifndef _BM_UTILITIES_H_
#define _BM_UTILITIES_H_

#include <string>
#include <vector>

namespace bm_utilities {

// 返回去除字符串两边空白字符的字符串
std::string strip(const std::string &str);

// 返回去除字符串左边空白字符的字符串
std::string lstrip(const std::string &str);

// 返回去除字符串右边空白字符的字符串
std::string rstrip(const std::string &str);

// 与split功能相同，但返回结果以参数的形式返回
int psplit(std::vector<std::string> &res, const std::string &str,
                        const std::string &sep="", int max_split=-1);

// 返回字符串按照sep分割之后得到的子串数组
inline std::vector<std::string> split(const std::string &str,
                    const std::string &sep="", int max_split=-1) {
    std::vector<std::string> res;
    psplit(res, str, sep, max_split);
    return res;
}

} // END namespace  bm_utilities

#endif //END _BM_UTILITIES_H_
