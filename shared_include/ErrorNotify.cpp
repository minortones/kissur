

#include "ErrorNotify.h"

ErrorNotify::ErrorNotify()
{
	strcpy_s(mErrString, sizeof(mErrString), "Run for your life yo!");
}


ErrorNotify::ErrorNotify(const char *error)
{
	strcpy_s(mErrString, sizeof(mErrString), error);
}


ErrorNotify::~ErrorNotify()
{}


void ErrorNotify::PrintError()
{
	printf("%s\nQuitting! \n", mErrString);
}
