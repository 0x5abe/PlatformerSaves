#pragma once
#include "Geode/binding/LevelInfoLayer.hpp"
#include <Geode/Geode.hpp>
#include <Geode/modify/LevelInfoLayer.hpp>
#include <sabe.persistenceapi/include/PersistenceAPI.hpp>

extern unsigned long long s_levelInfoLayerDelegate1;
extern unsigned long long s_levelInfoLayerDelegate2;

class $modify(PSLevelInfoLayer, LevelInfoLayer) {
public:
	// overrides

	$override
	void onPlay(cocos2d::CCObject* i_sender);
};