#include "PlayLayer.hpp"
#include "domain/CheckpointGameObjectReference.hpp"
#include <filesystem>
#include <geode.custom-keybinds/include/Keybinds.hpp>
#include <util/algorithm.hpp>
#include <util/filesystem.hpp>

using namespace geode::prelude;
using namespace persistencyUtils;

PSPlayLayer* s_currentPlayLayer = nullptr;
char s_psfMagicAndVer[] = "PSF v0.0.3";

// overrides

bool PSPlayLayer::init(GJGameLevel* i_level, bool i_useReplay, bool i_dontCreateObjects) {
	// for processing objects asynchronously every time
	s_currentPlayLayer = this;
	m_fields->m_signalForAsyncLoad = !i_dontCreateObjects;
	if (m_fields->m_signalForAsyncLoad) {
		m_loadingProgress = 1.0f;
	}
	m_fields->m_normalModeCheckpoints = cocos2d::CCArray::create();

	if (!PlayLayer::init(i_level, i_useReplay, i_dontCreateObjects)) return false;

	// for processing objects asynchronously every time
	if (m_fields->m_signalForAsyncLoad) {
		m_loadingProgress = 0.0f;
	}
	setupKeybinds();
	setupSavingIcon();

	return true;
}

void PSPlayLayer::createObjectsFromSetupFinished() {
	if (m_fields->m_loadingState == LoadingState::Setup) {
		PlayLayer::createObjectsFromSetupFinished();
	}
}

void PSPlayLayer::setupHasCompleted() {
	//log::info("[setupHasCompleted] begin");
	if (!m_isPlatformer) {
		m_fields->m_loadingState = LoadingState::Ready;
	}
	if (m_fields->m_loadingState != LoadingState::Ready) {
		log::info("[setupHasCompleted] hasnt finished loading checkpoints");

		loadGame();
		
		//m_fields->m_loadingProgress += (m_fields->m_loadingProgress/8.0f);
		//log::info("[setupHasCompleted] m_bytesRead: {}", m_fields->m_bytesRead);
		//log::info("[setupHasCompleted] m_bytesToRead: {}", m_fields->m_bytesToRead);
		if (m_fields->m_bytesToRead > 0) {
			m_fields->m_loadingProgress = (static_cast<float>(m_fields->m_bytesRead)/static_cast<float>(m_fields->m_bytesToRead));
		}
		// if (m_fields->m_loadingProgress != 1.0f) {
		m_loadingProgress = m_fields->m_loadingProgress;
		// } else {
		// 	m_loadingProgress = 0.99f;
		// }
		//log::info("[setupHasCompleted] m_loadingProgress: {}", m_loadingProgress);
	}
	if (m_fields->m_loadingState == LoadingState::Ready && !m_fields->m_cancelLevelLoad) {
		//log::info("[setupHasCompleted] finished loading SP");
		m_loadingProgress = 1.0f;

		// now reset the order of arrival that we're actually going into the level
		CCNode::resetGlobalOrderOfArrival();

		PlayLayer::setupHasCompleted();

		m_fields->m_loadingProgress = 0.0f;
		m_fields->m_bytesToRead = 0;
		m_fields->m_bytesRead = 0;
	}
}

void PSPlayLayer::onQuit() {
	if (!m_fields->m_onQuitCalled) {
		m_fields->m_onQuitCalled = true;
	}
	PlayLayer::onQuit();
	s_currentPlayLayer = nullptr;
}

