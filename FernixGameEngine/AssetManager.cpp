#include "AssetManager.h"
#include <iostream>


AssetManager::AssetManager()
{
}

void AssetManager::addRedirect(const std::string& name, const std::string& resourcePosition) {
	redirectedPaths[name] = resourcePosition;
}

void AssetManager::removeRedirect(const std::string& name) {
	redirectedPaths.erase(name);
}

std::string AssetManager::toRedirectPath(const std::string& path) {
	for (auto pair : redirectedPaths) {
		std::string name = pair.first;
		std::string redirect = pair.second;

		if (name == path) {
			return toRedirectPath(redirect);
		}
	}
	return path;
}

int AssetManager::generateID() {
	return RESOURCE_ID++;
}

std::string AssetManager::absolute(const std::string& path) {
	std::string redirected = toRedirectPath(path);
	return fs::absolute(redirected).string();
}

Option AssetManager::loadTexture(const std::string& path) {
	std::string abs_path = absolute(path);

	for (unsigned int i = 0; i < resources.size(); i++) {
		if (resources[i].filepath == abs_path) {
			Resource resource = { resources[i].ID, abs_path, ResourceType::Texture2D };
			return { "", resource };
		}
	}

	// load and create a texture 
	// -------------------------
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
										   // set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	float aniso = 0.0f;
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &aniso);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, aniso);

	// load image, create texture and generate mipmaps

	int width, height, nrChannels;

	unsigned char *data = stbi_load(abs_path.c_str(), &width, &height, &nrChannels, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		Resource resource = {texture, abs_path, ResourceType::Texture2D};

		resources.push_back(resource);
		stbi_image_free(data);
		return { "", resource };
	}

	stbi_image_free(data);

	Resource resource = { 0, abs_path, ResourceType::Texture2D };

	std::string errorMessage = ("Failed to load texture '" + path + "', '" + abs_path + "'");

	return { errorMessage, resource };
}

AssetManager::~AssetManager()
{
}
