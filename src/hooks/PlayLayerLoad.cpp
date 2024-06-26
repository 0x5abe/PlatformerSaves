#include "PlayLayer.hpp"
#include <filesystem>
#include <ui/PlayLevelMenuPopup.hpp>
#include <util/algorithm.hpp>
#include <util/filesystem.hpp>

using namespace geode::prelude;
using namespace persistenceAPI;

bool PSPlayLayer::readPsfLevelStringHash() {
	unsigned int l_savedLevelStringHash;

	m_fields->m_inputStream >> l_savedLevelStringHash;
	
	if (l_savedLevelStringHash != util::algorithm::hash_string(m_level->m_levelString.c_str())) {
		//log::info("[readPsfLevelStringHash] different levelstring hash");
		return false;
	}
	//log::info("[readPsfLevelStringHash] same levelstring hash");
	return true;
}

bool PSPlayLayer::readPsfVersion() {
	char l_psfMagicAndVer[sizeof(s_psfMagicAndVer)];

	m_fields->m_inputStream.read(l_psfMagicAndVer, sizeof(s_psfMagicAndVer));
	if (std::strncmp(s_psfMagicAndVer, l_psfMagicAndVer, sizeof(s_psfMagicAndVer))) {
		//log::info("[readPsfVersion] different version");
		return false;
	}
	//log::info("[readPsfVersion] same version");
	return true;
}

bool PSPlayLayer::readPsfFinishedSaving() {
	bool l_params[16-sizeof(s_psfMagicAndVer)];

	m_fields->m_inputStream.read(reinterpret_cast<char*>(l_params), 16-sizeof(s_psfMagicAndVer));
	if (l_params[0] == false) {
		//log::info("[readPsfFinishedSaving] did not finish writing");
		return false;
	}
	//log::info("[readPsfVersion] finished writing");
	return true;
}

