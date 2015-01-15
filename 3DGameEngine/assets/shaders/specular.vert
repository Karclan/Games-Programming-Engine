#version 430

layout (location=0) in vec3 pos;
layout (location=2) in vec3 normal;
layout (location=3) in vec2 uv;
layout (location=4) in vec3 vertCol;

// Matrix stuff
uniform mat4 mModel;
uniform mat4 mView;
uniform mat4 mProjection;

// Lighting Stuff
uniform vec3 lightDir;
uniform vec3 lightIntens;


// Note these output values must be called same thing as in values in frag shader!
out vec2 texCoord;
out vec4 colour;
out vec3 posInV; // pos in view space
out vec3 lightAD;
out vec3 worldNorm; // normal in world space
out vec3 oLightDir; // direction to light rather than from it

void main()
{
	// hard coded ambient for the time being
	vec3 amb = lightIntens * 0.5f;
	
	 // Simple Diffuse vertex shader lighting
    vec3 N = vec3(mModel * vec4(normal, 0.0)); // normal in model space. Note homo coord is 0.0 in vec4(normal) to cancel out translation?
    N = normalize(N);
    vec3 diff = lightIntens  * 0.5 * max(dot(N, -lightDir), 0.0); //calculate Diffuse Light Intensity making sure it is not negative 
    diff = clamp(diff, 0.0, 1.0);  // and is clamped 0 to 1 
 
	// Values passed to frag shader
	colour = vec4(vertCol, 1.0); // also multiplied by coefficient of reflectivity and added to ambient lighting, not implemented yet
	lightAD = amb + diff;
	texCoord = uv;
	posInV = vec3(mView * mModel * vec4(pos,1.0));
	worldNorm = N;
	oLightDir = lightDir;
   
	gl_Position = mProjection * mView * mModel * vec4(pos, 1.0);
}
