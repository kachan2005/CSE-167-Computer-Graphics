#include "OBJObject.h"

OBJObject::OBJObject(const char *filepath) 
{
	this->reset();
	parse(filepath);
}

void OBJObject::parse(const char *filepath)
{
	//TODO parse the OBJ file
	// Populate the face indices, vertices, and normals vectors with the OBJ Object data

	// file pointer
	FILE* fp;
	float x, y, z;  // vertex coordinates
	int c1, c2, count = 0;    // characters read from file
	char line[500];

	fp = fopen(filepath, "rb");  // make the file name configurable so you can load other files
	if (fp == NULL) { std::cerr << "error loading file" << std::endl; exit(-1); }  // just in case the file can't be found or is corrupt

	while ((c1 = fgetc(fp)) != EOF) {
		c2 = fgetc(fp);
		if ((c1 == 'v') && (c2 == ' ')) {
			fscanf(fp, "%f %f %f", &x, &y, &z);
			//std::cout <<count << ": v " << x << ", " << y << ", " << z << std::endl;
			vertices.push_back(glm::vec3(x, y, z));
		}
		else if ((c1 == 'v') && (c2 == 'n')) {
			fscanf(fp, " %f %f %f", &x, &y, &z);
			//std::cout << count << ": vn " << x << ", " << y << ", " << z << std::endl;
			glm::vec3 temp2 = glm::normalize(glm::vec3(x, y, z));
			x = convert_color(temp2.x);
			y = convert_color(temp2.y);
			z = convert_color(temp2.z);
			normals.push_back(glm::vec3(x, y, z));
		}
		else {
			//std::cout << count << ": " << (char)c1 << (char)c2 << std::endl;
		}
		count++;
		fgets(line, 500, fp); 
		//std::cout << line << std::endl;
	}
	std::cout << "vertices size: " << vertices.size() << std::endl;
	std::cout << "normals size: " << normals.size() << std::endl;

	fclose(fp);   // make sure you don't forget to close the file when done
}

void OBJObject::draw() 
{
	glMatrixMode(GL_MODELVIEW);

	// Push a save state onto the matrix stack, and multiply in the toWorld matrix
	glPushMatrix();
	glMultMatrixf(&(toWorld[0][0]));

	//glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(size, size, size));

	glBegin(GL_POINTS);
	// Loop through all the vertices of this OBJ Object and render them
	for (unsigned int i = 0; i < vertices.size(); ++i) 
	{
		glColor3f(normals[i].x, normals[i].y, normals[i].z);
		glNormal3f(normals[i].x, normals[i].y, normals[i].z);
		glVertex3f(vertices[i].x, vertices[i].y, vertices[i].z);
	}
	glEnd();

	// Pop the save state off the matrix stack
	// This will undo the multiply we did earlier
	glPopMatrix();
}


void OBJObject::update()
{
	glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(size, size, size));
	glm::mat4 spin = this->spin(0.005f * vertices.size() / 1000);
	//glm::mat4 distance1 = glm::translate(glm::mat4(1.0f), glm::vec3(this->deltaX1, this->deltaY1, this->deltaZ1));
	//glm::mat4 rot = glm::rotate(glm::mat4(1.0f), this->orbit_angle / 180.0f * glm::pi<float>(), glm::vec3(0.0f, 0.0f, 1.0f));
	
	//this->toWorld = rot * distance1 * spin * scale;
	this->toWorld = transform * spin * scale;
}

glm::mat4 OBJObject::spin(float deg)
{
	this->spin_angle += deg;
	if (this->spin_angle > 360.0f || this->spin_angle < -360.0f) this->spin_angle = 0.0f;
	// This creates the matrix to rotate the object
	return glm::rotate(glm::mat4(1.0f), this->spin_angle / 180.0f * glm::pi<float>(), glm::vec3(0.0f, 1.0f, 0.0f));
}


void OBJObject::reset() {
	this->toWorld = glm::mat4(1.0f);
	this->transform = glm::mat4(1.0f);
	this->point_size = 1; 
	this->spin_angle = 0.0f;
	this->size = 1.0f;
	glPointSize(this->point_size);
}

float OBJObject::convert_color(float num) {
	return (num + 1.0f) / 2.0f;
}