#include "bmhead.h"

namespace bm_server {

const char*
bmhead_error(int ret)
{
	if (ret > 0 || ret < 1 - (int)(sizeof(bmhead_errstr) / sizeof(char*))) {
		return bmhead_errstr[-BMHEAD_RET_UNKNOWN];
    }

	return bmhead_errstr[-ret];
}

} // END namespace bm_server
