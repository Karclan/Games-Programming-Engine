#version 430
// Uniform
uniform sampler2D tex;
uniform vec2 uvTile; // how many times should texture tile?

// Subroutine for getting tex colour so we can return plain white if no texture
//subroutine vec4 getTexColour();
//subroutine uniform getTexColour texColour;

// In values from vertex shader
in vec2 texCoord;
in vec4 colour;
in vec3 lightAD; // amb + diff light
in vec3 posInV; // pos in view space
in vec3 worldNorm; // normal in world space
in vec3 oLightDir; // direction from light

// Just used for shader without subroutines - int used like a bool
uniform int hasTex;

layout (location=0) out vec4 FragColour;

void main()
{
	// SPECULAR
	float specIntens = 1.0f; // obviously pointless but putting this here as a reminder - if you want to make it so you can dunamically set specular intensity
	float shininess = 400; // shininess, or rather power to raise specular hight to
	vec3 specK = vec3(1.0, 1.0, 1.0); // ditto coeeficient - should have ability to alter in main prog later
	
	vec3 V = normalize(-posInV); // assuming view is origin
	vec3 R = 2 * worldNorm * clamp(dot(oLightDir, worldNorm), 0.0, 1.0) - oLightDir; // custom reflect to stop negative highlight
	R = reflect(oLightDir, max(worldNorm, 0));
	
	vec3 spec = specK * specIntens * pow(dot(R, V), shininess);
	spec = clamp(spec, 0.0, 1.0);
	
	vec3 lightADS = clamp(lightAD + spec, 0.0, 1.0);
	
	
	
	// Without subroutines
	if(hasTex == 0) // false
	{
		FragColour =  colour;
	}
	else
	{
		FragColour =  texture(tex, texCoord * uvTile) * colour;
	}
	
	// With subroutines	
	//FragColour =  texColour() * colour;
}


// Possible sub routines for getting tex colour
/*
subroutine(getTexColour)
vec4 noTex()
{
	return vec4(1.0, 1.0, 1.0, 1.0);
}

subroutine(getTexColour)
vec4 sampleTex()
{
	return texture(tex, texCoord * uvTile);
}
* */
