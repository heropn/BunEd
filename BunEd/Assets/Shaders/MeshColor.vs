#version 460

layout(location = 0) in vec3 a_Pos;
layout(location = 1) in vec3 a_Normal;

out vec3 v_Normal;
out vec3 v_FragPos;

uniform mat4 u_Model;
uniform mat4 u_PV;

void main()
{
	gl_Position = u_Model * vec4(a_Pos, 1.0f);
	v_FragPos = gl_Position.xyz;
	v_Normal = normalize(u_Model * vec4(a_Normal, 0.0f)).xyz;

	gl_Position = u_PV * gl_Position;
}