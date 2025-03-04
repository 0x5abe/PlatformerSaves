#pragma once
#include <Geode/Geode.hpp>

using namespace geode::prelude;

namespace util::platform {
	// Already at limit since we use a 2 bit bitfield so ig we never supporting ios, etc.
	enum class PSPlatform {
		Win = 0,
		Android64 = 1,
		Android32 = 2,
		Mac = 3
	};

	inline void hideAndLockCursor(bool i_hide) {
		#if defined(GEODE_IS_WINDOWS)
			CCEGLView::get()->showCursor(!i_hide);
			bool l_lockCursor = GameManager::get()->getGameVariable("0128");
			if (l_lockCursor) {
				CCEGLView::get()->toggleLockCursor(i_hide);
			}
		#endif
	}
}