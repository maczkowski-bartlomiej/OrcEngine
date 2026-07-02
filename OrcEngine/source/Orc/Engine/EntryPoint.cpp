#include "OrcPch.hpp"
#include "Engine/EntryPoint.hpp"

int main(int, char**)
{
    orc::Config config = orc::getEngineConfig(); //user function callback
    orc::Engine engine(config);

    orc::onEngineStart(engine);
    engine.run();

    return 0;
}
