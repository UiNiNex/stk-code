uniform mat4 ModelMatrix;
uniform vec3 windDir;

#if __VERSION__ >= 330
layout(location = 0) in vec3 Position;
layout(location = 1) in vec3 Normal;
layout(location = 2) in vec3 Color;
layout(location = 3) in vec2 Texcoord;
layout(location = 4) in vec2 SecondTexcoord;
#else
in vec3 Position;
in vec3 Normal;
in vec3 Color;
in vec2 Texcoord;
in vec2 SecondTexcoord;
#endif

out vec2 uv;
out vec2 uv_bis;
out float camdist;

out vec3 normalInterp;
out vec3 vertPos;

void main() {
    vec3 dir = windDir;
    
    float scale = 4.0;
    float x = Position.x;
    float y = Position.y;

    // calculate a scale factor.
    
    float s = sin( (windDir.y+3.0*y)*scale );
    float c = cos( (windDir.y+5.0*x)*scale );
    vec3 zPosition = Position + (s * c * Color.r);
	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(zPosition, 1.);
	
	// FIXME We should NOT calculate the normal matrix in the shader. This waste ressource
	mat4 normalMatrix = transpose(inverse(ModelMatrix));
	
	vec4 vertPos4 = ViewMatrix * ModelMatrix * vec4(zPosition, 1.0);
    vertPos = vec3(vertPos4) / vertPos4.w;
    normalInterp = vec3(normalMatrix * vec4(Normal, 0.0));

	uv = Texcoord;
	uv_bis = SecondTexcoord;
	camdist = length(ViewMatrix * ModelMatrix *  vec4(zPosition, 1.));

}
