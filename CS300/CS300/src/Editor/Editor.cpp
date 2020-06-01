#include "Editor.h"
#include <IMGUI/imgui.h>
#include <IMGUI/imgui_impl_sdl.h>
#include <IMGUI/imgui_impl_opengl3.h>

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
	ImGui_ImplOpenGL3_Init("#version 130");
}

void Editor::Render()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Editor::StartFrame()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(mWindow);
	ImGui::NewFrame();
}

void Editor::ShutDown()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
}

void Editor::HandleEvent(SDL_Event* event)
{
	ImGui_ImplSDL2_ProcessEvent(event);
}
