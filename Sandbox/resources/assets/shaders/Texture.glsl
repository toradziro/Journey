#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexturePos;

uniform mat4 u_vpMatrix;
uniform mat4 u_modelTransform;

out vec2 v_TexturePos;

void main()
{
	v_TexturePos = a_TexturePos;
	gl_Position = u_vpMatrix * u_modelTransform * vec4(a_Position, 1.0f);
}


#type fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_TexturePos;

uniform sampler2D u_texture;

void main()
{
	color = texture(u_texture, v_TexturePos);
}
