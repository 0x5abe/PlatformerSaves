#pragma once
#include <Geode/Geode.hpp>

using namespace geode::prelude;

namespace util::platform {
	// Limit is 7 cause we use a 3 bit bitfield
	enum class PSPlatform {
		Win = 0,
		Android64 = 1,
		Android32 = 2,
		MacArm = 3,
		MacIntel = 4,
		Ios = 5,
		PlatformInvalid = 7,
	};

	// TODO: Support on Mac
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