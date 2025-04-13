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

	inline void toggleLockCursor(bool i_state) {
		#if defined(GEODE_IS_WINDOWS)
			CCEGLView::get()->toggleLockCursor(i_state);
		#elif defined(GEODE_IS_MAC)
			// TODO: implement someday
		#endif
	}

	inline void hideAndLockCursor(bool i_hide) {
		if (i_hide) {
			PlatformToolbox::hideCursor();
		} else {
			PlatformToolbox::showCursor();
		}
		bool l_lockCursor = GameManager::get()->getGameVariable("0128");
		if (l_lockCursor) {
			toggleLockCursor(i_hide);
		}
	}
}