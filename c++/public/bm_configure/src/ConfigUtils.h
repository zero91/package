#ifndef BM_CONFIGURE_CONFIG_UTILS_H
#define BM_CONFIGURE_CONFIG_UTILS_H

#include <string>
#include <vector>
#include "Constants.h"

namespace bm {
namespace configure {

class ConfigUtils {
public:
    struct DataPosition {
        std::string file_name;
        uint32_t file_line;
    };

public:
    static const DataPosition EMPTY_DATA_POS;
    static DataPosition cur_data_pos;

    // @str2str[把一个带有转义字符的字符串进行解析（把\xFF, \022, \n之类的文本转换成实际的字符）]
    //         [这个串必须是双引号开头、双引号结尾。]
    // 
    // @param [out]    buf [uint64* 转换结果]
    // @return [int32_t]  成功 [0]
    //                失败 [CONFIG_ERROR_FORMAT_ERROR]
    static int32_t str2str(const std::string& str, std::string* buf);

    // @str2int64          [把一个字符串转换成一个int64]
    // 
    // @param [in]     str [std::string类型的字符串]
    // @param [out]    buf [int64*  转换结果]
    // @return [int32_t]  成功 [0]
    //                失败 [CONFIG_ERROR_OUT_OF_RANGE] or
    //                     [CONFIG_ERROR_NULL_VALUE] or 
    //                     [CONFIG_ERROR_FORMAT_ERROR]
    static int32_t str2int64(const std::string& str, int64_t* buf);

    // @str2uint64         [把一个字符串转换成一个uint64]
    // 
    // @param [in]     str [std::string类型的字符串]
    // @param [out]    buf [uint64* 转换结果]
    // @return [int32_t]  成功 [0]
    //                失败 [CONFIG_ERROR_OUT_OF_RANGE] or
    //                     [CONFIG_ERROR_NULL_VALUE] or 
    //                     [CONFIG_ERROR_FORMAT_ERROR]
    static int32_t str2uint64(const std::string& str, uint64_t* buf);

    // @str2double         [把一个字符串转换成一个double]
    // 
    // @param [in]     str [std::string类型的字符串]
    // @param [out]    buf [double* 转换结果]
    // @return [int32_t]  成功 [0]
    //                失败 [CONFIG_ERROR_OUT_OF_RANGE] or
    //                     [CONFIG_ERROR_NULL_VALUE] or 
    //                     [CONFIG_ERROR_FORMAT_ERROR]
    static int32_t str2double(const std::string& str, double* buf);

    // @is_integer        [判断一个字符串是否是整数格式]
    // 
    // @param [in]    str [std::string类型的字符串]
    // @return [bool]  是 [true]
    //                 否 [false]
    static bool is_integer(const std::string& str);

    // @is_float          [判断一个字符串是否是浮点数格式（不检查精度范围）]
    // 
    // @param [in]    str [std::string类型的字符串]
    // @return [bool]  是 [true]
    //                 否 [false]
    static bool is_float(const std::string& str);

    // @set_error_info    [设置全局错误信息，方便查错]
    // 
    // @param [in] format [格式化字符串，与printf函数组使用方法类似]
    // @return [int32_t]      [格式化后字符串的长度]
    static int32_t set_error_info(const char* format, ...);

    // @get_error_info       [获取全局错误信息]
    //
    // @return [const char*] [错误信息字符串]
    static const char* get_error_info();

    // @line_parser       [分析单行配置，并得出key和value的值]
    // 
    // @param [in]        line [单行配置字符串]
    // @param [in]         key [存储分析所得的key值]
    // @param [in]       value [存储分析所得的value值]
    // @return [int32_t]  解析成功 [0]
    //                解析失败 [CONFIG_ERROR_FORMAT_ERROR]
    static int32_t line_parser(const std::string& line, std::string& key, std::string& value);

    // @section_parser         [分析得到section的名称]
    // 
    // @param [in]         str [待分析字符串]
    // @param [in]     section [section结果]
    // @return [int32_t]  解析成功 [0]
    //                解析失败 [CONFIG_ERROR_FORMAT_ERROR]
    static int32_t section_parser(const std::string& str, std::string& section);

    // @read_file              [按行读取文件内容]
    // 
    // @param [in]       fname [文件名称]
    // @param [in]     content [文件内容]
    // @return [int32_t]  读取成功 [0]
    //                读取失败 [CONFIG_ERROR_ERROR]
    static int32_t read_file(const std::string& fname, std::vector<std::string>& content);

private:
    static char error_buffer[4096];

}; // END class ConfigUtils

} // END namespace configure
} // END namespace bm
#endif // END BM_CONFIG_UTILS_H
