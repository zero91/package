#ifndef BM_CONFIGURE_CONFIG_GROUP_H
#define BM_CONFIGURE_CONFIG_GROUP_H

#include <vector>
#include <boost/unordered_map.hpp>

#include "ConfigUtils.h"
#include "ConfigUnit.h"
#include "ConfigError.h"
#include "Constants.h"

namespace bm {
namespace configure {

class ConfigGroup : public ConfigError {
public:
    typedef boost::unordered_map<std::string, ConfigUnit*> FieldMapType;

public:
    ConfigGroup() : _m_name(""), _m_dup_level(GROUP_DUP_LEVEL_0) {}

    ConfigGroup(const char* name, ConfigGroup* father=NULL,
                        ConfigUtils::DataPosition data_pos=ConfigUtils::DataPosition(),
                        int32_t dup_level=GROUP_DUP_LEVEL_0) : _m_dup_level(dup_level) {
        _m_data_pos = data_pos;
        create(name, father);
    }
    virtual ~ConfigGroup() { clear(); }

    virtual const ConfigUnit& operator[](const char* str) const;
    virtual const ConfigUnit& operator[](const std::string& str) const;
    virtual const ConfigUnit& operator[](int32_t idx) const;
    virtual ConfigUnit& operator[](const char* str);
    virtual ConfigUnit& operator[](const std::string& str);
    virtual ConfigUnit& operator[](int32_t idx);

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

    virtual const std::string& get_key() const { return _m_name; }
    virtual bool equals(const ConfigUnit& unit) const;

    virtual const ConfigUnit& get_sub_unit(uint32_t idx) const;

    virtual int32_t self_type() const { return CONFIG_TYPE_GROUP; }
    virtual uint32_t size() const { return _m_field_map.size(); }

    void print_dup() const;

    int32_t push(const std::string& key, ConfigUnit* unit);
    ConfigUnit* find_section(const std::string &section_name, uint32_t* depth);
    int32_t get_level() const { return _m_level; }

protected:
    void create(const char* name, ConfigGroup* father=NULL);
    virtual void clear();

protected:
    FieldMapType _m_field_map;
    std::vector<std::string> _m_field_list;
    std::string _m_name;
    int32_t _m_level;
    int32_t _m_dup_level;

    typedef boost::unordered_map<ConfigUnit*, std::vector<ConfigUtils::DataPosition> > DupMapType;
    DupMapType _m_dup_map;

}; // END class ConfigGroup

} // END namespace configure
} // END namespace bm

#endif  // BM_CONFIGURE_CONFIG_GROUP_H
