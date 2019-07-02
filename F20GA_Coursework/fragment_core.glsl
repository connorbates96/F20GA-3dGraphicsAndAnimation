#version 450


struct Material{

	//These contain both the colours and the intensity.
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	//These are the textures
	sampler2D textureDiffuse;
	sampler2D textureSpecular;

};

in vec3 vs_position;
in vec3 vs_color;
in vec2 vs_texcoord;
in vec3 vs_normal;

out vec4 fs_color;

uniform Material material;
uniform vec3 lightPos0;
uniform vec3 cameraPos;

//FUNCTIONS-------------------------------------------------------------------------


vec3 calculateAmbient(){
	return material.ambient;
}


vec3 calculateDiffuse(){

	vec3 posToLightDirVec = normalize(lightPos0 - vs_position);
	float diffuseIntensity = clamp(dot(posToLightDirVec, vs_normal), 0, 1);

	return material.diffuse * diffuseIntensity;
}


vec3 calculateSpecular(){

	vec3 lightToPosDirVec = normalize(vs_position - lightPos0);
	vec3 reflectDirVec = normalize(reflect(lightToPosDirVec, normalize(vs_normal)));
	vec3 posToViewDirVec = normalize(cameraPos - vs_position);
	float specularIntensity = pow(max(dot(posToViewDirVec, reflectDirVec), 0), 10);

	return material.specular * specularIntensity * texture(material.textureSpecular, vs_texcoord).rgb;
}

void main(){

	fs_color = texture(material.textureDiffuse, vs_texcoord) * (vec4(calculateAmbient(), 1.f) + vec4(calculateDiffuse(), 1.f) + vec4(calculateSpecular(), 1.f));

}