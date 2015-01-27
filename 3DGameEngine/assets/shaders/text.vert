#version 430

layout (location = 0) in vec4 vertex;
out vec2 texCoords;

uniform mat4 mProjection;

void main()
{
	texCoords = vertex.zw;
	gl_Position = mProjection * vec4(vertex.xy,0.0,1.0);
}
