// Simple texture shader

#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TextureCoord;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

out vec2 v_TextureCoord;

void main()
{
	v_TextureCoord = a_TextureCoord;
	gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
}


#type fragment
#version 330 core

layout(location = 0) out vec4 color;

uniform sampler2D u_Texture;
uniform vec4 u_Color;

in vec2 v_TextureCoord;

void main()
{
	color = texture(u_Texture, v_TextureCoord) * u_Color;
	//color = vec4(v_TextureCoord, 0.0f, 1.0f);
}
