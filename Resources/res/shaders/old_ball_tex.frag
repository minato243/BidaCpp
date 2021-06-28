varying vec2 v_texture_coord;
varying vec3 v_normal;

uniform sampler2D u_sampler0;
uniform vec3 u_light_color;
uniform vec3 u_light_pos;
uniform vec3 u_view_pos;
uniform vec3 u_ambient;

uniform vec3 u_diffuse;

uniform vec3 u_specular;
uniform float shininess;
uniform vec4 u_color;

void main(void)
{

	// gl_FragColor = vec4(v_normal, 1.0);

	vec3 ambient = u_light_color * u_ambient;

	vec3 norm = normalize(v_normal);
	vec3 lightDir = normalize(u_light_pos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = u_light_color * (diff * u_diffuse);

	vec3 viewDir = normalize(u_view_pos - v_normal);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
	vec3 specular = u_light_color * (spec * u_specular);

	vec3 result = ambient + diffuse + specular;

	gl_FragColor = vec4(result, 1.0) * texture2D(u_sampler0, v_texture_coord) * u_color;
}
