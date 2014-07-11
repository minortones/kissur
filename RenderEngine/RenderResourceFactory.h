/***
 This should not need to be instantiated as it's just a static class.
 The idea is to manage render elements so they're created just once and reused... forever.
 Client classes won't need to worry about memory management & the rest; just ask and receive, not questions asked.
***/

#pragma once

#include <map>
#include "defines.h"

#include "SimpleShaderContainer.h"
#include "Material.h"



typedef std::map<const char*, SimpleShaderContainer*>	ShaderLibraryMap;
typedef std::map<kiss32, Material>						MaterialLibraryMap;

class RenderResourceFactory
{
public:
	  RenderResourceFactory(void);
	  ~RenderResourceFactory(void);

	  static void init();

	  static void shutDown();

	  static void onShaderDelete(SimpleShaderContainer* shader);

	  static SimpleShaderContainer* findOrCreateShader(const char* shader_path);


private:

	static ShaderLibraryMap		sShaderLibrary;		// TODO: use proper strings not char pointers
	static MaterialLibraryMap	sMaterialLibrary;
	static bool					sActive;
};

