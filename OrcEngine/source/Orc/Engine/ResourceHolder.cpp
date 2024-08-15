#include "OrcPch.hpp"
#include "Engine/ResourceHolder.hpp"

#include <tinyxml2.h>

#include <sstream>

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
		return createRef<ResourceType>(); //Return default object
	}
}

template<typename ResourceType>
void ResourceHolder<ResourceType>::clear()
{
	m_resources.clear();
}

bool ResourceHolder<Font>::loadResources(const FilePath& xmlPath)
{
	ORC_LOG_INFO("Loading fonts...");

	tinyxml2::XMLDocument resourceFile;
	tinyxml2::XMLError errorResult = resourceFile.LoadFile(xmlPath.string().c_str());

	if (errorResult == tinyxml2::XMLError::XML_ERROR_EMPTY_DOCUMENT)
		errorResult = tinyxml2::XMLError::XML_SUCCESS;

	ORC_FATAL_CHECK(errorResult == tinyxml2::XMLError::XML_SUCCESS, "Failed to load XML file\n\tPath: {}\n\tReason: {}", xmlPath.string(), tinyxml2::XMLDocument::ErrorIDToName(errorResult));

	for (auto element = resourceFile.FirstChildElement("RESOURCE"); element != nullptr; element = element->NextSiblingElement("RESOURCE"))
	{
		const char* name = nullptr;
		const char* path = nullptr;

		errorResult = element->QueryStringAttribute("name", &name);
		ORC_FATAL_CHECK(errorResult == tinyxml2::XMLError::XML_SUCCESS, "Failed to read XML file\n\tPath: {}\n\tReason: {}", xmlPath.string(), tinyxml2::XMLDocument::ErrorIDToName(errorResult));

		errorResult = element->QueryStringAttribute("path", &path);
		ORC_FATAL_CHECK(errorResult == tinyxml2::XMLError::XML_SUCCESS, "Failed to read XML file\n\tPath: {}\n\tReason: {}", xmlPath.string(), tinyxml2::XMLDocument::ErrorIDToName(errorResult));

		Ref<Font> font = createRef<Font>();
		if (font->loadFromFile(path))
			m_resources[name] = font;
		else
			ORC_LOG_ERROR("Failed to load resource '{}'\n\tPath: '{}'", name, path);
	}

	return true; //Temporary without error checking
}

bool ResourceHolder<Shader>::loadResources(const FilePath& xmlPath)
{
	ORC_LOG_INFO("Loading shaders...");

	tinyxml2::XMLDocument resourceFile;
	tinyxml2::XMLError errorResult = resourceFile.LoadFile(xmlPath.string().c_str());

	if (errorResult == tinyxml2::XMLError::XML_ERROR_EMPTY_DOCUMENT)
		errorResult = tinyxml2::XMLError::XML_SUCCESS;

	ORC_FATAL_CHECK(errorResult == tinyxml2::XMLError::XML_SUCCESS, "Failed to load XML file\n\tPath: {}\n\tReason: {}", xmlPath.string(), tinyxml2::XMLDocument::ErrorIDToName(errorResult));

	for (auto element = resourceFile.FirstChildElement("RESOURCE"); element != nullptr; element = element->NextSiblingElement("RESOURCE"))
	{
		const char* name = nullptr;
		const char* path = nullptr;
		const char* path2 = nullptr;

		errorResult = element->QueryStringAttribute("name", &name);
		ORC_FATAL_CHECK(errorResult == tinyxml2::XMLError::XML_SUCCESS, "Failed to read XML file\n\tPath: {}\n\tReason: {}", xmlPath.string(), tinyxml2::XMLDocument::ErrorIDToName(errorResult));
		
		errorResult = element->QueryStringAttribute("path", &path);
		ORC_FATAL_CHECK(errorResult == tinyxml2::XMLError::XML_SUCCESS, "Failed to read XML file\n\tPath: {}\n\tReason: {}", xmlPath.string(), tinyxml2::XMLDocument::ErrorIDToName(errorResult));

		errorResult = element->QueryStringAttribute("path2", &path2);
		ORC_FATAL_CHECK(errorResult == tinyxml2::XMLError::XML_SUCCESS, "Failed to read XML file\n\tPath: {}\n\tReason: {}", xmlPath.string(), tinyxml2::XMLDocument::ErrorIDToName(errorResult));

		Ref<Shader> shader = createRef<Shader>();
		if (shader->loadFromFile(path, path2))
			m_resources[name] = shader;
		else
			ORC_LOG_ERROR("Failed to load resource '{}'\n\tPath: '{}'\n\tPath 2: '{}'", name, path, path2);
	}

	return true; //Temporary without error checking
}

