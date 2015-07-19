#ifndef BM_CONFIGURE_CONFIGURE_H
#define BM_CONFIGURE_CONFIGURE_H

#include <vector>
#include "ConfigGroup.h"

namespace bm {
namespace configure {

class Configure : public ConfigGroup {
public:
    Configure() : _m_section(this) { create(CONFIG_GLOBAL); }
    ~Configure() {}

    int32_t load(const char* path, const char* conf, int32_t dup_level=GROUP_DUP_LEVEL_0);

private:
    int32_t process_line(const std::string& str, int32_t level);
    int32_t push_pair(const std::string& key, const std::string& value);
    int32_t change_section(std::string str);

private:
    std::string _m_path;
    uint32_t _m_depth;
    ConfigGroup* _m_section;
    std::vector<std::pair<int32_t, std::string> > _m_conf_flist;

}; // END class Configure

} // END namespace configure
} // END namespace bm
#endif  // BM_CONFIGURE_CONFIGURE_H
