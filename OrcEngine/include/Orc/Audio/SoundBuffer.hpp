#pragma once

#include "Engine/Core.hpp"

namespace orc {

class SoundBuffer
{
public:
	SoundBuffer();
	SoundBuffer(const FilePath& filePath);
	~SoundBuffer();

	bool loadFromFile(const FilePath& filePath);

	AudioID getAudioID() const;

private:
	AudioID m_audioID;
};

}
