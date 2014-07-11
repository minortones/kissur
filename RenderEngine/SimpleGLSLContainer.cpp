/*
	Nnanna Kama
	Implementation of barebones class for managing GLSL Shaders
	Handles only one instance of a vertex and fragment shader each
*/


#include "SimpleShaderContainer.h"

#ifdef TARGET_GL_SHADERS
#include "RenderResourceFactory.h"
#include <stdarg.h>
#include "Macros.h"
#include "ErrorNotify.h"
#include "GL/glew.h"

ShaderContext	SimpleShaderContainer::_gShaderContext	= NULL;

const char	*myProgramName			= "CRAP_SLAP_TRAP",
			*gDefaultShaderFilename	= "__default",
			*gDefaultVertProgram	= "..\\media\\programs\\basicLight_v.glsl",
			*gDefaultFragProgram	= "..\\media\\programs\\basicLight_f.glsl";



//=================================================================================================================

SimpleShaderContainer::SimpleShaderContainer(void) :	mVertProgram(0), mFragProgram(0), mShaderProgram(0), mIsBound(false)
{
	
	if(!_gShaderContext)
	{
		getStaticContext();
	}

	if(!_gShaderContext)
	{
		throw ErrorNotify("GLSL Activated... NOT!" );
	}

	// check for graphics card version supported @TODO

}



//=================================================================================================================


SimpleShaderContainer::~SimpleShaderContainer(void)
{
	RenderResourceFactory::onShaderDelete(this);

	glDeleteShader( mVertProgram);
	glDeleteShader( mFragProgram);
	glDeleteProgram( mShaderProgram);

}


//=================================================================================================================


kissU32 SimpleShaderContainer::getStaticContext()
{
	if( _gShaderContext == NULL )
	{
		//init the extensions & set context to one
		glewInit();
		_gShaderContext = 1;
	}

	return _gShaderContext;
}



void SimpleShaderContainer::destroyContext()
{
	_gShaderContext = NULL;
}


void SimpleShaderContainer::destroyProgram(ShaderProgram& program)
{
	glDeleteProgram(program);
}


//=================================================================================================================


void SimpleShaderContainer::shaderErrorCallback(void)
{
	VOID_RETURN_IF_NULL ( _gShaderContext );

	GLenum lastError = glGetError();

	VOID_RETURN_IF_NULL(lastError);

	switch(lastError)
	{
	case GL_INVALID_ENUM:
		printf("GL_INVALID_ENUM: Whatcha lookin' for?\n");
		break;

	case GL_INVALID_OPERATION:
		printf("GL_INVALID_OPERATION: Bad move, son!\n");
		break;

	case GL_INVALID_VALUE:
		printf("This value ain't worth spit\n");
		break;

	case GL_STACK_OVERFLOW:
	case GL_STACK_UNDERFLOW:
	case GL_OUT_OF_MEMORY:
		printf("Memory issues, dawg\n");
		break;
	}
}



void SimpleShaderContainer::enableVertProfile(void)		{}

void SimpleShaderContainer::enableFragProfile(void)		{}

void SimpleShaderContainer::disableVertProfile(void)	{}

void SimpleShaderContainer::disableFragProfile(void)	{}

void SimpleShaderContainer::updateProgramParameters()	{}

void SimpleShaderContainer::unbindVertProgram(void)		{}

void SimpleShaderContainer::unbindFragProgram(void)		{}

//=================================================================================================================
/*
	@todo: check for valid filename paths.
*/

ShaderProgram SimpleShaderContainer::loadProgram(const char *filename, const char* entry, ShaderProfile target )
{
	ShaderProgram shader = glCreateShader( target );

	glShaderSource( shader, 1, &filename, NULL);

	glCompileShader(shader);

	// check if shader compiled
	kiss32 compiled = 0;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

	if (shader && compiled)
	{
		if(!mShaderProgram)
		{
			mShaderProgram = glCreateProgram();
		}

		glAttachShader(mShaderProgram, shader);

		glBindAttribLocation(mShaderProgram, SA_POSITION,	SP_POS);
		glBindAttribLocation(mShaderProgram, SA_NORMAL,		SP_NOR);

		glLinkProgram(mShaderProgram);

		if (target == GL_VERTEX_SHADER)
			mVertProgram = shader;
		else if (target == GL_FRAGMENT_SHADER)
			mFragProgram = shader;
	}
	else
	{
		glDeleteShader( shader);
		shader = NULL;
	}

	return shader;
}

//=================================================================================================================

