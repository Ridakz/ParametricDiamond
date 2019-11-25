#version 330
//paras pour reflexion et refraction
const float EtaR = 0.75;
const float EtaG = 0.45;
const float EtaB = 0.35;
const float FresnelCoef = 1.0;

uniform mat4 mvp;
uniform vec4 lightPos;
uniform vec3 lightColor;
uniform mat4 mv;
uniform mat3 normal_mat;
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec3 normal;

out vec3 Reflect;
out vec3 RefractR;
out vec3 RefractG;
out vec3 RefractB;
out vec3 Ratio;
out vec4 fColor;
out vec3 fNormal;


//! [0]

//material
//float ambientIntensity=(0.212671*ambr + 0.715160*ambg + 0.072169*ambb)/(0.212671*difr + 0.715160*difg + 0.072169*difb);
vec4 ambient=vec4(0.1745,0.01175,0.01175,1.0);
vec4 diffuseColor=vec4(0.61424,	0.04136	,0.04136,1.0);
vec4 specular=vec4(0.727811,0.626959,0.626959,1.0);
float shininess =0.6;


//Add light paras
float ambiantStrength =1.0;
vec4 LightAmbient=vec4(ambiantStrength*lightColor,1.0);

//diffuse light
vec3 lightDir=normalize(lightPos.xyz-position);
vec4 posDir=vec4(lightDir,1.0f);
vec3 norm = normalize(normal);
float diff = max(dot(norm, lightDir), 0.0);
vec4 lightDiffuse=vec4(diff*lightColor,1.0);


float specularStrength=0.8;
vec3 viewDir = normalize(vec3(0.0,0.0,5.0)-position);
vec3 reflectDir = reflect(-lightDir, norm);
float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
vec4 lightSpecular = vec4(specularStrength * spec * lightColor,1.0);

//about those paras,use default value
//see more in https://developer.apple.com/documentation/glkit/glkeffectpropertylight
float constantAttenuation=1.0;
float linearAttenuation=0.0;
float quadraticAttenuation=0.0;
vec3 spotDirection=vec3(0.0, 0.0, -1.0);
float spotCutoff=180.0;
float spotExponent=0.0;

float f( float Eta )
{
return ((1.0-Eta)*(1.0-Eta))/((1.0+Eta)*(1.0+Eta));
}


void DirectionalLight( in int i, in vec3 V, in vec3 normal,
 inout vec4 ambient, inout vec4 diffuse, inout vec4 specular)
{
 float nDotLi; // normal . light direction
 float nDotH; // normal . light half vector
 float pf; // power factor
 vec3 halfway_vector = normalize(V+posDir.xyz);
 nDotLi = max(0.0, dot(normal, normalize(vec3(posDir))));
 if (nDotLi == 0.0)
 pf = 0.0;
 else
 {
 nDotH = max(0.0, dot(normal, halfway_vector));
 pf = pow(nDotH, 4*shininess);
 }
 //3 paras for more than 1 light
 ambient += LightAmbient;
 diffuse += lightDiffuse * nDotLi;
 specular += lightSpecular * pf;
}

void PointLight( in int i, in vec3 V, in vec3 sp, in vec3 normal,
inout vec4 ambient, inout vec4 diffuse, inout vec4 specular )
{
 float nDotLi; // normal . light direction
 float nDotH; // normal . light half vector
 float pf; // power factor
 float attenuation; // computed attenuation factor
 float d; // distance from surface to light source
 vec3 L; // direction from surface to light position
 vec3 halfway_vector; // direction of maximum highlights
 L = position.xyz - sp;
 d = length( L );
 L = normalize( L );
 attenuation = 1.0 / (constantAttenuation +
 linearAttenuation * d +
 quadraticAttenuation * d * d);
 halfway_vector = normalize(L + V);
 nDotLi = max(0.0, dot(normal, L));
  nDotH = max(0.0, dot(normal, halfway_vector));
  if (nDotLi == 0.0)
  pf = 0.0;
  else
  pf = pow(nDotH, shininess);
  ambient += LightAmbient;
  diffuse += lightDiffuse* attenuation;
  specular += lightSpecular  * attenuation;
 }

