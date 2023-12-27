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

void checkAlError(std::source_location sourceLocation);
void checkAlcError(std::source_location sourceLocation, ALCdevice* device);
    
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