ShaderProgram SimpleShaderContainer::loadProgram(const char* name, const char* vp_filename, const char* fp_filename)
{
	if( strncmp( mName, name, MAX_NAME ) != 0 )
	{
		if(mShaderProgram)
		{
			glDeleteProgram( mShaderProgram);
			mShaderProgram = NULL;
		}

		strcpy_s( mName, MAX_NAME, name );
	}

	if(!mShaderProgram)
	{
		getShaderFromFile(vp_filename, GL_VERTEX_SHADER);
		getShaderFromFile(fp_filename, GL_FRAGMENT_SHADER);
	}
	
	return mShaderProgram;
}

//=================================================================================================================

ShaderProgram SimpleShaderContainer::loadProgram(int numFiles, ...)
{
	// USELESS. ONLY KEPT FOR REFERENCE.
	va_list ap;

	va_start( ap, numFiles);

	for (int ii = 0; ii < numFiles; ii++)
	{
		const char* filename = va_arg( ap, const char*);
		loadProgram(filename, NULL, GL_VERTEX_SHADER);
	}

	return mShaderProgram;
}

char* SimpleShaderContainer::getShaderFromFile(const char* filename, ShaderProfile target)
{
	FILE *shaderFile;
	char *text;
	long size;

	//must read files as binary to prevent problems from newline translation
	kiss32 error = fopen_s( &shaderFile, filename, "rb");

	if ( error != NULL )
		return 0;

	fseek( shaderFile, 0, SEEK_END);

	size = ftell(shaderFile);

	fseek( shaderFile, 0, SEEK_SET);

	text = new char[size+1];

	fread( text, size, 1, shaderFile);

	fclose( shaderFile);

	text[size] = '\0';

	loadProgram(text, NULL, target);

	delete []text;

	return NULL;

}

//=================================================================================================================


void SimpleShaderContainer::bindProgram(ShaderProgram& prog)
{
	glUseProgram(prog);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
}

void SimpleShaderContainer::unbindProgram()
{
	glUseProgram(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

	mIsBound = false;
}

//=================================================================================================================
/*
	This works with the assumption that the one shader file contains both vp and fp
*/

ShaderParameter SimpleShaderContainer::getNamedParam( const char* name )
{
	ParamShaderMap::iterator itr = mShaderParamMap.find(name);

	if( itr == mShaderParamMap.end() )
	{
		ShaderParameter param = glGetUniformLocation( mShaderProgram, name );

		if(param >= 0)
			mShaderParamMap[name] = param;

		return param;
	}	
	else
	{
		return itr->second;
	}

}

//=================================================================================================================


template<>
void SimpleShaderContainer::setFloatParameter(ShaderParameter param, float value)
{
	glUniform1f(param, value);
}

template<>
void SimpleShaderContainer::setFloatParameter(const char* name, const float value)
{
	ShaderParameter param = getNamedParam(name);
	if (param)
	{
		setFloatParameter(param, value);
	}
}


template<>
void SimpleShaderContainer::setVectorParameter(ShaderParameter param, const float* value)
{
	glUniform3fv(param, 3, value);
}

template<>
void SimpleShaderContainer::setVectorParameter(const char* name, const float* value)
{
	ShaderParameter param = getNamedParam(name);
	if (param)
	{
		setVectorParameter(param, value);
	}
}

template<>
void SimpleShaderContainer::setVectorParameter(ShaderParameter param, const VECTOR3* value)
{
	glUniform3f(param, value->x, value->y, value->z);
}

template<>
void SimpleShaderContainer::setVectorParameter(const char* name, const VECTOR3* value)
{
	ShaderParameter param = getNamedParam(name);
	if (param)
	{
		setVectorParameter(param, value);
	}
}


template<>
void SimpleShaderContainer::setMatrixParameter(ShaderParameter param, const Matrix4x4* value)
{
	glUniformMatrix4fv(param, 16, false, (float*)value->m);
}

template<>
void SimpleShaderContainer::setMatrixParameter(const char* name, const Matrix4x4* value)
{
	ShaderParameter param = getNamedParam(name);
	if(param)
	{
		setMatrixParameter(param, value);
	}
}

template<>
void SimpleShaderContainer::setMatrixParameter(ShaderParameter param, const float* value)
{
	glUniformMatrix4fv(param, 16, false, value);
}

template<>
void SimpleShaderContainer::setMatrixParameter(const char* name, const float* value)
{
	ShaderParameter param = getNamedParam(name);
	if (param)
	{
		setMatrixParameter(param, value);
	}
}

void SimpleShaderContainer::getMatrixParameter(ShaderParameter param, float* matrix)
{
	//cgGLGetMatrixParameterfr(param, matrix);
}



//=================================================================================================================

#endif