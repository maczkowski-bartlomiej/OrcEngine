#include "OrcPch.hpp"

#include "Engine/ResourceHolder.hpp"

#include <tinyxml2.h>

namespace orc {

template<typename ResourceType>
ResourceHolder<ResourceType>::ResourceHolder(const FilePath& xmlPath)
{
	loadResources(xmlPath);
}

template<typename ResourceType>
Ref<ResourceType> ResourceHolder<ResourceType>::getResource(std::string_view name) 
{
	if (auto find = m_resources.find(name); find != m_resources.end())
	{
		return find->second;
	}
	else
	{
		ORC_LOG_ERROR("Requested non-existing resource '{}'", name);
		return createRef<ResourceType>(); //Return empty object
	}

}
template<typename ResourceType>
void ResourceHolder<ResourceType>::loadResources(const FilePath& xmlPath)
{

}

void ResourceHolder<Shader>::loadResources(const FilePath& xmlPath)
{
	tinyxml2::XMLDocument resourceFile;
	tinyxml2::XMLError errorResult = resourceFile.LoadFile(xmlPath.string().c_str());
	ORC_ERROR_CHECK(errorResult == tinyxml2::XMLError::XML_SUCCESS, "Fatal occured while loading XML file\n\tpath: {}\n\treason: {}", xmlPath.string(), tinyxml2::XMLDocument::ErrorIDToName(errorResult));

	for (auto element = resourceFile.FirstChildElement("RESOURCE"); element != nullptr; element = element->NextSiblingElement("RESOURCE"))
	{
		const char* name = nullptr;
		const char* path = nullptr;
		const char* path2 = nullptr;

		errorResult = element->QueryStringAttribute("name", &name);
		ORC_ERROR_CHECK(errorResult == tinyxml2::XMLError::XML_SUCCESS, "Fatal occured while reading XML file\n\tpath: {}\n\treason: {}", xmlPath.string(), tinyxml2::XMLDocument::ErrorIDToName(errorResult));

		errorResult = element->QueryStringAttribute("path", &path);
		ORC_ERROR_CHECK(errorResult == tinyxml2::XMLError::XML_SUCCESS, "Fatal occured while reading XML file\n\tpath: {}\n\treason: {}", xmlPath.string(), tinyxml2::XMLDocument::ErrorIDToName(errorResult));

		errorResult = element->QueryStringAttribute("path2", &path2);
		ORC_ERROR_CHECK(errorResult == tinyxml2::XMLError::XML_SUCCESS, "Fatal occured while reading XML file\n\tpath: {}\n\treason: {}", xmlPath.string(), tinyxml2::XMLDocument::ErrorIDToName(errorResult));

		Ref<Shader> shader = createRef<Shader>();
		if (shader->loadFromFile(path, path2))
			m_resources[name] = shader;
		else
			ORC_LOG_ERROR("Couldn't load resource '{}'\n\tpath: '{}'\n\tpath 2: '{}'", name, path, path2);
	}
}

//todo: name: {} path: {}
void ResourceHolder<Texture>::loadResources(const FilePath& xmlPath)
{
	tinyxml2::XMLDocument resourceFile;
	tinyxml2::XMLError errorResult = resourceFile.LoadFile(xmlPath.string().c_str());
	ORC_ERROR_CHECK(errorResult == tinyxml2::XMLError::XML_SUCCESS, "Fatal occured while loading XML file\n\tpath: {}\n\treason: {}", xmlPath.string(), tinyxml2::XMLDocument::ErrorIDToName(errorResult));

	for (auto element = resourceFile.FirstChildElement("RESOURCE"); element != nullptr; element = element->NextSiblingElement("RESOURCE"))
	{
		const char* name = nullptr;
		const char* path = nullptr;

		errorResult = element->QueryStringAttribute("name", &name);
		ORC_ERROR_CHECK(errorResult == tinyxml2::XMLError::XML_SUCCESS, "Fatal occured while reading XML file\n\tpath: {}\n\treason: {}", xmlPath.string(), tinyxml2::XMLDocument::ErrorIDToName(errorResult));

		errorResult = element->QueryStringAttribute("path", &path);
		ORC_ERROR_CHECK(errorResult == tinyxml2::XMLError::XML_SUCCESS, "Fatal occured while reading XML file\n\tpath: {}\n\treason: {}", xmlPath.string(), tinyxml2::XMLDocument::ErrorIDToName(errorResult));

		Ref<Texture> texture = createRef<Texture>();
		if (texture->loadFromFile(path))
			m_resources[name] = texture;
		else
			ORC_LOG_ERROR("Couldn't load resource '{}'\n\tpath: '{}'", name, path);
	}
}

template class ResourceHolder<Shader>;
template class ResourceHolder<Texture>;

}