void PSPlayLayer::postUpdate(float i_unkFloat) {
	m_fields->m_inPostUpdate = true;
	m_fields->m_triedPlacingCheckpoint = m_tryPlaceCheckpoint;

	PlayLayer::postUpdate(i_unkFloat);
	
	m_fields->m_inPostUpdate = false;
	m_fields->m_triedPlacingCheckpoint = false;

	log::info("m_checkpointArray count: {}", m_checkpointArray->count());
	// log::info("m_gameState->m_dynamicObjActions1 size: {}", m_gameState.m_dynamicObjActions1.size());
	// for (int i = 0; i < m_gameState.m_dynamicObjActions1.size(); i++) {
	// 	log::info("m_dynamicObjActions1[{}]->m_gameObject1: {}", i, reinterpret_cast<uint64_t>(m_gameState.m_dynamicObjActions1[i].m_gameObject1));
	// 	log::info("m_dynamicObjActions1[{}]->m_gameObject2: {}", i, reinterpret_cast<uint64_t>(m_gameState.m_dynamicObjActions1[i].m_gameObject2));
	// 	log::info("m_dynamicObjActions1[{}]->m_gameObject3: {}", i, reinterpret_cast<uint64_t>(m_gameState.m_dynamicObjActions1[i].m_gameObject3));
	// 	log::info("m_dynamicObjActions1[{}]->m_gameObject4: {}", i, reinterpret_cast<uint64_t>(m_gameState.m_dynamicObjActions1[i].m_gameObject4));
	// 	log::info("m_dynamicObjActions1[{}]->m_gameObject5: {}", i, reinterpret_cast<uint64_t>(m_gameState.m_dynamicObjActions1[i].m_gameObject5));
	// 	log::info("m_dynamicObjActions1[{}]->m_gameObject6: {}", i, reinterpret_cast<uint64_t>(m_gameState.m_dynamicObjActions1[i].m_gameObject6));
	// 	log::info("m_dynamicObjActions1[{}]->m_gameObject7: {}", i, reinterpret_cast<uint64_t>(m_gameState.m_dynamicObjActions1[i].m_gameObject7));
	// 	log::info("m_dynamicObjActions1[{}]->m_gameObject8: {}", i, reinterpret_cast<uint64_t>(m_gameState.m_dynamicObjActions1[i].m_gameObject8));
	// 	if (m_gameState.m_dynamicObjActions1[i].m_gameObject1 != nullptr) log::info("m_dynamicObjActions1[{}]->m_gameObject1->m_uniqueID: {}", i, m_gameState.m_dynamicObjActions1[i].m_gameObject1->m_uniqueID);
	// 	if (m_gameState.m_dynamicObjActions1[i].m_gameObject2 != nullptr) log::info("m_dynamicObjActions1[{}]->m_gameObject2->m_uniqueID: {}", i, m_gameState.m_dynamicObjActions1[i].m_gameObject2->m_uniqueID);
	// 	if (m_gameState.m_dynamicObjActions1[i].m_gameObject3 != nullptr) log::info("m_dynamicObjActions1[{}]->m_gameObject3->m_uniqueID: {}", i, m_gameState.m_dynamicObjActions1[i].m_gameObject3->m_uniqueID);
	// 	if (m_gameState.m_dynamicObjActions1[i].m_gameObject4 != nullptr) log::info("m_dynamicObjActions1[{}]->m_gameObject4->m_uniqueID: {}", i, m_gameState.m_dynamicObjActions1[i].m_gameObject4->m_uniqueID);
	// 	if (m_gameState.m_dynamicObjActions1[i].m_gameObject5 != nullptr) log::info("m_dynamicObjActions1[{}]->m_gameObject5->m_uniqueID: {}", i, m_gameState.m_dynamicObjActions1[i].m_gameObject5->m_uniqueID);
	// 	if (m_gameState.m_dynamicObjActions1[i].m_gameObject6 != nullptr) log::info("m_dynamicObjActions1[{}]->m_gameObject6->m_uniqueID: {}", i, m_gameState.m_dynamicObjActions1[i].m_gameObject6->m_uniqueID);
	// 	if (m_gameState.m_dynamicObjActions1[i].m_gameObject7 != nullptr) log::info("m_dynamicObjActions1[{}]->m_gameObject7->m_uniqueID: {}", i, m_gameState.m_dynamicObjActions1[i].m_gameObject7->m_uniqueID);
	// 	if (m_gameState.m_dynamicObjActions1[i].m_gameObject8 != nullptr) log::info("m_dynamicObjActions1[{}]->m_gameObject8->m_uniqueID: {}", i, m_gameState.m_dynamicObjActions1[i].m_gameObject8->m_uniqueID);
		
	// }
	// log::info("m_gameState->m_dynamicObjActions2 size: {}", m_gameState.m_dynamicObjActions2.size());
	// for (int i = 0; i < m_gameState.m_dynamicObjActions2.size(); i++) {
	// 	log::info("m_dynamicObjActions2[{}]->m_gameObject1: {}", i, reinterpret_cast<uint64_t>(m_gameState.m_dynamicObjActions2[i].m_gameObject1));
	// 	log::info("m_dynamicObjActions2[{}]->m_gameObject2: {}", i, reinterpret_cast<uint64_t>(m_gameState.m_dynamicObjActions2[i].m_gameObject2));
	// 	log::info("m_dynamicObjActions2[{}]->m_gameObject3: {}", i, reinterpret_cast<uint64_t>(m_gameState.m_dynamicObjActions2[i].m_gameObject3));
	// 	log::info("m_dynamicObjActions2[{}]->m_gameObject4: {}", i, reinterpret_cast<uint64_t>(m_gameState.m_dynamicObjActions2[i].m_gameObject4));
	// 	log::info("m_dynamicObjActions2[{}]->m_gameObject5: {}", i, reinterpret_cast<uint64_t>(m_gameState.m_dynamicObjActions2[i].m_gameObject5));
	// 	log::info("m_dynamicObjActions2[{}]->m_gameObject6: {}", i, reinterpret_cast<uint64_t>(m_gameState.m_dynamicObjActions2[i].m_gameObject6));
	// 	log::info("m_dynamicObjActions2[{}]->m_gameObject7: {}", i, reinterpret_cast<uint64_t>(m_gameState.m_dynamicObjActions2[i].m_gameObject7));
	// 	log::info("m_dynamicObjActions2[{}]->m_gameObject8: {}", i, reinterpret_cast<uint64_t>(m_gameState.m_dynamicObjActions2[i].m_gameObject8));
	// 	if (m_gameState.m_dynamicObjActions2[i].m_gameObject1 != nullptr) log::info("m_dynamicObjActions2[{}]->m_gameObject1->m_uniqueID: {}", i, m_gameState.m_dynamicObjActions2[i].m_gameObject1->m_uniqueID);
	// 	if (m_gameState.m_dynamicObjActions2[i].m_gameObject2 != nullptr) log::info("m_dynamicObjActions2[{}]->m_gameObject2->m_uniqueID: {}", i, m_gameState.m_dynamicObjActions2[i].m_gameObject2->m_uniqueID);
	// 	if (m_gameState.m_dynamicObjActions2[i].m_gameObject3 != nullptr) log::info("m_dynamicObjActions2[{}]->m_gameObject3->m_uniqueID: {}", i, m_gameState.m_dynamicObjActions2[i].m_gameObject3->m_uniqueID);
	// 	if (m_gameState.m_dynamicObjActions2[i].m_gameObject4 != nullptr) log::info("m_dynamicObjActions2[{}]->m_gameObject4->m_uniqueID: {}", i, m_gameState.m_dynamicObjActions2[i].m_gameObject4->m_uniqueID);
	// 	if (m_gameState.m_dynamicObjActions2[i].m_gameObject5 != nullptr) log::info("m_dynamicObjActions2[{}]->m_gameObject5->m_uniqueID: {}", i, m_gameState.m_dynamicObjActions2[i].m_gameObject5->m_uniqueID);
	// 	if (m_gameState.m_dynamicObjActions2[i].m_gameObject6 != nullptr) log::info("m_dynamicObjActions2[{}]->m_gameObject6->m_uniqueID: {}", i, m_gameState.m_dynamicObjActions2[i].m_gameObject6->m_uniqueID);
	// 	if (m_gameState.m_dynamicObjActions2[i].m_gameObject7 != nullptr) log::info("m_dynamicObjActions2[{}]->m_gameObject7->m_uniqueID: {}", i, m_gameState.m_dynamicObjActions2[i].m_gameObject7->m_uniqueID);
	// 	if (m_gameState.m_dynamicObjActions2[i].m_gameObject8 != nullptr) log::info("m_dynamicObjActions2[{}]->m_gameObject8->m_uniqueID: {}", i, m_gameState.m_dynamicObjActions2[i].m_gameObject8->m_uniqueID);
	// }
	// log::info("m_player1: {}", reinterpret_cast<uint64_t>(m_player1));
	// log::info("m_player1->m_uniqueID: {}", m_player1->m_uniqueID);
	// log::info("m_player2: {}", reinterpret_cast<uint64_t>(m_player2));
	// log::info("m_player2->m_uniqueID: {}", m_player2->m_uniqueID);
}

