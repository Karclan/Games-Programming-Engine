#version 430


layout(location=0) in vec3 vertPosition;
layout(location=2) in vec3 vertNormal;
layout(location=3) in vec2 uvData;
layout(location=4) in vec3 vertCol;


uniform mat4 mModel;        //Matrix to covert into world coordinate system
uniform mat4 mView;         //Matrix to convert into camera coordinate system
uniform mat4 mProjection;   //Matrix to covert to normalised screen coordinates

out vec3 vertPos;
out vec3 vertNorm;

out vec2 texCoord;

void main()  
{
	vertPos = vec3(mModel * vec4(vertPosition,1.0)); 
	vertNorm = vertNormal;
	texCoord = uvData;
	gl_Position = mProjection * mView * mModel * vec4(vertPosition,1.0); //Transform from local to world to camera to NDCs
}
