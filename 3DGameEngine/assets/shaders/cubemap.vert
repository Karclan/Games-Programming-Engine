#version 430

layout(location=0)in vec3 i_cubemapVertex;

uniform mat4 u_ModelMatrix;
uniform mat4 u_ViewMatrix;
uniform mat4 u_ProjectionMatrix;

void main()
{
	gl_Position =  vec4(i_cubemapVertex,1.0);
}