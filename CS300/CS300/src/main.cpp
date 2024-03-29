/*!**************************************************************************
\file    main.cpp

\author  Nestor Uriarte 

\par     DP email:  nestor.uriarte@digipen.edu 

\par     Course:    CS300 

\par     assignemnt 0 

\date    Mon May 25 07:41:52 2020

\brief   This file contains the 
main execution function

The functions included are:
- int main()

***************************************************************************/
#include "CS300.h"

#undef main
int main()
 {
	Utils::InitSDL();
	InputManager.Initialize();

	Window myWindow;
	Editor myEditor(myWindow.GetSDLWindow(), myWindow.GetContext());

	GameObjectManager myManager;

	Utils::InitGL(&myWindow);
	Camera myCamera(glm::vec3(0, 0, 10), myWindow.GetViewport());

	myManager.AddLevelObject(myManager.CreateObject(glm::vec3(-20, 0, 0), glm::vec3(5, 5, 5), "SideKickLeft", Model::Shape::Sphere, true));
	myManager.AddLevelObject(myManager.CreateObject(glm::vec3(20, 0, 0), glm::vec3(5, 5, 5), "SideKickRight", Model::Shape::Sphere, true));
	
	myManager.AddObject(myManager.CreateObject(glm::vec3(0,  0,  0),  glm::vec3(20, 20, 20),  "Plane",    Model::Shape::Plane,    false));
	myManager.AddObject(myManager.CreateObject(glm::vec3(0,  0,  0),  glm::vec3(20, 20, 20),  "Cube",     Model::Shape::Cube,     false));
	myManager.AddObject(myManager.CreateObject(glm::vec3(0,  0,  0),  glm::vec3(20, 20, 20),  "Cone",     Model::Shape::Cone,     false));
	myManager.AddObject(myManager.CreateObject(glm::vec3(0,  0,  0),  glm::vec3(20, 20, 20),  "Cylinder", Model::Shape::Cylinder, false));
	myManager.AddObject(myManager.CreateObject(glm::vec3(0,  0,  0),  glm::vec3(20, 20, 20),  "Sphere",   Model::Shape::Sphere,   true));

	while (!myWindow.IsClosed())
	{
		myEditor.StartFrame();

		InputManager.StartFrame();

		SDL_Event event;

		while (SDL_PollEvent(&event))
		{
			myEditor.HandleEvent(&event);

			switch (event.type)
			{
			case SDL_QUIT:
				myWindow.SetQuit(true);
				break;
			case SDL_KEYDOWN:
			case SDL_KEYUP:
				InputManager.HandleKeyEvent(event);
			}
		}

		myWindow.Update();
		myManager.Update();
		myCamera.Update();

		myWindow.Clear();
		
		myCamera.GenerateEnviroment(myManager.mObjects, myManager.mAllObjects);
		myCamera.Render(myManager.mAllObjects);
		myEditor.Render();
		myWindow.SwapBuffers();


	}

	myWindow.DeleteContext();
	myWindow.DeleteWindow();

	SDL_Quit();

	return 0;
}