bool ResourceHolder<Texture>::loadResources(const FilePath& xmlPath)
{
	ORC_LOG_INFO("Loading textures...");

	tinyxml2::XMLDocument resourceFile;
	tinyxml2::XMLError errorResult = resourceFile.LoadFile(xmlPath.string().c_str());

	if (errorResult == tinyxml2::XMLError::XML_ERROR_EMPTY_DOCUMENT)
		errorResult = tinyxml2::XMLError::XML_SUCCESS;

	ORC_FATAL_CHECK(errorResult == tinyxml2::XMLError::XML_SUCCESS, "Failed to load XML file\n\tPath: {}\n\tReason: {}", xmlPath.string(), tinyxml2::XMLDocument::ErrorIDToName(errorResult));

	for (auto element = resourceFile.FirstChildElement("RESOURCE"); element != nullptr; element = element->NextSiblingElement("RESOURCE"))
	{
		const char* name = nullptr;
		const char* path = nullptr;

		errorResult = element->QueryStringAttribute("name", &name);
		ORC_FATAL_CHECK(errorResult == tinyxml2::XMLError::XML_SUCCESS, "Failed to read XML file\n\tPath: {}\n\tReason: {}", xmlPath.string(), tinyxml2::XMLDocument::ErrorIDToName(errorResult));

		errorResult = element->QueryStringAttribute("path", &path);
		ORC_FATAL_CHECK(errorResult == tinyxml2::XMLError::XML_SUCCESS, "Failed to read XML file\n\tPath: {}\n\tReason: {}", xmlPath.string(), tinyxml2::XMLDocument::ErrorIDToName(errorResult));

		Ref<Texture> texture = createRef<Texture>();
		if (texture->loadFromFile(path))
			m_resources[name] = texture;
		else
			ORC_LOG_ERROR("Failed to load resource '{}'\n\tPath: '{}'", name, path);
	}

	return true; //Temporary without error checking
}

bool ResourceHolder<Animation>::loadResources(const FilePath& xmlPath)
{
	ORC_LOG_INFO("Loading animations...");

	tinyxml2::XMLDocument resourceFile;
	tinyxml2::XMLError errorResult = resourceFile.LoadFile(xmlPath.string().c_str());

	if (errorResult == tinyxml2::XMLError::XML_ERROR_EMPTY_DOCUMENT)
		errorResult = tinyxml2::XMLError::XML_SUCCESS;

	ORC_FATAL_CHECK(errorResult == tinyxml2::XMLError::XML_SUCCESS, "Failed to load XML file\n\tPath: {}\n\tReason: {}", xmlPath.string(), tinyxml2::XMLDocument::ErrorIDToName(errorResult));

	for (auto element = resourceFile.FirstChildElement("RESOURCE"); element != nullptr; element = element->NextSiblingElement("RESOURCE"))
	{
		const char* name = nullptr;
		const char* frame = nullptr;

		uint32_t durationMs = 0;
		std::vector<FloatRect> frames;

		errorResult = element->QueryStringAttribute("name", &name);
		ORC_FATAL_CHECK(errorResult == tinyxml2::XMLError::XML_SUCCESS, "Failed to read XML file\n\tPath: {}\n\tReason: {}", xmlPath.string(), tinyxml2::XMLDocument::ErrorIDToName(errorResult));

		int frameCount = 1;
		while (auto attribute = element->FindAttribute(std::string("frame" + std::to_string(frameCount)).c_str()))
		{
			frame = attribute->Value();
			std::stringstream ss(frame);
			std::vector<std::string> dimensionString;

			while (ss.good())
			{
				std::string substr;
				std::getline(ss, substr, ',');
				dimensionString.push_back(substr);
			}

			if (dimensionString.size() != 4)
			{
				ORC_LOG_ERROR("Invalid dimension for animation '{}'", name);
				break;
			}

			FloatRect rect;
			rect.left = std::stof(dimensionString[0]);
			rect.top = std::stof(dimensionString[1]);
			rect.right = std::stof(dimensionString[2]);
			rect.bottom = std::stof(dimensionString[3]);

			frames.push_back(rect);
			frameCount++;
		}

		errorResult = element->QueryUnsignedAttribute("durationMs", &durationMs);
		ORC_FATAL_CHECK(errorResult == tinyxml2::XMLError::XML_SUCCESS, "Failed to read XML file\n\tPath: {}\n\tReason: {}", xmlPath.string(), tinyxml2::XMLDocument::ErrorIDToName(errorResult));

		if (frames.size())
		{
			Ref<Animation> animation = createRef<Animation>(frames, durationMs);
			m_resources[name] = animation;
		}
		else
		{
			ORC_LOG_ERROR("Failed to load resource '{}'", name);
		}
	}

	return true; //Temporary without error checking
}

template class ResourceHolder<Font>;
template class ResourceHolder<Shader>;
template class ResourceHolder<Texture>;
template class ResourceHolder<Animation>;

}
