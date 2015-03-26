#version 430

layout(location=0) in vec3 vertPosition;
layout(location=2) in vec3 vertNormal;
layout(location=3) in vec2 uvData;
layout(location=4) in vec3 vertCol;
layout(location=8) in vec3 i_tangent;
layout(location=9) in vec3 i_biTangent;

uniform mat4 mModel;        //Matrix to covert into world coordinate system
uniform mat4 mView;         //Matrix to convert into camera coordinate system
uniform mat4 mProjection;   //Matrix to covert to normalised screen coordinates

uniform mat3 NormalMatrix;
uniform vec3 viewPos;

uniform int u_HasNormalTexture;

out vec3 vertPos;
out vec3 vertNorm;
out vec2 texCoord;

void main()  
{
    if(u_HasNormalTexture==1)
	{
	vec3 worldPosition	 = vec3(M * vec4(vertPosition,1.0)); 
	vec3 worldNormal	 = normalize(NormalMatrix * vertNormal);
	vec3 worldTangent	 = normalize(NormalMatrix * i_tangent);
	vec3 worldBitangent  = normalize(i_biTangent);   
	
	vec3 worldDirToLight = normalize(u_light_position - worldPosition.xyz);
	vec3 worldDirToCam   = normalize(u_CameraPos      - worldPosition.xyz);
    }
   
	vertPos = vec3(mModel * vec4(vertPosition,1.0)); 
	vertNorm = vertNormal;
	texCoord = uvData;
	gl_Position = mProjection * mView * mModel * vec4(vertPosition,1.0); //Transform from local to world to camera to NDCs
}
