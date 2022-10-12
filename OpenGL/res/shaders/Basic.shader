#shader vertex
#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 color;
out vec3 vert_color;
void main()
{
	vert_color = color;
	gl_Position = vec4(pos.x, pos.y, pos.z, 1.0);
};

#shader fragment
#version 330 core
//in vec3 vert_color;
out vec4 frag_color;
uniform vec4 u_Color;
void main()
{
	//frag_color = vec4(vert_color, 1.0);
	frag_color = u_Color;
};
