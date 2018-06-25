#pragma once

#include "Entity.h"
#include "AssetManager.h"
#include "Shader.h"

class Shader;

extern AssetManager assetManager;


struct TexturesForIBL {
	unsigned int envMap;
	unsigned int irradianceMap;
	unsigned int prefilterMap;
	unsigned int convolutedBRDFMap;
};

class Skybox : public Entity {
private:
	Shader skybox;
	unsigned int envCubemap;
	unsigned int irradianceCubemap;
	unsigned int prefilterMap;
	unsigned int convolutedBRDFMap;
public:
	Skybox();
	void Init();
	void Render() override;

	void SetIrradiance(Shader shader);
};

// renders (and builds at first invocation) a sphere
// -------------------------------------------------
void renderSphere();

// renderCube() renders a 1x1 3D cube in NDC.
// -------------------------------------------------

void renderCube();

class StructType;
extern StructType* SkyboxType;