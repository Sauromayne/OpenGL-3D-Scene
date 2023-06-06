#include "libs.h"

// declare variables for camera
float lastX = 400, lastY = 300;
bool firstMouse = true;
float pitch, yaw, fov = 45.f;

//declare bool for ortho/perspective
bool ortho = false;

// camera
glm::vec3 cameraPos = glm::vec3(0.f, 0.f, 0.f);
glm::vec3 cameraFront = glm::vec3(0.f, 0.f, 0.f);
glm::vec3 cameraUp = glm::vec3(0.f, 1.f, 0.f);


// check for input and update the program accordingly
void updateInput(GLFWwindow* window)
{
	float cameraSpeed = 0.05f; // adjust accordingly
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
	else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		cameraPos += cameraSpeed * cameraFront;
	}
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		cameraPos -= cameraSpeed * cameraFront;
	}
	else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	}
	else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	}
	else if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
	{
		cameraPos.y -= 0.05f;
	}
	else if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
	{
		cameraPos.y += 0.05f;
	}
	else if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
	{
		// if ortho is already enabled, disable it
		if (ortho)
			ortho = false;
		// if orther is diabled, enable it
		else
			ortho = true;
	}
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.05f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(direction);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	fov -= (float)yoffset;
	if (fov < 1.0f)
		fov = 1.0f;
	if (fov > 45.0f)
		fov = 45.0f;
}

// function to get the framebuffer during resize
void framebuffer_resize(GLFWwindow* window, int framebufferWidth, int framebufferHeight)
{
	glViewport(0, 0, framebufferWidth, framebufferHeight);
}


// main function of program
int main()
{
	// Initializers (move to class later)--------------------------------------------------------------------------------------------------------------
	
	// Initialize GLFW
	glfwInit();

	// Window settings
	const int WINDOW_WIDTH = 800;
	const int WINDOW_HEIGHT = 600;
	int framebufferWidth = 0;
	int framebufferHeight = 0;
	
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

	// Create Window
	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Textured House Scene | Nicholas Podsiadlo", NULL, NULL);

	// get the frame buffer and view port
	glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight);
	glfwSetFramebufferSizeCallback(window, framebuffer_resize);

	glfwMakeContextCurrent(window);

	// get mouse callback
	glfwSetCursorPosCallback(window, mouse_callback);
	// disable curtsor
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	// get scroll wheel callback
	glfwSetScrollCallback(window, scroll_callback);

	// Initialize GLEW
	glewExperimental = GL_TRUE;

	// check GLEW initialized succesfully
	if (glewInit() != GLEW_OK)
	{
		std::cout << "GLEW Initialization failed" << "\n";
		glfwTerminate();
	}

	// OpenGL Options
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// can change later if we want (outline or fill)
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// Initializers End (move to class later)--------------------------------------------------------------------------------------------------------------

	// Shader Initialize
	Shader core_program("vertex_core.glsl", "fragment_core.glsl");

	//Model Mesh (add mesh for objects in scene)
	
	//Add mesh for House (cube)
	Cube house = Cube();
	Mesh houseModel(&house,
		glm::vec3(0.f, 0.75f, 0.f),		//position
		glm::vec3(0.f, 0.f, 0.f),		//rotation
		glm::vec3(1.5f));				//scale

	//Add mesh for Roof (pyramid)
	Pyramid roof = Pyramid();
	Mesh roofModel(&roof,
		glm::vec3(0.f, 2.3f, 0.f),		//position
		glm::vec3(0.f, 0.f, 0.f),		//rotation
		glm::vec3(0.8f));				//scale

	//Add mesh for Grass (plane)
	Plane grass = Plane();
	Mesh grassModel(&grass,
		glm::vec3(0.f, 0.f, 0.f),		//position
		glm::vec3(0.f, 0.f, 0.f),		//rotation
		glm::vec3(1.f));				//scale

	// Texture initialize (move to function later)----------------------------------------------
	GLuint texture0, texture1, texture2, texture3;
	// texture 1
	glGenTextures(1, &texture0);
	glBindTexture(GL_TEXTURE_2D, texture0);
	// set texture wrapping
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create textures, generate mipmaps
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true); //tell stb_image to flip loaded texture vertically
	unsigned char* data = stbi_load("Textures/stucco.jpg", &width, &height, &nrChannels, 0);
	
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << "\n";
	}
	stbi_image_free(data);
	
	// texture 2
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	// set texture wrapping
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create textures, generate mipmaps
	stbi_set_flip_vertically_on_load(true); //tell stb_image to flip loaded texture vertically
	data = stbi_load("Textures/roof_shingle.jpg", &width, &height, &nrChannels, 0);

	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << "\n";
	}
	stbi_image_free(data);

	// texture 3
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	// set texture wrapping
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create textures, generate mipmaps
	stbi_set_flip_vertically_on_load(true); //tell stb_image to flip loaded texture vertically
	data = stbi_load("Textures/grass.jpg", &width, &height, &nrChannels, 0);

	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << "\n";
	}
	stbi_image_free(data);

	// texture 4
	glGenTextures(1, &texture3);
	glBindTexture(GL_TEXTURE_2D, texture3);
	// set texture wrapping
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create textures, generate mipmaps
	stbi_set_flip_vertically_on_load(true); //tell stb_image to flip loaded texture vertically
	data = stbi_load("Textures/water.png", &width, &height, &nrChannels, 0);

	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << "\n";
	}
	stbi_image_free(data);
	// Texture Initialize End (move to function later)-----------------------------------------
	
	// Main program/render loop-----------------------------------------------------------------
	while (!glfwWindowShouldClose(window))
	{
		// Update input
		glfwPollEvents();

		// update
		updateInput(window);

		// clear
		glClearColor(0.f, 0.f, 0.f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		core_program.use();

		// Projection Matrix (World)
		float nearPlane = 0.1f;
		float farPlane = 100.f;
		glm::mat4 ProjectionMatrix(1.f);
		
		// check if the window has been resized
		glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight);

		// Set the default Projection as perspective
		ProjectionMatrix = glm::perspective(glm::radians(fov), static_cast<float>(framebufferWidth / framebufferHeight), nearPlane, farPlane);
		// Check if the user wants to view in ortho mode
		if (ortho)
		{
			ProjectionMatrix = glm::ortho(-5.0f, 5.0f, -5.0f, 5.0f, -50.0f, 50.0f);
		}
		// Send Projection Matrix
		core_program.setMat4fv(ProjectionMatrix, "ProjectionMatrix");
		
		
		// View Matrix (Camera)
		glm::mat4 ViewMatrix(1.f);
		ViewMatrix = glm::lookAt(
			cameraPos,					// Camera movement
			cameraPos + cameraFront,	// Where camera looks
			cameraUp					// Head is looking up ((0, -1, 0) to look down)
		);
		// send View Matrix
		core_program.setMat4fv(ViewMatrix, "ViewMatrix");
		
		// use the program
		core_program.use();

		// bind textures (house)
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture0);
		houseModel.render(&core_program); // render house
		
		// new texture (roof)
		glBindTexture(GL_TEXTURE_2D, texture1);
		roofModel.render(&core_program); //render roof
		
		// new texture (grass)
		glBindTexture(GL_TEXTURE_2D, texture2);
		// add water texture on top
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture3);
		grassModel.render(&core_program); // render grass
		
		
		// end draw
		glfwSwapBuffers(window);
		glFlush();

		glBindVertexArray(0);
		glUseProgram(0);
	}

	// End of program-------------------------------------------------------------------------
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}