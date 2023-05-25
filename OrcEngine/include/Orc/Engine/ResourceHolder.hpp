#pragma once

#include "Utility.hpp"

#include "Engine/Core.hpp"

#include "Graphics/Shader.hpp"
#include "Graphics/Texture.hpp"

#include <string>
#include <string_view>

namespace orc {

template<typename ResourceType>
class ResourceHolder
{
public:
	ResourceHolder(const FilePath& xmlPath);

	Reference<ResourceType> getResource(std::string_view name);

private:
	void loadResources(const FilePath& xmlPath);
	
	std::unordered_map<std::string, Reference<ResourceType>, utility::string_view_hash, std::equal_to<>> m_resources;
};

using ShaderHolder = ResourceHolder<Shader>;
using TextureHolder = ResourceHolder<Texture>;

//using FontHolder = ResourceHolder<Font>;
//using SoundBufferHolder = ResourceHolder<SoundBuffer>;

}
