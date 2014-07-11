
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Nnanna Kama : Simple MATERIAL STRUCT:
///////////////////////////////////////////////////////////////////////////////////////////////////////////////


#include "Structs.h"
#include "Material.h"
#include "SimpleShaderContainer.h"

void Material::SetShaderParams()
{
	if (ShaderContainer)
	{
		ShaderContainer->setVectorParameter(SP_EMISSIVE, Emissive);
		ShaderContainer->setVectorParameter(SP_AMBIENT, Ambient);
		ShaderContainer->setVectorParameter(SP_DIFFUSE, Diffuse);
		ShaderContainer->setVectorParameter(SP_SPECULAR, Specular);
		ShaderContainer->setFloatParameter(SP_SHININESS, Shininess);
	}
}