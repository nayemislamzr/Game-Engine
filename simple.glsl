#shader vertex
#version 330

layout (location = 0) in vec2 pos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(pos.xy,0.0,1.0);
}

#shader fragment
#version 330

out vec4 color;

void main()
{
	color = vec4(0.8,0.8,0.0,1.0);
}