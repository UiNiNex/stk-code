#ifdef Use_Bindless_Texture
layout(bindless_sampler) uniform sampler2D tex;
layout(bindless_sampler) uniform sampler2D sphereMap;
layout(bindless_sampler) uniform sampler2D glossMap;
#else
uniform sampler2D tex;
uniform sampler2D sphereMap;
uniform sampler2D glossMap;
#endif


uniform float fogmax;
uniform float startH;
uniform float endH;
uniform float start;
uniform float end;
uniform vec3 col;

in vec2 uv;
in vec4 color;
in vec3 nor;
out vec4 FragColor;

#stk_include "utils/SpecularBRDF.frag"
#stk_include "utils/SunMRP.frag"
#stk_include "utils/reflect.frag"

void main()
{
    vec4 diffusecolor = texture(tex, uv);
#ifdef Use_Bindless_Texture
#ifdef SRGBBindlessFix
    diffusecolor.xyz = pow(diffusecolor.xyz, vec3(2.2));
#endif
#endif
    diffusecolor.xyz *= pow(color.xyz, vec3(2.2));
    diffusecolor.a *= color.a;
    vec3 tmp = vec3(gl_FragCoord.xy / screen, gl_FragCoord.z);
    tmp = 2. * tmp - 1.;

    vec4 xpos = vec4(tmp, 1.0);
    xpos = InverseProjectionMatrix * xpos;
    xpos.xyz /= xpos.w;

    float dist = length(xpos.xyz);
    float fog = smoothstep(start, end, dist);

    fog = min(fog, fogmax);

    vec3 eyedir = -normalize(xpos.xyz);
    vec3 Lightdir = SunMRP(nor, eyedir);

    vec3 Specular = SpecularBRDF(nor, eyedir, Lightdir, vec3(1.), 0.7) * sun_col;
    vec4 finalcolor = vec4(col, 0.) * fog + diffusecolor *(1. - fog);
    float diffuse = max(dot(nor, Lightdir), 0.1);
    vec3 diffuse_coef = mix(vec3(0.3, 0.5, 0.8), sun_col, diffuse);

    float spec_map = texture(glossMap, uv).r;

    Specular *= spec_map;

    // Compute the reflexion
    vec3 r = reflect(nor, eyedir);
    float m = 2. * sqrt( pow( r.x, 2. ) + pow( r.y, 2. ) + pow( r.z + 1., 2. ) );
    vec2 vN = r.xy / m + .5;
    vec3 base = texture2D( sphereMap, vN ).rgb;

    //FragColor = vec4(finalcolor.rgb * finalcolor.a + Specular, finalcolor.a);
    finalcolor.rgb *= base * diffuse_coef; //* 0.5;

    //finalcolor.rgb = diffuse_coef;
    FragColor = vec4(finalcolor.rgb * finalcolor.a + Specular, finalcolor.a);
}
