#include "vkpch.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

const GLubyte GLFW_VER_MAJOR = 4;
const GLubyte GLFW_VER_MINOR = 6;

void glfw_error_callback(int error, const char* description)
{
	std::cerr << description << std::endl;
}

int main() 
{
	if (!glfwInit())
		std::cerr << "Failed to initialize GLFW" << std::endl;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, GLFW_VER_MAJOR);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, GLFW_VER_MINOR);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
	glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);
	glfwWindowHint(GLFW_FOCUSED, GLFW_TRUE); // Ignored for fullscreen
	glfwWindowHint(GLFW_CENTER_CURSOR, GLFW_TRUE); // Ignored for windowed
	glfwWindowHint(GLFW_FLOATING, GLFW_TRUE);
	glfwWindowHint(GLFW_SAMPLES, 4);

	glfwSetErrorCallback(glfw_error_callback);

	auto window = glfwCreateWindow(640, 480, "My Title", NULL, NULL);
	if (!window)
		std::cerr << "Failed to create GLFW window" << std::endl;

	glfwMakeContextCurrent(window);

	// Initialize the OpenGL loading library GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		std::cerr << "Failed to initialize GLAD" << std::endl;

	// Enable VSync (1 = enabled, 0 = disabled)
	glfwSwapInterval(1);

	IMGUI_CHECKVERSION();
	//ImGui::CreateContext();
	//auto io = ImGui::GetIO(); (void)io;
	//ImGui::StyleColorsDark();
	//ImGui_ImplGlfw_InitForOpenGL(window, true);
	//ImGui_ImplOpenGL3_Init("#version 460");

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	glfwDestroyWindow(window);
	glfwTerminate();
}