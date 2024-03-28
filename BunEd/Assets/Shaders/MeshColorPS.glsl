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

in vec3 v_Normal;
in vec3 v_FragPos;

out vec4 o_FragmentColor;

uniform Material u_Material;
uniform DirectionalLight u_DirLight;
uniform vec3 u_EyePos;

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

void main()
{
	vec3 lightColor = vec3(0.0f);
	vec3 toEye = normalize(u_EyePos - v_FragPos);

	lightColor += ComputeDirectionalLight(u_Material, u_DirLight, v_Normal, toEye);

	o_FragmentColor = u_Material.m_Color * vec4(lightColor, 1.0f);
}
