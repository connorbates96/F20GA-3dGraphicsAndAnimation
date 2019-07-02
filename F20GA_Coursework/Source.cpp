#include "World.h"

void World::initGLFW() {

	//Initialise GLFW
	if (glfwInit() == GLFW_FALSE) {
		std::cout << "ERROR::GAME.CPP::GLFW_INIT_FAILED" << "\n";
		glfwTerminate();
	}

}

void World::initWindow(const char* title, bool resizeable) {

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, this->GL_VERSION_MAJOR);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, this->GL_VERSION_MINOR);
	glfwWindowHint(GLFW_RESIZABLE, resizeable);

	this->window = glfwCreateWindow(this->WINDOW_WIDTH, this->WINDOW_HEIGHT, title, NULL, NULL);

	if (this->window == nullptr) {
		std::cout << "ERROR::GAME.CPP::GLFW_WINDOW_INIT_FAILED" << "/n";
		glfwTerminate();
	}

	//For a dynamic window size
	glfwGetFramebufferSize(this->window, &this->framebufferWidth, &this->framebufferHeight); //Used for getting the window size
	glfwSetFramebufferSizeCallback(this->window, World::framebuffer_resize_callback);

	glfwMakeContextCurrent(this->window); 

}

void World::initGLEW() {

	//Initialise GLEW (Needs Window and OpenGL Context)
	glewExperimental = GL_TRUE; //We want to use the new features

	if (glewInit() != GLEW_OK) {//GLEW initialisation error

		std::cout << "ERROR::GAME.CPP::GLEW_INIT_FAILED" << "\n";
		glfwTerminate();

	}

}

void World::initOpenGLOptions() {

	glEnable(GL_DEPTH_TEST); //Possible to use z coordinate

	glEnable(GL_TEXTURE_2D);
	//glEnable(GL_CULL_FACE); //Allows the culling of faces that are hidden
	//glCullFace(GL_BACK); //Cull back side of 'triangle'
	glFrontFace(GL_CCW); //CounterClockWise

						 //glEnable(GL_BLEND); //Enables blending of colours
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //Front and back of shape is filled with GL_FILL (GL_LINE for drawing outlines)

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

}

void World::initMatrices() {

	this->ViewMatrix = glm::mat4(1.f); //Set viewmatrix to identity matrix
	this->ViewMatrix = glm::lookAt(this->camPosition, this->camPosition + this->camFront, this->worldUp);

	this->ProjectionMatrix = glm::mat4(1.f); //Set ProjectionMatrix to identity matrix
	this->ProjectionMatrix = glm::perspective(glm::radians(this->fov), static_cast<float>(this->framebufferWidth) / this->framebufferHeight, this->nearPlane, this->farPlane);

}

void World::initShader() {

	this->shaders.push_back(new Shader("vertex_core.glsl", "", "fragment_core.glsl"));

}

void World::initTextures() {

	this->textures.push_back(new Texture("models/gp_floor_grid.png", GL_TEXTURE_2D));
	this->textures.push_back(new Texture("models/floor_specular.png", GL_TEXTURE_2D));
	this->textures.push_back(new Texture("models/wall.png", GL_TEXTURE_2D));
	this->textures.push_back(new Texture("models/wall_specular.png", GL_TEXTURE_2D));
	this->textures.push_back(new Texture("models/gp_ball.png", GL_TEXTURE_2D));
	this->textures.push_back(new Texture("models/ball_specular.png", GL_TEXTURE_2D));
	this->textures.push_back(new Texture("models/gp_particle.png", GL_TEXTURE_2D));

}

void World::initMaterials() {

	this->materials.push_back(new Material(glm::vec3(0.3f), glm::vec3(1.f), glm::vec3(0.8f), 0, 1));

}

