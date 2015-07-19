#ifndef BM_CONFIGURE_CONFIG_UNIT_H
#define BM_CONFIGURE_CONFIG_UNIT_H

#include <vector>
#include <string>
#include "Constants.h"
#include "ConfigUtils.h"

namespace bm {
namespace configure {

class ConfigGroup;
class ConfigUnit {
public:
    ConfigUnit() : _m_father(NULL) {}
    ConfigUnit(const std::string& key, const std::string& value,
        ConfigGroup *father=NULL, ConfigUtils::DataPosition data_pos=ConfigUtils::EMPTY_DATA_POS);

    virtual ~ConfigUnit() {}

    virtual const ConfigUnit& operator[](const char* /*str*/) const { return *get_err_instance(); }
    virtual const ConfigUnit& operator[](const std::string& /*str*/) const {
        return *get_err_instance();
    }
    virtual const ConfigUnit& operator[](int32_t /*idx*/) const { return *get_err_instance(); }

    virtual ConfigUnit& operator[](const char* /*str*/) { return *get_err_instance(); }
    virtual ConfigUnit& operator[](const std::string& /*str*/) { return *get_err_instance(); }
    virtual ConfigUnit& operator[](int32_t /*idx*/) { return *get_err_instance(); }

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
    virtual int32_t get_int64(int64_t* value, const int64_t &def) const;
    virtual int32_t get_uchar(unsigned char* value, const unsigned char& def) const;
    virtual int32_t get_uint16(uint16_t* value, const uint16_t& def) const;
    virtual int32_t get_uint32(uint32_t* value, const uint32_t& def) const;
    virtual int32_t get_uint64(uint64_t* value, const uint64_t& def) const;
    virtual int32_t get_float(float* value, const float& def) const;
    virtual int32_t get_double(double* value, const double& def) const;
    virtual int32_t get_string(std::string* value, const std::string& def) const;
    virtual int32_t get_raw_string(std::string* value, const std::string& def) const;
    virtual int32_t get_cstr(char* value, size_t len, const char* def) const;

    virtual bool equals(const ConfigUnit& unit) const { return unit.to_string() == to_string(); }
    virtual int32_t set_value(const std::string& value, int except=0);

    virtual int32_t self_type() const { return CONFIG_TYPE_UNIT; }
    virtual uint32_t size() const { return 1; }

    virtual const char* error_info(const ConfigErrorNo& err) const;

    virtual const ConfigUnit& get_sub_unit(uint32_t /*idx*/) const { return *get_err_instance(); }

    virtual const std::string& get_key() const { return _m_key; }

    virtual ConfigUnit* find_section(const std::string& /*section_name*/, uint32_t* /*depth*/) {
        return NULL;
    }

    virtual void clear() {}

    void set_data_pos(const ConfigUtils::DataPosition& pos) { _m_data_pos = pos; }
    const char* get_file_name() const { return _m_data_pos.file_name.c_str(); }
    uint32_t get_file_line() const { return _m_data_pos.file_line; }
    ConfigGroup* get_father() const { return _m_father; }

    static ConfigUnit* get_err_instance();
protected:
    virtual void throw_error(ConfigErrorNo* p_err_code, const int32_t& err_code) const;
    void set_error_key_path(const std::string &str) const;

protected:
    std::string _m_key;
    std::string _m_value;
    std::string _m_cstr; //专门的一个被解析后的string，只有当value以"为开头时才起作用
    int32_t _m_cstr_err;

    ConfigGroup* _m_father;
    ConfigUtils::DataPosition _m_data_pos;

}; // END class ConfigUnit

} // END namespace configure
} // END namespace bm
#endif  // BM_CONFIGURE_CONFIG_UNIT_H
