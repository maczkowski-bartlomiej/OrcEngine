#include "OrcPch.hpp"

#include "Audio/OpenAL.hpp"

namespace orc {

void checkAlError(std::source_location sourceLocation)
{
    ALCenum error = alGetError();
    if (error != AL_NO_ERROR)
    {
        switch (error)
        {
            case AL_INVALID_NAME: Logger::log(Logger::Level::Error, sourceLocation, "OpenAL error\n\tAL_INVALID_NAME: a bad name (ID) was passed"); break;
            case AL_INVALID_ENUM: Logger::log(Logger::Level::Error, sourceLocation, "OpenAL error\n\tAL_INVALID_ENUM: an invalid enum value was passed"); break;
            case AL_INVALID_VALUE: Logger::log(Logger::Level::Error, sourceLocation, "OpenAL error\n\tAL_INVALID_VALUE: an invalid value was passed"); break;
            case AL_INVALID_OPERATION: Logger::log(Logger::Level::Error, sourceLocation, "OpenAL error\n\tAL_INVALID_OPERATION: the requested operation is not valid"); break;
            case AL_OUT_OF_MEMORY: Logger::log(Logger::Level::Error, sourceLocation, "OpenAL error\n\tAL_OUT_OF_MEMORY: the requested operation resulted in OpenAL running out of memory"); break;
            default: Logger::log(Logger::Level::Error, sourceLocation, "OpenAL error\n\treason: {} ", error);
        }
    }
}

void checkAlcError(std::source_location sourceLocation, ALCdevice* device)
{
    ALCenum error = alcGetError(device);
    if (error != ALC_NO_ERROR)
    {
        switch (error)
        {
            case ALC_INVALID_VALUE: Logger::log(Logger::Level::Error, sourceLocation, "OpenAL error\n\tALC_INVALID_VALUE: an invalid value was passed"); break;
            case ALC_INVALID_DEVICE: Logger::log(Logger::Level::Error, sourceLocation, "OpenAL error\n\tALC_INVALID_DEVICE: a bad device was passed"); break;
            case ALC_INVALID_CONTEXT: Logger::log(Logger::Level::Error, sourceLocation, "OpenAL error\n\tALC_INVALID_CONTEXT: a bad context was passed"); break;
            case ALC_INVALID_ENUM:  Logger::log(Logger::Level::Error, sourceLocation, "OpenAL error\n\tALC_INVALID_ENUM: an unknown enum value was passed"); break;
            case ALC_OUT_OF_MEMORY: Logger::log(Logger::Level::Error, sourceLocation, "OpenAL error\n\tALC_OUT_OF_MEMORY: an unknown enum value was passed"); break;
            default: Logger::log(Logger::Level::Error, sourceLocation, "OpenAL error\n\treason: {} ", error);
        }
    }
}

}
