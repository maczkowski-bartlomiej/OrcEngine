#include "Script/Mono.hpp"
#include "Engine/Logger.hpp"

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>

namespace orc {

Mono::Mono()
{
    ORC_LOG_INFO("Initializing Mono...");

    mono_set_dirs("C:/Program Files/Mono/lib", "C:/Program Files/Mono/etc");

    MonoDomain* domain = mono_jit_init("OrcEngine");

    if (!domain)
    {
        ORC_LOG_ERROR("Failed to initialize Mono domain.");
    }
    else
    {
        ORC_LOG_INFO("Mono domain initialized successfully.");
    }
}

Mono::~Mono()
{
    ORC_LOG_INFO("Deinitializing Mono.");
}

}