void World::initMeshes() {

	////Floor
	std::tuple<std::vector<Vertex>, MinMaxValues> floorTuple = loadOBJ("models/gp_floor.obj");
	std::vector<Vertex> floorVertices = std::get<0>(floorTuple);
	this->meshes.push_back(new Mesh(floorVertices.data(), floorVertices.size(), nullptr, 0,
		glm::vec3(0.f, 0.f, 0.f),
		glm::vec3(0.f),
		glm::vec3(5.f)));
	this->minMaxValues.push_back(std::get<1>(floorTuple));

	//Back wall
	std::tuple<std::vector<Vertex>, MinMaxValues> wallTuple = loadOBJ("models/wall.obj");
	std::vector<Vertex> wallVertices = std::get<0>(wallTuple);
	this->meshes.push_back(new Mesh(wallVertices.data(), wallVertices.size(), nullptr, 0,
		glm::vec3(0.f, -0.1f, -12.5f),
		glm::vec3(0.f, 270.f, 0.f),
		glm::vec3(2.5f)));

	//Right Wall
	this->meshes.push_back(new Mesh(wallVertices.data(), wallVertices.size(), nullptr, 0,
		glm::vec3(12.5f, -0.1f, 0.f),
		glm::vec3(0.f, 180.f, 0.f),
		glm::vec3(2.5f)));

	//Left Wall
	this->meshes.push_back(new Mesh(wallVertices.data(), wallVertices.size(), nullptr, 0,
		glm::vec3(-12.5f, -0.1f, 0.f),
		glm::vec3(0.f, 0.f, 0.f),
		glm::vec3(2.5f)));

	////Ball
	std::tuple<std::vector<Vertex>, MinMaxValues> ballTuple = loadOBJ("models/gp_ball.obj");
	std::vector<Vertex> ballVertices = std::get<0>(ballTuple);
	this->minMaxValues.push_back(std::get<1>(ballTuple));
	this->meshes.push_back(new Mesh(ballVertices.data(), ballVertices.size(), nullptr, 0,
		glm::vec3(0.f, 15.f, 0.f),
		glm::vec3(0.f, 0.f, 0.f),
		glm::vec3(1.f)));

	//Runner  //0.783 for each half block in grid
	this->meshes.push_back(new Mesh(ballVertices.data(), ballVertices.size(), nullptr, 0,
		glm::vec3(2.349f, .6f, 0.783f),
		glm::vec3(0.f, 0.f, 0.f),
		glm::vec3(.6f)));

	//Chaser
	this->meshes.push_back(new Mesh(ballVertices.data(), ballVertices.size(), nullptr, 0,
		glm::vec3(-0.783f, .6f, -0.783f),
		glm::vec3(0.f, 0.f, 0.f),
		glm::vec3(.6f)));

	//Obstacle1
	this->meshes.push_back(new Mesh(wallVertices.data(), wallVertices.size(), nullptr, 0,
		glm::vec3(0.8f, -0.1f, -2.4f),
		glm::vec3(0.f, 0.f, 0.f),
		glm::vec3(7.f, 0.5f, 2.f)));

	//Obstacle2
	this->meshes.push_back(new Mesh(wallVertices.data(), wallVertices.size(), nullptr, 0,
		glm::vec3(4.65f, -0.1f, -3.86f),
		glm::vec3(0.f, 90.f, 0.f),
		glm::vec3(7.f, 0.5f, 0.9f)));

	//Obstacle3
	this->meshes.push_back(new Mesh(wallVertices.data(), wallVertices.size(), nullptr, 0,
		glm::vec3(5.5f, -0.1f, 6.3f),
		glm::vec3(0.f, 0.f, 0.f),
		glm::vec3(7.f, 0.5f, 0.923f)));

	//Obstacle4
	this->meshes.push_back(new Mesh(wallVertices.data(), wallVertices.size(), nullptr, 0,
		glm::vec3(-4.65f, -0.1f, 3.9f),
		glm::vec3(0.f, 90.f, 0.f),
		glm::vec3(7.f, 0.5f, 0.95f)));


	//Particle
	for (int i = 0; i < this->particleCount; i++) {
		std::tuple<std::vector<Vertex>, MinMaxValues> particleTuple = loadOBJ("models/gp_ball.obj");
		std::vector<Vertex> particleVertices = std::get<0>(particleTuple);
		this->minMaxValues.push_back(std::get<1>(particleTuple));

		float scale = 0.2 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (0.2 - 0.4)));
		this->meshes.push_back(new Mesh(particleVertices.data(), particleVertices.size(), nullptr, 0,
			glm::vec3(0.f, 1.f, 0.f),
			glm::vec3(0.f, 0.f, 0.f),
			glm::vec3(scale)));
	}

}


void World::initLights() {

	this->lights.push_back(new glm::vec3(-3.7f, 6.5f, 2.f));
}

void World::initUniforms() {

	this->shaders[SHADER_CORE_PROGRAM]->setMat4fv(ViewMatrix, "ViewMatrix");
	this->shaders[SHADER_CORE_PROGRAM]->setMat4fv(ProjectionMatrix, "ProjectionMatrix");

	this->shaders[SHADER_CORE_PROGRAM]->setVec3f(*this->lights[0], "lightPos0");
}

