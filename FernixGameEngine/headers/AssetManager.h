#pragma once

#include <map>
#include <vector>
#include <filesystem>
#include <stb_image.h>
#include <glad/glad.h>

namespace fs = std::experimental::filesystem;

enum ResourceType {
	Texture2D
};

struct Resource {
	int ID;
	std::string filepath;
	ResourceType type;
};

struct Option {
	const std::string error;
	const Resource resource;
};

class AssetManager
{
public:
	AssetManager();
	
	void addRedirect(const std::string& name, const std::string& resourcePosition);

	void removeRedirect(const std::string& name);

	std::string toRedirectPath(const std::string& path);

	std::string absolute(const std::string& path);

	int generateID();

	Option loadTexture(const std::string& path);
	
	~AssetManager();
private:
	int RESOURCE_ID;

	std::map<std::string, std::string> redirectedPaths;
	std::vector<Resource> resources;
};

