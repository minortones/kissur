
#include "InputListener.h"
#include "GLApplication.h"


kissU32	InputListener::mKeyDown = NULL;

InputListener::InputListener()
{}


InputListener::~InputListener()
{}


void InputListener::KeyDownCallback(unsigned char c, int x, int y)
{
	static int animating = NULL;

	switch (c)
	{
	case ' ':
		animating = !animating;
		if (animating)
		{
			//glutIdleFunc(idle);
		}
		else
		{
			//glutIdleFunc(NULL);
		}    
		break;
		
	case 'W':
	case 'w':
		mKeyDown |= KEYPRESS_UP;
		break;

	case 'A':
	case 'a':
		mKeyDown |= KEYPRESS_LEFT;
		break;

	case 'S':
	case 's':
		mKeyDown |= KEYPRESS_DOWN;
		break;

	case 'D':
	case 'd':
		mKeyDown |= KEYPRESS_RIGHT;
		break;

	case 27:  // Esc key
		break;

	default:
		mKeyDown = KEYPRESS_NONE;
	}
}


void InputListener::KeyUpCallback(unsigned char c, int x, int y)
{
	switch (c)
	{
	case 'W':
	case 'w':
		mKeyDown &= ~KEYPRESS_UP;
		break;

	case 'A':
	case 'a':
		mKeyDown &= ~KEYPRESS_LEFT;
		break;

	case 'S':
	case 's':
		mKeyDown &= ~KEYPRESS_DOWN;
		break;

	case 'D':
	case 'd':
		mKeyDown &= ~KEYPRESS_RIGHT;
		break;

	case 27:  // Esc key
		GLApplication::destroy(0);
		break;
	}
	printf("keyup %c\n", c);
}


void InputListener::Poll()
{

}