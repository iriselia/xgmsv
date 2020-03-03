

#include <functional>
#include <future>
#include <chrono>
#include <thread>
#include <csignal>

#include "imgui.h"
#include "imgui_impl_glfw_gl3.h"
#include "GL/gl3w.h"
#include "glfw/glfw3.h"

#include "common.h"

#include "core.h"

#include "network.h"




namespace server
{
	volatile int signal_status;


	namespace core
	{
		asio::io_context io_context;



		// IO loop
		void main_loop()
		{
			this_thread::assign_main_thread();

			try
			{

				while (network_services_online)
				{
					Sleep(100);
				}
				//io_context.run();

			}
			catch (std::exception & e)
			{
				printf("exception: %s", e.what());
				//std::cerr << "Exception: " << e.what() << "\n";
			}

			printf("server main_loop complete.\n");
		}


		// Signals
		std::function<void(int)> signal_handler = [](int signal) {};
		void signal_handler_wrapper(int signal) { signal_handler(signal); }
		void init_signals(std::function<void(int)> custom_signal_handler)
		{
			signal_handler = custom_signal_handler;

			std::signal(SIGINT, signal_handler_wrapper);
			std::signal(SIGTERM, signal_handler_wrapper);
		}

		void init_io_context(int thread_count)
		{
		}


		// GUI
		GLFWwindow* glfw_window;
		bool show_test_window = false;
		bool show_another_window = false;
		ImVec4 clear_color = ImColor(114, 144, 154);

		void init_gui()
		{
			// Setup window
			glfwSetErrorCallback([](int error, const char* description) { fprintf(stderr, "Error %d: %s\n", error, description); });
			if (!glfwInit())
			{
				assert(false, "imgui init failed.\n");
			}

			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#if __APPLE__
			glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
			glfw_window = glfwCreateWindow(1280, 720, "ImGui OpenGL3 example", NULL, NULL);
			glfwMakeContextCurrent(glfw_window);
			glfwSwapInterval(1); // Enable vsync
			gl3wInit();

			// Setup ImGui binding
			ImGui_ImplGlfwGL3_Init(glfw_window, true);
		}

		void render_gui()
		{
			// gui frame
			if (!glfwWindowShouldClose(glfw_window))
			{
				glfwPollEvents();
				ImGui_ImplGlfwGL3_NewFrame();

				// 1. Show a simple window
				// Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets appears in a window automatically called "Debug"
				{
					static float f = 0.0f;
					ImGui::Text("Hello, world!");
					ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
					ImGui::ColorEdit3("clear color", (float*)&clear_color);
					if (ImGui::Button("Test Window")) show_test_window ^= 1;
					if (ImGui::Button("Another Window")) show_another_window ^= 1;
					ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
				}

				// 2. Show another simple window, this time using an explicit Begin/End pair
				if (show_another_window)
				{
					ImGui::Begin("Another Window", &show_another_window);
					ImGui::Text("Hello from another window!");
					ImGui::End();
				}

				// 3. Show the ImGui test window. Most of the sample code is in ImGui::ShowTestWindow()
				if (show_test_window)
				{
					ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiCond_FirstUseEver);
					ImGui::ShowTestWindow(&show_test_window);
				}

				// Rendering
				int display_w, display_h;
				glfwGetFramebufferSize(glfw_window, &display_w, &display_h);
				glViewport(0, 0, display_w, display_h);
				glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
				glClear(GL_COLOR_BUFFER_BIT);
				ImGui::Render();
				glfwSwapBuffers(glfw_window);
			}
		}

		void stop_gui()
		{
			ImGui_ImplGlfwGL3_Shutdown();
			glfwTerminate();
		}
	}
}
