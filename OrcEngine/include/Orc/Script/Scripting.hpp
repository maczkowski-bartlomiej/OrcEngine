#pragma once

#include "Core/Types.hpp"

namespace orc {

class Scripting
{
public:
	Scripting();
	~Scripting();

	Scripting(const Scripting&) = delete;
	Scripting& operator=(const Scripting&) = delete;
	Scripting(Scripting&&) = delete;
	Scripting& operator=(Scripting&&) = delete;

private:
	void loadHostFxr();
	void initializeRuntime();

	void* m_hostfxrLib = nullptr;
	void* m_hostfxrInitializeForRuntimeConfig = nullptr;
	void* m_hostfxrGetRuntimeDelegate = nullptr;
	void* m_hostfxrClose = nullptr;
	void* m_loadAssemblyAndGetFunctionPointer = nullptr;
	FilePath m_hostfxrPath;
};

}
