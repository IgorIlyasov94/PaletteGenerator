#include "Application.h"

Application::Application()
{

}

Application::~Application()
{

}

int Application::Run()
{
	try
	{
		int colorSpaceIndex = GetColorSpaceIndex();

		PaletteGenerator paletteGenerator;

		if (colorSpaceIndex == 0)
		{
			uint16_t numberOfGradationsR = GetNumberOfGradations('R', 1, 256);
			uint16_t numberOfGradationsG = GetNumberOfGradations('G', 1, 256);
			uint16_t numberOfGradationsB = GetNumberOfGradations('B', 1, 256);

			paletteGenerator.GenerateRgb(numberOfGradationsR, numberOfGradationsG, numberOfGradationsB);
		}
		else if (colorSpaceIndex == 1)
		{
			uint8_t numberOfGradationsC = static_cast<uint8_t>(GetNumberOfGradations('C', 1, 100));
			uint8_t numberOfGradationsM = static_cast<uint8_t>(GetNumberOfGradations('M', 1, 100));
			uint8_t numberOfGradationsY = static_cast<uint8_t>(GetNumberOfGradations('Y', 1, 100));
			uint8_t numberOfGradationsK = static_cast<uint8_t>(GetNumberOfGradations('K', 1, 100));

			paletteGenerator.GenerateCmyk(numberOfGradationsC, numberOfGradationsM, numberOfGradationsY, numberOfGradationsK);
		}
		else if (colorSpaceIndex == 2)
		{
			uint16_t numberOfGradationsH = GetNumberOfGradations('H', 1, 360);
			uint8_t numberOfGradationsS = static_cast<uint8_t>(GetNumberOfGradations('S', 1, 100));
			uint8_t numberOfGradationsB = static_cast<uint8_t>(GetNumberOfGradations('B', 1, 100));

			paletteGenerator.GenerateHsb(numberOfGradationsH, numberOfGradationsS, numberOfGradationsB);
		}

		std::cout << "Done!" << std::endl << std::endl;

		std::string resultPaletteFilePath;

		GetSaveFilePathFromDialog(resultPaletteFilePath);

		paletteGenerator.SaveResultToFile(resultPaletteFilePath);

		std::cout << "File saved!" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}

	return 0;
}

int Application::GetColorSpaceIndex()
{
	std::cout << "Choose the color space" << std::endl;
	std::cout << "RGB - 0, CMYK - 1, HSB(HSV) - 2:" << std::endl;

	int colorSpaceIndex = -1;

	while (colorSpaceIndex == -1)
	{
		try
		{
			std::string inputData;

			std::getline(std::cin, inputData);
			std::cout << std::endl << std::endl;

			inputData.erase(std::remove_if(inputData.begin(), inputData.end(), isspace), inputData.end());

			colorSpaceIndex = std::stoi(inputData);

			if (colorSpaceIndex < 0 || colorSpaceIndex > 2)
				throw std::exception("");
		}
		catch (const std::exception&)
		{
			std::cout << "Wrong value! Enter number 0, 1 or 2:" << std::endl;

			colorSpaceIndex = -1;
		}
	}

	return colorSpaceIndex;
}

uint16_t Application::GetNumberOfGradations(const char channelName, uint16_t minNumberOfGradations, uint16_t maxNumberOfGradations)
{
	std::cout << "Enter integer number of the " << channelName << " gradation (" << minNumberOfGradations << " - " << maxNumberOfGradations << "):" << std::endl;

	uint16_t numberOfGradations{};

	while (numberOfGradations == 0)
	{
		try
		{
			std::string inputData;

			std::getline(std::cin, inputData);
			std::cout << std::endl << std::endl;

			inputData.erase(std::remove_if(inputData.begin(), inputData.end(), isspace), inputData.end());

			numberOfGradations = static_cast<uint16_t>(std::stoul(inputData));

			if (numberOfGradations < minNumberOfGradations || numberOfGradations > maxNumberOfGradations)
				throw std::exception("");
		}
		catch (const std::exception&)
		{
			std::cout << "Wrong value! Enter an integer number from " << minNumberOfGradations << " to " << maxNumberOfGradations << ":" << std::endl;

			numberOfGradations = -1;
		}
	}

	return numberOfGradations;
}
