#version 430

layout(location=0) in vec3 i_vertPosition;
layout(location=2) in vec3 i_vertNormal;
layout(location=3) in vec2 i_uvData;
layout(location=4) in vec3 i_vertCol;
layout(location=8) in vec3 i_tangent;
layout(location=9) in vec3 i_biTangent;

uniform mat4 mModel;        //Matrix to covert into world coordinate system
uniform mat4 mView;         //Matrix to convert into camera coordinate system
uniform mat4 mProjection;   //Matrix to covert to normalised screen coordinates

uniform mat3 NormalMatrix; //Change this name
uniform vec3 viewPos;	   //Change this name

out vec3 o_worldPosition;
out vec3 o_worldNormal;
out vec2 o_texCoord;
out vec3 o_worldTangent;
out vec3 o_worldBitangent;
out vec3 o_worldDirToCam;

void main()  
{
	o_texCoord 			= i_uvData;
	o_worldPosition		= vec3(mModel * vec4(i_vertPosition,1.0)); 
	o_worldNormal		= normalize(NormalMatrix * i_vertNormal);
	o_worldTangent	 	= normalize(NormalMatrix * i_tangent);
	o_worldBitangent  	= normalize(NormalMatrix * i_biTangent);   
	o_worldDirToCam   	= normalize(viewPos - o_worldPosition.xyz);
	 
	gl_Position = mProjection * mView * mModel * vec4(i_vertPosition,1.0); //Transform from local to world to camera to NDCs
}
