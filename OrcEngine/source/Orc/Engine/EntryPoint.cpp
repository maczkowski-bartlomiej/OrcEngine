#include "OrcPch.hpp"
#include "Engine/EntryPoint.hpp"

int main(int, char**)
{
    orc::Engine* engine = new orc::Engine();
    orc::Config config = orc::getEngineConfig(); //user function callback

    if (engine->init(config))
    {
        orc::onEngineStart(*engine); //user function callback

        engine->run(); //enter game engine loop
        engine->deinit();

        delete engine;
    }
    else
    {
        return -1;
    }

    return 0;
}
