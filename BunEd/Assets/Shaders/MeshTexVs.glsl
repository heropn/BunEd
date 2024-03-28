#version 460

layout(location = 0) in vec3 a_Pos;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec3 a_Tangent;
layout(location = 3) in vec2 a_TexCoord;

out vec2 v_TexCoord;
out mat3 v_TBN;

uniform mat4 u_Model;
uniform mat4 u_PV;

void main()
{
	gl_Position = u_PV * u_Model * vec4(a_Pos, 1.0);

	vec3 T = normalize(vec3(u_Model * vec4(a_Tangent, 0.0)));
	vec3 N = normalize(vec3(u_Model * vec4(a_Normal, 0.0)));
	T = normalize(T - dot(T, N) * N); // Making sure T and N are orthogonal by using Gram-Schmidt process
	vec3 B = cross(N, T);

	v_TBN = mat3(T, B, N);
	v_TexCoord = a_TexCoord;
}