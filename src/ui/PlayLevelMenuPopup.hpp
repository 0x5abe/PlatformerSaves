#pragma once
#include <Geode/Geode.hpp>

class PlayLevelMenuPopup : public FLAlertLayer {
public:
	static PlayLevelMenuPopup* create();

	bool init() override;

	void keyBackClicked() override;

private:
	bool m_validSaveExists = false;
	
	void setup();

	void onRemove(cocos2d::CCObject* i_sender);
	
	void onNewGame(cocos2d::CCObject* i_sender);

	void onContinue(cocos2d::CCObject* i_sender);

	void onClose(cocos2d::CCObject* i_sender);

	cocos2d::extension::CCScale9Sprite* m_background;
};