void World::updateUniforms() {

	//Update ViewMatrix (camera)
	this->ViewMatrix = this->camera.getViewMatrix();

	this->shaders[SHADER_CORE_PROGRAM]->setMat4fv(ViewMatrix, "ViewMatrix");
	this->shaders[SHADER_CORE_PROGRAM]->setVec3f(this->camera.getPosition(), "cameraPos");

	//Incase window has been resized, the projection matrix is changed so that warping doesn't occur and the image stays the same size ratio
	glfwGetFramebufferSize(this->window, &this->framebufferWidth, &this->framebufferHeight);

	this->ProjectionMatrix = glm::perspective(glm::radians(this->fov), static_cast<float>(this->framebufferWidth) / this->framebufferHeight, this->nearPlane, this->farPlane);

	this->shaders[SHADER_CORE_PROGRAM]->setMat4fv(ProjectionMatrix, "ProjectionMatrix");
}


//Constructors/Destructors
World::World(const char* title, const int WINDOW_WIDTH, const int WINDOW_HEIGHT, const int GL_VERSION_MAJOR, const int GL_VERSION_MINOR, bool resizeable)
	: WINDOW_WIDTH(WINDOW_WIDTH), WINDOW_HEIGHT(WINDOW_HEIGHT), GL_VERSION_MAJOR(GL_VERSION_MAJOR), GL_VERSION_MINOR(GL_VERSION_MINOR),
	camera(glm::vec3(0.f, 6.f, 12.f), glm::vec3(0.f, 0.f, 1.f), glm::vec3(0.f, 1.f, 0.f)) {

	//Initialise Variables
	this->window = nullptr;
	this->framebufferHeight = this->WINDOW_HEIGHT;
	this->framebufferWidth = this->WINDOW_WIDTH;

	//CAMERA
	this->camPosition = glm::vec3(0.f, 0.f, 1.f); //Y value is 1 so that the camera is pulled back a little and the drawings can be seen
	this->worldUp = glm::vec3(0.f, 1.f, 0.f); //Which direction is up in the world
	this->camFront = glm::vec3(0.f, 0.f, -1.f); //Which direction is straight ahead

	this->fov = 90.f; //Camera field of view
	this->nearPlane = 0.1f; //Small value so objects don't disappear in front of camera, will disappear just behind
	this->farPlane = 1000.f; //Draw distance

							 //DeltaTime
	this->dt = 0.f;
	this->curTime = 0.f;
	this->lastTime = 0.f;

	//MouseInput
	this->lastMouseX = 0.0;
	this->lastMouseY = 0.0;
	this->mouseX = 0.0;
	this->mouseY = 0.0;
	this->mouseOffsetX = 0.0;
	this->mouseOffsetY = 0.0;
	this->firstMouse = true;

	//Animation
	this->animationFrameCounter = 0;
	this->animationCycleCounter = 0;
	this->animationBeginCounter = 0;

	//Initialise EVERYTHING
	this->initGLFW();
	this->initWindow(title, resizeable);
	this->initGLEW();
	this->initOpenGLOptions();
	this->initMatrices();
	this->initShader();
	this->initTextures();
	this->initMaterials();
	this->initMeshes();
	this->initLights();
	this->initUniforms();
}


World::~World() {

	glfwDestroyWindow(this->window);
	glfwTerminate();

	//For loops to delete all the shaders, textures, materials and meshes
	for (size_t i = 0; i < this->shaders.size(); i++)
		delete this->shaders[i];

	for (size_t i = 0; i < this->textures.size(); i++)
		delete this->textures[i];

	for (size_t i = 0; i < this->materials.size(); i++)
		delete this->materials[i];

	for (size_t i = 0; i < this->meshes.size(); i++)
		delete this->meshes[i];

	for (size_t i = 0; i < this->lights.size(); i++)
		delete this->lights[i];
}

//Accessors
int World::getWindowShouldClose()
{
	return glfwWindowShouldClose(this->window);
}

int World::getAnimationBeginCounter()
{
	return this->animationBeginCounter;
}

int World::getAnimationCycleCounter()
{
	return this->animationCycleCounter;
}

//Modifiers
void World::setWindowShouldClose()
{
	glfwSetWindowShouldClose(this->window, GLFW_TRUE);
}


//Functions

