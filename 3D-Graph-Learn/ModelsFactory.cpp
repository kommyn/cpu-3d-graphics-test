#include "ModelsFactory.h"

ModelsFactory::~ModelsFactory() {
	for (const auto& model : m_models) {
		delete model.second;
	}
}

Model3D* ModelsFactory::LoadModel(const std::string& filePath) {
	std::map<std::string, Model3D*>::iterator model_iter = m_models.find(filePath);
	if (model_iter == m_models.end()) {
		try {
			Model3D* model = new Model3D();
			// TODO: Remove this. It is an temporary solution
			model->LoadModel(filePath, m_texturesFactory);
			m_models.insert({ filePath, model });
			return model;
		}
		catch (std::exception execpt) {
			return NULL;
		}
	}
	else {
		return model_iter->second;
	}
}

void ModelsFactory::SetTexturesFactory(TexturesFactory* texturesFactory) {
	m_texturesFactory = texturesFactory;
}