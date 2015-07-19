#ifndef BM_CONFIGURE_CONFIG_ERROR_H
#define BM_CONFIGURE_CONFIG_ERROR_H

#include "ConfigUtils.h"
#include "ConfigUnit.h"

namespace bm {
namespace configure {

class ConfigUnit;
class ConfigError : public ConfigUnit {
public:
    virtual ~ConfigError() {}
    virtual char to_char(ConfigErrorNo* p_err_code=NULL) const;
    virtual int16_t to_int16(ConfigErrorNo* p_err_code=NULL) const;
    virtual int32_t to_int32(ConfigErrorNo* p_err_code=NULL) const;
    virtual int64_t to_int64(ConfigErrorNo* p_err_code=NULL) const;
    virtual unsigned char to_uchar(ConfigErrorNo* p_err_code=NULL) const;
    virtual uint16_t to_uint16(ConfigErrorNo* p_err_code=NULL) const;
    virtual uint32_t to_uint32(ConfigErrorNo* p_err_code=NULL) const;
    virtual uint64_t to_uint64(ConfigErrorNo* p_err_code=NULL) const;
    virtual float to_float(ConfigErrorNo* p_err_code=NULL) const;
    virtual double to_double(ConfigErrorNo* p_err_code=NULL) const;
    virtual std::string to_string(ConfigErrorNo* p_err_code=NULL) const;
    virtual std::string to_raw_string(ConfigErrorNo* p_err_code=NULL) const;
    virtual const char* to_cstr(ConfigErrorNo* p_err_code=NULL) const;

    virtual char to_char(ConfigErrorNo* p_err_code, const char& def) const;
    virtual int16_t to_int16(ConfigErrorNo* p_err_code, const int16_t& def) const;
    virtual int32_t to_int32(ConfigErrorNo* p_err_code, const int32_t& def) const;
    virtual int64_t to_int64(ConfigErrorNo* p_err_code, const int64_t& def) const;
    virtual unsigned char to_uchar(ConfigErrorNo* p_err_code, const unsigned char& def) const;
    virtual uint16_t to_uint16(ConfigErrorNo* p_err_code, const uint16_t& def) const;
    virtual uint32_t to_uint32(ConfigErrorNo* p_err_code, const uint32_t& def) const;
    virtual uint64_t to_uint64(ConfigErrorNo* p_err_code, const uint64_t& def) const;
    virtual float to_float(ConfigErrorNo* p_err_code, const float& def) const;
    virtual double to_double(ConfigErrorNo* p_err_code, const double& def) const;
    virtual std::string to_string(ConfigErrorNo* p_err_code, const std::string& def) const;
    virtual std::string to_raw_string(ConfigErrorNo* p_err_code, const std::string& def) const;
    virtual const char* to_cstr(ConfigErrorNo* p_err_code, const char* def) const;

    virtual int32_t get_char(char* value) const;
    virtual int32_t get_int16(int16_t* value) const;
    virtual int32_t get_int32(int32_t* value) const;
    virtual int32_t get_int64(int64_t* value) const;
    virtual int32_t get_uchar(unsigned char* value) const;
    virtual int32_t get_uint16(uint16_t* value) const;
    virtual int32_t get_uint32(uint32_t* value) const;
    virtual int32_t get_uint64(uint64_t* value) const;
    virtual int32_t get_float(float* value) const;
    virtual int32_t get_double(double* value) const;
    virtual int32_t get_string(std::string* value) const;
    virtual int32_t get_raw_string(std::string* value) const;
    virtual int32_t get_cstr(char* value, size_t len) const;

    virtual int32_t get_char(char* value, const char& def) const;
    virtual int32_t get_int16(int16_t* value, const int16_t& def) const;
    virtual int32_t get_int32(int32_t* value, const int32_t& def) const;
    virtual int32_t get_int64(int64_t* value, const int64_t& def) const;
    virtual int32_t get_uchar(unsigned char* value, const unsigned char& def) const;
    virtual int32_t get_uint16(uint16_t* value, const uint16_t& def) const;
    virtual int32_t get_uint32(uint32_t* value, const uint32_t& def) const;
    virtual int32_t get_uint64(uint64_t* value, const uint64_t& def) const;
    virtual int32_t get_float(float* value, const float& def) const;
    virtual int32_t get_double(double* value, const double& def) const;
    virtual int32_t get_string(std::string* value, const std::string& def) const;
    virtual int32_t get_raw_string(std::string* value, const std::string& def) const;
    virtual int32_t get_cstr(char* value, size_t len, const char* def) const;

    virtual int32_t self_type() const { return CONFIG_TYPE_ERROR; }
    virtual uint32_t size() const { return 0; } 

    virtual bool equals(const ConfigUnit& unit) const {
        return unit.self_type() == CONFIG_TYPE_ERROR;
    }

    virtual int32_t set_value(const std::string& value, int except=0);
    virtual ConfigUnit& operator=(ConfigUnit& unit) { return ConfigUnit::operator=(unit); }
};

} // END namespace configure
} // END namespace bm
#endif // BM_CONFIG_ERROR_H
