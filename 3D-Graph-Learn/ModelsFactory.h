#pragma once

#include <map>
#include <exception>

#include "Model3D.h"
#include "TexturesFactory.h"

// TODO: Change here pointer to the m_texturesFactory on std::shared_ptr, I believe it is more safe (because the memory itself
//       will be protected from the deleting)
class ModelsFactory
{
private:
	TexturesFactory* m_texturesFactory;
	std::map<std::string, Model3D*> m_models;
public:
	ModelsFactory() = default;
	~ModelsFactory();

	Model3D* LoadModel(const std::string& filePath);

	void SetTexturesFactory(TexturesFactory* texturesFactory);
};

