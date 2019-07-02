#include "World.h"

//Private functions

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

	this->textures.push_back(new Texture("models/floor.png", GL_TEXTURE_2D));
	this->textures.push_back(new Texture("models/floor_specular.png", GL_TEXTURE_2D));
	this->textures.push_back(new Texture("models/wall.png", GL_TEXTURE_2D));
	this->textures.push_back(new Texture("models/wall_specular.png", GL_TEXTURE_2D));
	this->textures.push_back(new Texture("models/couch.png", GL_TEXTURE_2D));
	this->textures.push_back(new Texture("models/couch_specular.png", GL_TEXTURE_2D));
	this->textures.push_back(new Texture("models/table.png", GL_TEXTURE_2D));
	this->textures.push_back(new Texture("models/table_specular.png", GL_TEXTURE_2D));
	this->textures.push_back(new Texture("models/lamp.png", GL_TEXTURE_2D));
	this->textures.push_back(new Texture("models/lamp_specular.png", GL_TEXTURE_2D));
	this->textures.push_back(new Texture("models/fireplace.png", GL_TEXTURE_2D));
	this->textures.push_back(new Texture("models/fireplace_specular.png", GL_TEXTURE_2D));
	this->textures.push_back(new Texture("models/lamp2.png", GL_TEXTURE_2D));
	this->textures.push_back(new Texture("models/lamp2_specular.png", GL_TEXTURE_2D));
	this->textures.push_back(new Texture("models/robot_body.png", GL_TEXTURE_2D));
	this->textures.push_back(new Texture("models/robot_body_specular.png", GL_TEXTURE_2D));
	this->textures.push_back(new Texture("models/robot_arm.png", GL_TEXTURE_2D));
	this->textures.push_back(new Texture("models/robot_arm_specular.png", GL_TEXTURE_2D));
	this->textures.push_back(new Texture("models/ball.png", GL_TEXTURE_2D));
	this->textures.push_back(new Texture("models/ball_specular.png", GL_TEXTURE_2D));

}

void World::initMaterials() {

	this->materials.push_back(new Material(glm::vec3(0.3f), glm::vec3(1.f), glm::vec3(0.8f), 0, 1));

}

