#pragma once
#include <Geode/Geode.hpp>
#include <Geode/modify/LevelAreaInnerLayer.hpp>
#include <sabe.persistenceapi/include/PersistenceAPI.hpp>

class $modify(PSLevelAreaInnerLayer, LevelAreaInnerLayer) {
public:
	// overrides
	
	$override
	void onDoor(cocos2d::CCObject* i_sender);
};