#version 460

layout(location = 0) in vec2 a_Pos;
layout(location = 1) in vec2 a_TexCoord;

out vec2 v_TexCoord;

void main()
{
	gl_Position = vec4(a_Pos.xy, 0.0f, 1.0f);
	v_TexCoord = a_TexCoord;
}