void World::initMeshes() {

	//Floor
	std::vector<Vertex> floorVertices = loadOBJ("models/floor.obj");
	this->meshes.push_back(new Mesh(floorVertices.data(), floorVertices.size(), nullptr, 0,
			glm::vec3(0.f),
			glm::vec3(0.f),
			glm::vec3(1.f)));

	//Back wall
	std::vector<Vertex> wallVertices = loadOBJ("models/wall.obj");
	this->meshes.push_back(new Mesh(wallVertices.data(), wallVertices.size(), nullptr, 0,
			glm::vec3(0.f, -0.05f, -5.1f),
			glm::vec3(0.f, 270.f, 0.f),
			glm::vec3(1.f)));

	//Right Wall
	this->meshes.push_back(new Mesh(wallVertices.data(), wallVertices.size(), nullptr, 0,
			glm::vec3(5.1f, -0.05f, 0.f),
			glm::vec3(0.f, 180.f, 0.f),
			glm::vec3(1.f)));

	//Couch (BACK)
	std::vector<Vertex> couchVertices = loadOBJ("models/couch.obj");
	this->meshes.push_back(new Mesh(couchVertices.data(), couchVertices.size(), nullptr, 0,
			glm::vec3(0.2f, 0.f, -3.6f),
			glm::vec3(0.f, 270.f, 0.f),
			glm::vec3(0.5f, 0.5f, 0.85f)));

	//Couch (LEFT)
	this->meshes.push_back(new Mesh(couchVertices.data(), couchVertices.size(), nullptr, 0,
			glm::vec3(-3.2f, 0.f, -0.7f),
			glm::vec3(0.f, 0.f, 0.f),
			glm::vec3(0.5f, 0.5f, 0.35f)));

	//Table
	std::vector<Vertex> tableVertices = loadOBJ("models/table.obj");
	this->meshes.push_back(new Mesh(tableVertices.data(), tableVertices.size(), nullptr, 0,
			glm::vec3(-3.7f, 0.f, -3.8f),
			glm::vec3(0.f, 60.f, 0.f),
			glm::vec3(0.5f, 0.35f, 0.7f)));

	//Lamp
	std::vector<Vertex> lampVertices = loadOBJ("models/lamp.obj");
	this->meshes.push_back(new Mesh(lampVertices.data(), lampVertices.size(), nullptr, 0,
			glm::vec3(4.f, 0.05f, -4.f),
			glm::vec3(0.f),
			glm::vec3(.5f, .5f, .5f)));

	//Fireplace
	std::vector<Vertex> fireplaceVertices = loadOBJ("models/fireplace.obj");
	this->meshes.push_back(new Mesh(fireplaceVertices.data(), fireplaceVertices.size(), nullptr, 0,
			glm::vec3(3.2f, -0.2f, 3.2f),
			glm::vec3(0.f, 135.f, 0.f),
			glm::vec3(0.75f)));

	//Table Lamp
	std::vector<Vertex> lamp_tableVertices = loadOBJ("models/lamp2.obj");
	this->meshes.push_back(new Mesh(lamp_tableVertices.data(), lamp_tableVertices.size(), nullptr, 0,
			glm::vec3(-3.7f, 1.5f, -3.8f),
			glm::vec3(0.f, 0.f, 0.f),
			glm::vec3(0.2f)));

	//Robot Body
	std::vector<Vertex> robot_bodyVertices = loadOBJ("models/robot_body.obj");
	this->meshes.push_back(new Mesh(robot_bodyVertices.data(), robot_bodyVertices.size(), nullptr, 0,
			glm::vec3(-1.f, 0.f, -0.2f),
			glm::vec3(0.f, 0.f, 0.f),
			glm::vec3(0.4f)));

	//Robot Arm Left
	std::vector<Vertex> robot_armVertices = loadOBJ("models/robot_arm.obj");
	this->meshes.push_back(new Mesh(robot_armVertices.data(), robot_armVertices.size(), nullptr, 0,
			glm::vec3(-1.f, 2.2f, -0.6f),
			glm::vec3(14.f, 0.f, 0.f),
			glm::vec3(0.2f)));

	//Robot Arm Right
	this->meshes.push_back(new Mesh(robot_armVertices.data(), robot_armVertices.size(), nullptr, 0,
			glm::vec3(-1.f, 2.2f, 0.2f),
			glm::vec3(-14.f, 180.f, 0.f),
			glm::vec3(0.2f)));

	//Ball
	std::vector<Vertex> ballVertices = loadOBJ("models/ball.obj");
	this->meshes.push_back(new Mesh(ballVertices.data(), ballVertices.size(), nullptr, 0,
			glm::vec3(-0.f, 0.f, 0.5f),
			glm::vec3(0.f, 0.f, 0.f),
			glm::vec3(0.15f)));

}

