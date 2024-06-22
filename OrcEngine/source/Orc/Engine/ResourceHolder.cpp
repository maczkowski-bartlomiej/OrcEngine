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

void ResourceHolder<Font>::loadResources(const FilePath& xmlPath)
{
	tinyxml2::XMLDocument resourceFile;
	tinyxml2::XMLError errorResult = resourceFile.LoadFile(xmlPath.string().c_str());

	if (errorResult == tinyxml2::XMLError::XML_ERROR_EMPTY_DOCUMENT)
		errorResult = tinyxml2::XMLError::XML_SUCCESS;

	ORC_FATAL_CHECK(errorResult == tinyxml2::XMLError::XML_SUCCESS, "Fatal occured while loading XML file\n\tpath: {}\n\treason: {}", xmlPath.string(), tinyxml2::XMLDocument::ErrorIDToName(errorResult));

	for (auto element = resourceFile.FirstChildElement("RESOURCE"); element != nullptr; element = element->NextSiblingElement("RESOURCE"))
	{
		const char* name = nullptr;
		const char* path = nullptr;

		errorResult = element->QueryStringAttribute("name", &name);
		ORC_FATAL_CHECK(errorResult == tinyxml2::XMLError::XML_SUCCESS, "Fatal occured while reading XML file\n\tpath: {}\n\treason: {}", xmlPath.string(), tinyxml2::XMLDocument::ErrorIDToName(errorResult));

		errorResult = element->QueryStringAttribute("path", &path);
		ORC_FATAL_CHECK(errorResult == tinyxml2::XMLError::XML_SUCCESS, "Fatal occured while reading XML file\n\tpath: {}\n\treason: {}", xmlPath.string(), tinyxml2::XMLDocument::ErrorIDToName(errorResult));

		Ref<Font> font = createRef<Font>();
		if (font->loadFromFile(path))
			m_resources[name] = font;
		else
			ORC_LOG_ERROR("Couldn't load resource '{}'\n\tpath: '{}'", name, path);
	}
}

void ResourceHolder<Shader>::loadResources(const FilePath& xmlPath)
{
	tinyxml2::XMLDocument resourceFile;
	tinyxml2::XMLError errorResult = resourceFile.LoadFile(xmlPath.string().c_str());

	if (errorResult == tinyxml2::XMLError::XML_ERROR_EMPTY_DOCUMENT)
		errorResult = tinyxml2::XMLError::XML_SUCCESS;

	ORC_FATAL_CHECK(errorResult == tinyxml2::XMLError::XML_SUCCESS, "Fatal occured while loading XML file\n\tpath: {}\n\treason: {}", xmlPath.string(), tinyxml2::XMLDocument::ErrorIDToName(errorResult));

	for (auto element = resourceFile.FirstChildElement("RESOURCE"); element != nullptr; element = element->NextSiblingElement("RESOURCE"))
	{
		const char* name = nullptr;
		const char* path = nullptr;
		const char* path2 = nullptr;

		errorResult = element->QueryStringAttribute("name", &name);
		ORC_FATAL_CHECK(errorResult == tinyxml2::XMLError::XML_SUCCESS, "Fatal occured while reading XML file\n\tpath: {}\n\treason: {}", xmlPath.string(), tinyxml2::XMLDocument::ErrorIDToName(errorResult));
		
		errorResult = element->QueryStringAttribute("path", &path);
		ORC_FATAL_CHECK(errorResult == tinyxml2::XMLError::XML_SUCCESS, "Fatal occured while reading XML file\n\tpath: {}\n\treason: {}", xmlPath.string(), tinyxml2::XMLDocument::ErrorIDToName(errorResult));

		errorResult = element->QueryStringAttribute("path2", &path2);
		ORC_FATAL_CHECK(errorResult == tinyxml2::XMLError::XML_SUCCESS, "Fatal occured while reading XML file\n\tpath: {}\n\treason: {}", xmlPath.string(), tinyxml2::XMLDocument::ErrorIDToName(errorResult));

		Ref<Shader> shader = createRef<Shader>();
		if (shader->loadFromFile(path, path2))
			m_resources[name] = shader;
		else
			ORC_LOG_ERROR("Couldn't load resource '{}'\n\tpath: '{}'\n\tpath 2: '{}'", name, path, path2);
	}
}

void ResourceHolder<Texture>::loadResources(const FilePath& xmlPath)
{
	tinyxml2::XMLDocument resourceFile;
	tinyxml2::XMLError errorResult = resourceFile.LoadFile(xmlPath.string().c_str());

	if (errorResult == tinyxml2::XMLError::XML_ERROR_EMPTY_DOCUMENT)
		errorResult = tinyxml2::XMLError::XML_SUCCESS;

	ORC_FATAL_CHECK(errorResult == tinyxml2::XMLError::XML_SUCCESS, "Fatal occured while loading XML file\n\tpath: {}\n\treason: {}", xmlPath.string(), tinyxml2::XMLDocument::ErrorIDToName(errorResult));

	for (auto element = resourceFile.FirstChildElement("RESOURCE"); element != nullptr; element = element->NextSiblingElement("RESOURCE"))
	{
		const char* name = nullptr;
		const char* path = nullptr;

		errorResult = element->QueryStringAttribute("name", &name);
		ORC_FATAL_CHECK(errorResult == tinyxml2::XMLError::XML_SUCCESS, "Fatal occured while reading XML file\n\tpath: {}\n\treason: {}", xmlPath.string(), tinyxml2::XMLDocument::ErrorIDToName(errorResult));

		errorResult = element->QueryStringAttribute("path", &path);
		ORC_FATAL_CHECK(errorResult == tinyxml2::XMLError::XML_SUCCESS, "Fatal occured while reading XML file\n\tpath: {}\n\treason: {}", xmlPath.string(), tinyxml2::XMLDocument::ErrorIDToName(errorResult));

		Ref<Texture> texture = createRef<Texture>();
		if (texture->loadFromFile(path))
			m_resources[name] = texture;
		else
			ORC_LOG_ERROR("Couldn't load resource '{}'\n\tpath: '{}'", name, path);
	}
}

