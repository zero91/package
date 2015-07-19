#include "ConfigGroup.h"

#include <boost/foreach.hpp>
#include <log.h>

#include "ConfigException.h"
#include "ConfigArray.h"

namespace bm {
namespace configure {

const ConfigUnit& ConfigGroup::operator[](const char* str) const {
    return operator[](std::string(str));
}

const ConfigUnit& ConfigGroup::operator[](const std::string& str) const {
    FieldMapType::const_iterator it = _m_field_map.find(str);
    if (it != _m_field_map.end()) {
        return *it->second;
    }
    set_error_key_path(_m_name + "." + str);
    return *get_err_instance();
}

const ConfigUnit& ConfigGroup::operator[](int32_t idx) const {
    char idx_str[65];
    snprintf(idx_str, sizeof(idx_str), "%d", idx);
    set_error_key_path(_m_name + "[" + idx_str + "]");
    return *get_err_instance();
}

ConfigUnit& ConfigGroup::operator[](const char* str) {
    return operator[](std::string(str));
}

ConfigUnit& ConfigGroup::operator[](const std::string& str) {
    FieldMapType::iterator it = _m_field_map.find(str);
    if (it != _m_field_map.end()) {
        return *it->second;
    }
    set_error_key_path(_m_name + "." + str);
    return *get_err_instance();
}

ConfigUnit& ConfigGroup::operator[](int32_t idx) {
    char idx_str[65];
    snprintf(idx_str, sizeof(idx_str), "%d", idx);
    set_error_key_path(_m_name + "[" + idx_str + "]");
    return *get_err_instance();
}

char ConfigGroup::to_char(ConfigErrorNo* p_err_code) const {
    set_error_key_path(_m_name);
    throw_error(p_err_code, CONFIG_ERROR_GROUP_TO_UNIT);
    return static_cast<char>(0);
}

int16_t ConfigGroup::to_int16(ConfigErrorNo* p_err_code) const {
    set_error_key_path(_m_name);
    throw_error(p_err_code, CONFIG_ERROR_GROUP_TO_UNIT);
    return static_cast<int16_t>(0);
}

int32_t ConfigGroup::to_int32(ConfigErrorNo* p_err_code) const {
    set_error_key_path(_m_name);
    throw_error(p_err_code, CONFIG_ERROR_GROUP_TO_UNIT);
    return static_cast<int32_t>(0);
}

int64_t ConfigGroup::to_int64(ConfigErrorNo* p_err_code) const {
    set_error_key_path(_m_name);
    throw_error(p_err_code, CONFIG_ERROR_GROUP_TO_UNIT);
    return static_cast<int64_t>(0);
}

unsigned char ConfigGroup::to_uchar(ConfigErrorNo* p_err_code) const {
    set_error_key_path(_m_name);
    throw_error(p_err_code, CONFIG_ERROR_GROUP_TO_UNIT);
    return static_cast<unsigned char>(0);
}

uint16_t ConfigGroup::to_uint16(ConfigErrorNo* p_err_code) const {
    set_error_key_path(_m_name);
    throw_error(p_err_code, CONFIG_ERROR_GROUP_TO_UNIT);
    return static_cast<uint16_t>(0);
}

uint32_t ConfigGroup::to_uint32(ConfigErrorNo* p_err_code) const {
    set_error_key_path(_m_name);
    throw_error(p_err_code, CONFIG_ERROR_GROUP_TO_UNIT);
    return static_cast<uint32_t>(0);
}

uint64_t ConfigGroup::to_uint64(ConfigErrorNo* p_err_code) const {
    set_error_key_path(_m_name);
    throw_error(p_err_code, CONFIG_ERROR_GROUP_TO_UNIT);
    return static_cast<uint64_t>(0);
}

float ConfigGroup::to_float(ConfigErrorNo* p_err_code) const {
    set_error_key_path(_m_name);
    throw_error(p_err_code, CONFIG_ERROR_GROUP_TO_UNIT);
    return float(0);
}

double ConfigGroup::to_double(ConfigErrorNo* p_err_code) const {
    set_error_key_path(_m_name);
    throw_error(p_err_code, CONFIG_ERROR_GROUP_TO_UNIT);
    return double(0);
}

std::string ConfigGroup::to_string(ConfigErrorNo* p_err_code) const {
    set_error_key_path(_m_name);
    throw_error(p_err_code, CONFIG_ERROR_GROUP_TO_UNIT);
    return "";
}

std::string ConfigGroup::to_raw_string(ConfigErrorNo* p_err_code) const {
    set_error_key_path(_m_name);
    throw_error(p_err_code, CONFIG_ERROR_GROUP_TO_UNIT);
    return "";
}

const char* ConfigGroup::to_cstr(ConfigErrorNo* p_err_code) const {
    set_error_key_path(_m_name);
    throw_error(p_err_code, CONFIG_ERROR_GROUP_TO_UNIT);
    return NULL;
}

char ConfigGroup::to_char(ConfigErrorNo* p_err_code, const char& def) const {
    set_error_key_path(_m_name);
    if (p_err_code != NULL) {
        *p_err_code = CONFIG_ERROR_GROUP_TO_UNIT;
    }
    return def;
}

int16_t ConfigGroup::to_int16(ConfigErrorNo* p_err_code, const int16_t& def) const {
    set_error_key_path(_m_name);
    if (p_err_code != NULL) {
        *p_err_code = CONFIG_ERROR_GROUP_TO_UNIT;
    }
    return def;
}

int32_t ConfigGroup::to_int32(ConfigErrorNo* p_err_code, const int32_t& def) const {
    set_error_key_path(_m_name);
    if (p_err_code != NULL) {
        *p_err_code = CONFIG_ERROR_GROUP_TO_UNIT;
    }
    return def;
}

int64_t ConfigGroup::to_int64(ConfigErrorNo* p_err_code, const int64_t& def) const {
    set_error_key_path(_m_name);
    if (p_err_code != NULL) {
        *p_err_code = CONFIG_ERROR_GROUP_TO_UNIT;
    }
    return def;
}

unsigned char ConfigGroup::to_uchar(ConfigErrorNo* p_err_code, const unsigned char& def) const {
    set_error_key_path(_m_name);
    if (p_err_code != NULL) {
        *p_err_code = CONFIG_ERROR_GROUP_TO_UNIT;
    }
    return def;
}

uint16_t ConfigGroup::to_uint16(ConfigErrorNo* p_err_code, const uint16_t& def) const {
    set_error_key_path(_m_name);
    if (p_err_code != NULL) {
        *p_err_code = CONFIG_ERROR_GROUP_TO_UNIT;
    }
    return def;
}

uint32_t ConfigGroup::to_uint32(ConfigErrorNo* p_err_code, const uint32_t& def) const {
    set_error_key_path(_m_name);
    if (p_err_code != NULL) {
        *p_err_code = CONFIG_ERROR_GROUP_TO_UNIT;
    }
    return def;
}

uint64_t ConfigGroup::to_uint64(ConfigErrorNo* p_err_code, const uint64_t& def) const {
    set_error_key_path(_m_name);
    if (p_err_code != NULL) {
        *p_err_code = CONFIG_ERROR_GROUP_TO_UNIT;
    }
    return def;
}

float ConfigGroup::to_float(ConfigErrorNo* p_err_code, const float& def) const {
    set_error_key_path(_m_name);
    if (p_err_code != NULL) {
        *p_err_code = CONFIG_ERROR_GROUP_TO_UNIT;
    }
    return def;
}

double ConfigGroup::to_double(ConfigErrorNo* p_err_code, const double& def) const {
    set_error_key_path(_m_name);
    if (p_err_code != NULL) {
        *p_err_code = CONFIG_ERROR_GROUP_TO_UNIT;
    }
    return def;
}

std::string ConfigGroup::to_string(ConfigErrorNo* p_err_code, const std::string& def) const {
    set_error_key_path(_m_name);
    if (p_err_code != NULL) {
        *p_err_code = CONFIG_ERROR_GROUP_TO_UNIT;
    }
    return def;
}

std::string ConfigGroup::to_raw_string(ConfigErrorNo* p_err_code, const std::string& def) const {
    if (p_err_code != NULL) {
        *p_err_code = CONFIG_ERROR_GROUP_TO_UNIT;
    }
    return def;
}

const char* ConfigGroup::to_cstr(ConfigErrorNo* p_err_code, const char* def) const {
    set_error_key_path(_m_name);
    if (p_err_code != NULL) {
        *p_err_code = CONFIG_ERROR_GROUP_TO_UNIT;
    }
    return def;
}

int32_t ConfigGroup::get_char(char* /*value*/) const {
    set_error_key_path(_m_name);
    return CONFIG_ERROR_GROUP_TO_UNIT;
}

int32_t ConfigGroup::get_int16(int16_t* /*value*/) const {
    set_error_key_path(_m_name);
    return CONFIG_ERROR_GROUP_TO_UNIT;
}

int32_t ConfigGroup::get_int32(int32_t* /*value*/) const {
    set_error_key_path(_m_name);
    return CONFIG_ERROR_GROUP_TO_UNIT;
}

int32_t ConfigGroup::get_int64(int64_t* /*value*/) const {
    set_error_key_path(_m_name);
    return CONFIG_ERROR_GROUP_TO_UNIT;
}

int32_t ConfigGroup::get_uchar(unsigned char* /*value*/) const {
    set_error_key_path(_m_name);
    return CONFIG_ERROR_GROUP_TO_UNIT;
}

int32_t ConfigGroup::get_uint16(uint16_t* /*value*/) const {
    set_error_key_path(_m_name);
    return CONFIG_ERROR_GROUP_TO_UNIT;
}

int32_t ConfigGroup::get_uint32(uint32_t* /*value*/) const {
    set_error_key_path(_m_name);
    return CONFIG_ERROR_GROUP_TO_UNIT;
}

int32_t ConfigGroup::get_uint64(uint64_t* /*value*/) const {
    set_error_key_path(_m_name);
    return CONFIG_ERROR_GROUP_TO_UNIT;
}

int32_t ConfigGroup::get_float(float* /*value*/) const {
    set_error_key_path(_m_name);
    return CONFIG_ERROR_GROUP_TO_UNIT;
}

int32_t ConfigGroup::get_double(double* /*value*/) const {
    set_error_key_path(_m_name);
    return CONFIG_ERROR_GROUP_TO_UNIT;
}

int32_t ConfigGroup::get_string(std::string* /*value*/) const {
    set_error_key_path(_m_name);
    return CONFIG_ERROR_GROUP_TO_UNIT;
}

int32_t ConfigGroup::get_raw_string(std::string* /*value*/) const {
    set_error_key_path(_m_name);
    return CONFIG_ERROR_GROUP_TO_UNIT;
}

int32_t ConfigGroup::get_cstr(char* /*value*/, size_t /*len*/) const {
    set_error_key_path(_m_name);
    return CONFIG_ERROR_GROUP_TO_UNIT;
}

int32_t ConfigGroup::get_char(char* value, const char& def) const {
    set_error_key_path(_m_name);
    if (value != NULL) {
        *value = def;
    }
    return CONFIG_ERROR_GROUP_TO_UNIT;
}

int32_t ConfigGroup::get_int16(int16_t* value, const int16_t& def) const {
    set_error_key_path(_m_name);
    if (value != NULL) {
        *value = def;
    }
    return CONFIG_ERROR_GROUP_TO_UNIT;
}

int32_t ConfigGroup::get_int32(int32_t* value, const int32_t& def) const {
    set_error_key_path(_m_name);
    if (value != NULL) {
        *value = def;
    }
    return CONFIG_ERROR_GROUP_TO_UNIT;
}

int32_t ConfigGroup::get_int64(int64_t* value, const int64_t& def) const {
    set_error_key_path(_m_name);
    if (value != NULL) {
        *value = def;
    }
    return CONFIG_ERROR_GROUP_TO_UNIT;
}

int32_t ConfigGroup::get_uchar(unsigned char* value, const unsigned char& def) const {
    set_error_key_path(_m_name);
    if (value != NULL) {
        *value = def;
    }
    return CONFIG_ERROR_GROUP_TO_UNIT;
}

int32_t ConfigGroup::get_uint16(uint16_t* value, const uint16_t& def) const {
    set_error_key_path(_m_name);
    if (value != NULL) {
        *value = def;
    }
    return CONFIG_ERROR_GROUP_TO_UNIT;
}

int32_t ConfigGroup::get_uint32(uint32_t* value, const uint32_t& def) const {
    set_error_key_path(_m_name);
    if (value != NULL) {
        *value = def;
    }
    return CONFIG_ERROR_GROUP_TO_UNIT;
}

int32_t ConfigGroup::get_uint64(uint64_t* value, const uint64_t& def) const {
    set_error_key_path(_m_name);
    if (value != NULL) {
        *value = def;
    }
    return CONFIG_ERROR_GROUP_TO_UNIT;
}

int32_t ConfigGroup::get_float(float* value, const float& def) const {
    set_error_key_path(_m_name);
    if (value != NULL) {
        *value = def;
    }
    return CONFIG_ERROR_GROUP_TO_UNIT;
}

int32_t ConfigGroup::get_double(double* value, const double& def) const {
    set_error_key_path(_m_name);
    if (value != NULL) {
        *value = def;
    }
    return CONFIG_ERROR_GROUP_TO_UNIT;
}

int32_t ConfigGroup::get_string(std::string* value, const std::string& def) const {
    set_error_key_path(_m_name);
    if (value != NULL) {
        *value = def;
    }
    return CONFIG_ERROR_GROUP_TO_UNIT;
}

int32_t ConfigGroup::get_raw_string(std::string* value, const std::string& def) const {
    set_error_key_path(_m_name);
    if (value != NULL) {
        *value = def;
    }
    return CONFIG_ERROR_GROUP_TO_UNIT;
}

int32_t ConfigGroup::get_cstr(char* value, size_t len, const char* def) const {
    set_error_key_path(_m_name);
    if (value != NULL) {
        snprintf(value, len, "%s", def);
    }
    return CONFIG_ERROR_GROUP_TO_UNIT;
}

bool ConfigGroup::equals(const ConfigUnit& unit) const {
    try {
        BOOST_FOREACH(const FieldMapType::value_type &field, _m_field_map) {
            if (!field.second->equals(unit[field.first])) {
                return false;
            }
        }
        return true;
    } catch (ConfigException &e) {
        BM_LOG_WARNING("Config.ConfigGroup.equals() receive an exception[%s]", e.what());
        return false;
    }
    return false;
}

int32_t ConfigGroup::push(const std::string& key, ConfigUnit* unit) {
    if (key[0] != '@') {
        if (_m_field_map.find(key) == _m_field_map.end()) {
            _m_field_map[key] = unit;
            _m_field_list.push_back(key);
        } else {
            BM_LOG_WARNING("Configure: [%s] Duplicate key [%s]", _m_name.c_str(), key.c_str());
            throw ConfigException().append("Duplicate key [%s]", _m_name.c_str());
            return CONFIG_ERROR_DUPLICATED_KEY;
        }
    } else {
        FieldMapType::iterator it = _m_field_map.find(key.substr(1));
        if (it != _m_field_map.end()) {
            if (it->second->self_type() == CONFIG_TYPE_ARRAY) {
                (static_cast<ConfigArray*>(it->second))->push(key, unit);
            } else {
                BM_LOG_WARNING("Configure : ambiguity key [%s]", key.c_str());
                throw ConfigException().append("Ambiguity key [%s]", key.c_str());
                return CONFIG_ERROR_CONFIG_ERROR;
            }
        } else {
            ConfigArray *array = new(std::nothrow) ConfigArray(key.c_str() + 1, this);
            if (array == NULL) {
                BM_LOG_WARNING("Allocate memory for ConfigArray [%s] failed", key.c_str() + 1);
                throw ConfigException();
            }

            if (push(key.substr(1), array) == 0) {
                array->push(key.substr(1), unit);
            }
        }
    }
    return 0;
}

void ConfigGroup::create(const char* name, ConfigGroup* father) {
    const char* p = name;
    if (*p == '@') {
        ++p;
    }
    while (*p != 0) {
        if (!isalpha(*p) && !isdigit(*p) && *p != '_') {
            BM_LOG_WARNING("Configure: Unsupport Group name [%s]", name);
            throw ConfigException();
        }
        ++p;
    }
    _m_name = name;;
    _m_father = father;
    if (_m_father != NULL) {
        _m_level = _m_father->get_level() + 1;
    } else {
        _m_level = 0;
    }
}

const ConfigUnit& ConfigGroup::get_sub_unit(uint32_t idx) const {
    char idx_str[65];
    snprintf(idx_str, sizeof(idx_str), "%u", idx);
    if (idx >= _m_field_map.size()) {
        set_error_key_path(_m_name + "[" + idx_str + "]");
        return *get_err_instance();
    }

    ConfigUnit* p = _m_field_map.find(_m_field_list[idx])->second;
    if (p == NULL) {
        set_error_key_path(_m_name + "[" + idx_str + "]");
        return *get_err_instance();
    }
    return *p;
}

ConfigUnit* ConfigGroup::find_section(const std::string &section_name, uint32_t* depth) {
    if (section_name.size() == 0) {
        return this;
    }

    size_t global_len = strlen(CONFIG_GLOBAL);
    if (section_name.substr(0, global_len) == CONFIG_GLOBAL && 
            (section_name.size() == global_len || section_name[global_len] == '.')) {
        ConfigGroup* p_sec = this;
        while (p_sec->get_father() != NULL) {
            p_sec = p_sec->get_father();
        }
        *depth = 0;
        if (section_name.size() > global_len) {
            return p_sec->find_section(section_name.substr(global_len + 1), depth);
        }
        return p_sec;
    }

    size_t dot_idx = section_name.find('.');
    if (dot_idx != std::string::npos) {
        FieldMapType::iterator find_it = _m_field_map.find(section_name.substr(0, dot_idx));
        if (find_it == _m_field_map.end()) {
            BM_LOG_WARNING("Configure: No previous section named [%s]",
                                                section_name.substr(0, dot_idx).c_str());
            throw ConfigException();
        }
        ++(*depth);
        return find_it->second->find_section(section_name.substr(dot_idx + 1), depth);
    }

    FieldMapType::iterator field_it = _m_field_map.find(section_name);
    if (field_it == _m_field_map.end()) {
        ConfigGroup* new_group = new(std::nothrow) ConfigGroup(section_name.c_str(),
                                            this, ConfigUtils::cur_data_pos, _m_dup_level);
        if (new_group == NULL) {
            BM_LOG_WARNING("Allocate memory for new section [%s] failed", section_name.c_str());
            throw ConfigException();
        }

        if (section_name[0] != '@' && !(_m_dup_level == GROUP_DUP_LEVEL_0
                                        || (_m_dup_level == GROUP_DUP_LEVEL_1 && *depth > 1))) {
            _m_dup_map[new_group] =
                        std::vector<ConfigUtils::DataPosition>(1, ConfigUtils::cur_data_pos);
        }
        ++(*depth);
        push(section_name, new_group);
        return new_group;
    } else {
        ++(*depth);
        if (field_it->second->self_type() != CONFIG_TYPE_GROUP) {
            BM_LOG_WARNING("Config : Duplicate key or group name [%s]", section_name.c_str());
            throw ConfigException();
        } else if (!(_m_dup_level == GROUP_DUP_LEVEL_0 ||
                     (_m_dup_level == GROUP_DUP_LEVEL_1 && *depth > 1))) {
            DupMapType::iterator dup_it = _m_dup_map.find(field_it->second);
            if (dup_it == _m_dup_map.end()) {
                BM_LOG_WARNING("Config : dup-map failed to find [%s]", section_name.c_str());
                throw ConfigException();
            }
            dup_it->second.push_back(ConfigUtils::cur_data_pos);
        }
        return field_it->second;
    }
    return NULL;
}

void ConfigGroup::print_dup() const {
    for (DupMapType::const_iterator unit_it = _m_dup_map.begin();
                unit_it != _m_dup_map.end(); ++unit_it) {
        if (unit_it->second.size() <= 1) {
            continue;
        }
        for (uint32_t k = 0; k < unit_it->second.size(); ++k) {
            BM_LOG_NOTICE("[Group: %s][File: %s][Line: %u]",
                                    unit_it->first->get_key().c_str(),
                                    unit_it->second[k].file_name.c_str(),
                                    unit_it->second[k].file_line);
        }
    }
}

void ConfigGroup::clear() {
    BOOST_FOREACH(FieldMapType::value_type& item, _m_field_map) {
        delete item.second;
    }
    _m_field_map.clear();
    _m_field_list.clear();
}

} // END namespace configure
} // END namespace bm
