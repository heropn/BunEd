#version 460 core

out vec4 o_FragmentColor;

in vec2 v_TexCoord;

uniform sampler2D u_Texture;
uniform sampler2D u_NormalMap;

void main()
{
	o_FragmentColor = texture(u_Texture, v_TexCoord);
}
