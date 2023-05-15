#include "orcPch.hpp"
#include "Engine/ResourceHolder.hpp"

#include "Tools/Logger.hpp"

#include <tinyxml2.h>

namespace orc {

template<typename ResourceType>
ResourceHolder<ResourceType>::ResourceHolder(const std::string& xmlPath)
{
	loadResources(xmlPath);
}

template<typename ResourceType>
std::shared_ptr<ResourceType> ResourceHolder<ResourceType>::getResource(std::string_view name) 
{
	if (auto find = m_resources.find(); find != m_resources.end())
	{
		return find->second;
	}
	else
	{
		ORC_CORE_LOG_WARNING("Requested non-existing resource\n\tname: {}", name);
		return std::make_shared<ResourceType>();
	}
}

template<typename ResourceType>
void ResourceHolder<ResourceType>::loadResources(const std::string& xmlPath)
{
	tinyxml2::XMLDocument resourceFile;

	tinyxml2::XMLError errorResult{ resourceFile.LoadFile(xmlPath.c_str()) };
	ORC_CORE_THROW_RUNTIME_ERROR_CHECK(errorResult != tinyxml2::XMLError::XML_SUCCESS, "Fatal occured while loading XML file\n\tpath: {}\n\treason: {}", xmlPath, tinyxml2::XMLDocument::ErrorIDToName(errorResult));

	for (auto element = resourceFile.FirstChildElement("RESOURCE"); element != nullptr; element = resourceFile.NextSiblingElement("RESOURCE"))
	{
		const char* name, * path;

		errorResult = element->QueryStringAttribute("name", &name);
		ORC_CORE_THROW_RUNTIME_ERROR_CHECK(errorResult != tinyxml2::XMLError::XML_SUCCESS, "Fatal occured while reading XML file\n\tpath: {}\n\treason: {}", xmlPath, tinyxml2::XMLDocument::ErrorIDToName(errorResult));

		errorResult = element->QueryStringAttribute("path", &path);
		ORC_CORE_THROW_RUNTIME_ERROR_CHECK(errorResult != tinyxml2::XMLError::XML_SUCCESS, "Fatal occured while reading XML file\n\tpath: {}\n\treason: {}", xmlPath, tinyxml2::XMLDocument::ErrorIDToName(errorResult));
		
		std::shared_ptr<ResourceType> resource = std::make_shared<ResourceType>();
		if (!resource->loadFromFile(path))
		{
			ORC_CORE_LOG_WARNING("Failed to load resource\n\tpath: {}\n\tname: {}", path, name);
		}
		else
		{
			m_resources[name] = resource;
		}
	}
}

//template class ResourceHolder<Font>;
//template class ResourceHolder<Texture>;
//template class ResourceHolder<SoundBuffer>;

}
