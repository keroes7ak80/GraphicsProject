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
	GLuint programID = MyShader.LoadShaders("StandardShading.vertexshader", "StandardShading.fragmentshader");
	GLuint LightID = glGetUniformLocation(programID, "LightPosition_worldspace");
	// Get a handle for our "MVP" uniform
	GLuint MatrixID = glGetUniformLocation(programID, "MVP");
	// Get a handle for our "myTextureSampler" uniform
	GLuint TextureID = glGetUniformLocation(programID, "myTextureSampler");
	GLuint vertexPosition_modelspaceID = glGetAttribLocation(programID, "vertexPosition_modelspace");
	GLuint vertexUVID = glGetAttribLocation(programID, "vertexUV");
	GLuint ViewMatrixID = glGetUniformLocation(programID, "V");
	GLuint ModelMatrixID = glGetUniformLocation(programID, "M");
	// Load the texture
	//Important Note : To Load a model you have to create an Load texture ,object , GenBuffers,Predraw,Draw
	Texture MyTexture;
	GLuint Texture1 = MyTexture.loadDDS("uvmap.DDS");
	GLuint  MapTexture = MyTexture.loadBMP_custom("model.bmp");
	GLuint BallTexture = MyTexture.loadBMP_custom("Basketball.bmp");
	GLuint Car1Texture = MyTexture.loadBMP_custom("col1.bmp");
	GLuint PlateTexture = MyTexture.loadBMP_custom("wood.bmp");
	GLuint Image1= MyTexture.loadBMP_custom("wood.bmp");
	// Read our .obj file
	// Won't be used at the moment.
	Model  Model1("cube.obj");
	Model  Map("Project Final b.obj");
	Model ball("0basketball.obj");
	Model Car1("OFF-Roadcar.obj");
	Model Plate("Plate.obj");
	Model Image("Image.obj");
	// Load it into a VBO
	Model1.GenBuffers();
	Map.GenBuffers();
	ball.GenBuffers();
	Car1.GenBuffers();
	Plate.GenBuffers();
	Image.GenBuffers();
	Controls MyControls;
	double Bally = 2;
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
	double ZZ = 0;
	double CarGap = 34;
	double PlateGap = 40;
	double PlateGap1 = 20;
	double CarSpeed = 0.05;
	double BallSpeed = 0.1;
	double CarSpeed1 = CarSpeed+0.015;
	double CarSpeed2 = CarSpeed + 0.02;
	double PlateSpeed = 0.05;
	double PlateSpeed1 = PlateSpeed+0.035;
	int xxpos;
		int yypos;
	double plateY = 1.2;
	double Z = 0;
	///////////
	double x = 0;
	double r = (3.3 / 4);
	double Lball = Ballx + r;   //ball from left
	double Tball = Ballx + r;
	double Rball = Ballx - r;  //ball from right
	double Bball = Ballx - r;
	double w = (7.2 / 2);
	double l = (15 / 2);
	double p, Lp, Rp, Tp, Bp;  //plate 1
	double p2, Lp2, Rp2, Tp2, Bp2; //plate2
	double p3, Lp3, Rp3, Tp3, Bp3; //plate3
	double p4, Lp4, Rp4, Tp4, Bp4; //plate4
	double p5, Lp5, Rp5, Tp5, Bp5; //plate5
	double p6, Lp6, Rp6, Tp6, Bp6; //plate6
	double ballrad = 3.3 / 4;
	double carwidth = 2.1;
float rotateX=0.0f;
float rotateZ=0.0f;
	//while (glfwGetKey(window, GLFW_KEY_1) != GLFW_PRESS&&glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS)
	//{
		//glm::vec3 lightPos = glm::vec3(0, 0, -50);
		//glUniform3f(LightID, lightPos.x, lightPos.y, lightPos.z);
		//MyControls.computeMatricesFromInputs();
		// Clear the screen
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		//Image.PreDraw(Image1, TextureID);
		//Image.Draw(MatrixID, ModelMatrixID, ViewMatrixID,vec3(0,0,0),vec3(1,1,1), eulerAngleXYZ(3.14f/2.0f,0.0f, 0.0f));

	//	 Swap buffers
	//	glfwSwapBuffers(window);
	
