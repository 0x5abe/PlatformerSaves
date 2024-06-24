
#pragma once
#include "Geode/binding/PauseLayer.hpp"
#include "Geode/cocos/cocoa/CCObject.h"
#include <Geode/Geode.hpp>
#include <Geode/modify/PauseLayer.hpp>
#include <sabe.persistenceapi/include/PersistenceAPI.hpp>

class $modify(PSPauseLayer, PauseLayer) {
public:
	struct Fields {
		cocos2d::CCSprite* m_saveCheckpointsSprite;
		CCMenuItemSpriteExtra* m_saveCheckpointsButton;
		bool m_cancelSave;
	};

	// overrides
	$override
	void customSetup();

	$override
	void onEdit(cocos2d::CCObject* i_sender);

	$override
	void tryQuit(cocos2d::CCObject* i_sender);

	$override
	void onRestartFull(cocos2d::CCObject* i_sender);

	$override
	void onQuit(cocos2d::CCObject* i_sender);

	// custom methods

	void onSaveCheckpoints(cocos2d::CCObject* i_sender);
};