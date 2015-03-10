#version 430

in vec3 vertPos;
in vec3 vertNorm;
in vec2 texCoord;

#define MAX_SPOTLIGHTS 4
#define MAX_POINTLIGHTS 4

struct Material
{
vec3 diff;				//Diffuse and Ambient reflection value
vec3 spec;				//Specular reflection value
float specEx;			//Specular exponent <Shiny value>
};

struct SpotLight
{
vec3 position;			//Light Position

vec3  spotDir;			//Spotlight direction
float spotOutCut;		//Spotlight outer cuttoff angle
float spotInCut;		//Spotlight inner cuttoff angle
vec3 diff;            	//Diffuse  light intensity
vec3 spec;			   	//Specular light intensity
float constant;       	//Distance Attenuation amounts
float linear;       	//Distance Attenuation amounts
float quadratic;       //Distance Attenuation amounts
};

struct PointLight
{
vec3 position;			//Light Position

vec3 diff;            	//Diffuse  light intensity
vec3 spec;			   	//Specular light intensity

float constant;       	//Distance Attenuation amounts
float linear;       	//Distance Attenuation amounts
float quadratic;       //Distance Attenuation amounts
};

struct GlobalDirectionLight
{
vec3 amb;
vec3 diff;
vec3 spec;

vec3 direction;
};

uniform SpotLight spotLight[MAX_SPOTLIGHTS];
uniform PointLight pointLight[MAX_POINTLIGHTS];
uniform GlobalDirectionLight glDirLight;

uniform Material material;

uniform mat3 NormalMatrix;
uniform vec3 viewPos;


layout(location=0)uniform sampler2D u_DiffuseTexture;
layout(location=1)uniform sampler2D u_SpecularTexture;
layout(location=2)uniform sampler2D u_NormalTexture;
layout(location=3)uniform sampler2D u_HeightTexture;

uniform int u_HasDiffuseTexture;
uniform int u_HasSpecularTexture;
uniform int u_HasNormalTexture;
uniform int u_HasHeightTexture;

uniform vec2 uvTile;

uniform int numOfSpotLights;
uniform int numOfPointLights;

out vec4 FragColour;

vec3 CalSpotLight(SpotLight light, vec3 normal, vec3 viewDir, vec3 vertPos)
{
	vec3 lightDir = normalize(light.position - vertPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    
	float theta = dot(lightDir,normalize(-light.spotDir));
	float epsilon = (light.spotInCut - light.spotOutCut);
	
	float distance = length(light.position - vertPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

	if(theta > light.spotOutCut)
	{
		
			float intensity = clamp((theta - light.spotOutCut) / epsilon, 0.0, 1.0);
 
			float diffuse = max(dot(normal, lightDir), 0.0);
  
			float specular = pow(max(dot(viewDir, reflectDir), 0.0), material.specEx);
    
			vec3 diffuseCol = light.diff * diffuse * material.diff * intensity; 
			vec3 specularCol = light.spec * specular * material.spec * intensity;
			if(dot(viewDir,normal)>0)
			{
			specularCol=vec3(0,0,0);
			}
			diffuseCol *= attenuation;
			specularCol*= attenuation;

			return diffuseCol + specularCol;
		
	}
	return  vec3(0,0,0);
}

vec3 CalPointLight(PointLight light, vec3 normal, vec3 viewDir, vec3 vertPos)
{
	vec3 lightDir = normalize(light.position - vertPos);
    vec3 reflectDir = reflect(-lightDir, normal);
	
	float distance = length(light.position - vertPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

	float diffuse  = max(dot(normal, lightDir), 0.0);
	float specular = pow(max(dot(viewDir, reflectDir), 0.0), material.specEx);

	vec3 diffuseCol  = light.diff * diffuse  * material.diff; 
	vec3 specularCol = light.spec * specular * material.spec;

	if(dot(viewDir,normal)<0)
	{
	specularCol=vec3(0,0,0);
	}

	diffuseCol *= attenuation;
	specularCol*= attenuation;
		
	return  diffuseCol + specularCol;
}

vec3 calculateDirectionLight(GlobalDirectionLight light, vec3 normal, vec3 viewDir, vec3 vertPos)
{
	vec3 lightDir = normalize(light.direction);
    vec3 reflectDir = reflect(-lightDir, normal);
	
	//float distance = length(light.position - vertPos);
	//float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

	float diffuse  = max(dot(normal, lightDir), 0.0);
	float specular = pow(max(dot(viewDir, reflectDir), 0.0), material.specEx);

	vec3 ambientCol  = light.amb  * material.diff;
	vec3 diffuseCol  = light.diff * diffuse  * material.diff; 
	vec3 specularCol = light.spec * specular * material.spec;
	if(dot(viewDir,normal)<0)
	{
	specularCol=vec3(0,0,0);
	}

	//diffuseCol *= attenuation;
	//specularCol*= attenuation;
		
	return ambientCol + diffuseCol + specularCol;

}
void main()
{
	 //--Properties--//
	vec3 norm = normalize(NormalMatrix * vertNorm);
    vec3 viewDir = normalize(viewPos - vertPos);

	vec3 light= vec3(0.0,0.0,0.0);

	light += calculateDirectionLight(glDirLight,norm, viewDir, vertPos);

	for(int i=0;i<numOfPointLights;++i)
	{
		light += CalPointLight(pointLight[i], norm, viewDir, vertPos);
	}
	for(int i=0;i<numOfSpotLights;i++)
	{
		light += CalSpotLight(spotLight[i] ,norm, viewDir,vertPos);
	}

	vec4 result=vec4(light,1.0);

	if(u_HasDiffuseTexture ==1) // false
	{
		result *= texture(u_DiffuseTexture, texCoord * uvTile);
	}
	else
	{
		result.w = 1;
	}

	FragColour=result;
}
