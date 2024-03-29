#version 460 core

struct Material
{
	sampler2D	m_DiffuseTexture;
	sampler2D	m_SpecularTexture;
	sampler2D	m_NomalMap;
	float		m_Shinieness;
};

struct DirectionalLight
{
	vec3 m_Ambient;
	vec3 m_Diffuse;
	vec3 m_Specular;
	vec3 m_Direction;
};

out vec4 o_FragmentColor;

in vec2 v_TexCoord;
in mat3 v_TBN;

uniform Material u_Material;

vec3 ComputeDirectionalLight(Material mat, DirectionalLight dirLight, vec3 normal, vec3 toEye)
{
	vec3 color = dirLight.m_Ambient;

	vec3 lightVec = -dirLight.m_Direction;
	float diffuseFactor = dot(lightVec, normal);

	if (diffuseFactor > 0.0f)
	{
		color += dirLight.m_Diffuse * diffuseFactor;
	}

	return color;
}

void main()
{
	DirectionalLight dirLight;
	dirLight.m_Ambient = vec3(0.3f);
	dirLight.m_Diffuse = vec3(0.6f);
	dirLight.m_Specular = vec3(1.0f);
	dirLight.m_Direction = vec3(0.0f, -1.0f, 0.0f);
	//dirLight.m_Direction = normalize(transpose(v_TBN) * dirLight.m_Direction);

	vec3 lightColor = vec3(0.0f);

	vec3 normal = texture(u_Material.m_NomalMap, v_TexCoord).xyz;
	normal = normal * 2.0f - 1.0f;
	normal = normalize(v_TBN * normal);

	lightColor += ComputeDirectionalLight(u_Material, dirLight, normal, vec3(0.0f));

	o_FragmentColor = texture(u_Material.m_DiffuseTexture, v_TexCoord) * vec4(lightColor, 1.0f);
	o_FragmentColor = vec4(normal, 1.0f);
}
