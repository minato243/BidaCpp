#ifdef GL_ES
precision mediump float;
#endif

varying vec2 v_texture_coord;
varying vec3 v_normal;
varying vec3 v_position;

uniform vec3 u_light_color;

uniform vec3 u_light_pos_0;
uniform vec3 u_light_pos_1;
uniform vec3 u_light_pos_2;
uniform vec3 u_light_pos_3;
uniform vec3 u_light_pos_4;
uniform vec3 u_light_pos_5;

uniform float u_specular;

uniform int u_light_count;
uniform vec3 u_view_pos;
uniform vec4 u_color;


uniform sampler2D u_albedo_map;
// uniform vec3 u_albedo;
uniform float u_metallic;
uniform float u_roughness;
uniform float u_ao;


const float PI = 3.14159265359;


vec3 fresnelSchlick(float cosTheta, vec3 F0){
	return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

float DistributionGGX(vec3 N, vec3 H, float roughness){
	float a = roughness * roughness;
	float a2 = a * a;
	float NdotH = max(dot(N,H), 0.0);
	float NdotH2 = NdotH*NdotH;

	float num = a2;
	float denom = (NdotH2 * (a2 - 1.0) + 1.0);
	denom = PI * denom * denom;

	return num/denom;
}

float GeometrySchlickGGX(float NdotV, float roughness){
	float r = (roughness + 1.0);
	float k = (r*r) / 8.0;
	float num = NdotV;
	float denom = NdotV * (1.0 - k) + k;

	return num / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness){
	float NdotV = max(dot(N, V), 0.0);
	float NdotL = max(dot(N, L), 0.0);
	float ggx2 = GeometrySchlickGGX(NdotV, roughness);
	float ggx1 = GeometrySchlickGGX(NdotL, roughness);

	return ggx1 * ggx2;
}

void main(void)
{
	vec3 albedo = texture2D(u_albedo_map, v_texture_coord).rgb;
	albedo = vec3(pow(albedo.r, 2.2), pow(albedo.g, 2.2), pow(albedo.b, 2.2));
	vec3 N = normalize(v_normal);
	vec3 V = normalize(u_view_pos - v_position);

	vec3 F0 = vec3(0.04);
	F0 = mix(F0, albedo, u_metallic);

	vec3 Lo = vec3(0.0);
	// vec3 u_light_pos[4] = vec3[4](u_light_pos_0,u_light_pos_1,u_light_pos_2,u_light_pos_3);

	for(int i = 0 ; i < u_light_count ; i++){
		// Light source
		vec3 cur_light;
		if(i == 0){
			cur_light = u_light_pos_0;
		}
		else if(i == 1){
			cur_light = u_light_pos_1;
		}
		else if(i == 2){
			cur_light = u_light_pos_2;
		}
		else if(i == 3){
			cur_light = u_light_pos_3;
		}
		else if(i == 4){
			cur_light = u_light_pos_4;
		}
		else if(i == 5){
			cur_light = u_light_pos_5;
		}
		vec3 L = normalize(cur_light - v_position);
		vec3 H = normalize(V + L);

		float distance = length(cur_light - v_position)/300.0;
		float attenuation = 1.0/(distance*distance);
		vec3 radiance = u_light_color * attenuation;

		vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);


		float NDF = DistributionGGX(N, H, u_roughness);
		float G = GeometrySmith(N, V, L, u_roughness);

		vec3 numerator = NDF * G * F;
		float denominator = u_specular * max(dot(N, V), 0.0) * max(dot(N, L), 0.0);
		vec3 specular = numerator / max(denominator, 0.001);


		vec3 kS = F;
		vec3 kD = vec3(1.0) - kS;


		float NdotL = max(dot(N, L), 0.0);
		Lo += (kD * albedo / PI + specular) * radiance * NdotL;
	}

	vec3 ambient = vec3(0.06) * albedo * u_ao;
	vec3 color = ambient + Lo;

	color = color / (color + vec3(1.0));
	color = pow(color, vec3(1.0/2.2));


	gl_FragColor = vec4(color, 1.0) * u_color;
}