#version 460 core

struct Material
{
	//sampler2D	m_DiffuseTexture;
	//sampler2D	m_SpecularTexture;
	//sampler2D	m_NomalMap;
	vec4		m_Color;
	float		m_Shinieness;
};

struct DirectionalLight
{
	vec3 m_Ambient;
	vec3 m_Diffuse;
	vec3 m_Specular;
	vec3 m_Direction;
};

struct PointLight
{
	vec3 m_Ambient;
	vec3 m_Diffuse;
	vec3 m_Specular;
	vec3 m_Position;
	vec3 m_Attenuation;
};

struct SpotLight
{
	vec3 m_Ambient;
	vec3 m_Diffuse;
	vec3 m_Specular;
	vec3 m_Position;
	vec3 m_Direction;
	float m_InnerCutOffAngleCos;
	float m_OuterCutOffAngleCos;
};

vec3 ComputeDirectionalLight(Material mat, DirectionalLight dirLight, vec3 normal, vec3 toEye)
{
	vec3 color = dirLight.m_Ambient;

	vec3 lightVec = -dirLight.m_Direction;
	float diffuseFactor = dot(lightVec, normal);

	if (diffuseFactor > 0.0f)
	{
		color += dirLight.m_Diffuse * diffuseFactor;

		vec3 reflected = reflect(-lightVec, normal);
		float specFactor = pow(max(dot(toEye, reflected), 0.0), 32);

		color += specFactor * mat.m_Shinieness * dirLight.m_Specular;
	}

	return color;
}

vec3 ComputePointLight(Material mat, PointLight pointLight, vec3 pos, vec3 normal, vec3 toEye)
{
	vec3 lightVec = pointLight.m_Position - pos;
	float distance = length(lightVec);

	float att = 1.0f / dot(pointLight.m_Attenuation, vec3(1.0f, distance, distance * distance));
	vec3 color = pointLight.m_Ambient * att;

	lightVec /= distance;
	float diffuseFactor = dot(lightVec, normal);

	if (diffuseFactor > 0.0f)
	{
		color += pointLight.m_Diffuse * diffuseFactor * att;

		vec3 reflected = reflect(-lightVec, normal);
		float specFactor = pow(max(dot(toEye, reflected), 0.0), 32);

		color += specFactor * mat.m_Shinieness * pointLight.m_Specular * att;
	}

	return color;
}

vec3 ComputeSpotLight(Material mat, SpotLight spotLight, vec3 pos, vec3 normal, vec3 toEye)
{
	vec3 lightVec = normalize(spotLight.m_Position - pos);
	float theta = dot(-spotLight.m_Direction, lightVec);

	vec3 color = spotLight.m_Ambient;

	if (theta < spotLight.m_OuterCutOffAngleCos)
	{
		return color;
	}

	float diffuseFactor = dot(lightVec, normal);

	if (diffuseFactor > 0.0f)
	{
		float epsilon = spotLight.m_InnerCutOffAngleCos - spotLight.m_OuterCutOffAngleCos;
		float intensity = clamp((theta - spotLight.m_OuterCutOffAngleCos) / epsilon, 0.0f, 1.0f);

		color += spotLight.m_Diffuse * diffuseFactor * intensity;

		vec3 reflected = reflect(-lightVec, normal);
		float specFactor = pow(max(dot(toEye, reflected), 0.0), 32);

		color += specFactor * mat.m_Shinieness * spotLight.m_Specular * intensity;
	}

	return color;
}

in vec3 v_Normal;
in vec3 v_FragPos;

out vec4 o_FragmentColor;

uniform Material u_Material;

#define MAX_LIGHTS 4
uniform DirectionalLight u_DirLight;
uniform PointLight u_PointLights[MAX_LIGHTS];
uniform int u_PointLightsEnabledCount;
uniform SpotLight u_SpotLights[MAX_LIGHTS];
uniform int u_SpotLightsEnabledCount;

uniform vec3 u_EyePos;

void main()
{
	vec3 lightColor = vec3(0.0f);
	vec3 toEye = normalize(u_EyePos - v_FragPos);

	lightColor += ComputeDirectionalLight(u_Material, u_DirLight, v_Normal, toEye);

	for (int i = 0; i < u_PointLightsEnabledCount; i++)
	{
		lightColor += ComputePointLight(u_Material, u_PointLights[i], v_FragPos, v_Normal, toEye);
	}

	for (int i = 0; i < u_SpotLightsEnabledCount; i++)
	{
		lightColor += ComputeSpotLight(u_Material, u_SpotLights[i], v_FragPos, v_Normal, toEye);
	}

	o_FragmentColor = u_Material.m_Color * vec4(lightColor, 1.0f);
}
