#ifndef DEBUG_H
#define DEBUG_H

#if defined _DEBUG
#define KS_ASSERT(expr)	if ( !(expr) )	__debugbreak();
#else
#define KS_ASSERT
#define KS_ASSERT(expr)
#endif	// _DEBUG

#endif