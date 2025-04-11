#pragma once
#include "Geode/modify/Modify.hpp"
#include <Geode/Geode.hpp>
#include <Geode/modify/FMODAudioEngine.hpp>
#include <sabe.persistenceapi/include/PersistenceAPI.hpp>

class $modify(PSFMODAudioEngine, FMODAudioEngine) {
public:
	struct Fields {
		bool m_disableFadeOutMusic;
		bool m_disableLoadMusic;
		FMOD::ChannelGroup* m_backgroundMusicChannelBackup;
		gd::unordered_map<int,float> m_unkMapIntFloat1Backup;
		gd::unordered_map<int,float> m_unkMapIntFloat2Backup;
		gd::unordered_map<int,float> m_unkMapIntFloat3Backup;
	};

	// overrides

	$override
	void fadeOutMusic(float i_time, int i_channel);

	$override
	void loadMusic(gd::string i_path, float i_speed, float i_p2, float i_volume, bool i_shouldLoop, int i_p5, int i_p6, bool i_p7);

	// DEBUG STUFF
	// $override
	// void queueStartMusic(gd::string audioFilename, float i_p2, float i_p3, float i_p4, bool i_p5, int ms, int i_p6, int i_p7, int i_p8, int i_p9, bool i_p10, int i_p11, bool i_p12, bool i_p13);

	// custom methods

	void backupMusic();

	void restoreMusic();
};