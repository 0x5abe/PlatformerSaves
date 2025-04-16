#pragma once
#include "Geode/cocos/robtop/keyboard_dispatcher/CCKeyboardDelegate.h"
#include <Geode/Geode.hpp>
#include <Geode/modify/CCKeyboardDispatcher.hpp>
#include <sabe.persistenceapi/include/PersistenceAPI.hpp>

extern size_t s_levelInfoLayerDelegate1;
extern size_t s_levelInfoLayerDelegate2;
extern size_t s_editLevelLayerDelegate1;
extern size_t s_editLevelLayerDelegate2;

class $modify(PSCCKeyboardDispatcher, cocos2d::CCKeyboardDispatcher) {
public:
    // overrides

    $override
    void removeDelegate(cocos2d::CCKeyboardDelegate* i_delegate);
};