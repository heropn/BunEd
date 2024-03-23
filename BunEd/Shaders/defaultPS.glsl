#version 460 core

out vec4 o_FragmentColor;

in vec3 v_Color;

void main()
{
	o_FragmentColor = vec4(v_Color, 1.0f);
}