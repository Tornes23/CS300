#include "Editor.h"
#include <IMGUI/imgui.h>
#include <IMGUI/imgui_impl_sdl2.h>
#ifdef USE_OPENGL
#include <IMGUI/imgui_impl_opengl3.h>
#endif // USE_OPENGL
#ifdef USE_VULKAN
#include <IMGUI/imgui_impl_vulkan.h>
#endif // USE_VULKAN

Editor::Editor(SDL_Window* window, SDL_GLContext context)
{
	mWindow = window;
	mContext = context;

	Initialize();
}

Editor::~Editor()
{
	ShutDown();
}

void Editor::Initialize()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	// Setup Platform/Renderer bindings
	ImGui_ImplSDL2_InitForOpenGL(mWindow, mContext);
#ifdef USE_OPENGL
	ImGui_ImplOpenGL3_Init("#version 130");
#endif // USE_OPENGL
}

void Editor::Render()
{
	ImGui::Render();
#ifdef USE_OPENGL
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
#endif // USE_OPENGL

}

void Editor::StartFrame()
{
#ifdef USE_OPENGL
	ImGui_ImplOpenGL3_NewFrame();
#endif // USE_OPENGL

	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();
}

void Editor::ShutDown()
{
#ifdef USE_OPENGL
	ImGui_ImplOpenGL3_Shutdown();
#endif // USE_OPENGL

	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
}

void Editor::HandleEvent(SDL_Event* event)
{
	ImGui_ImplSDL2_ProcessEvent(event);
}
