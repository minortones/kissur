#version 120



varying vec3 in_pos;
varying vec3 in_nor;

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
	hPos = vec4(in_pos.xyz, 1) * modelViewProj;
	hNorm = vec4(in_nor.xyz, 1) * modelViewProj;
	gl_Position = vec4(in_pos.xyz, 1) * modelViewProj;
}