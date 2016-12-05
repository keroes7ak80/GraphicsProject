#pragma once
#include"Model.h"
class DrawnModel :public Model
{
public:
	DrawnModel() {}

	DrawnModel(GLuint MatrixID) //constructor of A Drawn model
	{
			mat4 ModelMatrix = mat4(1.0f);
			mat4 MVP = getProjectionMatrix() * getViewMatrix() * ModelMatrix;

			// Send our transformation to the currently bound shader, 
			// in the "MVP" uniform
			glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
			glDrawArrays(GL_TRIANGLES, 0, vertices.size());
	}
	DrawnModel(GLuint MatrixID, vec3 Translate = vec3(0, 0, 0), vec3 Scale = vec3(1, 1, 1), mat4 Rotate = eulerAngleYXZ(0.0f, 0.0f, 0.0f))
	{
		mat4 TranslationMatrix = translate(mat4(), Translate);
		mat4 ScalingMatrix = scale(mat4(), Scale);
		mat4 ModelMatrix = TranslationMatrix*ScalingMatrix*Rotate;
		mat4 MVP = getProjectionMatrix() * getViewMatrix() * ModelMatrix;

		// Send our transformation to the currently bound shader, 
		// in the "MVP" uniform
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
		glDrawArrays(GL_TRIANGLES, 0, vertices.size());
	}

};