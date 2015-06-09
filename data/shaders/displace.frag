uniform sampler2D displacement_tex;
uniform sampler2D mask_tex;
uniform sampler2D color_tex;
uniform sampler2D tex;
uniform vec2 dir;
uniform vec2 dir2;

in vec2 uv;
in vec2 uv_bis;
in float camdist;

in vec3 normalInterp;
in vec3 vertPos;

out vec4 FragColor;

const float maxlen = 0.02;

// TODO put these parameters in stk

const vec3 lightPos = vec3(143.83, 732.16, 1358.93);
const vec3 specColor = vec3(1.0, 1.0, 1.0);

void main()
{
	float horiz = texture(displacement_tex, uv + dir).x;
	float vert = texture(displacement_tex, (uv.yx + dir2) * vec2(0.9)).x;

	vec2 offset = vec2(horiz, vert);
	offset *= 2.0;
	offset -= 1.0;

	// Fade according to distance to cam
	float fade = 1.0 - smoothstep(1.0, 100.0, camdist);

	vec4 shiftval;
	shiftval.r = step(offset.x, 0.0) * -offset.x;
	shiftval.g = step(0.0, offset.x) * offset.x;
	shiftval.b = step(offset.y, 0.0) * -offset.y;
	shiftval.a = step(0.0, offset.y) * offset.y;

	vec2 shift;
	shift.x = -shiftval.x + shiftval.y;
	shift.y = -shiftval.z + shiftval.w;
	shift /= 50.;

	vec2 tc = gl_FragCoord.xy / screen;
	float mask = texture(mask_tex, tc + shift).x;
	tc += (mask < 1.) ? vec2(0.) : shift;

    vec4 col = texture(color_tex, tc);
    vec4 blend_tex = texture(tex, uv);
    col.rgb = blend_tex.rgb * blend_tex.a + (1. - blend_tex.a) * col.rgb;
    
    /* Compute the light ----------------------- */

    vec3 normal = normalize(normalInterp); 
    vec3 lightDir = normalize(lightPos - vertPos);

    float lambertian = max(dot(lightDir,normal), 0.0);
    float specular = 0.0;
    
    if (lambertian > 0.0){
        vec3 reflectDir = reflect(-lightDir, normal);
        vec3 viewDir = normalize(-vertPos);

        float specAngle = max(dot(reflectDir, viewDir), 0.0);
        specular = pow(specAngle, 4.0);
    }
    
    FragColor = vec4(normal, 1.);

    //FragColor = vec4(col.rgb, 1.);
    
    FragColor = vec4( lambertian*col.rgb + specular*specColor, 1.0);
}
