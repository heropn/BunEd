#version 460

layout(location = 0) in vec3 a_Pos;
layout(location = 1) in vec2 a_TexCoord;

out vec2 v_TexCoord;

uniform mat4 u_Model;
uniform mat4 u_PV;

void main()
{
	gl_Position = u_PV * u_Model * vec4(a_Pos, 1.0);
	v_TexCoord = a_TexCoord;
}