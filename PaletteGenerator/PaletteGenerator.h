#pragma once

#include "BMPLoader.h"

using double3 = struct
{
	double x;
	double y;
	double z;
};

using double4 = struct
{
	double x;
	double y;
	double z;
	double w;
};

class PaletteGenerator
{
public:
	PaletteGenerator();
	~PaletteGenerator();

	void GenerateRgb(uint16_t numberOfGradationsR, uint16_t numberOfGradationsG, uint16_t numberOfGradationsB);
	void GenerateCmyk(uint8_t numberOfGradationsC, uint8_t numberOfGradationsM, uint8_t numberOfGradationsY, uint8_t numberOfGradationsK);
	void GenerateHsb(uint16_t numberOfGradationsH, uint8_t numberOfGradationsS, uint8_t numberOfGradationsB);

	void SaveResultToFile(const std::string& filePath);

private:
	const double3 CmykToRgb(const double4& cmykSample) const;
	const double3 HsbToRgb(const double3& hsbSample) const;

	void FitRgbImageToSquare(std::vector<uint8_t>& _rgbImage, int& imageSideSize);

	std::vector<uint8_t> rgbImage;
};
