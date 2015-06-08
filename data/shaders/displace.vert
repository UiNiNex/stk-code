uniform mat4 ModelMatrix;
uniform vec3 windDir;

#if __VERSION__ >= 330
layout(location = 0) in vec3 Position;
layout(location = 2) in vec3 Color;
layout(location = 3) in vec2 Texcoord;
layout(location = 4) in vec2 SecondTexcoord;
#else
in vec3 Position;
in vec3 Color;
in vec2 Texcoord;
in vec2 SecondTexcoord;
#endif

out vec2 uv;
out vec2 uv_bis;
out float camdist;

void main() {
    vec3 dir = windDir;
    
    float scale = 4.0;
    float x = Position.x;
    float y = Position.y;

    // calculate a scale factor.
    
    float s = sin( (windDir.y+3.0*y)*scale );
    float c = cos( (windDir.y+5.0*x)*scale );
    float z = s * c;
    
	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(Position + (z * Color.r), 1.);
	uv = Texcoord;
	uv_bis = SecondTexcoord;
	camdist = length(ViewMatrix * ModelMatrix *  vec4(Position, 1.));

}
