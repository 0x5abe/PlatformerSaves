#pragma once
#include <Geode/Geode.hpp>

class PlayLevelMenuPopup : public FLAlertLayer {
public:
	static PlayLevelMenuPopup* create();

	bool init() override;

	void keyBackClicked() override;

private:
	void setup();

	void onClose(cocos2d::CCObject* i_sender);
	
	void onNewGame(cocos2d::CCObject* i_sender);

	void onContinue(cocos2d::CCObject* i_sender);

	bool validSaveExists();

	cocos2d::extension::CCScale9Sprite* m_background;
};
