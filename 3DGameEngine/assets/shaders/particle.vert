#version 430

layout(location=0)in vec4 i_particleVertex;
layout(location=1)in vec4 i_particleColour;

uniform mat4 u_ModelMatrix;
uniform mat4 u_ViewMatrix;
uniform mat4 u_ProjectionView;

uniform float u_PointSize;
out vec4 o_colour;

void main()
{
	vec4 eyePosition = u_ViewMatrix * i_particleVertex;

	gl_Position = u_ProjectionView * u_ViewMatrix * u_ModelMatrix * i_particleVertex;
	
	o_colour = i_particleColour;
	
	//float distance = length(eyePosition.xyz);
	//float attenuation = inversesqrt(0.1f*distance);
	gl_PointSize = u_PointSize;
}