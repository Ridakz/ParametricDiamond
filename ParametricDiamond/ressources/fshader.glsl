#version 330


in vec4 fColor;
in vec3 fNormal;
out vec4 fragColor;

in vec3 Reflect;
in vec3 RefractR;
in vec3 RefractG;
in vec3 RefractB;
in vec3 Ratio;


void main()
{

    vec3 refractColor, reflectColor;
    refractColor.r =fColor.r*RefractR.r;
    refractColor.g = fColor.g*RefractG.g;
    refractColor.b =  fColor.b*RefractB.b;
    reflectColor = Reflect*fColor.xyz;

    //vec4 fColor = vec4(mix(refractColor, reflectColor, Ratio),1.0);
    fragColor = fColor;

    //fragColor =fColor;
}
