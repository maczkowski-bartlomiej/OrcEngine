#pragma once

#include "Engine/Core.hpp"

#include "Graphics/Font.hpp"
#include "Graphics/Shader.hpp"
#include "Graphics/Texture.hpp"
#include "Graphics/Animator.hpp"

#include "Utility.hpp"

#include <string>
#include <string_view>

namespace orc {

template<typename ResourceType>
class ResourceHolder
{
public:
	ResourceHolder() = default;
	ResourceHolder(const FilePath& xmlPath);

	Ref<ResourceType> getResource(std::string_view name);

	bool loadResources(const FilePath& xmlPath);
	void clear();

private:
	std::unordered_map<std::string, Ref<ResourceType>, utility::string_view_hash, std::equal_to<>> m_resources;
};

using FontHolder = ResourceHolder<Font>;
using ShaderHolder = ResourceHolder<Shader>;
using TextureHolder = ResourceHolder<Texture>;
using AnimationHolder = ResourceHolder<Animation>;

}