//}
	
	do
	{
		
		glm::vec3 lightPos = glm::vec3(34, 50, -20);
		glUniform3f(LightID, lightPos.x, lightPos.y, lightPos.z);

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
		Map.Draw(MatrixID, ModelMatrixID, ViewMatrixID);
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);


		ball.PreDraw(BallTexture, TextureID);//this is made once
		ball.Draw(MatrixID, ModelMatrixID, ViewMatrixID, vec3(Ballx, Bally, Ballz), vec3(0.25, 0.25, 0.25),eulerAngleXYZ(rotateX, 0.0f, rotateZ));
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);




		if (glfwGetKey(window, GLFW_KEY_LEFT) != GLFW_PRESS)
{ Ballx -= BallSpeed;
rotateZ+=0.02f;
}
		if (glfwGetKey(window, GLFW_KEY_RIGHT) != GLFW_PRESS)
			
{rotateZ-=0.02f;
Ballx += BallSpeed;
}
		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		{
rotateX+=0.02f;
			Ballz += BallSpeed;
			Z++;
		}
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		{
rotateX-=0.02f;
			Z--;
			Ballz -= BallSpeed;
		}

		if (Z== 565 && ZZ ==0)
		{
			Ballx = 0;
			Bally = 4;
			Ballz = -32;
			CarSpeed = 0.1;
			PlateSpeed = 0.1;
			BallSpeed = 0.15;
			 PlateSpeed1 = PlateSpeed + 0.035;
			 CarSpeed1 = CarSpeed + 0.015;
			CarSpeed2 = CarSpeed + 0.02;
			ZZ = 1;
		
		}


		if (Z == 1130 && ZZ ==1)
		{
			Ballx = 0;
			Bally = 4;
			Ballz = -32;
			CarSpeed = 0.16;
			PlateSpeed = 0.16;
			BallSpeed = 0.18;
			PlateSpeed1 = PlateSpeed + 0.035;
			CarSpeed1 = CarSpeed + 0.015;
			CarSpeed2 = CarSpeed + 0.02;
			ZZ = 2;
		}
		if (Z >= 1700 && ZZ >=2)
		{
			Bally += BallSpeed;
			ZZ++;
			if ( ZZ == 300 )
				glfwTerminate();
		}
		Car1.PreDraw(Car1Texture, TextureID);
		if (movement > -68)
		{
			Car1.Draw(MatrixID, ModelMatrixID, ViewMatrixID, vec3(34 + movement, 1.6, -24), vec3(1, 1, 1));
			double cen11x = 34 + movement;
			bool checkx1 = (Ballx - ballrad<cen11x - carwidth&&Ballx + ballrad>cen11x - carwidth) || (Ballx + ballrad > cen11x + carwidth&&Ballx + ballrad < cen11x + carwidth) || (Ballx - ballrad > cen11x - carwidth&&Ballx + ballrad < cen11x + carwidth);
			bool checkz1 = (Ballz - ballrad<-24 - 0.7&&Ballz + ballrad>-24 - 0.7) || (Ballz - ballrad<-24 + 0.7&&Ballz + ballrad>-24 + 0.7) || (Ballz - ballrad > -24 - 0.7&&Ballz + ballrad < -24 + 0.7);
			if (checkx1&&checkz1)
			{
				glfwTerminate();
			}
			movement -= CarSpeed;
			time += 0.01;
		}
		else movement = 0;
		if (time > CarGap)
		{
			if (movement2 > -68)
			{
				Car1.Draw(MatrixID, ModelMatrixID, ViewMatrixID, vec3(34 + movement2, 1.6, -24), vec3(1, 1, 1));
				double cen21x = 34 + movement2;
				bool checkx2 = (Ballx - ballrad<cen21x - carwidth&&Ballx + ballrad>cen21x - carwidth) || (Ballx + ballrad > cen21x + carwidth&&Ballx + ballrad < cen21x + carwidth) || (Ballx - ballrad > cen21x - carwidth&&Ballx + ballrad < cen21x + carwidth);
				bool checkz2 = (Ballz - ballrad<-24 - 0.7&&Ballz + ballrad>-24 - 0.7) || (Ballz - ballrad<-24 + 0.7&&Ballz + ballrad>-24 + 0.7) || (Ballz - ballrad > -24 - 0.7&&Ballz + ballrad < -24 + 0.7);
				if (checkx2&&checkz2)
				{
					glfwTerminate();
				}
				movement2 -= CarSpeed;
			}
			else movement2 = 0;
		}
		//-----------------------------1111111111111111111
		if (movementA > -68)
		{
			Car1.Draw(MatrixID, ModelMatrixID, ViewMatrixID, vec3(-34 - movementA, 1.6, -20), vec3(1, 1, 1), eulerAngleXYZ(0.0f, 3.14f, 0.0f));
			double cen12x = -34 - movementA;
			bool checkx3 = (Ballx - ballrad<cen12x - carwidth&&Ballx + ballrad>cen12x - carwidth) || (Ballx + ballrad > cen12x + carwidth&&Ballx + ballrad < cen12x + carwidth) || (Ballx - ballrad > cen12x - carwidth&&Ballx + ballrad < cen12x + carwidth);
			bool checkz3 = (Ballz - ballrad<-20 - 0.7&&Ballz + ballrad>-20 - 0.7) || (Ballz - ballrad<-20 + 0.7&&Ballz + ballrad>-20 + 0.7) || (Ballz - ballrad > -20 - 0.7&&Ballz + ballrad < -20 + 0.7);
			if (checkx3&&checkz3)
			{
				glfwTerminate();
			}
			movementA -= CarSpeed;
			time += 0.01;
		}
		else movementA = 0;
		if (time > CarGap)
		{
			if (movement2A > -68)
			{
				Car1.Draw(MatrixID, ModelMatrixID, ViewMatrixID, vec3(-34 - movement2A, 1.6, -20), vec3(1, 1, 1), eulerAngleXYZ(0.0f, 3.14f, 0.0f));
				double cen22x = -34 - movement2A;
				bool checkx4 = (Ballx - ballrad<cen22x - carwidth&&Ballx + ballrad>cen22x - carwidth) || (Ballx + ballrad > cen22x + carwidth&&Ballx + ballrad < cen22x + carwidth) || (Ballx - ballrad > cen22x - carwidth&&Ballx + ballrad < cen22x + carwidth);
				bool checkz4 = (Ballz - ballrad<-20 - 0.7&&Ballz + ballrad>-20 - 0.7) || (Ballz - ballrad<-20 + 0.7&&Ballz + ballrad>-20 + 0.7) || (Ballz - ballrad > -20 - 0.7&&Ballz + ballrad < -20 + 0.7);
				if (checkx4&&checkz4)
				{
					glfwTerminate();
				}
				movement2A -= CarSpeed;
			}
			else movement2A = 0;
		}
		//----------------------------------111111111111111111111

		if (movementB > -68)
		{
			Car1.Draw(MatrixID, ModelMatrixID, ViewMatrixID, vec3(34 + movementB, 1.6, -6), vec3(1, 1, 1));
			double cen13x = 34 + movementB;
			bool checkx5 = (Ballx - ballrad<cen13x - carwidth&&Ballx + ballrad>cen13x - carwidth) || (Ballx + ballrad > cen13x + carwidth&&Ballx + ballrad < cen13x + carwidth) || (Ballx - ballrad > cen13x - carwidth&&Ballx + ballrad < cen13x + carwidth);
			bool checkz5 = (Ballz - ballrad<-6 - 0.7&&Ballz + ballrad>-6 - 0.7) || (Ballz - ballrad<-6 + 0.7&&Ballz + ballrad>-6 + 0.7) || (Ballz - ballrad > -6 - 0.7&&Ballz + ballrad < -6 + 0.7);
			if (checkx5&&checkz5)
			{
				glfwTerminate();
			}
			movementB -= CarSpeed1;
			time += 0.01;
		}
		else movementB = 0;
		if (time > CarGap)
		{
			if (movement2B > -68)
			{
				Car1.Draw(MatrixID, ModelMatrixID, ViewMatrixID, vec3(34 + movement2B, 1.6, -6), vec3(1, 1, 1));
				double cen23x = 34 + movement2B;
				bool checkx6 = (Ballx - ballrad<cen23x - carwidth&&Ballx + ballrad>cen23x - carwidth) || (Ballx + ballrad > cen23x + carwidth&&Ballx + ballrad < cen23x + carwidth) || (Ballx - ballrad > cen23x - carwidth&&Ballx + ballrad < cen23x + carwidth);
				bool checkz6 = (Ballz - ballrad<-6 - 0.7&&Ballz + ballrad>-6 - 0.7) || (Ballz - ballrad<-6 + 0.7&&Ballz + ballrad>-6 + 0.7) || (Ballz - ballrad > -6 - 0.7&&Ballz + ballrad < -6 + 0.7);
				if (checkx6&&checkz6)
				{
					glfwTerminate();
				}
				movement2B -= CarSpeed1;
			}
			else movement2B = 0;
		}
		//-----------------------------2222222222222222222
		if (movementC > -68)
		{
			Car1.Draw(MatrixID, ModelMatrixID, ViewMatrixID, vec3(-34 - movementC, 1.6, -2), vec3(1, 1, 1), eulerAngleXYZ(0.0f, 3.14f, 0.0f));
			double cen14x = -34 - movementC;
			bool checkx7 = (Ballx - ballrad<cen14x - carwidth&&Ballx + ballrad>cen14x - carwidth) || (Ballx + ballrad > cen14x + carwidth&&Ballx + ballrad < cen14x + carwidth) || (Ballx - ballrad > cen14x - carwidth&&Ballx + ballrad < cen14x + carwidth);
			bool checkz7 = (Ballz - ballrad<-2 - 0.7&&Ballz + ballrad>-2 - 0.7) || (Ballz - ballrad<-2 + 0.7&&Ballz + ballrad>-2 + 0.7) || (Ballz - ballrad > -2 - 0.7&&Ballz + ballrad < -2 + 0.7);
			if (checkx7&&checkz7)
			{
				glfwTerminate();
			}
			movementC -= CarSpeed;
			time += 0.01;
		}
		else movementC = 0;
		if (time > CarGap)
		{
			if (movement2C > -68)
			{
				Car1.Draw(MatrixID, ModelMatrixID, ViewMatrixID, vec3(-34 - movement2C, 1.6, -2), vec3(1, 1, 1), eulerAngleXYZ(0.0f, 3.14f, 0.0f));
				double cen24x = -34 - movement2C;
				bool checkx8 = (Ballx - ballrad<cen24x - carwidth&&Ballx + ballrad>cen24x - carwidth) || (Ballx + ballrad > cen24x + carwidth&&Ballx + ballrad < cen24x + carwidth) || (Ballx - ballrad > cen24x - carwidth&&Ballx + ballrad < cen24x + carwidth);
				bool checkz8 = (Ballz - ballrad<-2 - 0.7&&Ballz + ballrad>-2 - 0.7) || (Ballz - ballrad<-2 + 0.7&&Ballz + ballrad>-2 + 0.7) || (Ballz - ballrad > -2 - 0.7&&Ballz + ballrad < -2 + 0.7);
				if (checkx8&&checkz8)
				{
					glfwTerminate();
				}
				movement2C -= CarSpeed;
			}
			else movement2C = 0;
		}
		//----------------------------------22222222222222222

		if (movementD > -68)
		{
			Car1.Draw(MatrixID, ModelMatrixID, ViewMatrixID, vec3(34 + movementD, 1.6, 20), vec3(1, 1, 1));
			double cen15x = 34 + movementD;
			bool checkx9 = (Ballx - ballrad<cen15x - carwidth&&Ballx + ballrad>cen15x - carwidth) || (Ballx + ballrad > cen15x + carwidth&&Ballx + ballrad < cen15x + carwidth) || (Ballx - ballrad > cen15x - carwidth&&Ballx + ballrad < cen15x + carwidth);
			bool checkz9 = (Ballz - ballrad < 20 - 0.7&&Ballz + ballrad>20 - 0.7) || (Ballz - ballrad < 20 + 0.7&&Ballz + ballrad>20 + 0.7) || (Ballz - ballrad > 20 - 0.7&&Ballz + ballrad < 20 + 0.7);
			if (checkx9&&checkz9)
			{
				glfwTerminate();
			}
			movementD -= CarSpeed1;
			time += 0.01;
		}
		else movementD = 0;
		if (time > CarGap)
		{
			if (movement2D > -68)
			{
				Car1.Draw(MatrixID, ModelMatrixID, ViewMatrixID, vec3(34 + movement2D, 1.6, 20), vec3(1, 1, 1));
				double cen25x = 34 + movement2D;
				bool checkx10 = (Ballx - ballrad<cen25x - carwidth&&Ballx + ballrad>cen25x - carwidth) || (Ballx + ballrad > cen25x + carwidth&&Ballx + ballrad < cen25x + carwidth) || (Ballx - ballrad > cen25x - carwidth&&Ballx + ballrad < cen25x + carwidth);
				bool checkz10 = (Ballz - ballrad < 20 - 0.7&&Ballz + ballrad>20 - 0.7) || (Ballz - ballrad < 20 + 0.7&&Ballz + ballrad>20 + 0.7) || (Ballz - ballrad > 20 - 0.7&&Ballz + ballrad < 20 + 0.7);
				if (checkx10&&checkz10)
				{
					glfwTerminate();
				}
				movement2D -= CarSpeed1;
			}
			else movement2D = 0;
		}
		//-----------------------------3333333333333
		if (movementE > -68)
		{
			Car1.Draw(MatrixID, ModelMatrixID, ViewMatrixID, vec3(-34 - movementE, 1.6, 24), vec3(1, 1, 1), eulerAngleXYZ(0.0f, 3.14f, 0.0f));
			double cen16x = -34 - movementE;
			bool checkx11 = (Ballx - ballrad<cen16x - carwidth&&Ballx + ballrad>cen16x - carwidth) || (Ballx + ballrad > cen16x + carwidth&&Ballx + ballrad < cen16x + carwidth) || (Ballx - ballrad > cen16x - carwidth&&Ballx + ballrad < cen16x + carwidth);
			bool checkz11 = (Ballz - ballrad < 24 - 0.7&&Ballz + ballrad>24 - 0.7) || (Ballz - ballrad < 24 + 0.7&&Ballz + ballrad>24 + 0.7) || (Ballz - ballrad > 24 - 0.7&&Ballz + ballrad < 24 + 0.7);
			if (checkx11&&checkz11)
			{
				glfwTerminate();
			}
			movementE -= CarSpeed2;
			time += 0.01;
		}
		else movementE = 0;
		if (time > CarGap)
		{
			if (movement2E > -68)
			{
				Car1.Draw(MatrixID, ModelMatrixID, ViewMatrixID, vec3(-34 - movement2E, 1.6, 24), vec3(1, 1, 1), eulerAngleXYZ(0.0f, 3.14f, 0.0f));
				double cen26x = -34 - movement2E;
				bool checkx12 = (Ballx - ballrad<cen26x - carwidth&&Ballx + ballrad>cen26x - carwidth) || (Ballx + ballrad > cen26x + carwidth&&Ballx + ballrad < cen26x + carwidth) || (Ballx - ballrad > cen26x - carwidth&&Ballx + ballrad < cen26x + carwidth);
				bool checkz12 = (Ballz - ballrad < 24 - 0.7&&Ballz + ballrad>24 - 0.7) || (Ballz - ballrad < 24 + 0.7&&Ballz + ballrad>24 + 0.7) || (Ballz - ballrad > 24 - 0.7&&Ballz + ballrad < 24 + 0.7);
				if (checkx12&&checkz12)
				{
					glfwTerminate();
				}
				movement2E -= CarSpeed2;
			}
			else movement2E = 0;
		}
		//----------------------------------333333333



		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);

		Plate.PreDraw(PlateTexture, TextureID); // this is made once 
												//------------------------------1
												//------------------------------1
		if (pmovement > -68)
		{

			Plate.Draw(MatrixID, ModelMatrixID, ViewMatrixID, vec3(34 + pmovement, plateY, -13.5), vec3(0.5, 0.5, 1), eulerAngleXYZ(0.0f, 3.14f / 2.0f, 0.0f));
			pmovement -= PlateSpeed;

			p = 34 + pmovement;  //plate 1
			Lp = p + w;   //leftplat
			Rp = p - w;    //right plate
			Tp = -13.5 + l;    //upper plate
			Bp = -13.5 - l;    //bottom plate

			Lball = Ballx + r;
			Tball = Ballz + r;
			Rball = Ballx - r;
			Bball = Ballz - r;
			if (Tball<-9.5 && Bball>-18) {
				if (Lball > Rp &&  Rball < Lp && Tball<Tp && Bball>Bp) {
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
	/*	if (time > PlateGap)
		{
			if (pmovement2 > -68)
			{
				Plate.Draw(MatrixID, ModelMatrixID, ViewMatrixID, vec3(34 + pmovement2, plateY, -14), vec3(0.5, 0.5, 0.5), eulerAngleXYZ(0.0f, 3.14f / 2.0f, 0.0f));
				pmovement2 -= PlateSpeed;

				p = 34 + pmovement2;  //plate 1
				Lp = p + w;   //leftplat
				Rp = p - w;    //right plate
				Tp = -14 + l;    //upper plate
				Bp = -14 - l;    //bottom plate

				Lball = Ballx + r;
				Tball = Ballz + r;
				Rball = Ballx - r;
				Bball = Ballz - r;
				if (Tball<-9.5 && Bball>-18) 
				{
					if (Tball<-9.5 && Bball>-18 && Lball > Rp &&  Rball < Lp && Tball<Tp && Bball>Bp)
					{
						//on plate1
						Ballx -= PlateSpeed;
					}
					else 
					{
						if(time<=PlateGap)
						glfwTerminate();
					}
				}
				time += 0.01;
			}
			else {
				pmovement2 = 0;
			}
		}*/
		//------------------2

/*		if (pmovementB > -68)
		{

			Plate.Draw(MatrixID, ModelMatrixID, ViewMatrixID, vec3(34 + pmovementB, plateY, 12), vec3(0.5, 0.5, 1), eulerAngleXYZ(0.0f, 3.14f / 2.0f, 0.0f));
			pmovementB -= 0.05+PlateSpeed;
			p5 = 34 + pmovementB;  //plate 1
			Lp5 = p5 + w;   //leftplat
			Rp5 = p5 - w;    //right plate
			Tp5 = 12 + l;    //upper plate
			Bp5 = 12 - l;    //bottom plate

			Lball = Ballx + r;
			Tball = Ballz + r;
			Rball = Ballx - r;
			Bball = Ballz - r;
			time += 0.01;
			if (Tball <17 && Bball >= 9.9) {
				if (Lball > Rp5 &&  Rball<Lp5 /*&& Tball>Tp5 && Bball < Bp5) {
					Ballx -= PlateSpeed;
				}
				else {
						glfwTerminate();
				}
			}
		}
		else pmovementB = 0;*/
	/*	if (time > PlateGap)
		{
			if (pmovement2A > -68)
			{
				Plate.Draw(MatrixID, ModelMatrixID, ViewMatrixID, vec3(34 + pmovement2A, plateY, 4), vec3(0.5, 0.5, 0.5), eulerAngleXYZ(0.0f, 3.14f / 2.0f, 0.0f));
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

				if (Tball < 10.2 && Bball>0) {
					if (Lball > Rp4 && Rball < Lp4 && Tball<Tp4 && Bball>Bp4) {
						//on plate2
						Ballx -= PlateSpeed;
					}
					else {
						if (time <= PlateGap)
							glfwTerminate();
					}
				}
			}
			else pmovement2A = 0;
		}*/

		//-----------------------3

		if (pmovementA > -68)
		{

			Plate.Draw(MatrixID, ModelMatrixID, ViewMatrixID, vec3(34 + pmovementA, plateY, 8.5), vec3(0.5, 0.5, 2), eulerAngleXYZ(0.0f, 3.14f / 2.0f, 0.0f));
			pmovementA -= PlateSpeed1;

			p3 = 34 + pmovementA;  //plate 1
			Lp3 = p3 + w;   //leftplat
			Rp3 = p3 - w;    //right plate
			Tp3 = 8.5 + l*4;    //upper plate
			Bp3 = 4 - l*4;    //bottom plate

			Lball = Ballx + r;
			Tball = Ballz + r;
			Rball = Ballx - r;
			Bball = Ballz - r;
			time += 0.01;
			if (Tball<17 && Bball>0) {
				if (Lball > Rp3 &&  Rball < Lp3 && Tball<Tp3 && Bball>Bp3) {
					//on plate1
					Ballx -= PlateSpeed1;
				}
				else {
						glfwTerminate();
				}
			}
		}
		else pmovementA = 0;
	/*	if (time > PlateGap)
		{
			if (pmovement2B > -68)
			{
				Plate.Draw(MatrixID, ModelMatrixID, ViewMatrixID, vec3(34 + pmovement2B, plateY, 12), vec3(0.5, 0.5, 0.5), eulerAngleXYZ(0.0f, 3.14f / 2.0f, 0.0f));
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
				if (Tball < 17 && Bball >= 10.2) {
					if (Lball > Rp6 &&  Rball<Lp6 && Tball>Tp6 && Bball < Bp6) {
						Ballx -= PlateSpeed;
					}
					else {
						if (time <= PlateGap)
							glfwTerminate();
					}
				}
			}
			else pmovement2B = 0;
		}*/

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
