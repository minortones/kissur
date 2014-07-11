
/*
	Nnanna Kama
	Barebones class for managing Shaders
	Supports both GLSL and CG shaders (which don't have any good 3rd-party profiling tools).
	Uses a map to store parameter handles, so it's owning class don't gotta worry about such.
	ALL MATRIX GET & SET accessors presume a row-major format.
*/

#ifndef SIMPLESHADERCONTAINER_H
#define SIMPLESHADERCONTAINER_H

#include <map>
#include <string>
#include "Structs.h"
#include "Matrix4x4.h"

#ifdef TARGET_CG_SHADERS
#include "..\shared_include\Cg\cg.h"
#include "..\shared_include\Cg\cgGL.h"

	typedef CGprofile		ShaderProfile;
	typedef CGcontext		ShaderContext;
	typedef CGparameter		ShaderParameter;
	typedef	CGprogram		ShaderProgram;
#else
	typedef kissU32			ShaderProfile;
	typedef kissU32			ShaderContext;
	typedef kiss32			ShaderParameter;
	typedef kissU32			ShaderProgram;

	typedef enum ShaderAttribute
	{
		SA_POSITION		= 0,
		SA_NORMAL
	}ShaderAttribute;
#endif


	extern const char* myProgramName;
	extern const char* gDefaultVertProgram;
	extern const char* gDefaultFragProgram;
	extern const char* gDefaultShaderFilename;



typedef std::map<std::string, ShaderParameter >	ParamShaderMap;


class SimpleShaderContainer
{

public:
	  SimpleShaderContainer(void);

	  ~SimpleShaderContainer(void);
	
	static	ShaderContext	getStaticContext(void);

	static	void			destroyContext(void);

	static	void			destroyProgram(ShaderProgram& program);

	static	void			shaderErrorCallback(void);

	
	ShaderContext		getContext()	{ return SimpleShaderContainer::getStaticContext(); }

	ShaderProgram		loadProgram(int numFiles, ...);

	ShaderProgram		loadProgram(const char* name, const char* vp_filename, const char* fp_filename);

	ShaderProgram		loadProgram(const char *filename, const char *entry, ShaderProfile profile);

	ShaderProgram		getVertProgram(void) const	{return mVertProgram;}
	
	ShaderProgram		getFragProgram(void) const	{return mFragProgram;}

	ShaderProfile		getVertProfile(void) const	{return mVertProfile;}

	ShaderProfile		getFragProfile(void) const	{return mFragProfile;}

	char*				getShaderFromFile(const char* filename, ShaderProfile target);

	
// get named vertex and fragment program parameters

	ShaderParameter		getNamedParam( const char* name );



	
	void	enableVertProfile(void);

	void	enableFragProfile(void);

	void	disableVertProfile(void);

	void	disableFragProfile(void);

	template<typename N>
	void	setFloatParameter(N param, const float value);

	template<typename N, typename V>
	void	setVectorParameter( N param, const V* value );

	template<typename N, typename V>
	void	setMatrixParameter( N param, const V* value );

	void	updateProgramParameters();

	void	getMatrixParameter(ShaderParameter, float*);

	void	bindVertProgram()	{	bindProgram(mVertProgram); }

	void	bindFragProgram()	{	bindProgram(mFragProgram);	}

	void	bindProgram()		{	bindProgram(mShaderProgram); mIsBound = true; }

	void	bindProgram(ShaderProgram& prog);

	void	unbindProgram();

	void	unbindVertProgram(void);

	void	unbindFragProgram(void);

	bool	isBound() const			{ return mIsBound; }


private:

	static   ShaderContext	_gShaderContext;

	ShaderProfile			mVertProfile;			//for vertex shader

	ShaderProfile			mFragProfile;			//for fragment shader
	
	ShaderProgram			mVertProgram;

	ShaderProgram			mFragProgram;

	ShaderProgram			mShaderProgram;

	ParamShaderMap			mShaderParamMap;

	char					mName[MAX_NAME];

	bool					mIsBound;
};


#endif