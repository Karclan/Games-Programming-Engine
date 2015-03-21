#version 430

layout(location=0)in vec4 i_particleVertex;
layout(location=1)in vec4 i_particleColour;

uniform mat4 u_ModelView;
uniform mat4 u_ProjectionView;

out vec4 o_colour;

void Main()
{
	vec4 eyePosition = u_ModelView * gl_Vertex;
	gl_Position = u_ProjectionView * eyePosition;
	
	o_colour = i_particleColour;
	
	float distance = length(eyePosition.xyz);
	float attenuation = inversesqrt(0.1f*distance);
	gl_PointSize = 2.0f*attenuation;
}