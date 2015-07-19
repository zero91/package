#include "ConfigArray.h"
#include <log.h>
#include "ConfigException.h"

namespace bm {
namespace configure {

ConfigArray::ConfigArray(const char* name, ConfigGroup* father) {
    if (*name == '@') {
        ++name;
    }
    create(name, father);
}

int32_t ConfigArray::push(const std::string& /*key*/, ConfigUnit* unit) {
    _m_attr.push_back(unit);
    return 0;
}

const ConfigUnit& ConfigArray::operator[](int32_t idx) const {
    if (idx < static_cast<int32_t>(_m_attr.size())) {
        return *_m_attr[idx];
    }
    char idx_str[65];
    snprintf(idx_str, sizeof(idx_str), "%u", idx);
    set_error_key_path(_m_name + "[" + idx_str + "]");
    return *get_err_instance();
}

const ConfigUnit& ConfigArray::operator[](const char* str) const {
    set_error_key_path(_m_name + "." + str);
    return *get_err_instance();
}

const ConfigUnit& ConfigArray::operator[](const std::string& str) const {
    set_error_key_path(_m_name + "." + str);
    return *get_err_instance();
}

ConfigUnit& ConfigArray::operator[](int32_t idx) {
    if (idx < static_cast<int32_t>(_m_attr.size())) {
        return *_m_attr[idx];
    }
    char idx_str[65];
    snprintf(idx_str, sizeof(idx_str), "%u", idx);
    set_error_key_path(_m_name + "[" + idx_str + "]");
    return *get_err_instance();
}

ConfigUnit& ConfigArray::operator[](const char* str) {
    set_error_key_path(_m_name + "." + str);
    return *get_err_instance();
}

ConfigUnit& ConfigArray::operator[](const std::string& str) {
    set_error_key_path(_m_name + "." + str);
    return *get_err_instance();
}

bool ConfigArray::equals(const ConfigUnit& unit) const {
    try {
        for (uint32_t k = 0; k < _m_attr.size(); ++k) {
            if (!_m_attr[k]->equals(unit[k])) {
                return false;
            }
        }
        return true;
    } catch (ConfigException& e) {
        BM_LOG_WARNING("Config.ConfigArray.equals triggered an exception [%s]", e.what());
        return false;
    }
    return false;
}

void ConfigArray::clear() {
    for (uint32_t k = 0; k < _m_attr.size(); ++k) {
        if (_m_attr[k] != NULL) {
            delete _m_attr[k];
        }
    }
    _m_attr.clear();
}

} // END namespace configure
} // END namespace bm