CheckpointObject* PSPlayLayer::markCheckpoint() {
	CheckpointObject* l_checkpointObject = PlayLayer::markCheckpoint();

	if (m_fields->m_inPostUpdate) {
		if (m_fields->m_triedPlacingCheckpoint) {
			m_fields->m_triedPlacingCheckpoint = false;
		} else if (m_triggeredCheckpointGameObject != nullptr) {
			m_fields->m_normalModeCheckpoints->addObject(l_checkpointObject);
			m_fields->m_triggeredCheckpointGameObjects.push_back(CheckpointGameObjectReference(m_triggeredCheckpointGameObject));
		}
	}

	return l_checkpointObject;
}

// custom methods

std::string PSPlayLayer::getSaveFilePath(bool i_checkExists) {
	if (m_fields->m_saveSlot == -1) {
		return "";
	}

	std::string l_filePath = Mod::get()->getSaveDir().generic_string();
	
	switch(m_level->m_levelType) {
		case GJLevelType::Local:
			l_filePath.append(std::format("/saves/local/{}/slot{}{}", m_level->m_levelID.value(), m_fields->m_saveSlot, PSF_EXT));
			break;
		case GJLevelType::Editor:
			l_filePath.append(std::format("/saves/editor/{}_rev{}/slot{}{}", m_level->m_levelName.c_str(), m_level->m_levelRev, m_fields->m_saveSlot, PSF_EXT));
			break;
		case GJLevelType::Saved:
		default:
			l_filePath.append(std::format("/saves/online/{}/slot{}{}", m_level->m_levelID.value(), m_fields->m_saveSlot, PSF_EXT));
			break;
	}
	//log::info("Filepath: \"{}\"", l_filePath);
	if (i_checkExists && !std::filesystem::exists(l_filePath)) {
		//log::info("File doesnt exist: {}", l_filePath);
		return "";
	}
	return l_filePath;
}

