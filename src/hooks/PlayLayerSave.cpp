#include "PlayLayer.hpp"
#include <filesystem>
#include <util/algorithm.hpp>
#include <util/filesystem.hpp>

using namespace geode::prelude;
using namespace persistencyUtils;

void PSPlayLayer::writePsfHeader() {
	m_fields->m_outputStream.write(s_psfMagicAndVer,sizeof(s_psfMagicAndVer));
	m_fields->m_outputStream.writeZero(16-sizeof(s_psfMagicAndVer));
	unsigned int l_levelStringHash = util::algorithm::hash_string(m_level->m_levelString.c_str());
	m_fields->m_outputStream << l_levelStringHash;
}

void PSPlayLayer::saveGame() {
	//log::info("SaveGame Gets run");
	switch (m_fields->m_savingState) {
		case SavingState::Setup: {
			//log::info("Goes into beginning");
			
			// Todo: get last checkpoint in array and save it ONLY in normal mode
			if (m_fields->m_normalModeCheckpoints->count() == 0) {
				m_fields->m_savingState = SavingState::Ready;
				break;
			}
			m_fields->m_remainingCheckpointSaveCount = m_fields->m_normalModeCheckpoints->count();
		
			std::string l_filePath = getSaveFilePath();
			std::string l_fileDirectory = util::filesystem::getParentDirectoryFromPath(l_filePath);
			if (!std::filesystem::exists(l_fileDirectory) && !std::filesystem::create_directories(l_fileDirectory)) {
				m_fields->m_savingState = SavingState::Ready;
				break;
			}

			if (!m_fields->m_outputStream.setFileToWrite(l_filePath)) {
				m_fields->m_savingState = SavingState::Ready;
				break;
			}

			showSavingIcon(true);
			
			writePsfHeader();
			
			m_fields->m_outputStream << m_fields->m_remainingCheckpointSaveCount;

			m_fields->m_savingState = SavingState::Started;
			// falls through
		}
		case SavingState::Started: {
			if (m_fields->m_remainingCheckpointSaveCount > 0) {
				saveCheckpointToStream(m_fields->m_normalModeCheckpoints->count()-m_fields->m_remainingCheckpointSaveCount);
				m_fields->m_remainingCheckpointSaveCount--;
				//log::info("Remaining save count: {}", m_fields->m_remainingCheckpointSaveCount);
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
				break;
			}
			if (m_fields->m_remainingCheckpointSaveCount == 0) {
				m_fields->m_outputStream.seek(sizeof(s_psfMagicAndVer));
				bool o_finishedSaving = true;
				m_fields->m_outputStream.write((char*)&o_finishedSaving,sizeof(bool));
				endOutputStream();
				m_fields->m_savingState = SavingState::Ready;
			}
			// Todo use tasks maybe
			
		}
		case SavingState::Ready: {
			showSavingIcon(false);
			break;
		}
	}
}

void PSPlayLayer::saveCheckpointToStream(unsigned int i_index) {
	//log::info("Saving Startpoints to stream");
	static_cast<PSCheckpointObject*>(m_fields->m_normalModeCheckpoints->objectAtIndex(i_index))->save(m_fields->m_outputStream);
	//log::info("Saved Startpoints to stream");
}

void PSPlayLayer::endOutputStream() {
	m_fields->m_outputStream.end();
}