void PSPlayLayer::loadGame() {
	switch (m_fields->m_loadingState) {
		case LoadingState::Setup: {
			m_fields->m_loadingProgress = 0.0f;

			PlayLevelMenuPopup::create()->show();
			m_fields->m_loadingState = LoadingState::WaitingForPlayLevelMenuPopup;
			// falls through
		}
		case LoadingState::WaitingForPlayLevelMenuPopup: {
			// Todo fix this so it's not ugly
			if (m_fields->m_saveSlot == -1) {
				break;
			}
			else if (m_fields->m_saveSlot == -2) {
				m_fields->m_saveSlot = 0;
				m_fields->m_loadingState = LoadingState::Ready;
				//m_fields->m_loadingState = LoadingState::CancelLevelLoad;
				break;
			}
			else if (m_fields->m_saveSlot == -3) {
				m_fields->m_saveSlot = 0;
				m_fields->m_loadingState = LoadingState::Ready;
				break;
			}
			else {
				m_fields->m_loadingState = LoadingState::SetupFileRead;
			}
			// falls through
		}
		case LoadingState::SetupFileRead: {
			std::string l_filePath = getSaveFilePath(-1, true);
			if (l_filePath == "") {
				m_fields->m_loadingState = LoadingState::Ready;
				m_fields->m_loadingProgress = 0.99f;
				break;
			}

			LevelInfoLayer* l_levelInfoLayer = static_cast<LevelInfoLayer*>(CCScene::get()->getChildByID("LevelInfoLayer"));
			if (l_levelInfoLayer) {
				l_levelInfoLayer->m_progressTimer->setColor(ccColor3B(220, 32, 64));
			}

			m_fields->m_bytesToRead = std::filesystem::file_size(l_filePath);
			m_fields->m_bytesRead = 0;
			if(m_fields->m_bytesToRead == 0 || !m_fields->m_inputStream.setFileToRead(l_filePath, &m_fields->m_bytesRead)) {
				m_fields->m_loadingState = LoadingState::HandleFileError;
				break;
			}

			m_fields->m_loadingState = LoadingState::ReadVersion;
			// falls through
		}
		case LoadingState::ReadVersion: {
			if (!readPsfVersion()) {
				m_fields->m_loadingState = LoadingState::HandleIncorrectVersion;
				break;
			}
			m_fields->m_loadingState = LoadingState::ReadFinishedSaving;
			// falls through
		}
		case LoadingState::ReadFinishedSaving: {
			if (!readPsfFinishedSaving()) {
				m_fields->m_loadingState = LoadingState::HandleDidNotFinishSaving;
				break;
			}
			m_fields->m_loadingState = LoadingState::ReadHash;
			// falls through
		}
		case LoadingState::ReadHash: {
			if (!readPsfLevelStringHash()) {
				m_fields->m_loadingState = LoadingState::HandleIncorrectHash;
				break;
			}
			m_fields->m_loadingState = LoadingState::ReadCheckpointCount;
			// falls through
		}
		case LoadingState::ReadCheckpointCount: {
			m_fields->m_inputStream >> m_fields->m_remainingCheckpointLoadCount;
			m_fields->m_loadingState = LoadingState::ReadCheckpoint;
			// falls through
		}
		case LoadingState::ReadCheckpoint: {
			if (m_fields->m_remainingCheckpointLoadCount > 0) {
				loadCheckpointFromStream();
				m_fields->m_remainingCheckpointLoadCount--;
			}
			if (m_fields->m_remainingCheckpointLoadCount == 0) {
				m_fields->m_loadingState = LoadingState::ReadTriggeredCheckpointGameObjects;
			}
			// falls through
		}
		case LoadingState::ReadTriggeredCheckpointGameObjects: {
			loadTriggeredCheckpointGameObjectsFromStream();
			m_fields->m_loadingState = LoadingState::ReadTimePlayed;
			// falls through
		}
		case LoadingState::ReadTimePlayed: {
			m_fields->m_loadingState = LoadingState::Ready;
			// falls through
		}
		case LoadingState::Ready: {
			endInputStream();
			if (m_fields->m_normalModeCheckpoints->count() > 0) {
				m_fields->m_lastSavedCheckpointTimestamp = static_cast<PSCheckpointObject*>(m_fields->m_normalModeCheckpoints->lastObject())->m_fields->m_timestamp;
			}
			registerCheckpointsAndTriggeredCheckpointGameObjects();
			//log::info("!!!!!!!!!!!!!!!! DO NOTHING");
			break;
		}
		case LoadingState::HandleFileError: {
			CCEGLView::get()->showCursor(true);
			m_fields->m_loadingState = LoadingState::WaitingForPopup;
			createQuickPopup("Error loading game",
				"The save file for this level could not be opened",
				"Cancel",
				"Ok",
				[&](FLAlertLayer*, bool i_btn2) {
					if (i_btn2) {
						m_fields->m_loadingState = LoadingState::Ready;
					} else {
						m_fields->m_loadingState = LoadingState::Ready;
						//Todo: Fix cancel level load for Vanilla platformers m_fields->m_loadingState = LoadingState::CancelLevelLoad;
					}
					CCEGLView::get()->showCursor(false);
					bool l_lockCursor = GameManager::get()->getGameVariable("0128");
					if (l_lockCursor) {
						CCEGLView::get()->toggleLockCursor(true);
					}
				}
			);
			break;
		}
		case LoadingState::HandleIncorrectVersion: {
			CCEGLView::get()->showCursor(true);
			//log::info("!!!!!!!!!!!!!!!! CREATED POPUP");
			m_fields->m_loadingState = LoadingState::WaitingForPopup;
			createQuickPopup("Error loading game",
				"The version of the save file does not match the current one. <cy>Try to load it anyways</c>? (<cr>this might be unstable or crash the game</c>)",
				"Cancel",
				"Ok",
				[&](FLAlertLayer*, bool i_btn2) {
					if (i_btn2) {
						m_fields->m_loadingState = LoadingState::UpdateVersion;
					} else {
						m_fields->m_loadingState = LoadingState::Ready;
					}
					CCEGLView::get()->showCursor(false);
					bool l_lockCursor = GameManager::get()->getGameVariable("0128");
					if (l_lockCursor) {
						CCEGLView::get()->toggleLockCursor(true);
					}
				}
			);
			break;
		}
		case LoadingState::UpdateVersion: {
			if (true) {
				m_fields->m_loadingState = LoadingState::ReadFinishedSaving;
			} else {
				m_fields->m_loadingState = LoadingState::Ready;
			}
			break;
		}
		case LoadingState::HandleDidNotFinishSaving: {
			CCEGLView::get()->showCursor(true);
			m_fields->m_loadingState = LoadingState::WaitingForPopup;
			createQuickPopup("Error loading game",
				"The save file for this level appears to be <cr>corrupted</c>",
				"Cancel",
				"Ok",
				[&](FLAlertLayer*, bool i_btn2) {
					if (i_btn2) {
						m_fields->m_loadingState = LoadingState::Ready;
					} else {
						m_fields->m_loadingState = LoadingState::Ready;
						//Todo: Fix cancel level load for Vanilla platformers m_fields->m_loadingState = LoadingState::CancelLevelLoad;
					}
					CCEGLView::get()->showCursor(false);
					bool l_lockCursor = GameManager::get()->getGameVariable("0128");
					if (l_lockCursor) {
						CCEGLView::get()->toggleLockCursor(true);
					}
				}
			);
			break;
		}
		case LoadingState::HandleIncorrectHash: {
			CCEGLView::get()->showCursor(true);
			m_fields->m_loadingState = LoadingState::WaitingForPopup;
			if (m_level->m_levelType == GJLevelType::Editor) {
				createQuickPopup("Error loading game",
					"The version of the level in the save file does not match the current one. <cy>A new game will be started</c>",
					"Ok",
					nullptr,
					[&](FLAlertLayer*, bool i_btn2) {
						removeSaveFile();
						m_fields->m_loadingState = LoadingState::Ready;
						CCEGLView::get()->showCursor(false);
						bool l_lockCursor = GameManager::get()->getGameVariable("0128");
						if (l_lockCursor) {
							CCEGLView::get()->toggleLockCursor(true);
						}
					}
				);
			} else {
				createQuickPopup("Error loading game",
					"The version of the level in the save file does not match the current one. <cy>Try to load it anyways</c>? (<cr>this might be unstable or crash the game</c>)",
					"Cancel",
					"Ok",
					[&](FLAlertLayer*, bool i_btn2) {
						if (i_btn2) {
							m_fields->m_loadingState = LoadingState::ReadCheckpointCount;
						} else {
							m_fields->m_loadingState = LoadingState::Ready;
						}
						CCEGLView::get()->showCursor(false);
						bool l_lockCursor = GameManager::get()->getGameVariable("0128");
						if (l_lockCursor) {
							CCEGLView::get()->toggleLockCursor(true);
						}
					}
				);
			}
			break;
		}
		case LoadingState::WaitingForPopup: {
			break;
		}
		case LoadingState::CancelLevelLoad: {
			m_fields->m_cancelLevelLoad = true;
			LevelInfoLayer* l_levelInfoLayer = static_cast<LevelInfoLayer*>(CCScene::get()->getChildByID("LevelInfoLayer"));
			if (l_levelInfoLayer) {
				l_levelInfoLayer->m_progressTimer->setVisible(false);
			}
			m_fields->m_loadingState = LoadingState::Ready;
			break;
		}
	}
}

