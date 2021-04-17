#pragma once

#include <map>
#include <exception>

#include "Model3D.h"

class ModelsFactory
{
private:
	std::map<std::string, Model3D*> m_models;
public:
	ModelsFactory() = default;
	~ModelsFactory();

	Model3D* LoadModel(const std::string& filePath);
};

