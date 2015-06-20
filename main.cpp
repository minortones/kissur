
#include "AppLayer/GLApplication.h"
#include "EnumReflector.h"

#define kissEnum(name, ...) enum name { __VA_ARGS__, __COUNT};

enum FStates
{
	fsHang = -2,
	fsInvalid = -1,
	fsDead,
	fsBoot,
	fsLive
};

BEGINENUMSTRING(FStates, fsInvalid)
EXPORTENUMSTRING(fsDead)
EXPORTENUMSTRING(fsBoot)
FINISHENUMSTRING(fsLive)


//////////////////////////////////////////////////////////////////////////////////////////////////////
// MAIN ENTRY
//////////////////////////////////////////////////////////////////////////////////////////////////////
int main( int argc, char *argv[] )
{
	GLApplication* app = new GLApplication();
	Service<GLApplication>::Register( app );

	auto pstate		= fsDead;
	auto stateName	= Refl( pstate ).ToString();
	auto typeN		= Refl( pstate ).Typename();
	//typeN			= Refl( app ).Typename();
	
	app->init(argc, argv);
	app->go();

	delete app;
	return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// ENDOF MAIN
//////////////////////////////////////////////////////////////////////////////////////////////////////