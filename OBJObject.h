#ifndef OBJOBJECT_H
#define OBJOBJECT_H
#define _CRT_SECURE_NO_DEPRECATE

#include <stdio.h>
#include <iostream>

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#include <OpenGL/glext.h>
#include <OpenGL/gl.h> // Remove this line in future projects
#else
#include <GL/glew.h>
#endif

#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

class OBJObject
{
private:
std::vector<unsigned int> indices;

public:
	OBJObject(const char* filepath);

	void parse(const char* filepath);
	void draw();
	void update();
	glm::mat4 spin(float deg);
	void reset();
	float convert_color(float pos);

	glm::mat4 transform;
	glm::mat4 toWorld;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	int point_size;
	float spin_angle;
	float orbit_angle;
	float size;
	bool openGL_mode;
};

#endif