void PSPlayLayer::loadTriggeredCheckpointGameObjectsFromStream() {
	unsigned int l_size;
	m_fields->m_inputStream.read(reinterpret_cast<char*>(&l_size), 4);
	if (l_size != 0) {
		m_fields->m_triggeredCheckpointGameObjects.resize(l_size);
		for (int i = 0; i < l_size; i++) {
			m_fields->m_triggeredCheckpointGameObjects[i].load(m_fields->m_inputStream);
		}
	}
}

void PSPlayLayer::loadCheckpointFromStream() {
	PSCheckpointObject* l_checkpoint = reinterpret_cast<PSCheckpointObject*>(CheckpointObject::create());
	l_checkpoint->load(m_fields->m_inputStream); 

	GameObject* l_newPhysicalCPO = GameObject::createWithFrame("square_01_001.png");
	CC_SAFE_RETAIN(l_newPhysicalCPO);
	l_newPhysicalCPO->m_objectID = 0x2c;
	l_newPhysicalCPO->m_objectType = GameObjectType::Decoration;
	l_newPhysicalCPO->m_glowSprite = nullptr;
	l_newPhysicalCPO->m_unk292 = true; // who knows
	l_newPhysicalCPO->m_unk3ef = true; // who knows
	l_newPhysicalCPO->setOpacity(0);

	// TODO FIX THIS OFFSET 0X3D4
	int* l_unkField1 = reinterpret_cast<int*>(reinterpret_cast<size_t>(l_newPhysicalCPO)+0x3d4);
	*l_unkField1 = 3;

	CC_SAFE_RELEASE(l_checkpoint->m_physicalCheckpointObject);
	l_checkpoint->m_physicalCheckpointObject = l_newPhysicalCPO;
	
	l_checkpoint->m_physicalCheckpointObject->setStartPos(l_checkpoint->m_fields->m_position);

	m_fields->m_normalModeCheckpoints->addObject(l_checkpoint);
}

void PSPlayLayer::updateAsyncProcessCreateObjectsFromSetup() {
	cocos2d::SEL_CallFunc l_sel = callfunc_selector(PSPlayLayer::updateAsyncProcessCreateObjectsFromSetup);
	PSPlayLayer::processCreateObjectsFromSetup();
	if (m_loadingProgress >= 1.0) {
		//log::info("Finished loading!!!!!");
		l_sel = callfunc_selector(PSPlayLayer::endAsyncProcessCreateObjectsFromSetup);
	}
	 
	CCScene* l_currentScene = CCScene::get();
	if (l_currentScene) {
		//log::info("Loop load!!!!!");
		l_currentScene->runAction(
			CCSequence::create(
				CCDelayTime::create(0.0f),
				CCCallFunc::create(
					this,
					l_sel
				),
				nullptr
			)
		);
	}
}

void PSPlayLayer::endAsyncProcessCreateObjectsFromSetup() {
	if (m_fields->m_transitionFadeScene) {
		CCDirector::get()->replaceScene(m_fields->m_transitionFadeScene);
		CC_SAFE_RELEASE(m_fields->m_transitionFadeScene);
		m_fields->m_transitionFadeScene = nullptr;
	}
}

void PSPlayLayer::endInputStream() {
	m_fields->m_inputStream.end();
}