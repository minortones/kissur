#version 120


varying vec4 color;

varying vec4 hPos;
varying vec4 hNorm;


uniform vec3 globalAmbient;
uniform vec3 eyePosition;
uniform mat4 modelViewProj;
uniform vec3 lightColor = vec3( 1.0, 1.0, 1.0);
uniform vec3 lightPosition = vec3( 1.0, 1.0, 1.0);
uniform vec3 Ke = vec3( 1.0, 1.0, 1.0);
uniform vec3 Ka = vec3( 1.0, 1.0, 1.0);
uniform vec3 Kd = vec3( 1.0, 1.0, 1.0);
uniform vec3 Ks = vec3( 1.0, 1.0, 1.0);
uniform float  shininess = 0.0f;

void main()
{

	vec3 P = hPos.xyz;
	vec3 N = hNorm.xyz;

	// Compute emissive term
	vec3 emissive = Ke;

	// Compute ambient term
	vec3 ambient = Ka * globalAmbient;

	// Compute the diffuse term
	vec3 L = normalize(lightPosition - P);
	float diffuseLight = max(dot(N, L), 0);
	vec3 diffuse = Kd * lightColor * diffuseLight;

	// Compute the specular term
	vec3 V = normalize(eyePosition - P);
	vec3 H = normalize(L + V);
	float specularLight = pow(max(dot(N, H), 0), shininess);
	if (diffuseLight <= 0)
	{
		specularLight = 0;
	}
	vec3 specular = Ks * lightColor * specularLight;


	//color.xyz = vec3(1,1,1);//emissive + ambient + diffuse + specular;
	//color.a = 1;

	gl_FragColor.xyz = emissive + ambient + diffuse + specular; //vec4(1,1,1,1);
	//gl_FragData[0] = vec4( position.xyz, 0.0);
}