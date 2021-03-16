#pragma once

#include "Includes.h"
#include "PaletteGenerator.h"

class Application
{
public:
	Application();
	~Application();

	int Run();

private:
	int GetColorSpaceIndex();
	uint16_t GetNumberOfGradations(const char channelName, uint16_t minNumberOfGradations, uint16_t maxNumberOfGradations);
};
