#pragma once

#include <string>
#include <string_view>
#include <unordered_map>

#include "Core/Core.hpp"
#include "Graphics/Animator.hpp"
#include "Graphics/Font.hpp"
#include "Graphics/Shader.hpp"
#include "Graphics/Texture.hpp"

#include "Utility.hpp"

namespace orc {

template <typename ResourceType>
class Resources
{
public:
	Resources() = default;
	explicit Resources(const FilePath& xmlPath);

	Ref<ResourceType> getResource(std::string_view name) const;

	bool loadResources(const FilePath& xmlPath);
	void clear();

private:
	std::unordered_map<std::string, Ref<ResourceType>, string_view_hash, std::equal_to<>> m_resources;
};

using FontResources = Resources<Font>;
using ShaderResources = Resources<Shader>;
using TextureResources = Resources<Texture>;
using AnimationResources = Resources<Animation>;

}
