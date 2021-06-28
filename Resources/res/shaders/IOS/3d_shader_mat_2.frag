#if (GL_ES && GL_FRAGMENT_PRECISION_HIGH)
precision mediump float;
varying highp vec4 v_position;
#else
varying vec4 v_position;
#endif

// Values from vertex shader
// position: model position
// varying vec4 v_position;
varying vec2 v_texture_coord;
varying vec3 v_normal;


uniform vec3 u_light_color;

uniform int u_light_count;
uniform vec3 u_view_pos;
uniform vec4 u_color;

uniform sampler2D u_mrao_map;
const float PI = 3.14159265359;


void main(void)
{
	// vec3 position = v_position.xyz;
	// vec3 albedo = texture2D(CC_Texture0, v_texture_coord).rgb;
	// // albedo = pow(albedo, vec3(2.2));
	// float dX = (190.0 - abs(position.x) + 40.0) / 190.0;
	// float dY = (100.0 - abs(position.z) + 20.0) / 100.0;
	// float attenuation = dX* dY;
	// // float attenuation = 1.0 - (1.0 - dX)*(1.0 - dY);
	// attenuation = min(max(attenuation, 0.4), 1.5);
	// vec3 ambient = vec3(0.08);

	// vec3 color = attenuation * albedo * u_light_color + ambient;

	// gl_FragColor = vec4(color, 1.0) * u_color;
	
	vec3 position = v_position.xyz;
	vec3 albedo = texture2D(CC_Texture0, v_texture_coord).rgb;

	float ez = position.z / 90.0;
	float ex = position.x / 140.0;
	float radius = sqrt(ez * ez + ex * ex);

	float attenuation = 0.8 * (0.4 + 1.5 * (0.9 - 0.64 * radius));
	
	vec3 color = attenuation * albedo;

	gl_FragColor = vec4(color, 1.0) * u_color;
}
