#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TexturePos;

uniform mat4 u_vpMatrix;

out vec2 v_TexturePos;
out vec4 v_Color;

void main()
{
	v_Color = a_Color;
	v_TexturePos = a_TexturePos;
	gl_Position = u_vpMatrix * vec4(a_Position, 1.0f);
}


#type fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_TexturePos;
in vec4 v_Color;

uniform sampler2D u_texture;

void main()
{
	color = v_Color;
	//texture(u_texture, v_TexturePos) * v_Color;
}