void World::render() {

	//DRAW ---
	//Clear
	glClearColor(0.f, 0.f, 0.f, 1.f); //Red, Green, Blue, Transparency
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	//Update the uniforms
	this->updateUniforms();

	//Update uniforms
	this->materials[MAT_1]->sendToShader(*this->shaders[SHADER_CORE_PROGRAM]);

	//Use a Program
	this->shaders[SHADER_CORE_PROGRAM]->use();

	//Activate Texture
	this->textures[TEX_FLOOR]->bind(0);
	this->textures[TEX_FLOOR_SPECULAR]->bind(1);

	//Draw
	this->meshes[MODEL_FLOOR]->render(this->shaders[SHADER_CORE_PROGRAM]);
	this->textures[TEX_WALL]->bind(0);
	this->textures[TEX_WALL_SPECULAR]->bind(1);
	this->meshes[MODEL_WALL_BACK]->render(this->shaders[SHADER_CORE_PROGRAM]);
	this->meshes[MODEL_WALL_RIGHT]->render(this->shaders[SHADER_CORE_PROGRAM]);
	this->meshes[MODEL_WALL_LEFT]->render(this->shaders[SHADER_CORE_PROGRAM]);

	this->textures[TEX_BALL]->bind(0);
	this->textures[TEX_BALL_SPECULAR]->bind(1);

	if (!this->explosion) this->meshes[MODEL_BALL]->render(this->shaders[SHADER_CORE_PROGRAM]);
	else if(this->particleLife > 0){

		if (this->particleLife > 95) {
			this->meshes[MODEL_BALL]->render(this->shaders[SHADER_CORE_PROGRAM]);
		}

		this->textures[TEX_PARTICLE]->bind(0);

		for (int i = 0; i < this->particleCount; i++) {
			this->meshes[MODEL_PARTICLE + i]->render(this->shaders[SHADER_CORE_PROGRAM]);
		}
		this->particleLife--;
	}
	//else {
	//	this->meshes[MODEL_CHASER]->render(this->shaders[SHADER_CORE_PROGRAM]);
	//	this->meshes[MODEL_RUNNER]->render(this->shaders[SHADER_CORE_PROGRAM]);
	//	this->meshes[MODEL_OBSTACLE_ONE]->render(this->shaders[SHADER_CORE_PROGRAM]);
	//	this->meshes[MODEL_OBSTACLE_TWO]->render(this->shaders[SHADER_CORE_PROGRAM]);
	//	this->meshes[MODEL_OBSTACLE_THREE]->render(this->shaders[SHADER_CORE_PROGRAM]);
	//	this->meshes[MODEL_OBSTACLE_FOUR]->render(this->shaders[SHADER_CORE_PROGRAM]);
	//}
	this->meshes[MODEL_BALL]->render(this->shaders[SHADER_CORE_PROGRAM]);
	
	//End draw
	glfwSwapBuffers(window); //Back buffer is being drawn while front buffer is being shown. These are then swapped and the back buffer(old front) is drawn on.
	glFlush();

	//Unbind and reset everything
	glBindVertexArray(0);
	glUseProgram(0); 
	glActiveTexture(0);
	glBindTexture(GL_TEXTURE_2D, 0);

	this->animationBeginCounter++;
}



void World::updateBouncingBall() {

	float ballMin = this->meshes[MODEL_BALL]->getHeight() + this->minMaxValues[MINMAX_BALL].minY;
	//printf("ballMin = %f, minY = %f, BallVelocity: %f \n", ballMin, this->minMaxValues[MINMAX_BALL].minY, this->ballVelocity);
	float floorMax = this->meshes[MODEL_FLOOR]->getHeight() + this->minMaxValues[MINMAX_FLOOR].maxY;

	if (ballMin < (floorMax + 0.02)) {

		//if (this->ballVelocity > 0) {
		//	this->falling = true;
		//}

		printf("BallMinY: %f, FloorMaxY: %f, BallVelocity: %f \n", ballMin, floorMax, this->ballVelocity);
		if (this->ballVelocity < 1.1) {
			this->ballBouncing = false;
			this->meshes[MODEL_BALL]->setPosition(glm::vec3(0.f, 1.f, 0.f));
			initParticleRandomisation();
			return;
		}
		this->ballVelocity = -(this->ballVelocity * this->friction);
		//this->falling = false;

	}

	glm::vec3 positionUpdate = glm::vec3(0.f, -this->ballVelocity/2, 0.f);

	float velocityChange = (this->gravity)/500;
	//if (velocityChange > 100) return;
	this->ballVelocity = this->ballVelocity + velocityChange;

	this->meshes[MODEL_BALL]->move(positionUpdate);
	this->meshes[MODEL_BALL]->rotate(glm::vec3(1.f, 0.f, 0.f));
	this->meshes[MODEL_BALL]->rotate(glm::vec3(0.f, 1.f, 0.f));

}

