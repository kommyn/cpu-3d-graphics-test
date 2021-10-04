#pragma once

#include <iostream>
#include <string>
#include <exception>

#include "EngineBase.h"
#ifndef STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#endif

class Texture
{
private:
	int m_width, m_height, m_channels, m_size;
	unsigned char* m_pixels;
public:
	Texture(const std::string& filePath);
	~Texture();

	int GetWidth();
	int GetHeight();
	int GetChannels();
	int GetSize();
	// TODO: Looks like I get not correct data from this method. I should change it in two ways:
	//       1. Getting correct data of red, green, blue and alpha
	//       2. Get pixel data by index of this pixel, but not by the 4 * index
	RGBAColor* operator[](const int& index);
};

