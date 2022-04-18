#ifndef GAME_LEVEL_CLASS_H
#define GAME_LEVEL_CLASS_H

#include <map>
#include <iostream>
#include <string>

#include "StaticMesh.h"
#include "TextureAtlas.h"

class GameComponent;

class GameLevel {
public:
	std::map<const char*, StaticMesh> worldMeshes;
	std::map<const char*, Shader> renderShaders;
	std::map<const char*, TextureAtlas> atlases;
	//TextureAtlas coreAtlas;
	std::vector<GameComponent*> worldComponents;
	Camera* mainCamera = nullptr;
	GameLevel();
	Shader* getShader(const char* name);
	StaticMesh* getMesh(const  char* name);
	TextureAtlas* getAtlas(const char* name);
	void loadAsset(const char* fname, bool makeSharp=false, float size=1.0);
	void loadShader(const char* name, const char* vshName, const char* fshName);
	void loadAtlas(const char* fname, GLuint slot = GL_TEXTURE0, GLenum format = GL_RGBA);
};


class GameComponent {
public:
	glm::mat4 transform = glm::mat4(1.0);
	std::vector<GameComponent*> children = {};
	GameComponent* parent = nullptr;
	bool useWorldRotation = false;
	GameLevel* parentLevel = nullptr;
	float localScale = 1.0;
	bool visible = true;  // Propagates on children
	bool hidden = false;  // Only for this component
	GameComponent();
	virtual glm::mat4 worldMatrix();
	void attach(GameComponent* component);
	void attachTo(GameComponent* component);
	bool getVisible();
	virtual void draw(Camera& camera, int shaderSlot = RENDER_MAIN_PASS);
	virtual void update(float deltaTime);
	virtual void assignParent();
};

#endif