float World::getBallHeight()
{
	return this->meshes[MODEL_BALL]->getHeight();
}

float World::getBallVelocity()
{
	return this->ballVelocity;
}

bool World::isBallBouncing()
{
	return this->ballBouncing;
}

void World::initParticleRandomisation()
{
	srand(glfwGetTime()); //Initialise randomisation seed
	for (int i = 0; i < this->particleCount; i++) {

		float x = -0.3 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (0.3 - (-0.3))));
		float y = -0.3 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (0.3 - (-0.3))));
		float z = -0.3 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (0.3 - (-0.3))));

		this->particleRandoms.push_back(glm::vec3(x, y, z));
	}
	printf("PARTICLE RANDOMISATION: %d", this->particleRandoms.size());
	this->explosion = true;
}

void World::updateParticleMovement()
{

	for (int i = 0; i < this->particleCount; i++) {
		this->meshes[MODEL_PARTICLE + i]->move(this->particleRandoms[i]);
	}

}

int World::getParticleLife()
{
	return this->particleLife;
}


void World::updateDT() {

	this->curTime = static_cast<float>(glfwGetTime());
	this->dt = this->curTime - this->lastTime;
	this->lastTime = this->curTime;
}

void World::updateMouseInput() {

	glfwGetCursorPos(this->window, &this->mouseX, &this->mouseY);

	if (this->firstMouse) {

		this->lastMouseX = this->mouseX;
		this->lastMouseY = this->mouseY;
		this->firstMouse = false;

	}

	//Calculate Offset
	this->mouseOffsetX = this->mouseX - this->lastMouseX;
	this->mouseOffsetY = this->lastMouseY - this->mouseY; //Reversed because y is inverted

														  //Set last X and Y
	this->lastMouseX = this->mouseX;
	this->lastMouseY = this->mouseY;
}

void World::updateKeyboardInput() {

	//Escape to close window
	if (glfwGetKey(this->window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}

	//Movement
	if (glfwGetKey(this->window, GLFW_KEY_W) == GLFW_PRESS) {
		this->camera.move(dt, FORWARD);
	}
	if (glfwGetKey(this->window, GLFW_KEY_S) == GLFW_PRESS) {
		this->camera.move(dt, BACKWARD);
	}
	if (glfwGetKey(this->window, GLFW_KEY_A) == GLFW_PRESS) {
		this->camera.move(dt, LEFT);
	}
	if (glfwGetKey(this->window, GLFW_KEY_D) == GLFW_PRESS) {
		this->camera.move(dt, RIGHT);
	}
	if (glfwGetKey(this->window, GLFW_KEY_UP) == GLFW_PRESS) {
		this->meshes[MODEL_RUNNER]->move(glm::vec3(0.f, 0.f, -1.566f));
	}
	if (glfwGetKey(this->window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		this->meshes[MODEL_RUNNER]->move(glm::vec3(0.f, 0.f, 1.566f));
	}
	if (glfwGetKey(this->window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		this->meshes[MODEL_RUNNER]->move(glm::vec3(-1.566f, 0.f, 0.f));
	}
	if (glfwGetKey(this->window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		this->meshes[MODEL_RUNNER]->move(glm::vec3(1.566f, 0.f, 0.f));
	}
}

void World::updateInput() {

	glfwPollEvents();
	this->updateKeyboardInput();
	this->updateMouseInput();
	this->camera.updateInput(dt, -1, this->mouseOffsetX, this->mouseOffsetY);
}

void World::update() {

	//Update Input
	this->updateDT();
	this->updateInput();

}


//Static Functions
void World::framebuffer_resize_callback(GLFWwindow * window, int fbW, int fbH) { //Function for a resizeable window
	glViewport(0, 0, fbW, fbH);
}




//Main function
int main() {

	World world("F20GA Coursework", 1280, 960, 4, 5, true);


	//aStar search = aStar(std::make_tuple(1, 1), std::make_tuple(15, 15));

	//Main Loop for bouncing ball
	while (!world.getWindowShouldClose()) {

		world.update();
		if (world.isBallBouncing()) {
			world.updateBouncingBall();
		}
		else if (world.getParticleLife() > 0){
			world.updateParticleMovement();
		}
		world.render();

	}

	return 0;
}