void ResourceHolder<Animation>::loadResources(const FilePath& xmlPath)
{
	tinyxml2::XMLDocument resourceFile;
	tinyxml2::XMLError errorResult = resourceFile.LoadFile(xmlPath.string().c_str());

	if (errorResult == tinyxml2::XMLError::XML_ERROR_EMPTY_DOCUMENT)
		errorResult = tinyxml2::XMLError::XML_SUCCESS;

	ORC_FATAL_CHECK(errorResult == tinyxml2::XMLError::XML_SUCCESS, "Fatal occured while loading XML file\n\tpath: {}\n\treason: {}", xmlPath.string(), tinyxml2::XMLDocument::ErrorIDToName(errorResult));

	for (auto element = resourceFile.FirstChildElement("RESOURCE"); element != nullptr; element = element->NextSiblingElement("RESOURCE"))
	{
		const char* name = nullptr;
		const char* frame = nullptr;

		uint32_t durationMs = 0;
		std::vector<FloatRect> frames;

		errorResult = element->QueryStringAttribute("name", &name);
		ORC_FATAL_CHECK(errorResult == tinyxml2::XMLError::XML_SUCCESS, "Fatal occured while reading XML file\n\tpath: {}\n\treason: {}", xmlPath.string(), tinyxml2::XMLDocument::ErrorIDToName(errorResult));

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
			rect.x = std::stof(dimensionString[0]);
			rect.y = std::stof(dimensionString[1]);
			rect.width = std::stof(dimensionString[2]);
			rect.height = std::stof(dimensionString[3]);

			frames.push_back(rect);
			frameCount++;
		}

		errorResult = element->QueryUnsignedAttribute("durationMs", &durationMs);
		ORC_FATAL_CHECK(errorResult == tinyxml2::XMLError::XML_SUCCESS, "Fatal occured while reading XML file\n\tpath: {}\n\treason: {}", xmlPath.string(), tinyxml2::XMLDocument::ErrorIDToName(errorResult));

		if (frames.size())
		{
			Ref<Animation> animation = createRef<Animation>(frames, durationMs);
			m_resources[name] = animation;
		}
		else
		{
			ORC_LOG_ERROR("Couldn't load resource '{}'", name);
		}
	}
}

void ResourceHolder<SoundBuffer>::loadResources(const FilePath& xmlPath)
{
	tinyxml2::XMLDocument resourceFile;
	tinyxml2::XMLError errorResult = resourceFile.LoadFile(xmlPath.string().c_str());

	if (errorResult == tinyxml2::XMLError::XML_ERROR_EMPTY_DOCUMENT)
		errorResult = tinyxml2::XMLError::XML_SUCCESS;

	ORC_FATAL_CHECK(errorResult == tinyxml2::XMLError::XML_SUCCESS, "Fatal occured while loading XML file\n\tpath: {}\n\treason: {}", xmlPath.string(), tinyxml2::XMLDocument::ErrorIDToName(errorResult));
	
	for (auto element = resourceFile.FirstChildElement("RESOURCE"); element != nullptr; element = element->NextSiblingElement("RESOURCE"))
	{
		const char* name = nullptr;
		const char* path = nullptr;

		errorResult = element->QueryStringAttribute("name", &name);
		ORC_FATAL_CHECK(errorResult == tinyxml2::XMLError::XML_SUCCESS, "Fatal occured while reading XML file\n\tpath: {}\n\treason: {}", xmlPath.string(), tinyxml2::XMLDocument::ErrorIDToName(errorResult));

		errorResult = element->QueryStringAttribute("path", &path);
		ORC_FATAL_CHECK(errorResult == tinyxml2::XMLError::XML_SUCCESS, "Fatal occured while reading XML file\n\tpath: {}\n\treason: {}", xmlPath.string(), tinyxml2::XMLDocument::ErrorIDToName(errorResult));

		Ref<SoundBuffer> soundBuffer = createRef<SoundBuffer>();
		if (soundBuffer->loadFromFile(path))
			m_resources[name] = soundBuffer;
		else
			ORC_LOG_ERROR("Couldn't load resource '{}'\n\tpath: '{}'", name, path);
	}
}

template class ResourceHolder<Font>;
template class ResourceHolder<Shader>;
template class ResourceHolder<Texture>;
template class ResourceHolder<Animation>;
template class ResourceHolder<SoundBuffer>;

}
