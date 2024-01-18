#pragma once

#include "Engine/Core.hpp"

#include <vector>

namespace orc {

class MusicBuffer
{
public:
	MusicBuffer() = default;
	MusicBuffer(const FilePath& filePath);
	~MusicBuffer();

	bool loadFromFile(const FilePath& filePath);

private:
	static constexpr uint32_t BUFFER_SIZE = 8192;
	static constexpr uint32_t BUFFER_COUNT = 4;

	AudioID m_audioIDS[BUFFER_COUNT] = {};
	std::vector<short> m_buffer;
};

}
