uniform mat4 ModelMatrix;
uniform vec3 windDir;

#ifdef Explicit_Attrib_Location_Usable
layout(location = 0) in vec3 Position;
layout(location = 3) in vec2 Texcoord;
layout(location = 4) in vec2 SecondTexcoord;
#else
in vec3 Position;
in vec2 Texcoord;
in vec2 SecondTexcoord;
#endif

out vec2 uv;
out vec2 uv_bis;
out float camdist;

void main() {
	//gl_Position = ProjectionViewMatrix * ModelMatrix * vec4(Position, 1.);

    vec3 dir = windDir;
    float scale = 0.1;
 	float x = Position.x;
 	float y = Position.y;
 
 	// calculate a scale factor.
 	float s = sin( (windDir.y+20.0*y) ) *scale;
 	float c = cos( (windDir.y-5.0*x)*scale );
	vec3  z = vec3(0, s * c, 0);
 	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(Position + z, 1.);

	uv = Texcoord;
	uv_bis = SecondTexcoord;
	camdist = length(ViewMatrix * ModelMatrix *  vec4(Position, 1.));
}
