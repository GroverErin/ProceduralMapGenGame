//#include <vld.h>
#include "Application/Application.h"
#include <Utilities/Random/Noise/PerlinNoise.h>

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
{
	/// \todo Make this part of a config file. - Erin
	static constexpr unsigned int kGameWindowWidth = 1280;
	static constexpr unsigned int kGameWindowHeight = 720;

	SandboxApp app;
	if (!app.Initialize(kGameWindowWidth, kGameWindowHeight))
	{
		return 1; //Return a failure code.
	}
	app.Run();
	app.Shutdown();
	return 0;
}