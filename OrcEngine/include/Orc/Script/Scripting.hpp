#pragma once

#include "Core/Memory.hpp"
#include "Core/Types.hpp"

namespace orc {

class Scripting
{
public:
	explicit Scripting(const FilePath& configPath = "OrcEngine.Scripting.runtimeconfig.json");
	~Scripting();

	Scripting(const Scripting&) = delete;
	Scripting& operator=(const Scripting&) = delete;

	Scripting(Scripting&&) noexcept;
	Scripting& operator=(Scripting&&) noexcept;

private:
	struct Impl;
	UniquePtr<Impl> m_impl;
};

} // namespace orc
