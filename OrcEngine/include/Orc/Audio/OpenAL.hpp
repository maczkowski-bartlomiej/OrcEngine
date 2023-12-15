#pragma once

#include "Engine/Debug.hpp"

#include <type_traits>

#include <AL/al.h>
#include <AL/alc.h>

/*
Credits:
https://indiegamedev.net/2020/01/17/c-openal-function-call-wrapping/
*/

#define alCall(function, ...) alCallImpl(std::source_location::current(), function, __VA_ARGS__)
#define alcCall(function, device, ...) alcCallImpl(std::source_location::current(), function, device, __VA_ARGS__)

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
    
template<typename alFunction, typename... Params>
auto alCallImpl(std::source_location sourceLocation, alFunction function, Params... params)
    -> typename std::enable_if<std::is_same<void, decltype(function(params...))>::value, decltype(function(params...))>::type
{
    function(std::forward<Params>(params)...);
    checkAlError(sourceLocation);
}

template<typename alFunction, typename... Params>
auto alCallImpl(std::source_location sourceLocation, alFunction function, Params... params)
    -> typename std::enable_if<!std::is_same<void, decltype(function(params...))>::value, decltype(function(params...))>::type
{
    auto returnValue = function(std::forward<Params>(params)...);
    checkAlError(sourceLocation);
    return returnValue;
}

template<typename alcFunction, typename... Params>
auto alcCallImpl(std::source_location sourceLocation, alcFunction function, ALCdevice* device, Params... params)
    -> typename std::enable_if<std::is_same<void, decltype(function(params...))>::value, decltype(function(params...))>::type
{
    function( std::forward<Params>(params)...);
    checkAlcError(sourceLocation, device);
}

template<typename alcFunction, typename... Params>
auto alcCallImpl(std::source_location sourceLocation, alcFunction function, ALCdevice* device, Params... params)
    -> typename std::enable_if<!std::is_same<void, decltype(function(params...))>::value, decltype(function(params...))>::type
{
    auto returnValue = function(std::forward<Params>(params)...);
    checkAlcError(sourceLocation, device);
    return returnValue;
}

}
