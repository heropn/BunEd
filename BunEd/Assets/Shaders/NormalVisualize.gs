#version 460

layout(triangles) in;
layout(line_strip, max_vertices = 6) out;

in VS_OUT {
	vec3 v_Normal;
} gs_in[];

const float lineMagnitude = 1.0f;

void GenerateLine(int index)
{
	gl_Position = gl_in[index].gl_Position;
	EmitVertex();
	gl_Position = (gl_in[index].gl_Position + vec4(gs_in[index].v_Normal, 0.0) * lineMagnitude);
	EmitVertex();
	EndPrimitive();
}

void main()
{
	GenerateLine(0); // first vertex normal
	GenerateLine(1); // second vertex normal
	GenerateLine(2); // third vertex normal
}
