

#include <string.h>
#include <list>

#include "Macros.h"
#include "RenderResourceFactory.h"


ShaderLibraryMap	RenderResourceFactory::sShaderLibrary;
MaterialLibraryMap	RenderResourceFactory::sMaterialLibrary;
bool				RenderResourceFactory::sActive = false;


RenderResourceFactory::RenderResourceFactory(void)
{}


RenderResourceFactory::~RenderResourceFactory(void)
{}

void RenderResourceFactory::init()
{
	sActive = true;
}

void RenderResourceFactory::shutDown()
{
	sActive = false;

	for (ShaderLibraryMap::iterator itr = sShaderLibrary.begin(); itr != sShaderLibrary.end(); itr++)
	{
		SAFE_DELETE( itr->second );
	}

	sShaderLibrary.clear();

	SimpleShaderContainer::destroyContext();
}


void RenderResourceFactory::onShaderDelete(SimpleShaderContainer* shader)
{
	if (sActive)
	{
		ShaderLibraryMap::iterator itr = sShaderLibrary.begin();
		for ( ; itr != sShaderLibrary.end(); itr++)
		{
			if (itr->second == shader)
			{
				sShaderLibrary.erase(itr);
				break;
			}
		}
	}
}


SimpleShaderContainer* RenderResourceFactory::findOrCreateShader(const char* shader_name)
{
	ShaderLibraryMap::iterator itr = sShaderLibrary.find(shader_name);
	if ( itr != sShaderLibrary.end() )
	{
		return itr->second;
	}

	SimpleShaderContainer* shader = new SimpleShaderContainer();

	// prepend and append filename directories and stuff like that here.
	// @todo

	// load shader here
	// @todo


	sShaderLibrary[ shader_name ] = shader;

	return shader;

}