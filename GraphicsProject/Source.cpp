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
	double movementC = 0;
	double movement2C = 0;
	double movementD = 0;
	double movement2D = 0;
	double movementE = 0;
	double movement2E = 0;
	double pmovement = 0;
	double pmovement2 = 0;
	double pmovementA = 0;
	double pmovement2A = 0;
	double pmovementB = 0;
	double pmovement2B = 0;
	double time = 0;
	double CarGap = 34;
	double PlateGap = 40;
	double PlateGap1 = 20;
	double CarSpeed = 0.05;
	double PlateSpeed = 0.1;
	double PlateSpeed1 = 1;
	double plateY = 1.2;
	///////////
	double r = (3.3 / 4);
	double Lball = Ballx +r;   //ball from left
	double Tball = Ballx +r;
	double Rball = Ballx -r;  //ball from right
	double Bball = Ballx -r;
	double w = (7.2 / 2);
	double l = (10 / 2);
	double p,Lp,Rp,Tp,Bp ;  //plate 1
	double p2, Lp2, Rp2, Tp2, Bp2; //plate2
	double p3, Lp3, Rp3, Tp3, Bp3; //plate3
	double p4, Lp4, Rp4, Tp4, Bp4; //plate4
	double p5, Lp5, Rp5, Tp5, Bp5; //plate5
	double p6, Lp6, Rp6, Tp6, Bp6; //plate6
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
			//movement -= CarSpeed;
			time += 0.01;
		}
		else movement = 0;
		if (time > CarGap)
		{
			if (movement2 > -68)
			{
				Car1.Draw(MatrixID, vec3(34 + movement2, 1.6, -24), vec3(1, 1, 1));
				movement2 -= CarSpeed;
			}
			else movement2 = 0;
		}
		//-----------------------------1111111111111111111
		if (movementA > -68)
		{
			Car1.Draw(MatrixID, vec3(-34 - movementA, 1.6, -20), vec3(1, 1, 1), eulerAngleXYZ(0.0f, 3.14f, 0.0f));
			movementA -= CarSpeed;
			time += 0.01;
		}
		else movementA = 0;
		if (time > CarGap)
		{
			if (movement2A > -68)
			{
				Car1.Draw(MatrixID, vec3(-34 - movement2A, 1.6, -20), vec3(1, 1, 1), eulerAngleXYZ(0.0f, 3.14f, 0.0f));
				movement2A -= CarSpeed;
			}
			else movement2A = 0;
		}
		//----------------------------------111111111111111111111

		if (movementB > -68)
		{
			Car1.Draw(MatrixID, vec3(34 + movementB, 1.6, -6), vec3(1, 1, 1));
			movementB -= CarSpeed;
			time += 0.01;
		}
		else movementB = 0;
		if (time > CarGap)
		{
			if (movement2B > -68)
			{
				Car1.Draw(MatrixID, vec3(34 + movement2B, 1.6, -6), vec3(1, 1, 1));
				movement2B -= CarSpeed;
			}
			else movement2B = 0;
		}
		//-----------------------------2222222222222222222
		if (movementC > -68)
		{
			Car1.Draw(MatrixID, vec3(-34 - movementC, 1.6, -2), vec3(1, 1, 1), eulerAngleXYZ(0.0f, 3.14f, 0.0f));
			movementC -= CarSpeed;
			time += 0.01;
		}
		else movementC = 0;
		if (time > CarGap)
		{
			if (movement2C > -68)
			{
				Car1.Draw(MatrixID, vec3(-34 - movement2C, 1.6, -2), vec3(1, 1, 1), eulerAngleXYZ(0.0f, 3.14f, 0.0f));
				movement2C -= CarSpeed;
			}
			else movement2C = 0;
		}
		//----------------------------------22222222222222222

		if (movementD > -68)
		{
			Car1.Draw(MatrixID, vec3(34 + movementD, 1.6, 20), vec3(1, 1, 1));
			movementD -= CarSpeed;
			time += 0.01;
		}
		else movementD = 0;
		if (time > CarGap)
		{
			if (movement2D > -68)
			{
				Car1.Draw(MatrixID, vec3(34 + movement2D, 1.6, 20), vec3(1, 1, 1));
				movement2D -= CarSpeed;
			}
			else movement2D = 0;
		}
		//-----------------------------3333333333333
		if (movementE > -68)
		{
			Car1.Draw(MatrixID, vec3(-34 - movementE, 1.6, 24), vec3(1, 1, 1), eulerAngleXYZ(0.0f, 3.14f, 0.0f));
			movementE -= CarSpeed;
			time += 0.01;
		}
		else movementE = 0;
		if (time > CarGap)
		{
			if (movement2E > -68)
			{
				Car1.Draw(MatrixID, vec3(-34 - movement2E, 1.6, 24), vec3(1, 1, 1), eulerAngleXYZ(0.0f, 3.14f, 0.0f));
				movement2E -= CarSpeed;
			}
			else movement2E = 0;
		}
		//----------------------------------333333333


		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);

		Plate.PreDraw(PlateTexture, TextureID); // this is made once 
		//------------------------------1
		if (pmovement > -68)
		{

			Plate.Draw(MatrixID, vec3(34 + pmovement, plateY, -14), vec3(0.5, 0.5, 0.5), eulerAngleXYZ(0.0f, 3.14f / 2.0f, 0.0f));
			pmovement -= PlateSpeed;

			p = 34 + pmovement;  //plate 1
			Lp = p + w;   //leftplat
			Rp = p - w;    //right plate
			Tp = -14 + l;    //upper plate
			Bp = -14 - l;    //bottom plate

			 Lball = Ballx + r;   
			 Tball = Ballz + r;
			 Rball = Ballx - r;  
			 Bball = Ballz - r;
			 if (Tball<-9.5 && Bball>-18){
				 if (Lball > Rp &&  Rball < Lp && Tball<Tp && Bball>Bp){
					 //on plate1
					 Ballx -= PlateSpeed;
				 }
				 else {
					 glfwTerminate();
				 }
			 }
			time += 0.01;
		}
		else pmovement = 0;
		if (time > PlateGap)
		{
			if (pmovement2 > -68)
			{
				Plate.Draw(MatrixID, vec3(34 + pmovement2, plateY, -14), vec3(0.5, 0.5, 0.5), eulerAngleXYZ(0.0f, 3.14f / 2.0f, 0.0f));
				pmovement2 -= PlateSpeed;
				p2 = 34 + pmovement2;
				Lp2 = p2 + w;
				Rp2 = p2 - w;
				Tp2 = -14 + l;
				Bp2 = -14 - l;

				Lball = Ballx + r;
				Tball = Ballz + r;
				Rball = Ballx - r;
				Bball = Ballz - r;


				if (Lball>Rp2 && Rball < Lp2 && Tball<Tp2 && Bball>Bp2){
					//on plate2
					Ballx -= PlateSpeed;
				}
			}
			else pmovement2 = 0;
		}
		//------------------2
		
		if (pmovementA > -68)
		{

			Plate.Draw(MatrixID, vec3(34 + pmovementA, plateY, 4), vec3(0.5, 0.5, 0.5), eulerAngleXYZ(0.0f, 3.14f / 2.0f, 0.0f));
			pmovementA -= PlateSpeed;

			p3 = 34 + pmovementA;  //plate 1
			Lp3 = p3 + w;   //leftplat
			Rp3 = p3 - w;    //right plate
			Tp3 = 4 + l;    //upper plate
			Bp3 = 4 - l;    //bottom plate

			Lball = Ballx + r;
			Tball = Ballz + r;
			Rball = Ballx - r;
			Bball = Ballz - r;


			if (Lball > Rp3 &&  Rball<Lp3 && Tball<Tp3 && Bball>Bp3){
				//on plate1
				Ballx -= PlateSpeed;
			}

			time += 0.01;
		}
		else pmovementA = 0;
		if (time > PlateGap1)
		{
			if (pmovement2A > -68)
			{
				Plate.Draw(MatrixID, vec3(34 + pmovement2A, plateY, 4), vec3(0.5, 0.5, 0.5), eulerAngleXYZ(0.0f, 3.14f / 2.0f, 0.0f));
				pmovement2A -= PlateSpeed;

				p4 = 34 + pmovement2A;
				Lp4 = p4 + w;
				Rp4 = p4 - w;
				Tp4 = 4 + l;
				Bp4 = 4 - l;

				Lball = Ballx + r;
				Tball = Ballz + r;
				Rball = Ballx - r;
				Bball = Ballz - r;


				if (Lball>Rp4 && Rball < Lp4 && Tball<Tp4 && Bball>Bp4){
					//on plate2
					Ballx -= PlateSpeed;
				}
			}
			else pmovement2A = 0;
		}

		//-----------------------3

		if (pmovementB > -68)
		{

			Plate.Draw(MatrixID, vec3(34 + pmovementB, plateY, 12), vec3(0.5, 0.5, 0.5), eulerAngleXYZ(0.0f, 3.14f / 2.0f, 0.0f));
			pmovementB -= PlateSpeed;
			p5 = 34 + pmovementB;  //plate 1
			Lp5 = p5 + w;   //leftplat
			Rp5 = p5 - w;    //right plate
			Tp5 = 12 + l;    //upper plate
			Bp5 = 12 - l;    //bottom plate

			Lball = Ballx + r;
			Tball = Ballz + r;
			Rball = Ballx - r;
			Bball = Ballz - r;

			if (Lball > Rp5 &&  Rball<Lp5 && Tball>Tp5 && Bball<Bp5){
				Ballx -= PlateSpeed;
			}

			time += 0.01;
		}
		else pmovementB = 0;
		if (time > PlateGap1)
		{
			if (pmovement2B > -68)
			{
				Plate.Draw(MatrixID, vec3(34 + pmovement2B, plateY, 12), vec3(0.5, 0.5, 0.5), eulerAngleXYZ(0.0f, 3.14f / 2.0f, 0.0f));
				pmovement2B -= PlateSpeed;

				p6 = 34 + pmovement2B;  //plate 1
				Lp6 = p5 + w;   //leftplat
				Rp6 = p6 - w;    //right plate
				Tp6 = 12 + l;    //upper plate
				Bp6 = 12 - l;    //bottom plate

				Lball = Ballx + r;
				Tball = Ballz + r;
				Rball = Ballx - r;
				Bball = Ballz - r;

				if (Lball > Rp6 &&  Rball<Lp6 && Tball>Tp6 && Bball<Bp6){
					Ballx -= PlateSpeed;
				}

			}
			else pmovement2B = 0;
		}

		
		/*
		Plate.Draw(MatrixID, vec3(-24, plateY, -14), vec3(0.5, 0.5, 0.5), eulerAngleXYZ(0.0f, 3.14f / 2.0f, 0.0f));
		Plate.Draw(MatrixID, vec3(8, plateY, -14), vec3(0.5, 0.5, 0.5), eulerAngleXYZ(0.0f, 3.14f / 2.0f, 0.0f));
		Plate.Draw(MatrixID, vec3(24, 1.2, -14), vec3(0.5, 0.5, 0.5), eulerAngleXYZ(0.0f, 3.14f / 2.0f, 0.0f));*/


		/*
		Plate.Draw(MatrixID, vec3(-24, plateY, 4), vec3(0.5, 0.5, 0.5), eulerAngleXYZ(0.0f, 3.14f / 2.0f, 0.0f));
		Plate.Draw(MatrixID, vec3(8, plateY, 4), vec3(0.5, 0.5, 0.5), eulerAngleXYZ(0.0f, 3.14f / 2.0f, 0.0f));
		Plate.Draw(MatrixID, vec3(24, plateY, 4), vec3(0.5, 0.5, 0.5), eulerAngleXYZ(0.0f, 3.14f / 2.0f, 0.0f));*/

		/*
		Plate.Draw(MatrixID, vec3(-24, plateY, 12), vec3(0.5, 0.5, 0.5), eulerAngleXYZ(0.0f, 3.14f / 2.0f, 0.0f));
		Plate.Draw(MatrixID, vec3(8, plateY, 12), vec3(0.5, 0.5, 0.5), eulerAngleXYZ(0.0f, 3.14f / 2.0f, 0.0f));
		Plate.Draw(MatrixID, vec3(24, plateY, 12), vec3(0.5, 0.5, 0.5), eulerAngleXYZ(0.0f, 3.14f / 2.0f, 0.0f));
		*/

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);

		if (glfwGetKey(window, GLFW_KEY_A) != GLFW_PRESS)
			Ballx -= 0.1;
		if (glfwGetKey(window, GLFW_KEY_D) != GLFW_PRESS)
			Ballx += 0.1;
		if (glfwGetKey(window, GLFW_KEY_W) != GLFW_PRESS)
			Ballz -= 0.1;
		if (glfwGetKey(window, GLFW_KEY_S) != GLFW_PRESS)
			Ballz += 0.1;

		/////////////////////plate
		
		
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
