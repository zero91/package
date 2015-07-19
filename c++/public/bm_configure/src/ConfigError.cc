#include "ConfigError.h"
#include <log.h>

namespace bm {
namespace configure {

char ConfigError::to_char(ConfigErrorNo* p_err_code) const {
    throw_error(p_err_code, CONFIG_ERROR_NO_SUCH_KEY);
    return static_cast<char>(0);
}

int16_t ConfigError::to_int16(ConfigErrorNo* p_err_code) const {
    throw_error(p_err_code, CONFIG_ERROR_NO_SUCH_KEY);
    return static_cast<int16_t>(0);
}

int32_t ConfigError::to_int32(ConfigErrorNo* p_err_code) const {
    throw_error(p_err_code, CONFIG_ERROR_NO_SUCH_KEY);
    return static_cast<int32_t>(0);
}

int64_t ConfigError::to_int64(ConfigErrorNo* p_err_code) const {
    throw_error(p_err_code, CONFIG_ERROR_NO_SUCH_KEY);
    return static_cast<int64_t>(0);
}

unsigned char ConfigError::to_uchar(ConfigErrorNo* p_err_code) const {
    throw_error(p_err_code, CONFIG_ERROR_NO_SUCH_KEY);
    return static_cast<unsigned char>(0);
}

uint16_t ConfigError::to_uint16(ConfigErrorNo* p_err_code) const {
    throw_error(p_err_code, CONFIG_ERROR_NO_SUCH_KEY);
    return static_cast<uint16_t>(0);
}

uint32_t ConfigError::to_uint32(ConfigErrorNo* p_err_code) const {
    throw_error(p_err_code, CONFIG_ERROR_NO_SUCH_KEY);
    return static_cast<uint32_t>(0);
}

uint64_t ConfigError::to_uint64(ConfigErrorNo* p_err_code) const {
    throw_error(p_err_code, CONFIG_ERROR_NO_SUCH_KEY);
    return static_cast<uint64_t>(0);
}

float ConfigError::to_float(ConfigErrorNo* p_err_code) const {
    throw_error(p_err_code, CONFIG_ERROR_NO_SUCH_KEY);
    return static_cast<float>(0);
}

double ConfigError::to_double(ConfigErrorNo* p_err_code) const {
    throw_error(p_err_code, CONFIG_ERROR_NO_SUCH_KEY);
    return static_cast<double>(0);
}

std::string ConfigError::to_string(ConfigErrorNo* p_err_code) const {
    throw_error(p_err_code, CONFIG_ERROR_NO_SUCH_KEY);
    return "";
}

std::string ConfigError::to_raw_string(ConfigErrorNo* p_err_code) const {
    throw_error(p_err_code, CONFIG_ERROR_NO_SUCH_KEY);
    return "";
}

const char* ConfigError::to_cstr(ConfigErrorNo* p_err_code) const {
    throw_error(p_err_code, CONFIG_ERROR_NO_SUCH_KEY);
    return NULL;
}

char ConfigError::to_char(ConfigErrorNo* p_err_code, const char& def) const {
    if (p_err_code != NULL) {
        *p_err_code  = CONFIG_ERROR_NO_SUCH_KEY;
    }
    return def;
}

int16_t ConfigError::to_int16(ConfigErrorNo* p_err_code, const int16_t& def) const {
    if (p_err_code != NULL) {
        *p_err_code  = CONFIG_ERROR_NO_SUCH_KEY;
    }
    return def;
}

int32_t ConfigError::to_int32(ConfigErrorNo* p_err_code, const int32_t& def) const {
    if (p_err_code != NULL) {
        *p_err_code  = CONFIG_ERROR_NO_SUCH_KEY;
    }
    return def;
}

int64_t ConfigError::to_int64(ConfigErrorNo* p_err_code, const int64_t& def) const {
    if (p_err_code != NULL) {
        *p_err_code  = CONFIG_ERROR_NO_SUCH_KEY;
    }
    return def;
}

unsigned char ConfigError::to_uchar(ConfigErrorNo* p_err_code, const unsigned char& def) const {
    if (p_err_code != NULL) {
        *p_err_code  = CONFIG_ERROR_NO_SUCH_KEY;
    }
    return def;
}

uint16_t ConfigError::to_uint16(ConfigErrorNo* p_err_code, const uint16_t& def) const {
    if (p_err_code != NULL) {
        *p_err_code  = CONFIG_ERROR_NO_SUCH_KEY;
    }
    return def;
}

uint32_t ConfigError::to_uint32(ConfigErrorNo* p_err_code, const uint32_t& def) const {
    if (p_err_code != NULL) {
        *p_err_code  = CONFIG_ERROR_NO_SUCH_KEY;
    }
    return def;
}

uint64_t ConfigError::to_uint64(ConfigErrorNo* p_err_code, const uint64_t& def) const {
    if (p_err_code != NULL) {
        *p_err_code  = CONFIG_ERROR_NO_SUCH_KEY;
    }
    return def;
}

float ConfigError::to_float(ConfigErrorNo* p_err_code, const float& def) const {
    if (p_err_code != NULL) {
        *p_err_code  = CONFIG_ERROR_NO_SUCH_KEY;
    }
    return def;
}

double ConfigError::to_double(ConfigErrorNo* p_err_code, const double& def) const {
    if (p_err_code != NULL) {
        *p_err_code  = CONFIG_ERROR_NO_SUCH_KEY;
    }
    return def;
}

std::string ConfigError::to_string(ConfigErrorNo* p_err_code, const std::string& def) const {
    if (p_err_code != NULL) {
        *p_err_code  = CONFIG_ERROR_NO_SUCH_KEY;
    }
    return def;
}

std::string ConfigError::to_raw_string(ConfigErrorNo* p_err_code, const std::string& def) const {
    if (p_err_code != NULL) {
        *p_err_code  = CONFIG_ERROR_NO_SUCH_KEY;
    }
    return def;
}

const char* ConfigError::to_cstr(ConfigErrorNo* p_err_code, const char* def) const {
    if (p_err_code != NULL) {
        *p_err_code  = CONFIG_ERROR_NO_SUCH_KEY;
    }
    return def;
}

int32_t ConfigError::get_char(char* /*value*/) const {
    return CONFIG_ERROR_NO_SUCH_KEY;
}

int32_t ConfigError::get_int16(int16_t* /*value*/) const {
    return CONFIG_ERROR_NO_SUCH_KEY;
}

int32_t ConfigError::get_int32(int32_t* /*value*/) const {
    return CONFIG_ERROR_NO_SUCH_KEY;
}

int32_t ConfigError::get_int64(int64_t* /*value*/) const {
    return CONFIG_ERROR_NO_SUCH_KEY;
}

int32_t ConfigError::get_uchar(unsigned char* /*value*/) const {
    return CONFIG_ERROR_NO_SUCH_KEY;
}

int32_t ConfigError::get_uint16(uint16_t* /*value*/) const {
    return CONFIG_ERROR_NO_SUCH_KEY;
}

int32_t ConfigError::get_uint32(uint32_t* /*value*/) const {
    return CONFIG_ERROR_NO_SUCH_KEY;
}

int32_t ConfigError::get_uint64(uint64_t* /*value*/) const {
    return CONFIG_ERROR_NO_SUCH_KEY;
}

int32_t ConfigError::get_float(float* /*value*/) const {
    return CONFIG_ERROR_NO_SUCH_KEY;
}

int32_t  ConfigError::get_double(double* /*value*/) const {
    return CONFIG_ERROR_NO_SUCH_KEY;
}

int32_t ConfigError::get_string(std::string* /*value*/) const {
    return CONFIG_ERROR_NO_SUCH_KEY;
}

int32_t ConfigError::get_raw_string(std::string* /*value*/) const {
    return CONFIG_ERROR_NO_SUCH_KEY;
}

int32_t  ConfigError::get_cstr(char* /*value*/, size_t /*len*/) const {
    return CONFIG_ERROR_NO_SUCH_KEY;
}

int32_t ConfigError::get_char(char* value, const char& def) const {
    if (value != NULL) {
        *value = def;
    }
    return CONFIG_ERROR_NO_SUCH_KEY;
}

int32_t ConfigError::get_int16(int16_t* value, const int16_t& def) const {
    if (value != NULL) {
        *value = def;
    }
    return CONFIG_ERROR_NO_SUCH_KEY;
}

int32_t ConfigError::get_int32(int32_t* value, const int32_t& def) const {
    if (value != NULL) {
        *value = def;
    }
    return CONFIG_ERROR_NO_SUCH_KEY;
}

int32_t ConfigError::get_int64(int64_t* value, const int64_t& def) const {
    if (value != NULL) {
        *value = def;
    }
    return CONFIG_ERROR_NO_SUCH_KEY;
}

int32_t ConfigError::get_uchar(unsigned char* value, const unsigned char& def) const {
    if (value != NULL) {
        *value = def;
    }
    return CONFIG_ERROR_NO_SUCH_KEY;
}

int32_t ConfigError::get_uint16(uint16_t* value, const uint16_t& def) const {
    if (value != NULL) {
        *value = def;
    }
    return CONFIG_ERROR_NO_SUCH_KEY;
}

int32_t ConfigError::get_uint32(uint32_t* value, const uint32_t& def) const {
    if (value != NULL) {
        *value = def;
    }
    return CONFIG_ERROR_NO_SUCH_KEY;
}

int32_t ConfigError::get_uint64(uint64_t* value, const uint64_t& def) const {
    if (value != NULL) {
        *value = def;
    }
    return CONFIG_ERROR_NO_SUCH_KEY;
}

int32_t ConfigError::get_float(float* value, const float& def) const {
    if (value != NULL) {
        *value = def;
    }
    return CONFIG_ERROR_NO_SUCH_KEY;
}

int32_t ConfigError::get_double(double* value, const double& def) const {
    if (value != NULL) {
        *value = def;
    }
    return CONFIG_ERROR_NO_SUCH_KEY;
}

int32_t ConfigError::get_string(std::string* value, const std::string& def) const {
    if (value != NULL) {
        *value = def;
    }
    return CONFIG_ERROR_NO_SUCH_KEY;
}

int32_t ConfigError::get_raw_string(std::string* value, const std::string& def) const {
    if (value != NULL) {
        *value = def;
    }
    return CONFIG_ERROR_NO_SUCH_KEY;
}

int32_t ConfigError::get_cstr(char* value, size_t len, const char* def) const {
    if (value != NULL) {
        snprintf(value, len, "%s", def);
    }
    return CONFIG_ERROR_NO_SUCH_KEY;
}

int32_t ConfigError::set_value(const std::string& /*value*/, int except) {
    if (except) {
        if (ConfigUtils::set_error_info("%s", "Can't ConfigError::set_value") < 0) {
            BM_LOG_WARNING("ConfigUnit: failed to write error buffer");
        }
        throw_error(NULL, CONFIG_ERROR_NO_SUCH_KEY);
    }
    return CONFIG_ERROR_NO_SUCH_KEY;
}

} // END namespace configure
} // END namespace bm
