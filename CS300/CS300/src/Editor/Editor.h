#pragma once
#include <SDL2/SDL.h>

class Editor
{
public:
	Editor(SDL_Window* window = nullptr, SDL_GLContext context = nullptr);
	~Editor();
	void Initialize();
	void Render();
	void StartFrame();
	void ShutDown();
	void HandleEvent(SDL_Event* event);

private:

	SDL_Window* mWindow;
	SDL_GLContext mContext;
};