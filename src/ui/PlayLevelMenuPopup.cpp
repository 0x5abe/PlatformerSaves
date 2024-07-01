#include "PlayLevelMenuPopup.hpp"
#include "Geode/binding/PlayLayer.hpp"
#include "Geode/cocos/CCDirector.h"
#include "Geode/cocos/base_nodes/Layout.hpp"
#include "Geode/cocos/label_nodes/CCLabelBMFont.h"
#include <hooks/PlayLayer.hpp>

using namespace geode::prelude;

PlayLevelMenuPopup* PlayLevelMenuPopup::create() {
	PlayLevelMenuPopup* i_this = new PlayLevelMenuPopup();

	if (i_this && i_this->init()) {
		i_this->autorelease();
	} else {
		CC_SAFE_DELETE(i_this);
	}

	return i_this;
}

bool PlayLevelMenuPopup::init() {
	if (!FLAlertLayer::init(150)) {
		return false;
	}

	CCEGLView::get()->showCursor(true);
	setup();

	return true;
}

void PlayLevelMenuPopup::setup() {
	CCDirector* l_director = CCDirector::sharedDirector();
	CCSize l_winSize = l_director->getWinSize();

	CCSize l_size = CCSize(l_winSize/2);
	m_background = CCScale9Sprite::create("GJ_square01.png");
	m_background->setContentSize(l_size);
	m_background->setPosition(l_winSize / 2.0f);
	m_mainLayer->addChild(m_background, -2);

	m_buttonMenu = CCMenu::create();
	m_buttonMenu->ignoreAnchorPointForPosition(false);
	m_buttonMenu->setContentSize(l_size/2);
	m_buttonMenu->setPosition(l_winSize/2);
	m_buttonMenu->setLayout(ColumnLayout::create());
	m_mainLayer->addChild(m_buttonMenu, 10);
	CCSize l_contentSize = m_buttonMenu->getContentSize();

	ButtonSprite* l_continueButtonSprite = ButtonSprite::create("Continue", l_size.width/2, true, "goldFont.fnt", "GJ_button_01.png", .0f, 1.f);
	CCMenuItemSpriteExtra* l_continueButton = CCMenuItemSpriteExtra::create(
		l_continueButtonSprite,
		this,
		menu_selector(PlayLevelMenuPopup::onContinue)
	);
	l_continueButton->setID("continue-button"_spr);

	PSPlayLayer* l_playLayer = static_cast<PSPlayLayer*>(PlayLayer::get());
	if (l_playLayer && !l_playLayer->validSaveExists()) {
		l_continueButtonSprite->m_label->setColor({127,127,127});
		l_continueButtonSprite->m_BGSprite->setColor({127,127,127});
		l_continueButton->m_bEnabled = false;
	}

	m_buttonMenu->addChild(l_continueButton);
	m_buttonMenu->updateLayout();

	ButtonSprite* l_newGameButtonSprite = ButtonSprite::create("New game", l_size.width/2, true, "goldFont.fnt", "GJ_button_01.png", .0f, 1.f);
	CCMenuItemSpriteExtra* l_newGameButton = CCMenuItemSpriteExtra::create(
		l_newGameButtonSprite,
		this,
		menu_selector(PlayLevelMenuPopup::onNewGame)
	);
	l_newGameButton->setID("new-game-button"_spr);

	m_buttonMenu->addChild(l_newGameButton);
	m_buttonMenu->updateLayout();
}

void PlayLevelMenuPopup::onNewGame(CCObject* sender) {
	PSPlayLayer* l_playLayer = static_cast<PSPlayLayer*>(PlayLayer::get());
	if (l_playLayer && l_playLayer->m_fields->m_loadingState == LoadingState::WaitingForPlayLevelMenuPopup) {
		l_playLayer->m_fields->m_saveSlot = -3;
	}

	onClose(nullptr);
}

void PlayLevelMenuPopup::onContinue(CCObject* sender) {
	PSPlayLayer* l_playLayer = static_cast<PSPlayLayer*>(PlayLayer::get());
	if (l_playLayer && l_playLayer->m_fields->m_loadingState == LoadingState::WaitingForPlayLevelMenuPopup) {
		l_playLayer->m_fields->m_saveSlot = 0;
	}

	onClose(nullptr);
}

void PlayLevelMenuPopup::keyBackClicked() {
	PSPlayLayer* l_playLayer = static_cast<PSPlayLayer*>(PlayLayer::get());
	if (l_playLayer && l_playLayer->m_fields->m_loadingState == LoadingState::WaitingForPlayLevelMenuPopup) {
		l_playLayer->m_fields->m_saveSlot = 0;

		std::string l_filePath = l_playLayer->getSaveFilePath(-1, true);
		if (l_filePath == "") {
			l_playLayer->m_fields->m_saveSlot = -3;
		}
	}

	onClose(nullptr);
}

void PlayLevelMenuPopup::onClose(CCObject* sender) {
	CCEGLView::get()->showCursor(false);
	bool l_lockCursor = GameManager::get()->getGameVariable("0128");
	if (l_lockCursor) {
		CCEGLView::get()->toggleLockCursor(true);
	}
	removeFromParentAndCleanup(true);
}