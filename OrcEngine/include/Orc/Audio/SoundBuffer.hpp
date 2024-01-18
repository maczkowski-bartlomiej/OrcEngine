#pragma once

#include "Engine/Core.hpp"

namespace orc {

class SoundBuffer
{
public:
	SoundBuffer() = default;
	SoundBuffer(const FilePath& filePath);
	~SoundBuffer();

	bool loadFromFile(const FilePath& filePath);

	AudioID getAudioID() const;

private:
	AudioID m_audioID = 0;
};

}
