#pragma once

#include <map>
#include <exception>

#include "Texture.h"

class TexturesFactory
{
private:
	std::map<std::string, Texture*> m_textures;
public:
	TexturesFactory() = default;
	~TexturesFactory();

	Texture* LoadTexture(std::string filePath);
	void RemoveTexture(std::string filePath);
};

