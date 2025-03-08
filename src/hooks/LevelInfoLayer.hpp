#pragma once
#include "Geode/binding/LevelInfoLayer.hpp"
#include <Geode/Geode.hpp>
#include <Geode/modify/LevelInfoLayer.hpp>
#include <sabe.persistenceapi/include/PersistenceAPI.hpp>

extern size_t s_levelInfoLayerDelegate1;
extern size_t s_levelInfoLayerDelegate2;

class $modify(PSLevelInfoLayer, LevelInfoLayer) {
public:
	// overrides

	$override
	void onPlay(cocos2d::CCObject* i_sender);
};