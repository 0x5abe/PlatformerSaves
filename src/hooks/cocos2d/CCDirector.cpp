#include "CCDirector.hpp"
#include <hooks/PlayLayer.hpp>

using namespace geode::prelude;
using namespace persistenceUtils;

// overrides

bool PSCCDirector::replaceScene(CCScene* i_scene) {
	if (s_currentPlayLayer) {
		if (s_currentPlayLayer->m_fields->m_signalForAsyncLoad) {
			s_currentPlayLayer->m_fields->m_signalForAsyncLoad = false;
			s_currentPlayLayer->m_fields->m_transitionFadeScene = i_scene;
			CC_SAFE_RETAIN(s_currentPlayLayer->m_fields->m_transitionFadeScene);

			CCScene* l_currentScene = CCScene::get();
			if (l_currentScene) {
				l_currentScene->runAction(
					CCSequence::create(
						CCDelayTime::create(0.0f),
						CCCallFunc::create(
							s_currentPlayLayer,
							callfunc_selector(PSPlayLayer::updateAsyncProcessCreateObjectsFromSetup)
						),
						nullptr
					)
				);
			}

			return false;
		}

		if (s_currentPlayLayer->m_fields->m_cancelLevelLoad) {
			CCEGLView::get()->showCursor(true);
			CC_SAFE_RELEASE(s_currentPlayLayer->m_fields->m_transitionFadeScene);
			s_currentPlayLayer->m_fields->m_transitionFadeScene = nullptr;
			s_currentPlayLayer = nullptr;
			CCScene* l_currentScene = CCScene::get();

			LevelInfoLayer* l_levelInfoLayer = static_cast<LevelInfoLayer*>(CCScene::get()->getChildByID("LevelInfoLayer"));
			if (l_levelInfoLayer) {
				l_levelInfoLayer->m_isBusy = false;
				CCMenu* l_playMenu = static_cast<CCMenu*>(l_levelInfoLayer->getChildByID("play-menu"));
				if (!l_playMenu) {
					return false;
				}
				//Todo: Fix scale and color of play button
				l_playMenu->setTouchEnabled(true);
				CCNode* l_playButton = l_playMenu->getChildByID("play-button");
				if (!l_playButton) {
					return false;
				}
				CCNode* l_playButtonSprite = static_cast<CCNode*>(l_playButton->getChildren()->objectAtIndex(0));
				if (!l_playButtonSprite) {
					return false;
				}
				l_playButtonSprite->removeAllChildrenWithCleanup(true);
			}

			LevelSelectLayer* l_mainLayer = static_cast<LevelSelectLayer*>(CCScene::get()->getChildByID("main-layer"));
			if (l_mainLayer) {
				BoomScrollLayer* l_levelsList = static_cast<BoomScrollLayer*>(l_mainLayer->getChildByID("levels-list"));
				if (!l_levelsList) {
					return false;
				}
				ExtendedLayer* l_levelPages = static_cast<ExtendedLayer*>(l_levelsList->getChildByID("level-pages"));
				if (!l_levelPages) {
					return false;
				}
				LevelPage* l_levelPage1 = static_cast<LevelPage*>(l_levelPages->getChildByID("level-page-1"));
				if (l_levelPage1) {
					l_levelPage1->m_isBusy = false;
				}
				LevelPage* l_levelPage2 = static_cast<LevelPage*>(l_levelPages->getChildByID("level-page-2"));
				if (l_levelPage2) {
					l_levelPage2->m_isBusy = false;
				}
				LevelPage* l_levelPage3 = static_cast<LevelPage*>(l_levelPages->getChildByID("level-page-3"));
				if (l_levelPage3) {
					l_levelPage3->m_isBusy = false;
				}
				// isBusy2
				// it's for when trying to use a button to get into the level instead of click
				*reinterpret_cast<byte*>(geode::base::get()+0x685ff8) = 0;
			}

			return false;
		}
	}

	return CCDirector::replaceScene(i_scene);
}