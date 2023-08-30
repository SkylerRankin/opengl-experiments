#include <glad/glad.h>
#include <iostream>
#include <map>
#include <GLFW/glfw3.h>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "Scene.h"

class Driver {
private:
	enum class SceneType {
		DVD,
		RectPacking
	};

	struct SceneInfo {
		SceneInfo(std::string name, std::unique_ptr<Scene> scene) : name(name), scene(std::move(scene)) {}
		SceneInfo(SceneInfo& other) : name(other.name), scene(std::move(other.scene)) {}
		std::string name;
		std::unique_ptr<Scene> scene;
	};

	std::map<SceneType, SceneInfo> scenes = {};
	GLFWwindow* window = nullptr;
	ImGuiIO* imGuiIO = nullptr;
	SceneType currentScene = SceneType::DVD;

public:

	Driver() {
		SceneInfo dvdSceneInfo("dvd scene", std::move(std::make_unique<DvdScene>()));
		scenes.emplace(SceneType::DVD, dvdSceneInfo);

		SceneInfo rectPackingInfo("rectangle packing", std::move(std::make_unique<RectPacking>()));
		scenes.emplace(SceneType::RectPacking, rectPackingInfo);
	}

	void run() {
		initGLFW();
		initImGui();
		startRenderLoop();
		endRenderLoop();
	}

private:
	
	void initGLFW() {
		int openGLMajorVersion = 3;
		int openGLMinorVersion = 3;
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, openGLMajorVersion);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, openGLMinorVersion);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		window = glfwCreateWindow(800, 600, "opengl-experiments", nullptr, nullptr);
		if (!window) {
			std::cout << "Failed to create GLFW window." << std::endl;
			glfwTerminate();
			std::exit(-1);
		}

		glfwMakeContextCurrent(window);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			std::cout << "Failed to initialize GLAD." << std::endl;
			glfwTerminate();
			std::exit(-1);
		}

		glfwSetFramebufferSizeCallback(window, &frameBufferSizeCallback);
	}
	
	void initImGui() {
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

		// Setup Platform/Renderer backends
		ImGui_ImplGlfw_InitForOpenGL(window, true);          // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
		ImGui_ImplOpenGL3_Init();

		imGuiIO = &ImGui::GetIO();
	}
	
	void startRenderLoop() {
		scenes.at(currentScene).scene->setup(window);

		while (!glfwWindowShouldClose(window)) {
			handleInput();
			glfwPollEvents();

			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			ImGui::Begin("opengl-experiments");

			for (auto& pair : scenes) {
				ImGui::Text(pair.second.name.c_str());
				ImGui::SameLine();
				bool disabled = pair.first == currentScene;

				if (disabled) ImGui::BeginDisabled();
				ImGui::PushID(pair.second.name.c_str());
				if (ImGui::Button("Load")) {
					currentScene = pair.first;
				}
				ImGui::PopID();
				if (disabled) ImGui::EndDisabled();
			}

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / imGuiIO->Framerate, imGuiIO->Framerate);
			ImGui::End();

			scenes.at(currentScene).scene->render(window);
		
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
			glfwSwapBuffers(window);
		}
		}
	
	void endRenderLoop() {
			ImGui_ImplOpenGL3_Shutdown();
			ImGui_ImplGlfw_Shutdown();
			ImGui::DestroyContext();
			glfwTerminate();
		}
	
	void handleInput() {
	}

	static void frameBufferSizeCallback(GLFWwindow* window, int width, int height) {
		glViewport(0, 0, width, height);
	}
};

int main() {
	Driver driver;
	driver.run();
	return 0;
}