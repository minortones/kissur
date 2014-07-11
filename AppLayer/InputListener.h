#ifndef INPUTLISTENER_H
#define INPUTLISTENER_H

/************************************************************************/
//	Static class.
//	Obtain relevant input via GLUT callbacks.
//	Potentially insufficient for various other input devices.
/************************************************************************/


#include "defines.h"


// input key defines. @todo move to defines.h? 
#define		KEYPRESS_NONE	0
#define		KEYPRESS_UP		(1<<1)
#define		KEYPRESS_DOWN	(1<<2)
#define		KEYPRESS_LEFT	(1<<3)
#define		KEYPRESS_RIGHT	(1<<4)



class InputListener
{

private:

	InputListener();

	~InputListener();

	static	kissU32			mKeyDown;

public:

	static void Poll();

	static void		KeyDownCallback(unsigned char c, int x, int y);

	static void		KeyUpCallback(unsigned char c, int x, int y);

	static kissU32	getKeyDown()	{ return mKeyDown; }

	static void		clearCachedInput()	{ mKeyDown = 0; }


};

#endif