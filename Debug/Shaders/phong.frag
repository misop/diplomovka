#version 430

const float LineWidth = 0.5;
const bool Wireframe = true;

in _{
	vec4 vertex_eye;
	vec4 normal_eye;
	vec4 light_eye;
	vec3 height;
} g;

layout (location = 0) out vec4 fColor;

float evalMinDistanceToEdges(in vec3 height)
{
    float dist;

	vec3 ddxHeights = dFdx(height);
	vec3 ddyHeights = dFdy(height);
	vec3 ddHeights2 =  ddxHeights*ddxHeights + ddyHeights*ddyHeights;
	
    vec3 pixHeights2 = height * height / ddHeights2 ;
    
    dist = sqrt( min ( min (pixHeights2.x, pixHeights2.y), pixHeights2.z) );
    
    return dist;
}

void main(void) {	   
	vec4 diffuse_material = vec4(0.0, 0.75, 0.75, 1);
	vec4 color = vec4(0.0, 0.75, 0.75, 1);

	float alpha = 1;

	if (Wireframe) {	
		float dist = evalMinDistanceToEdges(height);

		if (dist > 0.5*LineWidth+1) discard;
		// Map the computed distance to the [0,2] range on the border of the line.
		dist = clamp((dist - (0.5*LineWidth - 1)), 0, 2);
		// Alpha is computed from the function exp2(-2(x)^2).
		dist *= dist;
		float alpha = exp2(-2.0*dist);
	}

	vec4 V = normalize(g.vertex_eye);
   	vec4 L = normalize(g.light_eye);
   	vec4 N = normalize(g.normal_eye); 

	float diffuse = clamp(dot(L, N), 0.0, 1.0);
   	vec4 R = reflect(-L, N);
   	float specular = sign(diffuse) * pow(clamp(dot(R, V), 0.0, 1.0), 16);
	
	vec4 ambient = vec4(0.2, 0.2, 0.2, 1.0);
	vec4 specularL = vec4(1, 1, 1, 1.0);

	color = 0.2 * (vec4(0.2, 0.2, 0.2, 1.0) + ambient) * (diffuse_material);
	color += diffuse * diffuse_material;
	color += specular * specularL;	
	color.a *= alpha;

	fColor = color;
}