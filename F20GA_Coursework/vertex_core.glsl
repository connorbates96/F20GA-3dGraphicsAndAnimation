#version 450

layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec3 vertex_color;
layout (location = 2) in vec2 vertex_texcoord;
layout (location = 3) in vec3 vertex_normal;

out vec3 vs_position;
out vec3 vs_color;
out vec2 vs_texcoord;
out vec3 vs_normal;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

void main(){


	vs_position = vec4(ModelMatrix * vec4(vertex_position, 1.f)).xyz;		//Using ModelMatrix, multiply by vertex position and only returns value after '.' (e.g. .xyz returns only X Y and Z)
	vs_color = vertex_color;
	vs_texcoord = vec2(vertex_texcoord.x, vertex_texcoord.y * -1.f);		//*-1.f flips the image so it isn't upside down
	vs_normal = mat3(ModelMatrix) * vertex_normal;							//Normal has to be in world space, going to move as the model moves. mat3 and not mat4 because we don't want to deal with the w value

	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(vertex_position, 1.f);

}