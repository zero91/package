#ifndef _BM_CONFIGURE_H_
#define _BM_CONFIGURE_H_

#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <cstdlib>

namespace bm_utilities {

class Configure {
public:
    Configure(const std::string &dir, const std::string &fname, char _assign_symbol=':');

    explicit Configure(const std::string &path, char _assign_symbol=':');

    ~Configure() { };

    const std::string& operator[](const char *key) const;
    const std::string& operator[](const std::string &key) const;

private:
    void read_configure(const std::string &path);

    bool get_key_value(const std::string &str, std::string &key, std::string &value);

private:
    char _assign_symbol; /* 赋值符号 */

    std::map<std::string, std::string> _data_map;
};

/* ------------------------------------------------------------------------- */
inline const std::string&
Configure::operator[](const char *key) const
{
    std::map<std::string, std::string>::const_iterator it = _data_map.find(key);

    if (it == _data_map.end()) {
        std::cerr << "Key [" << key << "] not exists!" << std::endl;
        exit(1);
    }
    return it->second;
}

inline const std::string&
Configure::operator[](const std::string &key) const
{
    return operator[](key.c_str());
}

} //End namespace bm_utilities
#endif // _BM_CONFIGURE_H_
