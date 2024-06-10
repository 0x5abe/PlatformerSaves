#pragma once
#include <Geode/Geode.hpp>
#include <Geode/modify/CheckpointObject.hpp>
#include <sabe.persistencyutils/include/PersistencyUtils.hpp>

class $modify(PSCheckpointObject, CheckpointObject) {
protected:
	friend void operator>>(persistencyUtils::InputStream& i_stream, PSCheckpointObject& o_value);
	friend void operator<<(persistencyUtils::OutputStream& o_stream, PSCheckpointObject& i_value);

public:
	struct Fields {
		bool m_wasLoaded = false;
		cocos2d::CCPoint m_position;
	};
	
	void load(persistencyUtils::InputStream& i_stream);
	void save(persistencyUtils::OutputStream& o_stream);

	void clean();

#if defined(PS_DEBUG) && defined(PS_DESCRIBE)
	void describe();
#endif
};