void PSPlayLayer::setupKeybinds() {
	// TodoRemove
	addEventListener<keybinds::InvokeBindFilter>(
		[this](keybinds::InvokeBindEvent* event) {
			if (event->isDown()) {
				log::info("m_isPlatformer: {}", m_isPlatformer);
				if (m_fields->m_savingState != SavingState::Ready) return ListenerResult::Propagate;
				m_fields->m_savingState = SavingState::Setup;
				CCScene* l_currentScene = CCScene::get();
				if (l_currentScene) {
					l_currentScene->runAction(
						CCSequence::create(
							CCDelayTime::create(0.0f),
							CCCallFunc::create(
								this,
								callfunc_selector(PSPlayLayer::saveGame)
							),
							nullptr
						)
					);
				}
			}
			return ListenerResult::Propagate;
		},
		"test-key-2"_spr
	);
	// EndTodo
}

void PSPlayLayer::setupSavingIcon() {
	CCSize l_winSize = CCDirector::sharedDirector()->getWinSize();
	float l_separation = l_winSize.height/10;

	m_fields->m_savingIcon = CCSprite::create("loadingCircle.png");
	m_fields->m_savingIcon->setBlendFunc({ GL_ONE, GL_ONE });
	m_fields->m_savingIcon->setPosition({l_winSize.width-l_separation,l_separation});
	m_fields->m_savingIcon->setZOrder(100);
	m_fields->m_savingIcon->setScale(0.5f);

	m_fields->m_savingIcon->runAction(CCRepeatForever::create(CCRotateBy::create(1.0f, 360.f)));
	m_fields->m_savingIcon->pauseSchedulerAndActions();
	//m_fields->m_savingIcon->setVisible(false);
}

void PSPlayLayer::showSavingIcon(bool i_show) {
	CCScene* l_currentScene = CCScene::get();
	if (!l_currentScene || !m_fields->m_savingIcon) return;
	if (i_show) {
		if (!l_currentScene->getChildren()->containsObject(m_fields->m_savingIcon)) {
			l_currentScene->addChild(m_fields->m_savingIcon);
		}
		m_fields->m_savingIcon->resumeSchedulerAndActions();
		m_fields->m_savingIcon->setVisible(true);
		return;	
	}
	m_fields->m_savingIcon->pauseSchedulerAndActions();
	m_fields->m_savingIcon->setVisible(false);
}