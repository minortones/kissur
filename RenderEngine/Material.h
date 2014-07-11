
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Nnanna Kama : Simple MATERIAL STRUCT:
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef MATERIAL_H
#define MATERIAL_H

class SimpleShaderContainer;


struct Material
{
	float	Ambient[3];
	float	Diffuse[3];
	float	Emissive[3];
	float	Specular[3];
	float	Shininess;

	SimpleShaderContainer*	ShaderContainer;

	Material() : ShaderContainer(NULL), Shininess(0.01f)
	{
		Ambient[0]			= Diffuse[0] = Emissive[0] = Specular[0] = 0.3f;
		Ambient[1]			= Diffuse[1] = Emissive[1] = Specular[1] = 0.3f;
		Ambient[2]			= Diffuse[2] = Emissive[2] = Specular[2] = 0.3f;
	}

	Material(SimpleShaderContainer* pShader) : ShaderContainer( pShader ), Shininess(0.01f)
	{
		Ambient[0]			= Diffuse[0] = Emissive[0] = Specular[0] = 0.3f;
		Ambient[1]			= Diffuse[1] = Emissive[1] = Specular[1] = 0.3f;
		Ambient[2]			= Diffuse[2] = Emissive[2] = Specular[2] = 0.3f;
	}

	void SetEmissive(float x, float y, float z)
	{
		Emissive[0] = x; Emissive[1] = y; Emissive[2] = z;
	}
	void SetDiffuse(float x, float y, float z)
	{
		Diffuse[0] = x; Diffuse[1] = y; Diffuse[2] = z;
	}
	void SetAmbient(float x, float y, float z)
	{
		Ambient[0] = x; Ambient[1] = y; Ambient[2] = z;
	}
	void SetSpecular(float x, float y, float z)
	{
		Specular[0] = x; Specular[1] = y; Specular[2] = z;
	}

	void SetShaderParams();
};





///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	EXTRA STATICS
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

static void setBrassMaterial(Material* mat)
{
	mat->SetEmissive( 0.0f,  0.0f,  0.0f );
	mat->SetAmbient( 0.33f, 0.22f, 0.03f );
	mat->SetDiffuse( 0.78f, 0.57f, 0.11f );
	mat->SetSpecular( 0.99f, 0.91f, 0.81f );
	mat->Shininess = 27.8f;
}

static void setRedPlasticMaterial(Material* mat)
{
	mat->SetEmissive( 0.0f,  0.0f,  0.0f);
	mat->SetAmbient( 0.0f, 0.0f, 0.0f );
	mat->SetDiffuse( 0.5f, 0.0f, 0.0f );
	mat->SetSpecular( 0.7f, 0.6f, 0.6f );
	mat->Shininess = 32.0f;
}

static void setEmissiveLightColorOnly(Material* mat, const VECTOR3& pEmissiveCol )
{
	mat->SetEmissive( pEmissiveCol.x,  pEmissiveCol.y, pEmissiveCol.z );
}

#endif