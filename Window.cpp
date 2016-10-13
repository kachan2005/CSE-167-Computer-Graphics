#include "Window.h"

const char* window_title = "GLFW Starter Project";
Cube cube(5.0f);
OBJObject* bear = new OBJObject("C:\\Users\\Ka\\Desktop\\CSE167StarterCode-master\\bear.obj");
OBJObject* bunny = new OBJObject("C:\\Users\\Ka\\Desktop\\CSE167StarterCode-master\\bunny.obj");
OBJObject* dragon = new OBJObject("C:\\Users\\Ka\\Desktop\\CSE167StarterCode-master\\dragon.obj");
OBJObject* current = bear;

int Window::width;
int Window::height;
float* Window::pixels;
float* Window::z_buffer;
bool Window::openGL_mode;
bool Window::extra_credit1 = false;
bool Window::debug = false;

void Window::initialize_objects()
{
}

void Window::clean_up()
{
	delete[] pixels;
	delete[] z_buffer;
}

GLFWwindow* Window::create_window(int width, int height, bool openGL_mode)
{
	// Initialize GLFW.
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return NULL;
	}

	// 4x antialiasing
	glfwWindowHint(GLFW_SAMPLES, 4);

	// Create the GLFW window
	GLFWwindow* window = glfwCreateWindow(width, height, window_title, NULL, NULL); 
	Window::pixels = new float[width * height * 3];
	Window::z_buffer = new float[width * height];
	std::fill_n(z_buffer, width * height, FLT_MAX);
	Window::openGL_mode = true;

	// Check if the window could not be created
	if (!window)
	{
		fprintf(stderr, "Failed to open GLFW window.\n");
		glfwTerminate();
		return NULL;
	}

	// Make the context of the window
	glfwMakeContextCurrent(window);

	// Set swap interval to 1
	glfwSwapInterval(1);

	// Call the resize callback to make sure things get drawn immediately
	//Window::openGL_mode = openGL_mode;
	Window::resize_callback(window, width, height);

	return window;
}

void Window::resize_callback(GLFWwindow* window, int width, int height)
{

	Window::width = width;
	Window::height = height;
	// Set the viewport size
	glViewport(0, 0, width, height);
	// Set the matrix mode to GL_PROJECTION to determine the proper camera properties
	glMatrixMode(GL_PROJECTION);
	// Load the identity matrix
	glLoadIdentity();
	// Set the perspective of the projection viewing frustum
	gluPerspective(60.0, double(width) / (double)height, 1.0, 1000.0);
	// Move camera back 20 units so that it looks at the origin (or else it's in the origin)
	glTranslatef(0, 0, -20);

	//resize for resterizer
	delete[] pixels;
	delete[] z_buffer;
	Window::pixels = new float[width * height * 3];
	Window::z_buffer = new float[width * height];
	std::fill_n(z_buffer, width * height, FLT_MAX);
}

void Window::idle_callback()
{
	// Perform any updates as necessary. Here, we will spin the cube slightly.
	//cube.update();
	current->update();
}

void Window::display_callback(GLFWwindow* window)
{
	if (Window::openGL_mode) {
		// Clear the color and depth buffers
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// Set the matrix mode to GL_MODELVIEW
		glMatrixMode(GL_MODELVIEW);
		// Load the identity matrix
		glLoadIdentity();

		// Render objects
		//cube.draw();
		//bear->draw();
		//bunny->draw();
		//dragon->draw();
		current->draw();


		// Gets events, including input such as keyboard and mouse or window resizing
		glfwPollEvents();
		// Swap buffers
		glfwSwapBuffers(window);
	}
	else {
		clearBuffer();
		rasterize();

		// glDrawPixels writes a block of pixels to the framebuffer
		glDrawPixels(Window::width, Window::height, GL_RGB, GL_FLOAT, pixels);

		// Gets events, including input such as keyboard and mouse or window resizing
		glfwPollEvents();
		// Swap buffers
		glfwSwapBuffers(window);
	}
	
}

