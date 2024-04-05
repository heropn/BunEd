#version 460

layout(location = 0) in vec3 a_Pos;
layout(location = 1) in vec3 a_Normal;

out VS_OUT
{
	vec3 v_Normal;
	vec3 v_FragPos;
} vs_out;

uniform mat4 u_Model;
uniform mat4 u_PV;

void main()
{
	gl_Position = u_Model * vec4(a_Pos, 1.0f);
	vs_out.v_FragPos = gl_Position.xyz;
	vs_out.v_Normal = normalize(u_Model * vec4(a_Normal, 0.0f)).xyz;

	gl_Position = u_PV * gl_Position;
}