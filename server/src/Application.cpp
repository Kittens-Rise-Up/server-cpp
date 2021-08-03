#include "vkpch.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <chrono>
#include <date/date.h>
#include <enet/enet.h>

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

void HandleCommand(char* cmd) 
{
	std::cout << cmd << std::endl;
	std::string str(cmd);

	using namespace std::chrono;
	auto now = time_point_cast<milliseconds>(system_clock::now());
	std::string time = date::format("%H:%M:%OS", now);

	std::string msg = "[" + time + "] [INFO] " + "Unknown Command: '" + str + "'";

	AddMessage(msg);
}

int main() 
{
	// GLFW
	if (!glfwInit()) 
	{
		std::cerr << "Failed to initialize GLFW" << std::endl;
		return EXIT_FAILURE;
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
		return EXIT_FAILURE;
	}
		

	glfwMakeContextCurrent(window);

	// Initialize the OpenGL loading library GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) 
	{
		std::cerr << "Failed to initialize GLAD" << std::endl;
		return EXIT_FAILURE;
	}

	// Enable VSync (1 = enabled, 0 = disabled)
	glfwSwapInterval(1);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	auto io = ImGui::GetIO();
	io.Fonts->AddFontFromFileTTF("res/RobotoMono-Regular.ttf", 18);
	io.ConfigWindowsMoveFromTitleBarOnly = true;

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
	//windowImGuiFlags |= ImGuiWindowFlags_NoScrollWithMouse;
	windowImGuiFlags |= ImGuiWindowFlags_NoFocusOnAppearing;
		
	char log[128] = "My name is bob";
	auto inputReclaimFocus = true;

	// ENet
	if (enet_initialize() != 0)
	{
		std::cerr << "Failed to initialize ENet" << std::endl;
		return EXIT_FAILURE;
	}
	

	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		
		ImGui::PushStyleVar(ImGuiStyleVar_ScrollbarSize, 12);
		ImGui::PushStyleVar(ImGuiStyleVar_ScrollbarRounding, 15);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
		//ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.8f, 0.8f, 0.8f, 1.0f));

		// LOGGER
		ImGui::SetNextWindowPos(ImVec2(0, 0));
		ImGui::SetNextWindowSize(ImVec2(windowWidth, windowHeight - inputHeight - 25));
		if (ImGui::Begin("Logger", &loggerOpen, windowImGuiFlags)) 
		{
			for (int i = 0; i < m_Messages.size(); i++)
				ImGui::TextWrapped(m_Messages[i].c_str());

			if (m_ScrollToBottom) 
			{
				ImGui::SetScrollHereY(1.0f); // Scroll to bottom
				m_ScrollToBottom = false;
			}

			ImGui::End();
		}

		ImGui::SetNextWindowPos(ImVec2(0, windowHeight - inputHeight - 25));
		ImGui::SetNextWindowSize(ImVec2(windowWidth, inputHeight));
		if (ImGui::Begin("Input", &loggerOpen, windowImGuiFlags)) 
		{
			ImGui::PushItemWidth(windowWidth);
			if (ImGui::InputText(".", input, IM_ARRAYSIZE(input), ImGuiInputTextFlags_EnterReturnsTrue))
			{
				auto s = input;
				HandleCommand(s);

				strcpy(s, ""); // Clear input
				inputReclaimFocus = true;
				
			}
			ImGui::PopItemWidth();

			if (inputReclaimFocus)
			{
				ImGui::SetKeyboardFocusHere(-1);
				inputReclaimFocus = false;
			}

			ImGui::End();
		}

		ImGui::PopStyleColor(2);
		ImGui::PopStyleVar(3);

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

	atexit(enet_deinitialize);
}