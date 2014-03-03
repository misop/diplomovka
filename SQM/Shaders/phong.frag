#version 430

in vec4 vertex_eye;
in vec4 normal_eye;
in vec4 light_eye;

layout (location = 0) out vec4 fColor;

void main(void) {	   
	vec4 diffuse_material = vec4(0.0, 0.75, 0.75, 1);
	vec4 color = vec4(0.0, 0.75, 0.75, 1);

	vec4 V = normalize(vertex_eye);
   	vec4 L = normalize(light_eye);
   	vec4 N = normalize(normal_eye); 

	float diffuse = clamp(dot(L, N), 0.0, 1.0);
   	vec4 R = reflect(-L, N);
   	float specular = sign(diffuse) * pow(clamp(dot(R, V), 0.0, 1.0), 16);
	
	vec4 ambient = vec4(0.2, 0.2, 0.2, 1.0);
	vec4 specularL = vec4(1, 1, 1, 1.0);

	color = 0.2 * (vec4(0.2, 0.2, 0.2, 1.0) + ambient) * (diffuse_material);
	color += diffuse * diffuse_material;
	color += specular * specularL;
	
	fColor = color;
	//fColor = vec4(0.0, 0.75, 0.75, 1);
}
