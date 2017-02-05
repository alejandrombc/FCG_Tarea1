#version 400
precision highp float;
struct DirLight {
	vec3 direction;
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct Modelo {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shinyP_M;
	float shinyBP_M;
	float fresnel_M;
	float albido_M;
	float roughnessOren_M;
	float roughness_M;
	int currentSpecular_M;
	int currentDifuso_M;
};

in vec3 normales;
in vec3 FragPos;
in vec3 viewPos; 
in vec2 coordTex;
in vec4 color_vertex;


out vec4 color; 

uniform DirLight dirLight;
uniform Modelo model;
uniform sampler2D tex;
uniform int position_i;
uniform bool bb;
uniform vec3 view;
uniform int hayTex;
uniform int currentInterpolacion;
uniform float currentLight;
uniform float pointConstant;
uniform float pointLinear;
uniform float pointQuadratic;
uniform float innerCut;
uniform float outerCut;
uniform int hayLuz;


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


vec4 CalcDifLight(Modelo modelos, DirLight light, vec3 normal, vec3 viewDir)
{
	vec3 lightDir = normalize(light.direction - FragPos);
	vec4 diffuse;
	float diff;
	if(modelos.currentDifuso_M == 1){
   		diff = max(dot(normal, lightDir), 0.0);
	} else if(modelos.currentDifuso_M == 2){
		diff = orenNayarDiffuse(lightDir, viewDir, normal, modelos.roughnessOren_M, modelos.albido_M);
	}

	diffuse = vec4(light.diffuse * (diff * modelos.diffuse), 1.0);
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

	//Vector medio
	vec3 H = normalize(lightDirection + viewDirection);

	//Atenuacion geometrica
	float NdotH = max(dot(surfaceNormal, H), 0.0);
	float VdotH = max(dot(viewDirection, H), 0.000001);
	float LdotH = max(dot(lightDirection, H), 0.000001);
	float x = 2.0 * NdotH / VdotH;

	float auxiliar = (2*NdotH*VdotN)/(VdotH);
	float auxiliar2 = (2*NdotH*LdotN)/(LdotH);
	float G = min(1.0, min(auxiliar, auxiliar2));
  
	//Beckman (la D de la formula)
	float D = DistribucionBeckman(NdotH, roughness);

	//Finalmente el fresnel
	float F = fresnel + ((1 - fresnel)*pow((1-VdotH),5));

	//Multiplico y divido
	return  G * F * D / max(PI * VdotN * LdotN, 0.000001);
}

vec4 CalcSpecLight(Modelo modelos, DirLight light, vec3 normal, vec3 viewDir)
{
	vec3 lightDir = normalize(light.direction - FragPos);
	vec4 specular;
	float spec;
	if(modelos.currentSpecular_M == 1){

		vec3 halfwayDir = normalize(lightDir + viewDir);
		spec = pow(max(dot(normal, halfwayDir), 0.0), modelos.shinyBP_M);

	} else if(modelos.currentSpecular_M == 2){

	   vec3 reflectDir = reflect(-lightDir, normal);
	   spec = pow(max(dot(viewDir, reflectDir), 0.0), modelos.shinyP_M);

	} else if(modelos.currentSpecular_M == 3){
		spec = cookTorranceSpecular(lightDir,viewDir, normal, modelos.roughness_M, modelos.fresnel_M);
	}


	specular = vec4(light.specular * (spec * modelos.specular), 1.0);
	return (specular);
}

void main()
{
	if(currentInterpolacion == 1){
		if(bb){
			color = vec4(1.0, 0.0, 0.0, 1.0);
		}else if (hayLuz != 1){
			color = vec4(0.0, 0.0, 0.0, 1.0);
		}else{
			color = color_vertex;
			if(position_i != 7 && hayTex == -1) color = color * texture2D(tex, coordTex);
		}
	}else{
		if(bb){
			color = vec4(1.0, 0.0, 0.0, 1.0);
		}else if (hayLuz != 1){
			color = vec4(0.0, 0.0, 0.0, 1.0);
		}
		else{

			vec3 norm = normalize(normales);
			vec3 viewDir = normalize(viewPos - FragPos);
			vec4 result = CalcDifLight(model, dirLight, norm, viewDir);
			result += CalcSpecLight(model, dirLight, norm, viewDir);
		
			if(position_i == 7 && (currentLight == 2 || currentLight == 1)) color = vec4(dirLight.specular * dirLight.diffuse, 1.0);
			else{
				float attenuation = 1.0;
				float intensity = 1.0;
				if(currentLight == 2){
					// Attenuacion
					float distance = length(dirLight.direction - FragPos);
					attenuation = 1.0 / (pointConstant + pointLinear * distance + pointQuadratic * (distance * distance));  
				}else if(currentLight == 3){

					vec3 lightDir = normalize(dirLight.direction - FragPos);
					// Spotlight (soft edges)
					float theta = dot(lightDir, normalize(-dirLight.position)); 
					float epsilon = (innerCut - outerCut);
					intensity = clamp((theta - outerCut) / epsilon, 0.0, 1.0);

					// Attenuacion
					float distance = length(dirLight.direction - FragPos);
					attenuation = 1.0 / (pointConstant + pointLinear * distance + pointQuadratic * (distance * distance));  
				}

				//Color ambiental
				result += vec4(dirLight.ambient, 1.0) * vec4(model.ambient,1.0);


				if(position_i == 7 && (currentLight == 3)) color = vec4(dirLight.specular * dirLight.diffuse, 1.0) * attenuation * intensity;
				else if(hayTex == -1) color = result * attenuation * intensity * texture2D(tex, coordTex);
				else color = result * attenuation * intensity;
			}
		}
	}
}