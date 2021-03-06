#include "GameLevel.h"

GameLevel::GameLevel(RandomGenerator* generator) {
	for (int i = 0; i < characterNames.size(); i++) {
		int index = generator->range(0, characterNames.size());
		std::swap(characterNames[i], characterNames[index]);
	}
}

void GameLevel::loadAsset(const char* fname, bool makeSharp, float size) {
	std::cout << "Loading asset " << fname << '\n';
	worldMeshes[fname] = StaticMesh(fname, size, makeSharp);
}

void GameLevel::loadShader(const char* name, const char* vshName, const char* fshName) {
	std::cout << "Loading shader " << name << '\n';
	renderShaders[name] = Shader(vshName, fshName);
}

void GameLevel::loadAtlas(const char* fname, GLuint slot, GLenum format) {
	std::cout << "Loading atlas " << fname << '\n';
	atlases[fname] = TextureAtlas(fname, slot, format);
}

StaticMesh* GameLevel::getMesh(const char* fname) {
	if (worldMeshes.find(fname) == worldMeshes.end())
		throw std::runtime_error::runtime_error(std::string(fname) + " was not found");
	return &worldMeshes[fname];
}

Shader* GameLevel::getShader(const char* name) {
	if (renderShaders.find(name) == renderShaders.end()) 
		throw std::runtime_error::runtime_error(std::string(name) + " was not found");
	return &renderShaders[name];
}

TextureAtlas* GameLevel::getAtlas(const char* name) {
	if (atlases.find(name) == atlases.end())
		throw std::runtime_error::runtime_error(std::string(name) + " was not found");
	return &atlases[name];
}

GameComponent::GameComponent() {}

glm::mat4 GameComponent::worldMatrix() {
	if (!parent) return transform;
	if (!useWorldRotation) return parent->worldMatrix() * transform;
	glm::mat4 rawMatrix = parent->worldMatrix() * transform;
	return glm::mat4(
		glm::vec4(glm::vec3(transform[0]), 0.0),
		glm::vec4(glm::vec3(transform[1]), 0.0),
		glm::vec4(glm::vec3(transform[2]), 0.0),
		glm::vec4(glm::vec3(rawMatrix[3]), 1.0)
	);
}

void GameComponent::attach(GameComponent* component) {
	component->detach();
	children.push_back(component);
	component->parent = this;
}

void GameComponent::attachTo(GameComponent* component) {
	component->attach(this);
}

void GameComponent::draw(Camera& camera, int shaderSlot) {
	// Don't make any draw calls if invisible.
	if (!visible) return;
	for (GameComponent* component : children) {
		component->draw(camera, shaderSlot);
	}
	//std::cout << "Called draw on " << children.size() << " components\n";
}

void GameComponent::update(float deltaTime) {
	for (GameComponent* component : children) {
		if (component) component->update(deltaTime);
	}
}

bool GameComponent::getVisible() {
	if (!parent) return visible;
	return parent->getVisible() && visible;
}

void GameComponent::assignParent() {
	for (GameComponent* component : children) {
		component->parent = this;
		component->assignParent();
	}
}

int GameComponent::childIndex(GameComponent* child) {
	for (int i = 0; i < children.size(); i++) {
		if (children[i] == child) return i;
	}
	throw std::runtime_error::runtime_error("Child not found.");
}

void GameComponent::detach() {
	if (!parent) return;
	parent->removeChild(this);
	parent = nullptr;
}

float GameComponent::getScale(glm::vec3 sample) {
	return glm::length(glm::mat3(worldMatrix()) * sample) / glm::length(sample);
}

void GameComponent::removeChild(GameComponent* child) {
	int index = childIndex(child);
	std::swap(children[index], children[children.size() - 1]);
	children.pop_back();
}

template <typename T>
std::vector<T> GameComponent::getChildrenOfClass() {
	std::vector<T> answer = std::vector<T>();
	for (GameComponent* child : children) {
		if ((T)child) answer.push_back((T)child);
	}
	return answer;
}