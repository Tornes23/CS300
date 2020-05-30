#include "Input.h"

#pragma region // General Input Set Up //


InputHandler::InputHandler() {}

void InputHandler::Initialize()
{
	for (unsigned i = 0; i < KEYBOARD_KEY_AMOUNT; ++i)
	{
		mKeyCurrent[i] = 0;
		mKeyPrevious[i] = 0;
	}

	mbCapsLocked = false;
}
void InputHandler::StartFrame()
{
	for (unsigned i = 0; i < KEYBOARD_KEY_AMOUNT; ++i) 
		mKeyPrevious[i] = mKeyCurrent[i];
}

#pragma endregion

#pragma region // KEYBOARD //

void InputHandler::HandleKeyEvent(SDL_Event event)
{
	SDL_Keycode ScanCode = event.key.keysym.sym;

	int masked_key = ScanCode & ~SDLK_SCANCODE_MASK;

	/* Numeric Pad */
	if (ScanCode >= SDLK_KP_1 && ScanCode <= SDLK_KP_9)
	{
		mKeyCurrent[ScanCode - SDLK_KP_1 + '1'] = event.key.state ? true : false;
		return;
	}
	else if (ScanCode == SDLK_KP_0)
	{
		mKeyCurrent['0'] = event.key.state ? true : false;
		return;
	}
	/* TAB */
	else if (ScanCode == SDLK_TAB)
	{
		mKeyCurrent[SDLK_TAB] = event.key.state ? true : false;
		return;
	}
	/* Special keys */
	else if (ScanCode == 13) // Enter
	{
		mKeyCurrent[13] = event.key.state ? true : false;
		return;
	}
	else if (ScanCode == SDLK_CAPSLOCK) // CapsLock
	{
		mKeyCurrent[2] = event.key.state ? true : false;
		return;
	}

	mKeyCurrent[masked_key] = event.key.state ? true : false;
}

bool InputHandler::KeyIsDown(Key index)
{
	return mKeyCurrent[index];
}
bool InputHandler::KeyIsUp(Key index)
{
	return !KeyIsDown(index);
}
bool InputHandler::KeyIsTriggered(Key index)
{
	if (mKeyCurrent[index] == true)
	{
		if (mKeyCurrent[index] != mKeyPrevious[index])
			return true;
	}
	return false;
}
bool InputHandler::KeyIsReleased(Key index)
{
	if (mKeyCurrent[index] == false)
	{
		if (mKeyCurrent[index] != mKeyPrevious[index])
			return true;
	}
	return false;
}


bool InputHandler::KeyByUnsignedIsTriggered(unsigned index)
{
	if (mKeyCurrent[index] == true)
	{
		if (mKeyCurrent[index] != mKeyPrevious[index])
			return true;
	}
	return false;
}

bool InputHandler::KeyByUnsignedIsPressed(unsigned index)
{
	return mKeyCurrent[index];
}

#pragma endregion

