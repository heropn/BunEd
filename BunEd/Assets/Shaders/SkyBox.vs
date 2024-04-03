#version 460

layout(location = 0) in vec3 a_Pos;

out vec3 v_TexCoord;

uniform mat4 u_PV;

void main()
{
	v_TexCoord = a_Pos;
	gl_Position = u_PV * vec4(a_Pos, 1.0f);
	gl_Position = gl_Position.xyww; // optimization to make opengl think skybox has max depth
}