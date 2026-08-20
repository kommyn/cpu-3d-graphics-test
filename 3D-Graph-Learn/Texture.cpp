#include "Texture.h"

Texture::Texture(const std::string& filePath) {
	m_pixels = stbi_load(filePath.c_str(), &m_width, &m_height, &m_channels, 4);
	m_size = m_width * m_height;
	// TODO: This is placeholder. I need to change this for something better
	if (!m_pixels) throw std::runtime_error("Texture: failed to load " + filePath);
}

Texture::~Texture() {
	stbi_image_free(m_pixels);
}

int Texture::GetWidth() {
	return m_width;
}

int Texture::GetHeight() {
	return m_height;
}

int Texture::GetChannels() {
	return m_channels;
}

int Texture::GetSize() {
	return m_size;
}

bool Texture::TryGetPixel(long long index, RGBAColor& outColor) const {
	if (index < 0 || index + 3 >= 4LL * m_size) return false;
	outColor = { m_pixels[index + 2], m_pixels[index + 1], m_pixels[index], m_pixels[index + 3] };
	return true;
}
