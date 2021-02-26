#include "Texture.h"

Texture::Texture(const std::string& filePath) {
	m_pixels = stbi_load(filePath.c_str(), &m_width, &m_height, &m_channels, 4);
	m_size = m_width * m_height;
	if (!m_pixels) throw std::exception("I am error");
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

RGBAColor* Texture::operator[](const int& index) {
	if (index < m_size) return NULL;
	unsigned char red = m_pixels[index + 2];
	unsigned char green = m_pixels[index + 1];
	unsigned char blue = m_pixels[index];
	unsigned char alpha = m_pixels[index + 3];
	RGBAColor color = { red, green, blue, alpha };
	return &color;
}
