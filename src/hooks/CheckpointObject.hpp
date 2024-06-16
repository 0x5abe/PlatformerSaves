#pragma once
#include <Geode/Geode.hpp>
#include <Geode/modify/CheckpointObject.hpp>
#include <sabe.persistenceutils/include/PersistenceUtils.hpp>

class $modify(PSCheckpointObject, CheckpointObject) {
protected:
	friend void operator>>(persistenceUtils::InputStream& i_stream, PSCheckpointObject& o_value);
	friend void operator<<(persistenceUtils::OutputStream& o_stream, PSCheckpointObject& i_value);

public:
	struct Fields {
		bool m_wasLoaded = false;
		cocos2d::CCPoint m_position;
		double m_timePlayed;
	};
	
	void load(persistenceUtils::InputStream& i_stream);
	void save(persistenceUtils::OutputStream& o_stream);

	void clean();

#if defined(PS_DEBUG) && defined(PS_DESCRIBE)
	void describe();
#endif
};