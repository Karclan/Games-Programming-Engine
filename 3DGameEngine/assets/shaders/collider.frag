#version 430

in vec3 vertPos;
in vec3 vertNorm;
in vec2 texCoord;

out vec4 FragColour;


void main()
{
	FragColour = vec4(0.0, 1.0, 0.0, 0.3f);
}
