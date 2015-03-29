#version 430

layout(location=0)in vec4 i_cubemapVertex;

uniform mat4 u_ModelMatrix;
uniform mat4 u_ViewMatrix;
uniform mat4 u_ProjectionMatrix;

void main()
{
	gl_Position =  i_cubemapVertex;
}