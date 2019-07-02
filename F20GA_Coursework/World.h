#pragma once

#include "Camera.h"
#include "Material.h"
#include "Mesh.h"
#include "OBJLoader.h"
#include "Primitives.h"
#include "Shader.h"
#include "Texture.h"
#include "Vertex.h"
#include "AstarSearch.h"

//ENUMERATIONS
enum shader_enum { SHADER_CORE_PROGRAM = 0 };
enum texture_enum { TEX_FLOOR = 0, TEX_FLOOR_SPECULAR, TEX_WALL, TEX_WALL_SPECULAR, TEX_BALL, TEX_BALL_SPECULAR, TEX_PARTICLE};
enum material_enum { MAT_1 = 0 };
enum mesh_enum { MODEL_FLOOR = 0, MODEL_WALL_BACK, MODEL_WALL_RIGHT, MODEL_WALL_LEFT, MODEL_BALL, MODEL_RUNNER, MODEL_CHASER, MODEL_OBSTACLE_ONE, MODEL_OBSTACLE_TWO, MODEL_OBSTACLE_THREE, MODEL_OBSTACLE_FOUR, MODEL_PARTICLE};
enum minmax_enum { MINMAX_FLOOR = 0, MINMAX_BALL, MINMAX_PARTICLE};

class World {

private:

	//Variables for F20GP
	float gravity = 70.0;
	float ballVelocity = 0.0;
	float friction = 0.8;
	bool firstRun = true;
	bool ballBouncing = true;
	bool falling = true;
	int particleCount = 70;
	std::vector<glm::vec3> particleRandoms;
	bool explosion = false;
	int particleLife = 100;

	//VARIABLES
	//Window
	GLFWwindow* window;
	const int WINDOW_WIDTH;
	const int WINDOW_HEIGHT;
	int framebufferWidth;
	int framebufferHeight;
	//OpenGL Context
	const int GL_VERSION_MAJOR;
	const int GL_VERSION_MINOR;
	//Delta Time
	float dt;
	float curTime;
	float lastTime;
	//Mouse Input
	double lastMouseX;
	double lastMouseY;
	double mouseX;
	double mouseY;
	double mouseOffsetX;
	double mouseOffsetY;
	bool firstMouse;
	//Camera
	Camera camera;
	//Matrices
	glm::mat4 ViewMatrix;
	glm::vec3 camPosition;
	glm::vec3 worldUp;
	glm::vec3 camFront;
	glm::mat4 ProjectionMatrix;
	float fov;
	float nearPlane;
	float farPlane;
	//Shaders
	std::vector<Shader*> shaders; //Shader* means we will use dynamic memory
	//Textures
	std::vector<Texture*> textures;
	//Materials
	std::vector<Material*> materials;
	//Meshes
	std::vector<Mesh*> meshes;
	//Lights
	std::vector<glm::vec3*> lights;
	//Boundaries
	std::vector<MinMaxValues> minMaxValues;
	//OBJ Variables
	//std::vector<glm::vec3> out_vertices;
	//std::vector<glm::vec2> out_texcoords;
	//std::vector<glm::vec3> out_normals;
	//Animation
	int animationFrameCounter;
	int animationCycleCounter;
	int animationBeginCounter;


	//PRIVATE FUNCTIONS
	void initGLFW();
	void initWindow(const char* title, bool resizeable);
	void initGLEW(); //AFTER CONTEXT CREATION!! - needs opengl window ready
	void initOpenGLOptions();
	void initMatrices();
	void initShader();
	void initTextures();
	void initMaterials();
	void initMeshes();
	void initLights();
	void initUniforms();

	void updateUniforms();

	//STATIC VARIABLES

public:
	//CONSTRUCTORS/DECONSTRUCTORS
	World(const char* title, const int WINDOW_WIDTH, const int WINDOW_HEIGHT, const int GL_VERSION_MAJOR, const int GL_VERSION_MINOR, bool resizeable);
	virtual ~World();


	//ACCESSORS

	int getWindowShouldClose();
	int getAnimationBeginCounter();
	int getAnimationCycleCounter();

	//MODIFIERS

	void setWindowShouldClose();

	//FUNCTIONS
	void updateDT();
	void updateMouseInput();
	void updateKeyboardInput();
	void updateInput();
	void update();
	void render();

	//Functions for F20GP
	void updateBouncingBall();
	float getBallHeight();
	float getBallVelocity();
	bool isBallBouncing();
	void initParticleRandomisation();
	void updateParticleMovement();
	int getParticleLife();

	//STATIC VARIABLES

	//STATIC FUNCTIONS
	static void framebuffer_resize_callback(GLFWwindow* window, int fbW, int fbH);

};