void World::initLights() {

	this->lights.push_back(new glm::vec3(-3.7f, 2.5f, -3.8f));
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
	camera(glm::vec3(0.f, 0.f, 1.f), glm::vec3(0.f, 0.f, 1.f), glm::vec3(0.f, 1.f, 0.f)) {

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
	this->textures[TEX_COUCH]->bind(0);
	this->textures[TEX_COUCH_SPECULAR]->bind(1);
	this->meshes[MODEL_COUCH_BACK]->render(this->shaders[SHADER_CORE_PROGRAM]);
	this->meshes[MODEL_COUCH_LEFT]->render(this->shaders[SHADER_CORE_PROGRAM]);
	this->textures[TEX_TABLE]->bind(0);
	this->textures[TEX_TABLE_SPECULAR]->bind(1);
	this->meshes[MODEL_TABLE]->render(this->shaders[SHADER_CORE_PROGRAM]);
	this->textures[TEX_LAMP]->bind(0);
	this->textures[TEX_LAMP_SPECULAR]->bind(1);
	this->meshes[MODEL_LAMP]->render(this->shaders[SHADER_CORE_PROGRAM]);
	this->textures[TEX_FIREPLACE]->bind(0);
	this->textures[TEX_FIREPLACE_SPECULAR]->bind(1);
	this->meshes[MODEL_FIREPLACE]->render(this->shaders[SHADER_CORE_PROGRAM]);
	this->textures[TEX_LAMP_TABLE]->bind(0);
	this->textures[TEX_LAMP_TABLE_SPECULAR]->bind(1);
	this->meshes[MODEL_LAMP_TABLE]->render(this->shaders[SHADER_CORE_PROGRAM]);
	this->textures[TEX_ROBOT_BODY]->bind(0);
	this->textures[TEX_ROBOT_BODY_SPECULAR]->bind(1);
	this->meshes[MODEL_ROBOT_BODY]->render(this->shaders[SHADER_CORE_PROGRAM]);
	this->textures[TEX_ROBOT_ARM]->bind(0);
	this->textures[TEX_ROBOT_ARM_SPECULAR]->bind(1);
	this->meshes[MODEL_ROBOT_ARM_LEFT]->render(this->shaders[SHADER_CORE_PROGRAM]);
	this->meshes[MODEL_ROBOT_ARM_RIGHT]->render(this->shaders[SHADER_CORE_PROGRAM]);
	this->textures[TEX_BALL]->bind(0);
	this->textures[TEX_BALL_SPECULAR]->bind(1);
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

void World::animate() {

	if (this->animationCycleCounter < 3)
		this->throwBall();
	else
		this->wallFall();

	this->animationFrameCounter++;
	this->render();
}

void World::throwBall() {

	if (this->animationFrameCounter < 20) {
		this->meshes[MODEL_ROBOT_BODY]->rotate(glm::vec3(0.f, 0.f, -1.5f)); //Robot moves forward slightly
		this->meshes[MODEL_ROBOT_ARM_RIGHT]->move(glm::vec3(0.05f, -0.025f, 0.f)); //Robot moves forward slightly
		this->meshes[MODEL_ROBOT_ARM_LEFT]->move(glm::vec3(0.025f, -0.025f, 0.f)); //Robot moves forward slightly
		this->meshes[MODEL_ROBOT_ARM_RIGHT]->rotate(glm::vec3(0.f, 0.f, 0.25f)); //Robot's right arm moves towards ball
		this->meshes[MODEL_ROBOT_ARM_LEFT]->rotate(glm::vec3(0.f, 0.f, -1.125f)); //Robot's left arm tilts back slightly
	}
	//Robot pick up ball
	else if (this->animationFrameCounter < 40) {
		this->meshes[MODEL_ROBOT_BODY]->rotate(glm::vec3(0.f, 0.f, 1.5f)); //Robot moves forward slightly
		this->meshes[MODEL_ROBOT_ARM_RIGHT]->move(glm::vec3(-0.05f, 0.025f, 0.f)); //Robot moves forward slightly
		this->meshes[MODEL_ROBOT_ARM_LEFT]->move(glm::vec3(-0.025f, 0.025f, 0.f)); //Robot moves forward slightly
		this->meshes[MODEL_ROBOT_ARM_RIGHT]->rotate(glm::vec3(0.f, 0.f, -0.25f)); //Robot's right arm moves towards ball
		this->meshes[MODEL_ROBOT_ARM_LEFT]->rotate(glm::vec3(0.f, 0.f, 1.125f)); //Robot's left arm tilts back slightly
		this->meshes[MODEL_BALL]->move(glm::vec3(-0.05f, 0.025f, 0.f)); //Ball moves with arm
	}
	//Robot throw ball
	//Move arm behind
	else if (this->animationFrameCounter < 45) {
		this->meshes[MODEL_ROBOT_ARM_RIGHT]->rotate(glm::vec3(0.f, 0.7f, 4.5f));
		this->meshes[MODEL_BALL]->move(glm::vec3(-0.15f, 0.055f, 0.f));
	}
	else if (this->animationFrameCounter < 52) {
		this->meshes[MODEL_ROBOT_ARM_RIGHT]->rotate(glm::vec3(0.f, 0.7f, 4.5f));
		this->meshes[MODEL_BALL]->move(glm::vec3(-0.1f, 0.06f, 0.f));
	}
	else if (this->animationFrameCounter < 60) {
		this->meshes[MODEL_ROBOT_ARM_RIGHT]->rotate(glm::vec3(0.f, 0.7f, 4.5f));
		this->meshes[MODEL_BALL]->move(glm::vec3(-0.0355f, 0.11f, 0.f));
	}
	//Move arm above
	else if (this->animationFrameCounter < 68) {
		this->meshes[MODEL_ROBOT_ARM_RIGHT]->rotate(glm::vec3(1.5f, 0.f, 4.5f));
		this->meshes[MODEL_BALL]->move(glm::vec3(0.0355f, 0.11f, 0.f));
	}
	else if (this->animationFrameCounter < 75) {
		this->meshes[MODEL_ROBOT_ARM_RIGHT]->rotate(glm::vec3(1.5f, 0.f, 4.5f));
		this->meshes[MODEL_BALL]->move(glm::vec3(0.1f, 0.06f, 0.f));
	}
	else if (this->animationFrameCounter < 80) {
		this->meshes[MODEL_ROBOT_ARM_RIGHT]->rotate(glm::vec3(1.5f, 0.f, 4.5f));
		this->meshes[MODEL_BALL]->move(glm::vec3(0.15f, 0.055f, 0.f));
	}
	//Move Arm to original position and ball towards wall
	else if (this->animationFrameCounter < 120) {
		//Arm Movement
		if (this->animationFrameCounter == 80)
			this->meshes[MODEL_ROBOT_ARM_RIGHT]->rotate(glm::vec3(0.f, -14.f, 0.f));
		this->meshes[MODEL_ROBOT_ARM_RIGHT]->rotate(glm::vec3(-0.75f, 0.f, 4.5f));
		this->meshes[MODEL_BALL]->move(glm::vec3(0.149f, 0.f, 0.f));
		if (this->animationFrameCounter == 118 || this->animationFrameCounter == 119) {
			this->meshes[MODEL_BALL]->scaleChange(glm::vec3(-0.02f, 0.02f, 0.f));
			this->meshes[MODEL_WALL_RIGHT]->rotate(glm::vec3(0.f, 0.f, 1.f));
		}
	}
	//Ball Bounce back
	else if (this->animationFrameCounter < 122) { //Unsquish and move back tiny bir
		this->meshes[MODEL_BALL]->move(glm::vec3(-0.149f, 0.f, 0.f));
		this->meshes[MODEL_BALL]->scaleChange(glm::vec3(0.02f, -0.02f, 0.f));
		this->meshes[MODEL_WALL_RIGHT]->rotate(glm::vec3(0.f, 0.f, -1.f));
	}
	else if (this->animationFrameCounter < 140) { //Fall and move back
		this->meshes[MODEL_BALL]->move(glm::vec3(-0.1f, -0.1f, 0.f));
	}
	else if (this->animationFrameCounter < 150) {
		this->meshes[MODEL_BALL]->move(glm::vec3(-0.075f, -0.155f, 0.f));
	}
	else if (this->animationFrameCounter < 152) {
		this->meshes[MODEL_BALL]->move(glm::vec3(-0.06f, -0.15f, 0.f));
		this->meshes[MODEL_BALL]->scaleChange(glm::vec3(0.01f, -0.01f, 0.f));
	}
	else if (this->animationFrameCounter < 154) {
		this->meshes[MODEL_BALL]->move(glm::vec3(-0.04f, 0.15f, 0.f));
		this->meshes[MODEL_BALL]->scaleChange(glm::vec3(-0.01f, 0.01f, 0.f));
	}
	else if (this->animationFrameCounter < 168) {
		this->meshes[MODEL_BALL]->move(glm::vec3(-0.035f, 0.01f, 0.f));
	}
	else if (this->animationFrameCounter < 172) {
		this->meshes[MODEL_BALL]->move(glm::vec3(-0.035f, 0.0075f, 0.f));
	}
	else if (this->animationFrameCounter < 176) {
		this->meshes[MODEL_BALL]->move(glm::vec3(-0.03f, -0.0075f, 0.f));
	}
	else if (this->animationFrameCounter < 180) {
		this->meshes[MODEL_BALL]->move(glm::vec3(-0.028f, -0.01f, 0.f));
	}
	else if (this->animationFrameCounter < 193) {
		this->meshes[MODEL_BALL]->move(glm::vec3(-0.026f, -0.03f, 0.f));
	}
	else if (this->animationFrameCounter < 195) {
		this->meshes[MODEL_BALL]->move(glm::vec3(-0.025f, -0.03f, 0.f));
		this->meshes[MODEL_BALL]->scaleChange(glm::vec3(0.005f, -0.005f, 0.f));
	}
	else if (this->animationFrameCounter < 197) {
		this->meshes[MODEL_BALL]->move(glm::vec3(-0.025f, 0.03f, 0.f));
		this->meshes[MODEL_BALL]->scaleChange(glm::vec3(-0.005f, 0.005f, 0.f));
	}
	else if (this->animationFrameCounter < 200) {
		this->meshes[MODEL_BALL]->move(glm::vec3(-0.025f, 0.03f, 0.f));
	}
	else if (this->animationFrameCounter < 205) {
		this->meshes[MODEL_BALL]->move(glm::vec3(-0.025f, -0.02f, 0.f));
	}
	else if (this->animationFrameCounter < 215) {
		this->meshes[MODEL_BALL]->move(glm::vec3(-0.025f, 0.f, 0.f));
	}
	else if (this->animationFrameCounter < 220) {
		this->meshes[MODEL_BALL]->move(glm::vec3(-0.0324f, 0.f, 0.f));
	}
	else {
		this->animationFrameCounter = -1;
		this->animationCycleCounter++;
	}
}

void World::wallFall() {

	if (this->animationFrameCounter < 20) {
		this->meshes[MODEL_ROBOT_BODY]->rotate(glm::vec3(0.f, 0.f, -1.5f)); //Robot moves forward slightly
		this->meshes[MODEL_ROBOT_ARM_RIGHT]->move(glm::vec3(0.05f, -0.025f, 0.f)); //Robot moves forward slightly
		this->meshes[MODEL_ROBOT_ARM_LEFT]->move(glm::vec3(0.025f, -0.025f, 0.f)); //Robot moves forward slightly
		this->meshes[MODEL_ROBOT_ARM_RIGHT]->rotate(glm::vec3(0.f, 0.f, 0.25f)); //Robot's right arm moves towards ball
		this->meshes[MODEL_ROBOT_ARM_LEFT]->rotate(glm::vec3(0.f, 0.f, -1.125f)); //Robot's left arm tilts back slightly
	}
	//Robot pick up ball
	else if (this->animationFrameCounter < 40) {
		this->meshes[MODEL_ROBOT_BODY]->rotate(glm::vec3(0.f, 0.f, 1.5f)); //Robot moves forward slightly
		this->meshes[MODEL_ROBOT_ARM_RIGHT]->move(glm::vec3(-0.05f, 0.025f, 0.f)); //Robot moves forward slightly
		this->meshes[MODEL_ROBOT_ARM_LEFT]->move(glm::vec3(-0.025f, 0.025f, 0.f)); //Robot moves forward slightly
		this->meshes[MODEL_ROBOT_ARM_RIGHT]->rotate(glm::vec3(0.f, 0.f, -0.25f)); //Robot's right arm moves towards ball
		this->meshes[MODEL_ROBOT_ARM_LEFT]->rotate(glm::vec3(0.f, 0.f, 1.125f)); //Robot's left arm tilts back slightly
		this->meshes[MODEL_BALL]->move(glm::vec3(-0.05f, 0.025f, 0.f)); //Ball moves with arm
	}
	//Robot throw ball
	//Move arm behind
	else if (this->animationFrameCounter < 45) {
		this->meshes[MODEL_ROBOT_ARM_RIGHT]->rotate(glm::vec3(0.f, 0.7f, 4.5f));
		this->meshes[MODEL_BALL]->move(glm::vec3(-0.15f, 0.055f, 0.f));
	}
	else if (this->animationFrameCounter < 52) {
		this->meshes[MODEL_ROBOT_ARM_RIGHT]->rotate(glm::vec3(0.f, 0.7f, 4.5f));
		this->meshes[MODEL_BALL]->move(glm::vec3(-0.1f, 0.06f, 0.f));
	}
	else if (this->animationFrameCounter < 60) {
		this->meshes[MODEL_ROBOT_ARM_RIGHT]->rotate(glm::vec3(0.f, 0.7f, 4.5f));
		this->meshes[MODEL_BALL]->move(glm::vec3(-0.0355f, 0.11f, 0.f));
	}
	//Move arm above
	else if (this->animationFrameCounter < 68) {
		this->meshes[MODEL_ROBOT_ARM_RIGHT]->rotate(glm::vec3(1.5f, 0.f, 4.5f));
		this->meshes[MODEL_BALL]->move(glm::vec3(0.0355f, 0.11f, 0.f));
	}
	else if (this->animationFrameCounter < 75) {
		this->meshes[MODEL_ROBOT_ARM_RIGHT]->rotate(glm::vec3(1.5f, 0.f, 4.5f));
		this->meshes[MODEL_BALL]->move(glm::vec3(0.1f, 0.06f, 0.f));
	}
	else if (this->animationFrameCounter < 80) {
		this->meshes[MODEL_ROBOT_ARM_RIGHT]->rotate(glm::vec3(1.5f, 0.f, 4.5f));
		this->meshes[MODEL_BALL]->move(glm::vec3(0.15f, 0.055f, 0.f));
	}
	//Move Arm to original position and ball towards wall
	else if (this->animationFrameCounter < 120) {
		//Arm Movement
		if (this->animationFrameCounter == 80)
			this->meshes[MODEL_ROBOT_ARM_RIGHT]->rotate(glm::vec3(0.f, -14.f, 0.f));
		this->meshes[MODEL_ROBOT_ARM_RIGHT]->rotate(glm::vec3(-0.75f, 0.f, 4.5f));
		this->meshes[MODEL_BALL]->move(glm::vec3(0.149f, 0.f, 0.f));
		if (this->animationFrameCounter == 118 || this->animationFrameCounter == 119) {
			this->meshes[MODEL_BALL]->scaleChange(glm::vec3(-0.02f, 0.02f, 0.f));
			this->meshes[MODEL_WALL_RIGHT]->rotate(glm::vec3(0.f, 0.f, 1.f));
		}
	}
	//Ball Bounce back
	else if (this->animationFrameCounter < 122) { //Unsquish and move back tiny bir
		this->meshes[MODEL_BALL]->move(glm::vec3(-0.149f, 0.f, 0.f));
		this->meshes[MODEL_BALL]->scaleChange(glm::vec3(0.02f, -0.02f, 0.f));
		this->meshes[MODEL_WALL_RIGHT]->rotate(glm::vec3(0.f, 0.f, 1.f));
	}
	else if (this->animationFrameCounter < 140) { //Fall and move back
		this->meshes[MODEL_BALL]->move(glm::vec3(-0.1f, -0.1f, 0.f));
		this->meshes[MODEL_WALL_RIGHT]->rotate(glm::vec3(0.f, 0.f, 1.f));
		this->meshes[MODEL_ROBOT_ARM_RIGHT]->rotate(glm::vec3(0.f, -0.7f, -4.5f));
		this->meshes[MODEL_ROBOT_ARM_LEFT]->rotate(glm::vec3(0.f, 0.7f, 4.5f));
	}
	else if (this->animationFrameCounter < 150) {
		this->meshes[MODEL_BALL]->move(glm::vec3(-0.075f, -0.155f, 0.f));
		this->meshes[MODEL_WALL_RIGHT]->rotate(glm::vec3(0.f, 0.f, 1.f));
		this->meshes[MODEL_ROBOT_ARM_RIGHT]->rotate(glm::vec3(0.f, -0.7f, -4.5f));
		this->meshes[MODEL_ROBOT_ARM_LEFT]->rotate(glm::vec3(0.f, 0.7f, 4.5f));
		this->meshes[MODEL_ROBOT_BODY]->rotate(glm::vec3(0.f, 0.f, 2.f));
		this->meshes[MODEL_ROBOT_ARM_RIGHT]->move(glm::vec3(-0.04f, -0.03f, 0.f));
		this->meshes[MODEL_ROBOT_ARM_LEFT]->move(glm::vec3(-0.04f, -0.03f, 0.f));
	}
	else if (this->animationFrameCounter < 152) {
		this->meshes[MODEL_BALL]->move(glm::vec3(-0.06f, -0.15f, 0.f));
		this->meshes[MODEL_BALL]->scaleChange(glm::vec3(0.01f, -0.01f, 0.f));
		this->meshes[MODEL_WALL_RIGHT]->rotate(glm::vec3(0.f, 0.f, 1.f));
		this->meshes[MODEL_ROBOT_BODY]->move(glm::vec3(0.12f, 0.f, 0.f));
		this->meshes[MODEL_ROBOT_ARM_RIGHT]->move(glm::vec3(0.12f, 0.f, 0.f));
		this->meshes[MODEL_ROBOT_ARM_LEFT]->move(glm::vec3(0.12f, 0.f, 0.f));
	}
	else if (this->animationFrameCounter < 154) {
		this->meshes[MODEL_BALL]->move(glm::vec3(-0.04f, 0.15f, 0.f));
		this->meshes[MODEL_BALL]->scaleChange(glm::vec3(-0.01f, 0.01f, 0.f));
		this->meshes[MODEL_WALL_RIGHT]->rotate(glm::vec3(0.f, 0.f, 1.f));
		this->meshes[MODEL_ROBOT_BODY]->move(glm::vec3(0.12f, 0.f, 0.f));
		this->meshes[MODEL_ROBOT_ARM_RIGHT]->move(glm::vec3(0.12f, 0.f, 0.f));
		this->meshes[MODEL_ROBOT_ARM_LEFT]->move(glm::vec3(0.12f, 0.f, 0.f));
	}
	else if (this->animationFrameCounter < 168) {
		this->meshes[MODEL_BALL]->move(glm::vec3(-0.035f, 0.01f, 0.f));
		this->meshes[MODEL_WALL_RIGHT]->rotate(glm::vec3(0.f, 0.f, 1.f));
		this->meshes[MODEL_ROBOT_BODY]->move(glm::vec3(0.12f, 0.f, 0.f));
		this->meshes[MODEL_ROBOT_ARM_RIGHT]->move(glm::vec3(0.12f, 0.f, 0.f));
		this->meshes[MODEL_ROBOT_ARM_LEFT]->move(glm::vec3(0.12f, 0.f, 0.f));
	}
	else if (this->animationFrameCounter < 172) {
		this->meshes[MODEL_BALL]->move(glm::vec3(-0.035f, 0.0075f, 0.f));
		this->meshes[MODEL_WALL_RIGHT]->rotate(glm::vec3(0.f, 0.f, 1.f));
		this->meshes[MODEL_ROBOT_BODY]->move(glm::vec3(0.12f, 0.f, 0.f));
		this->meshes[MODEL_ROBOT_ARM_RIGHT]->move(glm::vec3(0.12f, 0.f, 0.f));
		this->meshes[MODEL_ROBOT_ARM_LEFT]->move(glm::vec3(0.12f, 0.f, 0.f));
	}
	else if (this->animationFrameCounter < 176) {
		this->meshes[MODEL_BALL]->move(glm::vec3(-0.03f, -0.0075f, 0.f));
		this->meshes[MODEL_WALL_RIGHT]->rotate(glm::vec3(0.f, 0.f, 1.f));
		this->meshes[MODEL_ROBOT_BODY]->move(glm::vec3(0.12f, 0.f, 0.f));
		this->meshes[MODEL_ROBOT_ARM_RIGHT]->move(glm::vec3(0.12f, 0.f, 0.f));
		this->meshes[MODEL_ROBOT_ARM_LEFT]->move(glm::vec3(0.12f, 0.f, 0.f));
	}
	else if (this->animationFrameCounter < 180) {
		this->meshes[MODEL_BALL]->move(glm::vec3(-0.028f, -0.01f, 0.f));
		this->meshes[MODEL_WALL_RIGHT]->rotate(glm::vec3(0.f, 0.f, 1.f));
		this->meshes[MODEL_ROBOT_BODY]->move(glm::vec3(0.12f, 0.f, 0.f));
		this->meshes[MODEL_ROBOT_ARM_RIGHT]->move(glm::vec3(0.12f, 0.f, 0.f));
		this->meshes[MODEL_ROBOT_ARM_LEFT]->move(glm::vec3(0.12f, 0.f, 0.f));
	}
	else if (this->animationFrameCounter < 182) {
		this->meshes[MODEL_BALL]->move(glm::vec3(-0.026f, -0.03f, 0.f));
		this->meshes[MODEL_WALL_RIGHT]->rotate(glm::vec3(0.f, 0.f, 1.f));
		this->meshes[MODEL_ROBOT_BODY]->move(glm::vec3(0.12f, 0.f, 0.f));
		this->meshes[MODEL_ROBOT_ARM_RIGHT]->move(glm::vec3(0.12f, 0.f, 0.f));
		this->meshes[MODEL_ROBOT_ARM_LEFT]->move(glm::vec3(0.12f, 0.f, 0.f));
	}
	else if (this->animationFrameCounter < 193) {
		this->meshes[MODEL_BALL]->move(glm::vec3(-0.026f, -0.03f, 0.f));
		this->meshes[MODEL_WALL_RIGHT]->rotate(glm::vec3(0.f, 0.f, 1.f));
		this->meshes[MODEL_ROBOT_BODY]->move(glm::vec3(0.12f, 0.f, 0.f));
		this->meshes[MODEL_ROBOT_ARM_RIGHT]->move(glm::vec3(0.12f, 0.f, 0.f));
		this->meshes[MODEL_ROBOT_ARM_LEFT]->move(glm::vec3(0.12f, 0.f, 0.f));
	}
	else if (this->animationFrameCounter < 195) {
		this->meshes[MODEL_BALL]->move(glm::vec3(-0.025f, -0.03f, 0.f));
		this->meshes[MODEL_BALL]->scaleChange(glm::vec3(0.005f, -0.005f, 0.f));
		this->meshes[MODEL_WALL_RIGHT]->rotate(glm::vec3(0.f, 0.f, 1.f));
		this->meshes[MODEL_ROBOT_BODY]->move(glm::vec3(0.12f, 0.f, 0.f));
		this->meshes[MODEL_ROBOT_ARM_RIGHT]->move(glm::vec3(0.12f, 0.f, 0.f));
		this->meshes[MODEL_ROBOT_ARM_LEFT]->move(glm::vec3(0.12f, 0.f, 0.f));
	}
	else if (this->animationFrameCounter < 200) {
		this->meshes[MODEL_BALL]->move(glm::vec3(-0.025f, 0.03f, 0.f));
		this->meshes[MODEL_WALL_RIGHT]->rotate(glm::vec3(0.f, 0.f, 1.f));
		this->meshes[MODEL_ROBOT_BODY]->move(glm::vec3(0.12f, 0.f, 0.f));
		this->meshes[MODEL_ROBOT_ARM_RIGHT]->move(glm::vec3(0.12f, 0.f, 0.f));
		this->meshes[MODEL_ROBOT_ARM_LEFT]->move(glm::vec3(0.12f, 0.f, 0.f));
	}
	else if (this->animationFrameCounter < 205) {
		this->meshes[MODEL_BALL]->move(glm::vec3(-0.025f, -0.02f, 0.f));
		this->meshes[MODEL_WALL_RIGHT]->rotate(glm::vec3(0.f, 0.f, 1.f));
		this->meshes[MODEL_ROBOT_ARM_RIGHT]->move(glm::vec3(0.15f, -0.1f, 0.f));
		this->meshes[MODEL_ROBOT_ARM_LEFT]->move(glm::vec3(0.15f, -0.1f, 0.f));
		this->meshes[MODEL_ROBOT_ARM_RIGHT]->rotate(glm::vec3(0.f, 0.8f, 4.8f));
		this->meshes[MODEL_ROBOT_ARM_LEFT]->rotate(glm::vec3(0.f, -0.8f, -4.8f));
		this->meshes[MODEL_ROBOT_BODY]->rotate(glm::vec3(0.f, 0.f, -6.f));
	}
	else if (this->animationFrameCounter < 208) {
		this->meshes[MODEL_BALL]->move(glm::vec3(-0.025f, 0.f, 0.f));
		this->meshes[MODEL_WALL_RIGHT]->rotate(glm::vec3(0.f, 0.f, 1.f));
		this->meshes[MODEL_ROBOT_ARM_RIGHT]->move(glm::vec3(0.15f, -0.1f, 0.f));
		this->meshes[MODEL_ROBOT_ARM_LEFT]->move(glm::vec3(0.15f, -0.1f, 0.f));
		this->meshes[MODEL_ROBOT_ARM_RIGHT]->rotate(glm::vec3(0.f, 0.8f, 4.8f));
		this->meshes[MODEL_ROBOT_ARM_LEFT]->rotate(glm::vec3(0.f, -0.8f, -4.8f));
		this->meshes[MODEL_ROBOT_BODY]->rotate(glm::vec3(0.f, 0.f, -6.f));
	}
	else if (this->animationFrameCounter < 215) {
		this->meshes[MODEL_BALL]->move(glm::vec3(-0.025f, 0.f, 0.f));
		this->meshes[MODEL_ROBOT_ARM_RIGHT]->move(glm::vec3(0.1f, -0.08f, 0.f));
		this->meshes[MODEL_ROBOT_ARM_LEFT]->move(glm::vec3(0.1f, -0.08f, 0.f));
		this->meshes[MODEL_ROBOT_BODY]->rotate(glm::vec3(0.f, 0.f, -6.f));
	}
	else if (this->animationFrameCounter < 220) {
		this->meshes[MODEL_BALL]->move(glm::vec3(-0.0324f, 0.f, 0.f));
	}
	else {
		this->animationFrameCounter = -1;
		this->animationCycleCounter++;
	}
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

	//Main Loop
	while (!world.getWindowShouldClose()) {

		//UPDATE INPUT ---
		world.update();

		if (world.getAnimationBeginCounter() < 200)
			world.render();
		else if (world.getAnimationCycleCounter() < 4)
			world.animate();
		else
			world.render();
	}

	return 0;
}