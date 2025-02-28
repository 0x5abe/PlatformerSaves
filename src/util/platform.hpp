#pragma once
#include <Geode/Geode.hpp>

using namespace geode::prelude;

namespace util::platform {
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