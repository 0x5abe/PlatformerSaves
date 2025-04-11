#include "CCDirector.hpp"
#include <cstdint>
#include <hooks/PlayLayer.hpp>
#include <hooks/FMODAudioEngine.hpp>
#include <util/platform.hpp>

using namespace geode::prelude;
using namespace persistenceAPI;
using namespace util::platform;

#if defined(GEODE_IS_WINDOWS)
	#define DIDCLICK_OFFSET 0x6a3048
#elif  defined(GEODE_IS_ANDROID64)
	#define DIDCLICK_OFFSET 0x12114a1
#elif  defined(GEODE_IS_ANDROID32)
	#define DIDCLICK_OFFSET 0xaac0fd
#endif

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
			hideAndLockCursor(false);
			CC_SAFE_RELEASE(s_currentPlayLayer->m_fields->m_transitionFadeScene);
			s_currentPlayLayer->m_fields->m_transitionFadeScene = nullptr;
			GameManager::get()->m_gameLayer = nullptr;
			GameManager::get()->m_playLayer = nullptr;
			s_currentPlayLayer = nullptr;

			// restore loadMusic
			PSFMODAudioEngine* l_audioEngine = static_cast<PSFMODAudioEngine*>(FMODAudioEngine::get());
			l_audioEngine->m_fields->m_disableLoadMusic = false;

			CCScene* l_currentScene = CCScene::get();

			LevelInfoLayer* l_levelInfoLayer = static_cast<LevelInfoLayer*>(CCScene::get()->getChildByID("LevelInfoLayer"));
			if (l_levelInfoLayer) {
				l_levelInfoLayer->m_isBusy = false;
				CCMenu* l_playMenu = static_cast<CCMenu*>(l_levelInfoLayer->getChildByID("play-menu"));
				if (!l_playMenu) {
					return false;
				}
				l_playMenu->setTouchEnabled(true);
				CCNode* l_playButton = l_playMenu->getChildByID("play-button");
				if (!l_playButton) {
					return false;
				}
				CCSprite* l_playButtonSprite = static_cast<CCSprite*>(l_playButton->getChildren()->objectAtIndex(0));
				if (!l_playButtonSprite) {
					return false;
				}
				l_playButtonSprite->removeAllChildrenWithCleanup(true);
				l_playButtonSprite->setColor({255, 255, 255});
				l_levelInfoLayer->m_songWidget->m_buttonMenu->setTouchEnabled(true);
			}

			EditLevelLayer* l_editLevelLayer = static_cast<EditLevelLayer*>(CCScene::get()->getChildByID("EditLevelLayer"));
			if (l_editLevelLayer) {
				// isBusy
				l_editLevelLayer->m_exiting = false;
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
				// didClick
				// it's for when trying to use a button to get into the level instead of click
				*reinterpret_cast<uint8_t*>(geode::base::get()+DIDCLICK_OFFSET) = 0;
			}

			LevelAreaInnerLayer* l_levelAreaInnerLayer = static_cast<LevelAreaInnerLayer*>(CCScene::get()->getChildByID("LevelAreaInnerLayer"));
			if (l_levelAreaInnerLayer) {
				l_levelAreaInnerLayer->m_enteringLevel = false;
				l_levelAreaInnerLayer->m_exiting = false;
				if (l_levelAreaInnerLayer->getChildrenCount() == 3) {
					l_levelAreaInnerLayer->getChildren()->removeObjectAtIndex(2);
				}
			}

			return false;
		}
	}

	return CCDirector::replaceScene(i_scene);
}