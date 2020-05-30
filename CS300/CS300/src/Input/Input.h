#pragma once

#include <SDL2/SDL.h>

#define KEYBOARD_KEY_AMOUNT 257

enum Key
{
	Enter		= 13,
	Plus	    = SDL_SCANCODE_KP_PLUS,
	Minus	    = SDL_SCANCODE_KP_MINUS,
	Control		= SDL_SCANCODE_LCTRL,
	LShift		= SDL_SCANCODE_LSHIFT,
	BloqMay		= 2,
	Delete		= 8,
	Supr		= 127,
	Left		= SDL_SCANCODE_LEFT,
	Right		= SDL_SCANCODE_RIGHT,
	Up			= SDL_SCANCODE_UP,
	Down		= SDL_SCANCODE_DOWN,
	Esc			= 27,
	KeySpace	= 32,
	F1			= SDL_SCANCODE_F1,
	num0 = SDLK_0,
	num1 = SDLK_1,
	num2 = SDLK_2,
	num3 = SDLK_3,
	num4 = SDLK_4,
	num5 = SDLK_5,
	num6 = SDLK_6,
	num7 = SDLK_7,
	num8 = SDLK_8,
	num9 = SDLK_9,
	F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,
	A = 'a', B, C, D, E, F, G, H, I, J, K, L, M, 
	N, O, P, Q, R, S, T, U, V, W, X, Y, Z
};

class InputHandler
{
public:
	void Initialize();
	void StartFrame();
	void HandleKeyEvent(SDL_Event event);


	bool KeyIsDown(Key index);
	bool KeyIsUp(Key index);
	bool KeyIsTriggered(Key index);
	bool KeyIsReleased(Key index);

	bool KeyByUnsignedIsTriggered(unsigned);
	bool KeyByUnsignedIsPressed(unsigned);
	bool GetCapsLocked() { return mbCapsLocked; }
	void ToogleCapsLock() { mbCapsLocked = !mbCapsLocked; }

	InputHandler(InputHandler const&) = delete;
	void operator=(InputHandler const&) = delete;

	static InputHandler& GetInstance()
	{
		static InputHandler instance;
		return instance;
	}

private:
	InputHandler();
	/* KeyboardKeys (256) */
	bool mKeyCurrent[KEYBOARD_KEY_AMOUNT];
	bool mKeyPrevious[KEYBOARD_KEY_AMOUNT];

	bool		mbCapsLocked;
};

#define InputManager (InputHandler::GetInstance())

#define KeyDown(i) InputManager.KeyIsDown(i)
#define KeyUp(i) InputManager.KeyIsUp(i)
#define KeyTriggered(i) InputManager.KeyIsTriggered(i)
#define KeyReleased(i) InputManager.KeyIsReleased(i)
