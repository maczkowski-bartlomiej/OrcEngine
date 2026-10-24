#pragma once

#include "Core/Types.hpp"

namespace orc {

class Scripting
{
public:
	explicit Scripting(const FilePath& configPath = "OrcEngine.Scripting.runtimeconfig.json");
	~Scripting();

	Scripting(const Scripting&) = delete;
	Scripting& operator=(const Scripting&) = delete;
	Scripting(Scripting&&) = delete;
	Scripting& operator=(Scripting&&) = delete;

private:
	void loadHostFxr();
	void initializeRuntime();

	FilePath m_configPath;
	void* m_hostfxrLib{nullptr};
};

} // namespace orc
