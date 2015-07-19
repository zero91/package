#include <log.h>

#include "ConfigUnit.h"
#include "ConfigUtils.h"
#include "ConfigError.h"
#include "ConfigGroup.h"
#include "ConfigException.h"

namespace bm {
namespace configure {

ConfigUnit::ConfigUnit(const std::string& key, const std::string& value,
                        ConfigGroup *father, ConfigUtils::DataPosition data_pos) :
                                _m_key(key), _m_value(value), _m_cstr(value), _m_cstr_err(0),
                                _m_father(father), _m_data_pos(data_pos) {
    for (uint32_t k = 0; k < key.size(); ++k) {
        if (k == 0 && key[k] == '@') {
            continue;
        }
        if (!isalpha(key[k]) && !isdigit(key[k]) && key[k] != '_') {
            BM_LOG_WARNING("Unsupport key format [%s]", key.c_str());
            throw ConfigException();
        }
    }

    if (_m_value.size() > 0 && _m_value[0] == '"') {
        _m_cstr_err = ConfigUtils::str2str(_m_value, &_m_cstr);
        if (_m_cstr_err != CONFIG_ERROR_OK) {
            BM_LOG_WARNING("Config Error format: key[%s], value[%s]",
                            _m_key.c_str(), _m_value.c_str());
        }
    }
}

char ConfigUnit::to_char(ConfigErrorNo* p_err_code) const {
    char ret;
    throw_error(p_err_code, get_char(&ret));
    return ret;
}

int16_t ConfigUnit::to_int16(ConfigErrorNo* p_err_code) const {
    int16_t ret;
    throw_error(p_err_code, get_int16(&ret));
    return ret;
}

int32_t ConfigUnit::to_int32(ConfigErrorNo* p_err_code) const {
    int32_t ret;
    throw_error(p_err_code, get_int32(&ret));
    return ret;
}

int64_t ConfigUnit::to_int64(ConfigErrorNo* p_err_code) const {
    int64_t ret;
    throw_error(p_err_code, get_int64(&ret));
    return ret;
}

unsigned char ConfigUnit::to_uchar(ConfigErrorNo* p_err_code) const {
    unsigned char ret;
    throw_error(p_err_code, get_uchar(&ret));
    return ret;
}

uint16_t ConfigUnit::to_uint16(ConfigErrorNo* p_err_code) const {
    uint16_t ret;
    throw_error(p_err_code, get_uint16(&ret));
    return ret;
}

uint32_t ConfigUnit::to_uint32(ConfigErrorNo* p_err_code) const {
    uint32_t ret;
    throw_error(p_err_code, get_uint32(&ret));
    return ret;
}

uint64_t ConfigUnit::to_uint64(ConfigErrorNo* p_err_code) const {
    uint64_t ret;
    throw_error(p_err_code, get_uint64(&ret));
    return ret;
}

float ConfigUnit::to_float(ConfigErrorNo* p_err_code) const {
    float ret;
    throw_error(p_err_code, get_float(&ret));
    return ret;
}

double ConfigUnit::to_double(ConfigErrorNo* p_err_code) const {
    double ret;
    throw_error(p_err_code, get_double(&ret));
    return ret;
}

std::string ConfigUnit::to_string(ConfigErrorNo* p_err_code) const {
    std::string ret;
    throw_error(p_err_code, get_string(&ret));
    return ret;
}

std::string ConfigUnit::to_raw_string(ConfigErrorNo* p_err_code) const {
    std::string ret;
    throw_error(p_err_code, get_raw_string(&ret));
    return ret;
}

const char* ConfigUnit::to_cstr(ConfigErrorNo* p_err_code) const {
    if (_m_cstr_err != CONFIG_ERROR_OK) {
        set_error_key_path(_m_key);
        throw_error(p_err_code, _m_cstr_err);
    }
    return _m_cstr.c_str();
}

char ConfigUnit::to_char(ConfigErrorNo* p_err_code, const char& def) const {
    char ret;
    *p_err_code = static_cast<ConfigErrorNo>(get_char(&ret, def));
    return ret;
}

int16_t ConfigUnit::to_int16(ConfigErrorNo* p_err_code, const int16_t& def) const {
    int16_t ret;
    *p_err_code = static_cast<ConfigErrorNo>(get_int16(&ret, def));
    return ret;
}

int32_t ConfigUnit::to_int32(ConfigErrorNo* p_err_code, const int32_t& def) const {
    int32_t ret;
    *p_err_code = static_cast<ConfigErrorNo>(get_int32(&ret, def));
    return ret;
}

int64_t ConfigUnit::to_int64(ConfigErrorNo* p_err_code, const int64_t& def) const {
    int64_t ret;
    *p_err_code = static_cast<ConfigErrorNo>(get_int64(&ret, def));
    return ret;
}

unsigned char ConfigUnit::to_uchar(ConfigErrorNo* p_err_code, const unsigned char& def) const {
    unsigned char ret;
    *p_err_code = static_cast<ConfigErrorNo>(get_uchar(&ret, def));
    return ret;
}

uint16_t ConfigUnit::to_uint16(ConfigErrorNo* p_err_code, const uint16_t& def) const {
    uint16_t ret;
    *p_err_code = static_cast<ConfigErrorNo>(get_uint16(&ret, def));
    return ret;
}

uint32_t ConfigUnit::to_uint32(ConfigErrorNo* p_err_code, const uint32_t& def) const {
    uint32_t ret;
    *p_err_code = static_cast<ConfigErrorNo>(get_uint32(&ret, def));
    return ret;
}

uint64_t ConfigUnit::to_uint64(ConfigErrorNo* p_err_code, const uint64_t& def) const {
    uint64_t ret;
    *p_err_code = static_cast<ConfigErrorNo>(get_uint64(&ret, def));
    return ret;
}

float ConfigUnit::to_float(ConfigErrorNo* p_err_code, const float& def) const {
    float ret;
    *p_err_code = static_cast<ConfigErrorNo>(get_float(&ret, def));
    return ret;
}

double ConfigUnit::to_double(ConfigErrorNo* p_err_code, const double& def) const {
    double ret;
    *p_err_code = static_cast<ConfigErrorNo>(get_double(&ret, def));
    return ret;
}

std::string ConfigUnit::to_string(ConfigErrorNo* p_err_code, const std::string& def) const {
    std::string ret;
    *p_err_code = static_cast<ConfigErrorNo>(get_string(&ret, def));
    return ret;
}

std::string ConfigUnit::to_raw_string(ConfigErrorNo* p_err_code, const std::string& def) const {
    std::string ret;
    *p_err_code = static_cast<ConfigErrorNo>(get_raw_string(&ret, def));
    return ret;
}

const char* ConfigUnit::to_cstr(ConfigErrorNo* p_err_code, const char* def) const {
    if (_m_cstr_err != CONFIG_ERROR_OK) {
        set_error_key_path(_m_key);
        throw_error(p_err_code, _m_cstr_err);
        return def;
    }
    return _m_cstr.c_str();
}

int32_t ConfigUnit::get_char(char* value) const {
    if (value == NULL) {
        set_error_key_path(_m_key);
        return CONFIG_ERROR_NULL_BUFFER;
    }

    if (_m_cstr_err != CONFIG_ERROR_OK) {
        set_error_key_path(_m_key);
        return _m_cstr_err;
    }

    if (_m_cstr.size() > 0) {
        *value = static_cast<char>(_m_cstr[0]);
    } else {
        set_error_key_path(_m_key);
        return CONFIG_ERROR_NULL_VALUE;
    }
    return CONFIG_ERROR_OK;
}

int32_t ConfigUnit::get_int16(int16_t* value) const {
    if (value == NULL) {
        set_error_key_path(_m_key);
        return CONFIG_ERROR_NULL_BUFFER;
    }

    int64_t val;
    int32_t ret = ConfigUtils::str2int64(_m_value, &val);
    if (ret != CONFIG_ERROR_OK) {
        set_error_key_path(_m_key);
        return ret;
    }

    if (val > static_cast<int64_t>(0x7fff) || val < -static_cast<int64_t>(0x8000) ) {
        set_error_key_path(_m_key);
        return CONFIG_ERROR_OUT_OF_RANGE;
    }
    *value = static_cast<int16_t>(val);
    return CONFIG_ERROR_OK;
}

int32_t ConfigUnit::get_int32(int32_t* value) const {
    if (value == NULL) {
        set_error_key_path(_m_key);
        return CONFIG_ERROR_NULL_BUFFER;
    }

    int64_t val;
    int32_t ret = ConfigUtils::str2int64(_m_value, &val);
    if (ret != CONFIG_ERROR_OK) {
        set_error_key_path(_m_key);
        return ret;
    }
    if (val > static_cast<int64_t>(0x7fffffff) || val < -static_cast<int64_t>(0x80000000)) {
        set_error_key_path(_m_key);
        return CONFIG_ERROR_OUT_OF_RANGE;
    }
    *value = static_cast<int32_t>(val);
    return CONFIG_ERROR_OK;
}

int32_t ConfigUnit::get_int64(int64_t* value) const {
    if (value == NULL) {
        set_error_key_path(_m_key);
        return CONFIG_ERROR_NULL_BUFFER;
    }

    int64_t val;
    int32_t ret = ConfigUtils::str2int64(_m_value, &val);
    if (ret != CONFIG_ERROR_OK) {
        set_error_key_path(_m_key);
        return ret;
    }
    *value = val;
    return CONFIG_ERROR_OK;
}

int32_t ConfigUnit::get_uchar(unsigned char* value) const {
    if (value == NULL) {
        set_error_key_path(_m_key);
        return CONFIG_ERROR_NULL_BUFFER;
    }

    if (_m_cstr_err != CONFIG_ERROR_OK) {
        set_error_key_path(_m_key);
        return _m_cstr_err;
    }

    if (_m_cstr.size() > 0) {
        *value = static_cast<unsigned char>(_m_cstr[0]);
    } else {
        set_error_key_path(_m_key);
        return CONFIG_ERROR_NULL_VALUE;
    }
    return CONFIG_ERROR_OK;
}

int32_t ConfigUnit::get_uint16(uint16_t* value) const {
    if (value == NULL) {
        set_error_key_path(_m_key);
        return CONFIG_ERROR_NULL_BUFFER;
    }
    uint64_t val;
    int32_t ret = ConfigUtils::str2uint64(_m_value, &val);
    if (ret != CONFIG_ERROR_OK) {
        set_error_key_path(_m_key);
        return ret;
    }

    if (val > static_cast<uint64_t>(0xffff)) {
        set_error_key_path(_m_key);
        return CONFIG_ERROR_OUT_OF_RANGE;
    }
    *value = static_cast<uint16_t>(val);
    return CONFIG_ERROR_OK;
}

int32_t ConfigUnit::get_uint32(uint32_t* value) const {
    if (value == NULL) {
        set_error_key_path(_m_key);
        return CONFIG_ERROR_NULL_BUFFER;
    }

    uint64_t val;
    int32_t ret = ConfigUtils::str2uint64(_m_value, &val);
    if (ret != CONFIG_ERROR_OK) {
        set_error_key_path(_m_key);
        return ret;
    }

    if (val > static_cast<uint64_t>(0xffffffff)) {
        set_error_key_path(_m_key);
        return CONFIG_ERROR_OUT_OF_RANGE;
    }
    *value = static_cast<uint32_t>(val);
    return CONFIG_ERROR_OK;
}

int32_t ConfigUnit::get_uint64(uint64_t* value) const {
    if (value == NULL) {
        set_error_key_path(_m_key);
        return CONFIG_ERROR_NULL_BUFFER;
    }

    uint64_t val;
    int32_t ret = ConfigUtils::str2uint64(_m_value, &val);
    if (ret != CONFIG_ERROR_OK) {
        set_error_key_path(_m_key);
        return ret;
    }
    *value = val;
    return CONFIG_ERROR_OK;
}

int32_t ConfigUnit::get_float(float* value) const {
    if (value == NULL) {
        set_error_key_path(_m_key);
        return CONFIG_ERROR_NULL_BUFFER;
    }

    double buf = 0.0;
    int32_t ret = ConfigUtils::str2double(_m_value, &buf);
    if (ret != CONFIG_ERROR_OK) {
        set_error_key_path(_m_key);
        return ret;
    }
    *value = static_cast<float>(buf);
    return ret;
}

int32_t ConfigUnit::get_double(double* value) const {
    if (value == NULL) {
        set_error_key_path(_m_key);
        return CONFIG_ERROR_NULL_BUFFER;
    }

    int32_t ret = ConfigUtils::str2double(_m_value, value);
    if (ret != CONFIG_ERROR_OK) {
        set_error_key_path(_m_key);
    }
    return ret;
}

int32_t ConfigUnit::get_string(std::string* value) const {
    if (value == NULL) {
        set_error_key_path(_m_key);
        return CONFIG_ERROR_NULL_BUFFER;
    }

    if (_m_cstr_err != CONFIG_ERROR_OK) {
        set_error_key_path(_m_key);
        return _m_cstr_err;
    }
    *value = _m_cstr;
    return 0;
}

int32_t ConfigUnit::get_raw_string(std::string* value) const {
    if (value == NULL) {
        set_error_key_path(_m_key);
        return CONFIG_ERROR_NULL_BUFFER;
    }
    *value = _m_value;
    return CONFIG_ERROR_OK;
}

int32_t ConfigUnit::get_cstr(char* value, size_t len) const {
    if (value == NULL) {
        set_error_key_path(_m_key);
        return CONFIG_ERROR_NULL_BUFFER;
    }

    if (_m_cstr_err != CONFIG_ERROR_OK) {
        set_error_key_path(_m_key);
        return _m_cstr_err;
    }
    snprintf(value, len, "%s", _m_cstr.c_str());
    return CONFIG_ERROR_OK;
}

int32_t ConfigUnit::get_char(char* value, const char& def) const {
    int32_t ret = get_char(value);
    if (ret != CONFIG_ERROR_OK) {
        *value = def;
    }
    return ret;
}

int32_t ConfigUnit::get_int16(int16_t* value, const int16_t& def) const {
    int32_t ret = get_int16(value);
    if (ret != CONFIG_ERROR_OK) {
        *value = def;
    }
    return ret;
}

int32_t ConfigUnit::get_int32(int32_t* value, const int32_t& def) const {
    int32_t ret = get_int32(value);
    if (ret != CONFIG_ERROR_OK) {
        *value = def;
    }
    return ret;
}

int32_t ConfigUnit::get_int64(int64_t* value, const int64_t& def) const {
    int32_t ret = get_int64(value);
    if (ret != CONFIG_ERROR_OK) {
        *value = def;
    }
    return ret;
}

int32_t ConfigUnit::get_uchar(unsigned char* value, const unsigned char& def) const {
    int32_t ret = get_uchar(value);
    if (ret != CONFIG_ERROR_OK) {
        *value = def;
    }
    return ret;
}

int32_t ConfigUnit::get_uint16(uint16_t* value, const uint16_t& def) const {
    int32_t ret = get_uint16(value);
    if (ret != CONFIG_ERROR_OK) {
        *value = def;
    }
    return ret;
}

int32_t ConfigUnit::get_uint32(uint32_t* value, const uint32_t& def) const {
    int32_t ret = get_uint32(value);
    if (ret != CONFIG_ERROR_OK) {
        *value = def;
    }
    return ret;
}

int32_t ConfigUnit::get_uint64(uint64_t* value, const uint64_t& def) const {
    int32_t ret = get_uint64(value);
    if (ret != CONFIG_ERROR_OK) {
        *value = def;
    }
    return ret;
}

int32_t ConfigUnit::get_float(float* value, const float& def) const {
    int32_t ret = get_float(value);
    if (ret != CONFIG_ERROR_OK) {
        *value = def;
    }
    return ret;
}

int32_t ConfigUnit::get_double(double* value, const double& def) const {
    int32_t ret = get_double(value);
    if (ret != CONFIG_ERROR_OK) {
        *value = def;
    }
    return ret;
}

int32_t ConfigUnit::get_string(std::string* value, const std::string& def) const {
    int32_t ret = get_string(value);
    if (ret != CONFIG_ERROR_OK) {
        *value = def;
    }
    return ret;
}

int32_t ConfigUnit::get_raw_string(std::string* value, const std::string& def) const {
    int32_t ret = get_raw_string(value);
    if (ret != CONFIG_ERROR_OK) {
        *value = def;
    }
    return ret;
}

int32_t ConfigUnit::get_cstr(char* value, size_t len, const char* def) const {
    int32_t ret = get_cstr(value, len);
    if (ret != CONFIG_ERROR_OK) {
        snprintf(value, len, "%s", def);
    }
    return ret;
}

int32_t ConfigUnit::set_value(const std::string& value, int except) {
    _m_value = value;
    _m_cstr = _m_value;
    if (_m_value.size() > 0 && _m_value[0] == '"') {
        _m_cstr_err = ConfigUtils::str2str(_m_value, &_m_cstr);
        if (_m_cstr_err != CONFIG_ERROR_OK) {
            BM_LOG_WARNING("Config Error format: key[%s],value[%s]",
                                    _m_key.c_str(), _m_value.c_str());
            if (except) {
                throw_error(NULL, _m_cstr_err);
            }
            return _m_cstr_err;
        }
    }
    return CONFIG_ERROR_OK;
}

void ConfigUnit::set_error_key_path(const std::string &str) const {
    std::string err_key_path(str);
    for (const ConfigGroup* p = _m_father; p != NULL; p = p->_m_father) {
        if (p->get_key() == CONFIG_GLOBAL) {
            break;
        }
        err_key_path = p->get_key() + "." + err_key_path;
    }

    if (ConfigUtils::set_error_info("%s", err_key_path.c_str()) < 0) {
        BM_LOG_WARNING("Configure: failed to write Error key path to error buffer");
    }
}

ConfigUnit* ConfigUnit::get_err_instance() {
    static ConfigUnit* instance = NULL;
    if (instance == NULL) {
        instance = new(std::nothrow) ConfigError();
        if (instance == NULL) {
            BM_LOG_WARNING("Allocate memory for ConfigError failed!");
        }
    }
    return instance;
}

const char* ConfigUnit::error_info(const ConfigErrorNo &err) const {
    switch (err) {
    case CONFIG_ERROR_OK:
        return "OK";
    case CONFIG_ERROR_NO_SUCH_KEY:
        return "No such key";
    case CONFIG_ERROR_DUPLICATED_KEY:
        return "Duplicated key";
    case CONFIG_ERROR_OUT_OF_RANGE:
        return "Out of range";
    case CONFIG_ERROR_GROUP_TO_UNIT:
        return "Use group as key";
    case CONFIG_ERROR_FORMAT_ERROR:
        return "Format error";
    case CONFIG_ERROR_CONSTRAINT_ERROR:
        return "Constraint error";
    case CONFIG_ERROR_CONFIG_ERROR:
        return "Config error";
    case CONFIG_ERROR_NULL_VALUE:
        return "Value is null";
    case CONFIG_ERROR_NULL_BUFFER:
        return "Given buffer is null";
    case CONFIG_ERROR_ERROR:
        return "Something wrong";
    case CONFIG_ERROR_UNKNOWN:
        // pass to default
    default:
        return "Unknown error";
    }
    return "ERROR";
}

void ConfigUnit::throw_error(ConfigErrorNo* p_err_code, const int32_t& err_code) const {
    if (p_err_code != NULL) {
        *p_err_code  = static_cast<ConfigErrorNo>(err_code);
    } else {
        ConfigThrower::throwException(static_cast<ConfigErrorNo>(err_code));
    }
}

}; // END namespace configure
}; //END namespace bm
