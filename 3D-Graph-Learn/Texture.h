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
	RGBAColor* operator[](const int& index);
};