void Window::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// Check for a key press
	if (action == GLFW_PRESS || action == GLFW_REPEAT){
		float distance = 3.0f, deg = 10.0f;
		bool state1 = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS;
		bool state2 = glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS;

		switch (key){
			case GLFW_KEY_ESCAPE: // Close the window. This causes the program to also terminate.
				glfwSetWindowShouldClose(window, GL_TRUE);
				break;

			case GLFW_KEY_P:
				if (state1 || state2 ) {
					std::cout << "push P" << std::endl;
					current->point_size += 1;
				}
				else {
					if (current->point_size > 1) {
						current->point_size -= 1;
					}
					std::cout << "push p " << std::endl;
				}
				glPointSize(current->point_size);
				break;

			case GLFW_KEY_F1:
				current = bunny;
				current->reset();
				std::cout << "push f1" << std::endl;
				break;

			case GLFW_KEY_F2:
				current = bear;
				current->reset();
				std::cout << "push f2" << std::endl;
				break;

			case GLFW_KEY_F3:
				current = dragon;
				current->reset();
				std::cout << "push f3" << std::endl;
				break;

			case GLFW_KEY_X:
				if (state1 || state2) {
					std::cout << "push X" << std::endl;
				}
				else {
					distance *= -1;
					std::cout << "push x" << std::endl;
				}
				current->transform = glm::translate(glm::mat4(1.0f), glm::vec3(distance, 0.0f, 0.0f)) * current->transform;
				break;


			case GLFW_KEY_Y:
				if (state1 || state2) {
					std::cout << "push Y" << std::endl;
				}
				else {
					distance *= -1;
					std::cout << "push y" << std::endl;
				}
				current->transform = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, distance, 0.0f)) * current->transform;
				break;


			case GLFW_KEY_Z:
				if (state1 || state2) {
					std::cout << "push Z" << std::endl;
				}
				else {
					distance *= -1;
					std::cout << "push z" << std::endl;
				}
				current->transform = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, distance)) * current->transform;
				break;

			case GLFW_KEY_S:
				if (state1 || state2) {
					std::cout << "push S" << std::endl;
					current->size += 0.5f;
				}
				else {
					if (current->size > 0.5f) {
						current->size -= 0.5f;
					}
					std::cout << "push s " << std::endl;
				}
				break;

			case GLFW_KEY_O:
				if (state1 || state2) {
					std::cout << "push O" << std::endl;
					current->orbit_angle -= 18.0f;
					deg *= -1;
				}
				else {
					std::cout << "push o " << std::endl;
					current->orbit_angle += 18.0f;
				}
				
				if (current->orbit_angle > 360.0f || current->orbit_angle < -360.0f) {
					current->orbit_angle = 0.0f;
				}
				current->transform = glm::rotate(glm::mat4(1.0f), deg / 180.0f * glm::pi<float>(), glm::vec3(0.0f, 0.0f, 1.0f))  * current->transform;

				break;

			case GLFW_KEY_R:
				current->reset();
				break;

			case GLFW_KEY_M:
				Window::openGL_mode = !Window::openGL_mode;
				break;

			case GLFW_KEY_E:
				Window::extra_credit1 = !Window::extra_credit1;
				break;

			case GLFW_KEY_D:
				Window::debug = !Window::debug;
				break;
		}
	}
}


// Draw a point into the frame buffer
void Window::drawPoint(int x, int y, float z, float r, float g, float b)
{
	int offset = y* Window::width * 3 + x * 3;
	//std::cout << offset << std::endl;
	if (x >= 0 && x < Window::width && y >= 0 && y < Window::height) {
		if (z < z_buffer[offset / 3]) {
			pixels[offset] = r;
			pixels[offset + 1] = g;
			pixels[offset + 2] = b;
			z_buffer[offset / 3] = z;
		}
	}
}

void Window::rasterize(){
	// Put your main rasterization loop here
	// It should go over the point model and call drawPoint for every point in it
	int width = Window::width;
	int height = Window::height;

	glm::mat4 wld2cam = glm::lookAt(glm::vec3(0, 0, 20),
		glm::vec3(0, 0, 0),
		glm::vec3(0, 1, 0));
	glm::mat4 cam2proj = glm::perspective(glm::radians(60.0f), float(width) / (float)height, 1.0f, 1000.0f);
	glm::mat4 proj2dis(1.0f);
	proj2dis[0] = glm::vec4(width / 2.0f, 0.0f, 0.0f, 0.0f);
	proj2dis[1] = glm::vec4(0.0f, height / 2.0f, 0.0f, 0.0f);
	proj2dis[2] = glm::vec4(0.0f, 0.0f, 0.5f, 0.0f);
	proj2dis[3] = glm::vec4(width / 2.0f, height / 2.0f, 0.5f, 1.0f);

	glm::mat4 rasterizer = proj2dis * cam2proj * wld2cam * current->toWorld;
	glm::vec4 point;
	int half_size = current->point_size / 2;
	double total = 0.0;
	int count = 0;

	for (unsigned int i = 0; i < current->vertices.size(); ++i) {
		point = rasterizer * glm::vec4(current->vertices[i], 1.0f);
		int x = (int)(point.x / point.w);
		int y = (int)(point.y / point.w);
		float z = point.z;
		float depth_size = ceil( (current->point_size * 30.0f) / (z * 2.0f)  ) ;
		//float depth_size = half_size;
		total += z;
		
		for (int j = -1 * depth_size; j < depth_size + (current->point_size % 2); j++) {
			for (int k = -1 * depth_size; k < depth_size + (current->point_size % 2); k++) {
				if (depth_size > current->point_size && Window::extra_credit1) {
					drawPoint(x + j, y + k, z, 1.0f, 1.0f, 1.0f);
				}
				else {
					drawPoint(x + j, y + k, z, current->normals[i].x, current->normals[i].y, current->normals[i].z);

				}
			}
		}

		if (depth_size > current->point_size) {
			count++;
			//std::cout << i << " : " << depth_size << " : " << current->point_size << std::endl;
		}
		//std::cout << i << " : " << x << ", " << y << ", " << point.z << ", " << point.w << std::endl;

	}
	if (Window::debug) {
		std::cout << "average z-buffer = " << total / current->vertices.size() << ", point increase " << count / (float)(current->vertices.size()) << std::endl;
	}
}

void Window::clearBuffer(){
	for (int i = 0; i<Window::width*Window::height; ++i){
		pixels[i * 3] = 0.0f;
		pixels[i * 3 + 1] = 0.0f;
		pixels[i * 3 + 2] = 0.0f;
		z_buffer[i] = FLT_MAX;
	}
}