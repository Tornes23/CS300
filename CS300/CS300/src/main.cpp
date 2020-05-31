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

	Window myWindow;
	GameObjectManager myManager;
	Camera myCamera;

	Utils::InitGL(&myWindow);
	InputManager.Initialize();

	myManager.AddLevelObject(myManager.CreateObject(glm::vec3(0, -15, 0), glm::vec3(50, 50, 1), "Floor", Model::Shape::Plane, true, glm::vec3(0, 0, -1)));

	myManager.AddObject(myManager.CreateObject(glm::vec3(0,  0,  25),  glm::vec3(10, 10, 10),  "Plane",    Model::Shape::Plane, true));
	myManager.AddObject(myManager.CreateObject(glm::vec3(0,  0,  25),  glm::vec3(10, 10, 10),  "Cube",     Model::Shape::Cube));
	myManager.AddObject(myManager.CreateObject(glm::vec3(0,  0,  25),  glm::vec3(5,  10,  5),  "Cone",     Model::Shape::Cone));
	myManager.AddObject(myManager.CreateObject(glm::vec3(0,  0,  25),  glm::vec3(5,  10,  5),  "Cylinder", Model::Shape::Cylinder));
	myManager.AddObject(myManager.CreateObject(glm::vec3(0,  0,  25),  glm::vec3(10, 10, 10),  "Sphere",   Model::Shape::Sphere));

	myCamera.AddShader("./src/Shader/programs/Texture.vs"       , "./src/Shader/programs/Texture.fs"       );
	myCamera.AddShader("./src/Shader/programs/Mapping.vs"       , "./src/Shader/programs/Mapping.fs"       );
	myCamera.AddShader("./src/Shader/programs/Normals.vs"       , "./src/Shader/programs/Normals.fs"       );
	myCamera.AddShader("./src/Shader/programs/NormalsAverage.vs", "./src/Shader/programs/NormalsAverage.fs");

	while (!myWindow.IsClosed())
	{
		InputManager.StartFrame();

		SDL_Event event;

		while (SDL_PollEvent(&event))
		{
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
		
		myCamera.Render(myWindow ,myManager.mAllObjects);

	}

	myWindow.DeleteContext();
	myWindow.DeleteWindow();

	SDL_Quit();

	return 0;
}

