#include "TexturesFactory.h"

TexturesFactory::~TexturesFactory() {
	for (const auto& texture : m_textures) {
		delete texture.second;
	}
}

Texture* TexturesFactory::LoadTexture(std::string filePath) {
	std::map<std::string, Texture*>::iterator texture_iter = m_textures.find(filePath);
	if (texture_iter == m_textures.end()) {
		try {
			Texture *texture = new Texture(filePath);
			m_textures.insert({ filePath, texture });
			return texture;
		}
		catch (std::exception execpt) {
			return NULL;
		}
	}
	else {
		return texture_iter->second;
	}
	return NULL;
}

void TexturesFactory::RemoveTexture(std::string filePath) {
	std::map<std::string, Texture*>::iterator texture_iter = m_textures.find(filePath);
	m_textures.erase(texture_iter);
}