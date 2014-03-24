#version 430

const float LineWidth = 0.01;

layout(location = 0) in vec4 vertex_eye;
layout(location = 1) in vec4 normal_eye;
layout(location = 2) in vec4 light_eye;
layout(location = 5) in vec2 uv;
layout(location = 6) in vec3 height;

layout(binding=0) uniform sampler2D DiffuseSampler;

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
	vec4 diffuse_material = texture(DiffuseSampler, uv);
	vec4 color = vec4(0.0, 0.75, 0.75, 1);
	
	float dist = evalMinDistanceToEdges(height);
	
	if (dist > 0.5*LineWidth+1) discard;
	// Map the computed distance to the [0,2] range on the border of the line.
	dist = clamp((dist - (0.5*LineWidth - 1)), 0, 2);
	// Alpha is computed from the function exp2(-2(x)^2).
	dist *= dist;
	float alpha = exp2(-2.0*dist);

	vec4 V = normalize(vertex_eye);
   	vec4 L = normalize(light_eye);
   	vec4 N = normalize(normal_eye); 
   	//vec4 H = normalize(L + V); 
	

	//float diffuse = clamp(abs(dot(L, N)), 0.0, 1.0);
	float diffuse = clamp(dot(L, N), 0.0, 1.0);
	
	vec4 ambient = vec4(0.2, 0.2, 0.2, 1.0);

	color = 0.2 * (vec4(0.2, 0.2, 0.2, 1.0) + ambient) * (diffuse_material);
	color += diffuse * diffuse_material;
	color.a *= alpha;

	fColor = color;
}