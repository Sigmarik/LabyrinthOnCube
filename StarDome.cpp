#include "StarDome.h"

glm::mat4 myLookAt(glm::vec3 start, glm::vec3 end, glm::vec3 up) {
	glm::vec3 forward = -glm::normalize(end - start);
	glm::vec3 right = glm::normalize(glm::cross(forward, up));
	glm::vec3 newUp = glm::normalize(glm::cross(forward, right));
	return glm::mat4(
		glm::vec4(right, 0.0),
		glm::vec4(newUp, 0.0),
		glm::vec4(forward, 0.0),
		glm::vec4(start, 1.0)
	);
}

StarDome::StarDome(GameLevel* level, int count, RandomGenerator* generator) {
	parentLevel = level;
	StaticMeshComponent* mesh = new StaticMeshComponent();
	mesh->mesh = new StaticMesh();
	mesh->mesh->shaders[RENDER_MAIN_PASS] = parentLevel->getShader("StarShader");
	children.push_back(mesh);
	for (int i = 0; i < count; i++) {
		float x = 0.0, y = 0.0, z = 0.0;
		do {
			x = (float)generator->range(-100, 100) / 100.0f;
			y = (float)generator->range(-100, 100) / 100.0f;
			z = (float)generator->range(-100, 100) / 100.0f;
		} while (x * x + y * y + z * z > 1.0);
		glm::vec3 position = glm::normalize(glm::vec3(x, y, z)) * 5.0f;
		//position = glm::vec3(x, y, z) * 5.0f;
		parentLevel->getMesh("StarSquare.txt")->assignInt(i);
		mesh->mesh->attach(parentLevel->getMesh("StarSquare.txt"), 
			myLookAt(position, glm::vec3(0.0), glm::vec3(0.0, 1.0, 0.0)) * 
			glm::scale(glm::vec3(0.06f * (float)generator->range(25, 125) / 100.0f)));
	}
	//std::cout << "Created " << count << " stars with " << mesh->mesh->vertices.size() << " vertices\n";
	mesh->mesh->updateBuffers();
}