void SpotLight( in int i, in vec3 V, in vec3 sp, in vec3 normal,
 inout vec4 ambient, inout vec4 diffuse, inout vec4 specular )
{
 float nDotLi; // normal . light direction
 float nDotH; // normal . light half vector
 float pf; // power factor
 float spotDot; // cosine of angle between spotlight
 float spotAttenuation; // spotlight attenuation factor
 float attenuation; // computed attenuation factor
 float d; // distance from surface to light source
 vec3 L; // direction from surface to light position
 vec3 halfway_vector; // direction of maximum highlights
 L = position.xyz - sp;
 d = length( L );
 L = normalize( L );
 attenuation = 1.0 / (constantAttenuation +
 linearAttenuation * d +
 quadraticAttenuation * d * d);
  float cos_cutoff;
  // See if point on surface is inside the cone of illumination
  spotDot = dot( -L, normalize(spotDirection) );
  cos_cutoff = cos( spotCutoff ) ;
  if (spotDot < cos_cutoff)
  spotAttenuation = 0.0;
  else
  spotAttenuation = pow(spotDot, spotExponent);
  attenuation *= spotAttenuation;
  halfway_vector = normalize( L + V );
   nDotLi = max( 0.0, dot(normal, L) );
   nDotH = max( 0.0, dot(normal, halfway_vector) );
   if (nDotLi == 0.0)
   pf = 0.0;
   else
   pf = pow(nDotH, shininess);
   ambient += LightAmbient;
   diffuse += lightDiffuse * nDotLi * attenuation;
   specular += lightSpecular * pf * attenuation;
  }

void getEyeSpace( out vec3 norm, out vec3 position )
{
        norm = normalize( normal_mat * normal );
        position = vec3( mv * vec4( position, 1 ) );
}
vec3 light( int lightIndex, vec3 position, vec3 norm )
{
        vec3 s = normalize( vec3( lightPos.xyz - position ) );
        vec3 v = normalize( -position.xyz );
        vec3 r = reflect( -s, norm );

        vec3 ambient = LightAmbient.xyz * ambient.xyz;

        float sDotN = max( dot( s, norm ), 0.0 );
        vec3 diffuse = lightDiffuse.xyz * lightDiffuse.xyz * sDotN;

        vec3 spec = vec3( 0.0 );
        if ( sDotN > 0.0 )
                spec = lightSpecular.xyz * specular.xyz * pow( max( dot(r,v) , 0.0 ), shininess );

        return ambient + diffuse + spec;
}

void main()
{

    vec4 pos4=vec4(position,1.0);
    vec4 norm4=vec4(normal,1.0);
    vec3 sp = vec3( mv*pos4 );
    vec3 V = -normalize( sp );
    vec3 unit_normal = normalize( normal_mat*normal.xyz );
    vec4 amb = vec4(0.0); vec4 diff = vec4(0.0); vec4 spec = vec4(0.0);

    float factor = pow((1.0 - dot(-V, unit_normal)), FresnelCoef);
    Ratio.r = f(EtaR) + (1.0-f(EtaR)) * factor;
    Ratio.g = f(EtaG) + (1.0-f(EtaG)) * factor;
    Ratio.b = f(EtaB) + (1.0-f(EtaB)) * factor;
    RefractR = refract(V, unit_normal, EtaR);
    RefractG = refract(V, unit_normal, EtaG);
    RefractB = refract(V, unit_normal, EtaB);
    Reflect = reflect(V, unit_normal);
     if (posDir.w == 0.0)
     DirectionalLight(1, V, unit_normal, amb, diff, spec);
     else if (spotCutoff == 180.0)
     PointLight(1, V, sp, unit_normal, amb, diff, spec);
     else
     SpotLight(1, V, sp, unit_normal, amb, diff, spec);


    fColor=vec4(color,1.0);
    //fColor = (ambient * LightAmbient +diffuseColor *lightDiffuse + specular * lightSpecular)*vec4(color,1.0);




    gl_Position = mvp * vec4( position, 1.0 );


    fNormal = normal;

}
//! [0]
