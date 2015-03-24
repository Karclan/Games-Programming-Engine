#version 430

layout (location=0) in vec3 pos;
layout (location=2) in vec3 normal;
layout (location=3) in vec2 uv;
layout (location=4) in vec3 vertCol;
layout (location=5) in ivec4 BoneIDs;
layout (location=6) in vec4 Weights;

// Matrix stuff
uniform mat4 mModel;
uniform mat4 mView;
uniform mat4 mProjection;

//animation
const int _maxBones = 4;
uniform mat4 mBones[_maxBones];

out vec4 normal0;
out vec4 colour; // note this MUST be same name as input of frag shader

void main()
{

	//animation
	mat4 mBoneTransform = mBones[BoneIDs[0]] * Weights[0];
	mBoneTransform = mBones[BoneIDs[1]] * Weights[1];
	mBoneTransform = mBones[BoneIDs[2]] * Weights[2];
	mBoneTransform = mBones[BoneIDs[3]] * Weights[3];


	vec4 animPos = mBoneTransform * vec4(pos, 1.0);
	gl_Position = mProjection * mView * mModel * vec4(animPos.xyz, 1.0);

	vec4 animNormal = mBoneTransform * vec4(normal, 0.0);
	normal0 = mView * mModel * animNormal;
    colour = vec4(vertCol, 1.0);

    //gl_Position = mProjection * mView * mModel * vec4(pos, 1.0);

}