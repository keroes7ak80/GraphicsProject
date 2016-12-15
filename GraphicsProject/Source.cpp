// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>

// Include GLEW
#include <GL/glew.h>
// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>//rotation
#include <glm/gtx/norm.hpp>//rotation
using namespace glm;

#include "Shader.h"
#include"Texture.h"
#include"Model.h"
#include"Controls.h"

int main(void)
{
	// Initialise GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		getchar();
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	/*glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);*/

	// Open a window and create its OpenGL context
	window = glfwCreateWindow(1024, 768, "Street Crossing", NULL, NULL);
	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	// Hide the mouse and enable unlimited mouvement
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set the mouse at the center of the screen
	glfwPollEvents();
	glfwSetCursorPos(window, 1024 / 2, 768 / 2);

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

	// Cull triangles which normal is not towards the camera
	glEnable(GL_NONE);


	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);
	Shader MyShader;
	// Create and compile our GLSL program from the shaders
	GLuint programID = MyShader.LoadShaders("TransformVertexShader.vertexshader", "TextureFragmentShader.fragmentshader");

	// Get a handle for our "MVP" uniform
	GLuint MatrixID = glGetUniformLocation(programID, "MVP");
	// Get a handle for our "myTextureSampler" uniform
	GLuint TextureID = glGetUniformLocation(programID, "myTextureSampler");
	GLuint vertexPosition_modelspaceID = glGetAttribLocation(programID, "vertexPosition_modelspace");
	GLuint vertexUVID = glGetAttribLocation(programID, "vertexUV");

	// Load the texture
	//Important Note : To Load a model you have to create an Load texture ,object , GenBuffers,Predraw,Draw
	Texture MyTexture;
	GLuint Texture1 = MyTexture.loadDDS("uvmap.DDS");
	GLuint  MapTexture = MyTexture.loadBMP_custom("model.bmp");
	GLuint BallTexture = MyTexture.loadBMP_custom("Basketball.bmp");
	GLuint Car1Texture = MyTexture.loadBMP_custom("col1.bmp");
	GLuint PlateTexture = MyTexture.loadBMP_custom("wood.bmp");
	// Read our .obj file
	   // Won't be used at the moment.
	Model  Model1("cube.obj");
	Model  Map("Project Final b.obj");
	Model ball("0basketball.obj");
	Model Car1("OFF-Roadcar.obj");
	Model Plate("Plate.obj");
	// Load it into a VBO
	Model1.GenBuffers();
	Map.GenBuffers();
	ball.GenBuffers();
	Car1.GenBuffers();
	Plate.GenBuffers();
	Controls MyControls;
	double Ballx = 0;
	double Bally = 4;
	double Ballz = -32;
	double movement = 0;
	double movement2 = 0;
	double movementA = 0;
	double movement2A = 0;
	double movementB = 0;
	double movement2B = 0;
	double time = 0;
	do {


		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use our shader
		glUseProgram(programID);

		// Compute the MVP matrix from keyboard and mouse input
		MyControls.computeMatricesFromInputs();
		mat4 ProjectionMatrix = getProjectionMatrix();
		mat4 ViewMatrix = getViewMatrix();
		//Important Note : To Draw any model we must make pre draw Function then Draw the Disable glDisableVertexAttribArray for 1 and 0


	/*	Model1.PreDraw(Texture1,TextureID);
		Model1.Draw(MatrixID, vec3(0, 5, 0), vec3(1, 1, 1));
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		*/

		Map.PreDraw(MapTexture, TextureID); // this is made once 
		Map.Draw(MatrixID);
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);

		ball.PreDraw(BallTexture, TextureID);//this is made once
		ball.Draw(MatrixID, vec3(Ballx, 2, Ballz), vec3(0.25, 0.25, 0.25));
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);


		Car1.PreDraw(Car1Texture, TextureID);


		if (movement > -68)
		{
			Car1.Draw(MatrixID, vec3(34 + movement, 1.6, -24), vec3(1, 1, 1));
			movement -= 0.01;
			time += 0.01;
		}
		else movement = 0;
		if (time > 34)
		{
			if (movement2 > -68)
			{
				Car1.Draw(MatrixID, vec3(34 + movement2, 1.6, -24), vec3(1, 1, 1));
				movement2 -= 0.01;
			}
			else movement2 = 0;
		}
		//-----------------------------1111111111111111111
		if (movement > -68)
		{
			Car1.Draw(MatrixID, vec3(-34 - movement, 1.6, -20), vec3(1, 1, 1), eulerAngleXYZ(0.0f, 3.14f, 0.0f));
			movement -= 0.01;
			time += 0.01;
		}
		else movement = 0;
		if (time > 34)
		{
			if (movement2 > -68)
			{
				Car1.Draw(MatrixID, vec3(-34 - movement2, 1.6, -20), vec3(1, 1, 1), eulerAngleXYZ(0.0f, 3.14f, 0.0f));
				movement2 -= 0.01;
			}
			else movement2 = 0;
		}
		//----------------------------------111111111111111111111

		if (movement > -68)
		{
			Car1.Draw(MatrixID, vec3(34 + movement, 1.6, -6), vec3(1, 1, 1));
			movement -= 0.01;
			time += 0.01;
		}
		else movement = 0;
		if (time > 8.5)
		{
			if (movement2 > -68)
			{
				Car1.Draw(MatrixID, vec3(34 + movement2, 1.6, -6), vec3(1, 1, 1));
				movement2 -= 0.01;
			}
			else movement2 = 0;
		}
		//-----------------------------2222222222222222222
		if (movement > -68)
		{
			Car1.Draw(MatrixID, vec3(-34 - movement, 1.6, -2), vec3(1, 1, 1), eulerAngleXYZ(0.0f, 3.14f, 0.0f));
			movement -= 0.01;
			time += 0.01;
		}
		else movement = 0;
		if (time > 8.5)
		{
			if (movement2 > -68)
			{
				Car1.Draw(MatrixID, vec3(-34 - movement2, 1.6, -2), vec3(1, 1, 1), eulerAngleXYZ(0.0f, 3.14f, 0.0f));
				movement2 -= 0.01;
			}
			else movement2 = 0;
		}
		//----------------------------------22222222222222222

		if (movement > -68)
		{
			Car1.Draw(MatrixID, vec3(34 + movement, 1.6, 20), vec3(1, 1, 1));
			movement -= 0.01;
			time += 0.01;
		}
		else movement = 0;
		if (time > 8.5)
		{
			if (movement2 > -68)
			{
				Car1.Draw(MatrixID, vec3(34 + movement2, 1.6, 20), vec3(1, 1, 1));
				movement2 -= 0.01;
			}
			else movement2 = 0;
		}
		//-----------------------------3333333333333
		if (movement > -68)
		{
			Car1.Draw(MatrixID, vec3(-34 - movement, 1.6, 24), vec3(1, 1, 1), eulerAngleXYZ(0.0f, 3.14f, 0.0f));
			movement -= 0.01;
			time += 0.01;
		}
		else movement = 0;
		if (time > 8.5)
		{
			if (movement2 > -68)
			{
				Car1.Draw(MatrixID, vec3(-34 - movement2, 1.6, 24), vec3(1, 1, 1), eulerAngleXYZ(0.0f, 3.14f, 0.0f));
				movement2 -= 0.01;
			}
			else movement2 = 0;
		}
		//----------------------------------333333333


		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);

		Plate.PreDraw(PlateTexture, TextureID); // this is made once 
		Plate.Draw(MatrixID, vec3(-6, 0.3, -3.5), vec3(0.5, 0.5, 0.5), eulerAngleXYZ(0.0f, 3.14f / 2.0f, 0.0f));
		Plate.Draw(MatrixID, vec3(2, 0.3, -3.5), vec3(0.5, 0.5, 0.5), eulerAngleXYZ(0.0f, 3.14f / 2.0f, 0.0f));
		Plate.Draw(MatrixID, vec3(6, 0.3, -3.5), vec3(0.5, 0.5, 0.5), eulerAngleXYZ(0.0f, 3.14f / 2.0f, 0.0f));

		Plate.Draw(MatrixID, vec3(-6, 0.3, 1), vec3(0.5, 0.5, 0.5), eulerAngleXYZ(0.0f, 3.14f / 2.0f, 0.0f));
		Plate.Draw(MatrixID, vec3(2, 0.3, 1), vec3(0.5, 0.5, 0.5), eulerAngleXYZ(0.0f, 3.14f / 2.0f, 0.0f));
		Plate.Draw(MatrixID, vec3(6, 0.3, 1), vec3(0.5, 0.5, 0.5), eulerAngleXYZ(0.0f, 3.14f / 2.0f, 0.0f));


		Plate.Draw(MatrixID, vec3(-6, 0.3, 3), vec3(0.5, 0.5, 0.5), eulerAngleXYZ(0.0f, 3.14f / 2.0f, 0.0f));
		Plate.Draw(MatrixID, vec3(2, 0.3, 3), vec3(0.5, 0.5, 0.5), eulerAngleXYZ(0.0f, 3.14f / 2.0f, 0.0f));
		Plate.Draw(MatrixID, vec3(6, 0.3, 3), vec3(0.5, 0.5, 0.5), eulerAngleXYZ(0.0f, 3.14f / 2.0f, 0.0f));


		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);

		if (glfwGetKey(window, GLFW_KEY_A) != GLFW_PRESS)
			Ballx -= 0.04;
		if (glfwGetKey(window, GLFW_KEY_D) != GLFW_PRESS)
			Ballx += 0.04;
		if (glfwGetKey(window, GLFW_KEY_W) != GLFW_PRESS)
			Ballz -= 0.04;
		if (glfwGetKey(window, GLFW_KEY_S) != GLFW_PRESS)
			Ballz += 0.04;


		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);

	// Cleanup VBO and shader
	Model1.DeleteBuffers();
	Map.DeleteBuffers();
	ball.DeleteBuffers();
	Car1.DeleteBuffers();
	glDeleteProgram(programID);
	glDeleteTextures(1, &TextureID);
	glDeleteVertexArrays(1, &VertexArrayID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}

