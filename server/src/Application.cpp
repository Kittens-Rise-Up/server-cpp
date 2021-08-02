#include "vkpch.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

const GLubyte GLFW_VER_MAJOR = 4;
const GLubyte GLFW_VER_MINOR = 6;

std::vector<std::string> m_Messages;
bool m_ScrollToBottom;
char input[128] = "";

void GLFWErrorCallback(int error, const char* description)
{
	std::cerr << description << std::endl;
}

void AddMessage(std::string message) 
{
	m_Messages.push_back(message);
	m_ScrollToBottom = true;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ENTER && action == GLFW_PRESS) 
	{
		std::cout << "Enter" << std::endl;
		memset(input, 0, 128);
	}
		
}

int main() 
{
	if (!glfwInit()) 
	{
		std::cerr << "Failed to initialize GLFW" << std::endl;
		return 1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, GLFW_VER_MAJOR);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, GLFW_VER_MINOR);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
	glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);
	glfwWindowHint(GLFW_FOCUSED, GLFW_TRUE); // Ignored for fullscreen
	glfwWindowHint(GLFW_CENTER_CURSOR, GLFW_TRUE); // Ignored for windowed
	glfwWindowHint(GLFW_FLOATING, GLFW_FALSE);
	glfwWindowHint(GLFW_SAMPLES, 4);

	glfwSetErrorCallback(GLFWErrorCallback);

	auto window = glfwCreateWindow(640, 480, "Server", NULL, NULL);
	if (!window) 
	{
		std::cerr << "Failed to create GLFW window" << std::endl;
		return 1;
	}
		

	glfwMakeContextCurrent(window);

	// Initialize the OpenGL loading library GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		std::cerr << "Failed to initialize GLAD" << std::endl;

	// Enable VSync (1 = enabled, 0 = disabled)
	glfwSwapInterval(1);

	glfwSetKeyCallback(window, key_callback);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	auto io = ImGui::GetIO();
	io.Fonts->AddFontFromFileTTF("res/RobotoMono-Regular.ttf", 18);

	ImGui::StyleColorsDark();

	auto style = &ImGui::GetStyle();
	style->Colors[ImGuiCol_FrameBg] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 460");

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	auto setupLogger = true;
	auto loggerOpen = true;

	auto myFloat = 1.0f;

	float windowWidth = 640;
	float windowHeight = 480;
	float inputHeight = 25;

	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(ImVec2(windowWidth, windowHeight));

	int index = 0;

	int windowImGuiFlags = 0;
	windowImGuiFlags |= ImGuiWindowFlags_NoTitleBar;
	windowImGuiFlags |= ImGuiWindowFlags_NoMove;
	windowImGuiFlags |= ImGuiWindowFlags_NoResize;
	windowImGuiFlags |= ImGuiWindowFlags_NoScrollbar;
	windowImGuiFlags |= ImGuiWindowFlags_NoScrollWithMouse;
	windowImGuiFlags |= ImGuiWindowFlags_NoFocusOnAppearing;

	for (int i = 0; i < 50; i++) 
	{
		std::string message = "Hello world! " + std::to_string(++index);
		AddMessage(message);
	}
		
	char log[128] = "My name is bob";

	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		
		ImGui::PushStyleVar(ImGuiStyleVar_ScrollbarSize, 12);
		ImGui::PushStyleVar(ImGuiStyleVar_ScrollbarRounding, 15);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));

		// LOGGER
		ImGui::SetNextWindowPos(ImVec2(0, 0));
		ImGui::SetNextWindowSize(ImVec2(windowWidth, windowHeight));
		ImGui::Begin("Logger", &loggerOpen, windowImGuiFlags);

		auto width = ImGui::CalcTextSize(input).x;
		if (width > 200)
			log[75] = '\n';

		ImGui::InputTextMultiline("", log, IM_ARRAYSIZE(log), ImVec2(windowWidth, windowHeight - 30), ImGuiInputTextFlags_NoHorizontalScroll | ImGuiInputTextFlags_ReadOnly);

		ImGui::PushItemWidth(windowWidth);
		
		ImGui::InputText(".", input, IM_ARRAYSIZE(input));
		//ImGui::SetKeyboardFocusHere(0);

		ImGui::PopItemWidth();
		ImGui::End();

		ImGui::PopStyleColor();
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));

		// INPUT

		


		ImGui::PopStyleColor();
		ImGui::PopStyleVar(4);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();
}