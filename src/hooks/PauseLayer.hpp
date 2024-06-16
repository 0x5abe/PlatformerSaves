
#pragma once
#include "Geode/binding/PauseLayer.hpp"
#include "Geode/cocos/cocoa/CCObject.h"
#include <Geode/Geode.hpp>
#include <Geode/modify/PauseLayer.hpp>
#include <sabe.persistenceutils/include/PersistenceUtils.hpp>

class $modify(PSPauseLayer, PauseLayer) {
public:

	// overrides
	$override
	void onEdit(cocos2d::CCObject* i_sender);

	$override
	void tryQuit(cocos2d::CCObject* i_sender);
};