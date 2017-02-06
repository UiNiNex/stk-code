// Basic reflexion function
// source: https://www.clicktorelease.com/blog/creating-spherical-environment-mapping-shader
vec3 reflect(vec3 nor, vec3 eyedir)
{
    return eyedir - 2. * dot( nor, eyedir ) * nor;
}