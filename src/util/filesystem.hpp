#pragma once
#include <Geode/Geode.hpp>

#define PSF_EXT ".psf"

namespace util::filesystem {
	inline std::string getParentDirectoryFromPath(const std::string& i_filePath)
	{
		size_t l_pos = i_filePath.find_last_of("\\/");
		return (std::string::npos == l_pos)
			? ""
			: i_filePath.substr(0, l_pos);
	}

	
	inline std::string getSaveFilePath(GJGameLevel* i_level, int i_slot, bool i_checkExists = false) {
		std::string l_filePath = geode::Mod::get()->getSaveDir().generic_string();
		std::string l_cleanLevelName = i_level->m_levelName;
		l_cleanLevelName.erase(std::remove(l_cleanLevelName.begin(), l_cleanLevelName.end(), '.'), l_cleanLevelName.end());
		l_cleanLevelName.erase(std::remove(l_cleanLevelName.begin(), l_cleanLevelName.end(), '/'), l_cleanLevelName.end());
		l_cleanLevelName.erase(std::remove(l_cleanLevelName.begin(), l_cleanLevelName.end(), '\\'), l_cleanLevelName.end());

		switch(i_level->m_levelType) {
			case GJLevelType::Local:
				l_filePath.append(fmt::format("/saves/local/{}/slot{}{}", i_level->m_levelID.value(), i_slot, PSF_EXT));
				break;
			case GJLevelType::Editor:
				l_filePath.append(fmt::format("/saves/editor/{}_rev{}/slot{}{}", l_cleanLevelName.c_str(), i_level->m_levelRev, i_slot, PSF_EXT));
				break;
			case GJLevelType::Saved:
			default:
				l_filePath.append(fmt::format("/saves/online/{}/slot{}{}", i_level->m_levelID.value(), i_slot, PSF_EXT));
				break;
		}
		//log::info("Filepath: \"{}\"", l_filePath);

		if (i_checkExists && !std::filesystem::exists(l_filePath)) {
			//log::info("File doesnt exist: {}", l_filePath);
			return "";
		}
		return l_filePath;
	}

	inline bool validSaveExists(GJGameLevel* i_level) {
		std::string l_filePath;
		for (int i = 0; i < 4; i++) {
			l_filePath = util::filesystem::getSaveFilePath(i_level, i, true);
			if (l_filePath != "") {
				return true;
			}
		}
		return false;
	}

		
	inline void removeSaveFile(GJGameLevel* i_level, int i_slot) {
		std::string l_filePath = getSaveFilePath(i_level, i_slot, true);
		if (l_filePath != "") {
			std::filesystem::remove(l_filePath);
		}
	}
}