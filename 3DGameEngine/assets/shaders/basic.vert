#version 430

layout (location=0) in vec3 pos;
layout (location=2) in vec3 normal;
layout (location=3) in vec2 uv;
layout (location=4) in vec3 vertCol;

// Matrix stuff
uniform mat4 mModel;
uniform mat4 mView;
uniform mat4 mProjection;

out vec4 colour; // note this MUST be same name as input of frag shader

void main()
{
    colour = vec4(vertCol, 1.0);

    gl_Position = mProjection * mView * mModel * vec4(pos, 1.0);

}