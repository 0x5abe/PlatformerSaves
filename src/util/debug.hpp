#pragma once
#if defined(PS_DEBUG) && (defined(PS_DESCRIBE) || defined(PS_SEPARATORS))
#include <iomanip>
#include <sstream>
#include <string>
#endif

#if defined(PS_DEBUG) && defined(PS_DESCRIBE)
inline std::string hexStr(const uint8_t *data, int len)
{
	std::stringstream ss;
	ss << std::hex;
	for( int i(0) ; i < len; ++i )
		ss << std::setw(2) << std::setfill('0') << (int)data[i];
	return ss.str();
}
#endif