#include "PaletteGenerator.h"

PaletteGenerator::PaletteGenerator()
{

}

PaletteGenerator::~PaletteGenerator()
{

}

void PaletteGenerator::GenerateRgb(uint16_t numberOfGradationR, uint16_t numberOfGradationG, uint16_t numberOfGradationB)
{
	for (uint16_t rValue = 0; rValue < numberOfGradationR; rValue++)
	{
		for (uint16_t gValue = 0; gValue < numberOfGradationG; gValue++)
		{
			for (uint16_t bValue = 0; bValue < numberOfGradationB; bValue++)
			{
				double3 rgbSample{};
				rgbSample.x = static_cast<double>(rValue) / static_cast<double>(numberOfGradationR);
				rgbSample.y = static_cast<double>(gValue) / static_cast<double>(numberOfGradationG);
				rgbSample.z = static_cast<double>(bValue) / static_cast<double>(numberOfGradationB);

				rgbImage.push_back(static_cast<uint8_t>(rgbSample.x * 255.0));
				rgbImage.push_back(static_cast<uint8_t>(rgbSample.y * 255.0));
				rgbImage.push_back(static_cast<uint8_t>(rgbSample.z * 255.0));
			}
		}
	}
}

void PaletteGenerator::GenerateCmyk(uint8_t numberOfGradationC, uint8_t numberOfGradationM, uint8_t numberOfGradationY, uint8_t numberOfGradationK)
{
	for (uint8_t cValue = 0; cValue < numberOfGradationC; cValue++)
	{
		for (uint8_t mValue = 0; mValue < numberOfGradationM; mValue++)
		{
			for (uint8_t yValue = 0; yValue < numberOfGradationY; yValue++)
			{
				for (uint8_t kValue = 0; kValue < numberOfGradationK; kValue++)
				{
					double4 cmykSample{};
					cmykSample.x = static_cast<double>(cValue) / static_cast<double>(numberOfGradationC);
					cmykSample.y = static_cast<double>(mValue) / static_cast<double>(numberOfGradationM);
					cmykSample.z = static_cast<double>(yValue) / static_cast<double>(numberOfGradationY);
					cmykSample.w = static_cast<double>(kValue) / static_cast<double>(numberOfGradationK);

					double3 rgbSample = CmykToRgb(cmykSample);

					rgbImage.push_back(static_cast<uint8_t>(rgbSample.x * 255.0));
					rgbImage.push_back(static_cast<uint8_t>(rgbSample.y * 255.0));
					rgbImage.push_back(static_cast<uint8_t>(rgbSample.z * 255.0));
				}
			}
		}
	}
}

void PaletteGenerator::GenerateHsb(uint16_t numberOfGradationH, uint8_t numberOfGradationS, uint8_t numberOfGradationB)
{
	for (uint16_t hValue = 0; hValue < numberOfGradationH; hValue++)
	{
		for (uint8_t sValue = 0; sValue < numberOfGradationS; sValue++)
		{
			for (uint8_t bValue = 0; bValue < numberOfGradationB; bValue++)
			{
				double3 hsbSample{};
				hsbSample.x = static_cast<double>(hValue) / static_cast<double>(numberOfGradationH);
				hsbSample.y = static_cast<double>(sValue) / static_cast<double>(numberOfGradationS);
				hsbSample.z = static_cast<double>(bValue) / static_cast<double>(numberOfGradationB);

				double3 rgbSample = HsbToRgb(hsbSample);

				rgbImage.push_back(static_cast<uint8_t>(rgbSample.x * 255.0));
				rgbImage.push_back(static_cast<uint8_t>(rgbSample.y * 255.0));
				rgbImage.push_back(static_cast<uint8_t>(rgbSample.z * 255.0));
			}
		}
	}
}

void PaletteGenerator::SaveResultToFile(const std::string& filePath)
{
	int imageSideSize{};

	FitRgbImageToSquare(rgbImage, imageSideSize);

	if (filePath.find(".bmp") == std::string::npos)
	{
		std::string fixedFilePath = filePath;
		fixedFilePath.append(".bmp");

		BMPLoader::Save(fixedFilePath, rgbImage, imageSideSize, imageSideSize);

		return;
	}

	BMPLoader::Save(filePath, rgbImage, imageSideSize, imageSideSize);
}

const double3 PaletteGenerator::CmykToRgb(const double4& cmykSample) const
{
	double3 rgbSample{};

	rgbSample.x = (1.0 - cmykSample.x) * (1.0 - cmykSample.w);
	rgbSample.y = (1.0 - cmykSample.y) * (1.0 - cmykSample.w);
	rgbSample.z = (1.0 - cmykSample.z) * (1.0 - cmykSample.w);

	return rgbSample;
}

const double3 PaletteGenerator::HsbToRgb(const double3& hsbSample) const
{
	double3 rgbSample{};

	auto hue = static_cast<uint32_t>(hsbSample.x * 360.0);
	auto saturation = hsbSample.y;
	auto brightness = hsbSample.z;

	auto brightnessMin = (1.0 - saturation) * brightness;

	auto additionalValue = (brightness - brightnessMin) * static_cast<double>(hue % 60) / 60.0;

	auto brightnessInc = brightnessMin + additionalValue;
	auto brightnessDec = brightness - additionalValue;

	auto hueI = (hue / 60) % 6;

	switch (hueI)
	{
	case 0:
		rgbSample.x = brightness;
		rgbSample.y = brightnessInc;
		rgbSample.z = brightnessMin;
		break;

	case 1:
		rgbSample.x = brightnessDec;
		rgbSample.y = brightness;
		rgbSample.z = brightnessMin;
		break;

	case 2:
		rgbSample.x = brightnessMin;
		rgbSample.y = brightness;
		rgbSample.z = brightnessInc;
		break;

	case 3:
		rgbSample.x = brightnessMin;
		rgbSample.y = brightnessDec;
		rgbSample.z = brightness;
		break;

	case 4:
		rgbSample.x = brightnessInc;
		rgbSample.y = brightnessMin;
		rgbSample.z = brightness;
		break;

	case 5:
		rgbSample.x = brightness;
		rgbSample.y = brightnessMin;
		rgbSample.z = brightnessDec;
		break;
	}

	return rgbSample;
}

void PaletteGenerator::FitRgbImageToSquare(std::vector<uint8_t>& _rgbImage, int& imageSideSize)
{
	const size_t channelNumber = 3u;

	imageSideSize = AlignToNextPowerOf2(static_cast<int>(std::sqrt(_rgbImage.size() / channelNumber)));

	int numberOfMissingTexels = imageSideSize * imageSideSize - _rgbImage.size() / channelNumber;

	for (int missingTexelId = 0; missingTexelId < numberOfMissingTexels; missingTexelId++)
	{
		_rgbImage.push_back(0);
		_rgbImage.push_back(0);
		_rgbImage.push_back(0);
	}
}
