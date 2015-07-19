#ifndef BM_CONFIGURE_CONFIG_ARRAY_H
#define BM_CONFIGURE_CONFIG_ARRAY_H

#include <vector>
#include <string>
#include "ConfigGroup.h"

namespace bm {
namespace configure {

class ConfigArray : public ConfigGroup {
public:
    ConfigArray(const char* name, ConfigGroup* father=NULL);
    virtual ~ConfigArray() { clear(); }

    virtual int32_t push(const std::string &key, ConfigUnit* unit);

    virtual int32_t self_type() const { return CONFIG_TYPE_ARRAY; }

    inline const ConfigUnit& operator[](int32_t idx) const;
    inline const ConfigUnit& operator[](const char* str) const;
    inline const ConfigUnit& operator[](const std::string& str) const;
    inline ConfigUnit& operator[](int32_t idx);
    inline ConfigUnit& operator[](const char* str);
    inline ConfigUnit& operator[](const std::string& str);

    virtual uint32_t size() const { return _m_attr.size(); }
    virtual inline bool equals(const ConfigUnit& unit) const;

protected:
    virtual void clear();

protected:
    std::vector<ConfigUnit*> _m_attr;

}; // END class ConfigArray

} // END namespace configure
} // END namespace bm
#endif  // BM_CONFIGURE_CONFIG_ARRAY_H
