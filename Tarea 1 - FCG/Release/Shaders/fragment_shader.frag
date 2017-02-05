#version 400
precision highp float;
struct DirLight {
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

in vec3 normales;
in vec3 FragPos;
in vec3 viewPos; 
in vec2 coordTex;

out vec4 color; 

uniform DirLight dirLight;
uniform sampler2D tex;
uniform int position_i;
uniform int bb;
uniform vec3 view;
uniform int hayTex;
uniform int currentInterpolacion;
uniform int currentDifuso;
uniform int currentSpecular;
uniform float fresnelR;
uniform float roughnessR;
uniform float shininessBP;
uniform float shininessP;
uniform float albido;
uniform float roughnessOren;

#define PI 3.14159265

float orenNayarDiffuse(
  vec3 lightDirection,
  vec3 viewDirection,
  vec3 surfaceNormal,
  float roughness,
  float albedo) {
  
  float LdotV = dot(lightDirection, viewDirection);
  float NdotL = dot(lightDirection, surfaceNormal);
  float NdotV = dot(surfaceNormal, viewDirection);

  float s = LdotV - NdotL * NdotV;
  float t = mix(1.0, max(NdotL, NdotV), step(0.0, s));

  float sigma2 = roughness * roughness;
  float A = 1.0 + sigma2 * (albedo / (sigma2 + 0.13) + 0.5 / (sigma2 + 0.33));
  float B = 0.45 * sigma2 / (sigma2 + 0.09);

  return albedo * max(0.0, NdotL) * (A + B * s / t) / PI;
}


vec4 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
	vec3 lightDir = normalize(light.direction - FragPos);
	vec4 diffuse;
	vec3 lightColor_d = vec3(1.0, 1.0, 1.0);
	vec3 color_d = vec3(1.0, 0.0, 0.0);
	float diff;
	if(currentDifuso == 1){
   		diff = max(dot(normal, lightDir), 0.0);
	} else if(currentDifuso == 2){
		diff = orenNayarDiffuse(lightDir, viewDir, normal, roughnessOren, albido);
	}

	if(position_i != 7){
		if(hayTex == -1){
			diffuse = (diff * texture2D(tex, coordTex));
		}else{
			diffuse = vec4(lightColor_d * (diff * color_d), 1.0);
		}
	}else{
		diffuse = vec4(0.8, 0.8, 0.8, 1.0);
	}

	return (diffuse);
}

float DistribucionBeckman(float NdotH, float roughness) {
    float mSquared = roughness * roughness;
	float r1 = 1.0 / ( 4.0 * mSquared * pow(NdotH, 4.0));
    float r2 = (NdotH * NdotH - 1.0) / (mSquared * NdotH * NdotH);
    return roughness = r1 * exp(r2);
}

float cookTorranceSpecular( vec3 lightDirection, vec3 viewDirection, vec3 surfaceNormal, float roughness, float fresnel) {

	float VdotN = max(dot(viewDirection, surfaceNormal), 0.0);
	float LdotN = max(dot(lightDirection, surfaceNormal), 0.0);

	//Half angle vector
	vec3 H = normalize(lightDirection + viewDirection);

	//Geometric term
	float NdotH = max(dot(surfaceNormal, H), 0.0);
	float VdotH = max(dot(viewDirection, H), 0.000001);
	float LdotH = max(dot(lightDirection, H), 0.000001);
	float x = 2.0 * NdotH / VdotH;

	float auxiliar = (2*NdotH*VdotN)/(VdotH);
	float auxiliar2 = (2*NdotH*LdotN)/(LdotH);
	float G = min(1.0, min(auxiliar, auxiliar2));
  
	//Distribution term
	float D = DistribucionBeckman(NdotH, roughness);

	//Fresnel term
	float F = fresnel + ((1 - fresnel)*pow((1-VdotH),5));

	//Multiply terms and done
	return  G * F * D / max(PI * VdotN * LdotN, 0.000001);
}

vec4 CalcSpecLight(DirLight light, vec3 normal, vec3 viewDir)
{
	vec3 lightDir = normalize(light.direction - FragPos);
	vec3 lightColor_s = vec3(1.0, 1.0, 1.0);
	vec3 color_s = vec3(1.0, 1.0, 1.0);
	vec4 specular;
	float spec;
	if(currentSpecular == 1){

		vec3 halfwayDir = normalize(lightDir + viewDir);
		spec = pow(max(dot(normal, halfwayDir), 0.0), shininessBP);

	} else if(currentSpecular == 2){

	   vec3 reflectDir = reflect(-lightDir, normal);
	   spec = pow(max(dot(viewDir, reflectDir), 0.0), shininessP);

	} else if(currentSpecular == 3){
		spec = cookTorranceSpecular(lightDir,viewDir, normal, roughnessR, fresnelR);
	}

	if(position_i != 7){
		if(hayTex == -1){
			specular = vec4(lightColor_s * (spec * color_s), 1.0);
		}else{
			specular = vec4(lightColor_s * (spec * color_s), 1.0);
		}
	}else{
		specular = vec4(0.8, 0.8, 0.8, 1.0);
	}

	return (specular);
}

void main()
{
	if(bb == 1){
		color = vec4(1.0, 0.0, 0.0, 1.0);
	}else{
		vec3 norm = normalize(normales);
		vec3 viewDir = normalize(viewPos - FragPos);
		vec4 result = CalcDirLight(dirLight, norm, viewDir);
		result += CalcSpecLight(dirLight, norm, viewDir);
		color = result;
	}
}