#include "CCKeyboardDispatcher.hpp"
#include "Geode/cocos/robtop/keyboard_dispatcher/CCKeyboardDispatcher.h"
#include <hooks/PlayLayer.hpp>

using namespace geode::prelude;
using namespace persistenceAPI;

// overrides

void PSCCKeyboardDispatcher::removeDelegate(cocos2d::CCKeyboardDelegate* i_delegate) {
    if (s_levelInfoLayerDelegate1 == reinterpret_cast<size_t>(i_delegate)
        || s_levelInfoLayerDelegate2 == reinterpret_cast<size_t>(i_delegate)
        || s_editLevelLayerDelegate1 == reinterpret_cast<size_t>(i_delegate)
        || s_editLevelLayerDelegate2 == reinterpret_cast<size_t>(i_delegate)
    ) {
        return;
    }
    CCKeyboardDispatcher::removeDelegate(i_delegate);
}