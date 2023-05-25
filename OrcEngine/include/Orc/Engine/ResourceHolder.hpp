#pragma once

#include "Tools/Utility.hpp"
#include "System/System.hpp"

#include <string>
#include <string_view>

namespace orc {

template<typename ResourceType>
class ResourceHolder
{
public:
	ResourceHolder(const std::string& xmlPath);
	Reference<ResourceType> getResource(std::string_view name);

private:
	void loadResources(const std::string& xmlPath);

	utility::StringViewUnorderedMap<std::shared_ptr<ResourceType>> m_resources;
};

//using FontHolder = ResourceHolder<Font>;
//using TextureHolder = ResourceHolder<Texture>;
//using SoundBufferHolder = ResourceHolder<SoundBuffer>;

}
