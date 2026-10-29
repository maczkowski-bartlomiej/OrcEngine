#include "Engine/ResourceHolder.hpp"

#include <sstream>

#include <tinyxml2.h>

namespace orc {

template <typename ResourceType>
Resources<ResourceType>::Resources(const FilePath& xmlPath)
{
	loadResources(xmlPath);
}

template <typename ResourceType>
const Ref<ResourceType>& Resources<ResourceType>::getResource(std::string_view name) const
{
	const auto find = m_resources.find(name);
	ORC_FATAL_CHECK(find != m_resources.end(), "Requested non-existing resource '{}'.", name);
	return find->second;
}

template <typename ResourceType>
void Resources<ResourceType>::clear()
{
	m_resources.clear();
}

template <>
void Resources<Font>::loadResources(const FilePath& xmlPath)
{
	ORC_LOG_INFO("Loading fonts.");

	tinyxml2::XMLDocument resourceFile;
	tinyxml2::XMLError errorResult = resourceFile.LoadFile(xmlPath.string().c_str());

	if (errorResult == tinyxml2::XMLError::XML_ERROR_EMPTY_DOCUMENT)
	{
		errorResult = tinyxml2::XMLError::XML_SUCCESS;
	}

	ORC_FATAL_CHECK(errorResult == tinyxml2::XMLError::XML_SUCCESS, "Failed to load XML file...\n\tPath: {}\n\tReason: {}", xmlPath.string(), tinyxml2::XMLDocument::ErrorIDToName(errorResult));

	for (auto element = resourceFile.FirstChildElement("RESOURCE"); element != nullptr; element = element->NextSiblingElement("RESOURCE"))
	{
		const char* name = nullptr;
		const char* path = nullptr;

		errorResult = element->QueryStringAttribute("name", &name);
		ORC_FATAL_CHECK(errorResult == tinyxml2::XMLError::XML_SUCCESS, "Failed to read XML file...\n\tPath: {}\n\tReason: {}", xmlPath.string(), tinyxml2::XMLDocument::ErrorIDToName(errorResult));

		errorResult = element->QueryStringAttribute("path", &path);
		ORC_FATAL_CHECK(errorResult == tinyxml2::XMLError::XML_SUCCESS, "Failed to read XML file...\n\tPath: {}\n\tReason: {}", xmlPath.string(), tinyxml2::XMLDocument::ErrorIDToName(errorResult));

		Ref<Font> font = createRef<Font>();
		font->loadFromFile(path);
		m_resources[name] = font;
	}

	ORC_LOG_INFO("Resources:\n\tType: Fonts\n\tManifest: {}\n\tLoaded: {}", xmlPath.string(), m_resources.size());
}

template <>
void Resources<Shader>::loadResources(const FilePath& xmlPath)
{
	ORC_LOG_INFO("Loading shaders.");

	tinyxml2::XMLDocument resourceFile;
	tinyxml2::XMLError errorResult = resourceFile.LoadFile(xmlPath.string().c_str());

	if (errorResult == tinyxml2::XMLError::XML_ERROR_EMPTY_DOCUMENT)
	{
		errorResult = tinyxml2::XMLError::XML_SUCCESS;
	}

	ORC_FATAL_CHECK(errorResult == tinyxml2::XMLError::XML_SUCCESS, "Failed to load XML file...\n\tPath: {}\n\tReason: {}", xmlPath.string(), tinyxml2::XMLDocument::ErrorIDToName(errorResult));

	for (auto element = resourceFile.FirstChildElement("RESOURCE"); element != nullptr; element = element->NextSiblingElement("RESOURCE"))
	{
		const char* name = nullptr;
		const char* path = nullptr;
		const char* path2 = nullptr;

		errorResult = element->QueryStringAttribute("name", &name);
		ORC_FATAL_CHECK(errorResult == tinyxml2::XMLError::XML_SUCCESS, "Failed to read XML file...\n\tPath: {}\n\tReason: {}", xmlPath.string(), tinyxml2::XMLDocument::ErrorIDToName(errorResult));

		errorResult = element->QueryStringAttribute("path", &path);
		ORC_FATAL_CHECK(errorResult == tinyxml2::XMLError::XML_SUCCESS, "Failed to read XML file...\n\tPath: {}\n\tReason: {}", xmlPath.string(), tinyxml2::XMLDocument::ErrorIDToName(errorResult));

		errorResult = element->QueryStringAttribute("path2", &path2);
		ORC_FATAL_CHECK(errorResult == tinyxml2::XMLError::XML_SUCCESS, "Failed to read XML file...\n\tPath: {}\n\tReason: {}", xmlPath.string(), tinyxml2::XMLDocument::ErrorIDToName(errorResult));

		Ref<Shader> shader = createRef<Shader>();
		shader->loadFromFile(path, path2);
		m_resources[name] = shader;
	}

	ORC_LOG_INFO("Resources:\n\tType: Shaders\n\tManifest: {}\n\tLoaded: {}", xmlPath.string(), m_resources.size());
}

template <>
void Resources<Texture>::loadResources(const FilePath& xmlPath)
{
	ORC_LOG_INFO("Loading textures.");

	tinyxml2::XMLDocument resourceFile;
	tinyxml2::XMLError errorResult = resourceFile.LoadFile(xmlPath.string().c_str());

	if (errorResult == tinyxml2::XMLError::XML_ERROR_EMPTY_DOCUMENT)
	{
		errorResult = tinyxml2::XMLError::XML_SUCCESS;
	}

	ORC_FATAL_CHECK(errorResult == tinyxml2::XMLError::XML_SUCCESS, "Failed to load XML file...\n\tPath: {}\n\tReason: {}", xmlPath.string(), tinyxml2::XMLDocument::ErrorIDToName(errorResult));

	for (auto element = resourceFile.FirstChildElement("RESOURCE"); element != nullptr; element = element->NextSiblingElement("RESOURCE"))
	{
		const char* name = nullptr;
		const char* path = nullptr;

		errorResult = element->QueryStringAttribute("name", &name);
		ORC_FATAL_CHECK(errorResult == tinyxml2::XMLError::XML_SUCCESS, "Failed to read XML file...\n\tPath: {}\n\tReason: {}", xmlPath.string(), tinyxml2::XMLDocument::ErrorIDToName(errorResult));

		errorResult = element->QueryStringAttribute("path", &path);
		ORC_FATAL_CHECK(errorResult == tinyxml2::XMLError::XML_SUCCESS, "Failed to read XML file...\n\tPath: {}\n\tReason: {}", xmlPath.string(), tinyxml2::XMLDocument::ErrorIDToName(errorResult));

		Ref<Texture> texture = createRef<Texture>();
		texture->loadFromFile(path);
		m_resources[name] = texture;
	}

	ORC_LOG_INFO("Resources:\n\tType: Textures\n\tManifest: {}\n\tLoaded: {}", xmlPath.string(), m_resources.size());
}

template <>
void Resources<Animation>::loadResources(const FilePath& xmlPath)
{
	ORC_LOG_INFO("Loading animations.");

	tinyxml2::XMLDocument resourceFile;
	tinyxml2::XMLError errorResult = resourceFile.LoadFile(xmlPath.string().c_str());

	if (errorResult == tinyxml2::XMLError::XML_ERROR_EMPTY_DOCUMENT)
	{
		errorResult = tinyxml2::XMLError::XML_SUCCESS;
	}

	ORC_FATAL_CHECK(errorResult == tinyxml2::XMLError::XML_SUCCESS, "Failed to load XML file...\n\tPath: {}\n\tReason: {}", xmlPath.string(), tinyxml2::XMLDocument::ErrorIDToName(errorResult));

	for (auto element = resourceFile.FirstChildElement("RESOURCE"); element != nullptr; element = element->NextSiblingElement("RESOURCE"))
	{
		const char* name = nullptr;
		const char* frame = nullptr;

		uint32_t durationMs = 0;
		std::vector<FloatRect> frames;

		errorResult = element->QueryStringAttribute("name", &name);
		ORC_FATAL_CHECK(errorResult == tinyxml2::XMLError::XML_SUCCESS, "Failed to read XML file...\n\tPath: {}\n\tReason: {}", xmlPath.string(), tinyxml2::XMLDocument::ErrorIDToName(errorResult));

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

			ORC_FATAL_CHECK(dimensionString.size() == 4, "Invalid dimension for animation '{}'", name);

			FloatRect rect;
			rect.left = std::stof(dimensionString[0]);
			rect.top = std::stof(dimensionString[1]);
			rect.width = std::stof(dimensionString[2]);
			rect.height = std::stof(dimensionString[3]);

			frames.push_back(rect);
			frameCount++;
		}

		errorResult = element->QueryUnsignedAttribute("durationMs", &durationMs);
		ORC_FATAL_CHECK(errorResult == tinyxml2::XMLError::XML_SUCCESS, "Failed to read XML file...\n\tPath: {}\n\tReason: {}", xmlPath.string(), tinyxml2::XMLDocument::ErrorIDToName(errorResult));

		ORC_FATAL_CHECK(!frames.empty(), "Failed to load resource '{}'.", name);
		Ref<Animation> animation = createRef<Animation>(frames, durationMs);
		m_resources[name] = animation;
	}

	ORC_LOG_INFO("Resources:\n\tType: Animations\n\tManifest: {}\n\tLoaded: {}", xmlPath.string(), m_resources.size());
}

template class Resources<Font>;
template class Resources<Shader>;
template class Resources<Texture>;
template class Resources<Animation>;

}
