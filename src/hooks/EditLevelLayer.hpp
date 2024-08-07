#pragma once
#include <Geode/Geode.hpp>
#include <Geode/modify/EditLevelLayer.hpp>
#include <sabe.persistenceapi/include/PersistenceAPI.hpp>

extern unsigned long long s_editLevelLayerDelegate1;
extern unsigned long long s_editLevelLayerDelegate2;

class $modify(PSEditLevelLayer, EditLevelLayer) {
public:
	// overrides
	
	$override
	void onPlay(cocos2d::CCObject* i_sender);
};