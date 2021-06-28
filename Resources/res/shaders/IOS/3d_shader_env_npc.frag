// #version 130
#ifdef GL_ES
#ifdef GL_FRAGMENT_PRECISION_HIGH
precision highp float;
#endif
#endif

varying vec2 v_texture_coord;
varying vec3 v_normal;
varying vec4 v_position;

uniform vec3 u_light_color;

uniform vec3 u_light_pos_0;
uniform vec3 u_light_pos_1;
uniform vec3 u_light_pos_2;
uniform vec3 u_light_pos_3;
uniform vec3 u_light_pos_4;
uniform vec3 u_light_pos_5;

// uniform float u_lod_optimize;

uniform int u_light_count;
uniform vec3 u_view_pos;
uniform vec4 u_color;

uniform samplerCube u_irradiance_map;
uniform samplerCube u_env_map;
uniform sampler2D u_brdf_lut;
// uniform sampler2D u_mrao_map;

// uniform vec3 u_albedo;
// uniform float u_metallic;
// uniform float u_roughness;
// uniform float u_ao;


const float PI = 3.14159265359;


vec3 fresnelSchlick(float cosTheta, vec3 F0){
	return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness){
	return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(1.0 - cosTheta, 5.0);
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

// use for rectangle lightsource but unfortunately there is too much calculating in some point => DEPRECATE
float rectangleSolidAngle(vec3 position, vec3 p0, vec3 p1, vec3 p2, vec3 p3){
	vec3 v0 = p0 - position;
	vec3 v1 = p1 - position;
	vec3 v2 = p2 - position;
	vec3 v3 = p3 - position;

	vec3 n0 = normalize(cross(v0, v1));
	vec3 n1 = normalize(cross(v1, v2));
	vec3 n2 = normalize(cross(v2, v3));
	vec3 n3 = normalize(cross(v3, v0));

	float g0 = acos(dot(-n0, n1));
	float g1 = acos(dot(-n1, n2));
	float g2 = acos(dot(-n2, n3));
	float g3 = acos(dot(-n3, n0));

	return g0 + g1 + g2 + g3 - 2.0 * PI;
}

vec3 CalculatePlaneIntersection(vec3 viewPosition, vec3 reflection, vec3 lightDir, vec3 rectangleCenter){
	return viewPosition + reflection * (dot(lightDir, viewPosition - rectangleCenter)/dot(-reflection, lightDir));
}

// Tex4 always the (blurred) convolution map
vec4 textureCubeLod(samplerCube tex, samplerCube tex2, vec3 uvw, float lod){
	return mix(textureCube(tex, uvw, 5.0), textureCube(tex2, uvw, 5.0), fract(lod/2.0));
}

void main(void)
{
	vec3 albedo = texture2D(CC_Texture0, v_texture_coord).rgb;
	// vec3 albedo = vec3(0.57, 1.00, 0.52);
	albedo = pow(albedo, vec3(2.2));

	// albedo = vec3(pow(albedo.r, 2.2), pow(albedo.g, 2.2), pow(albedo.b, 2.2));
	// float metallic = texture2D(u_mrao_map, v_texture_coord).r;
	// float roughness = texture2D(u_mrao_map, v_texture_coord).g;
	// float ao = texture2D(u_mrao_map, v_texture_coord).b;
	float metallic = 0.0;
	float roughness = 0.99;
	float ao = 0.2;

	vec3 N = normalize(v_normal);
	vec3 V = normalize(u_view_pos - v_position.xyz);
	float NdotV = max(dot(N, V), 0.001);

	vec3 r = reflect(-V, N);

	// Reflectance at normal incidence; if dia-electric (plastic) use F0 = 0.04
	// If it's metal, use the albedo color 
	vec3 F0 = vec3(0.04);
	F0 = mix(F0, albedo, metallic);

	vec3 Lo = vec3(0.0);
	vec3 position = v_position.xyz;
	
	// // u_light_count = 0;
	for(int i = 0 ; i < u_light_count; i++){
		// Light source
		vec3 cur_light = u_light_pos_0;
		
		if(i == 1){
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
		vec3 light = cur_light - position;

		vec3 L = normalize(light);

		float distance = length(light)/400.0;

		vec3 H = normalize(V + L);
		float attenuation = 1.0/distance/distance;
		vec3 radiance = u_light_color * attenuation;

		vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);


		float NDF = DistributionGGX(N, H, roughness);
		float G = GeometrySmith(N, V, L, roughness);

		vec3 numerator = NDF * G * F;
		float denominator = 2.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0);
		vec3 specular = numerator / max(denominator, 0.001);


		vec3 kS = F;
		vec3 kD = vec3(1.0) - kS;
		kD *= 1.0 - metallic;

		float NdotL = max(dot(N, L), 0.0);
		specular = clamp(specular, 0.0, 1.5);

		Lo += (kD * albedo / PI + specular) * radiance * NdotL;
	}

	// IBL: Calculate diffuse and specular lightning by surround environment.
	vec3 F = fresnelSchlickRoughness(max(dot(N, V), 0.001), F0, roughness);
	// vec3 F = fresnelSchlickRoughness(NdotV, F0, roughness);
	vec3 kS = F;
	vec3 kD = 1.0 - kS;
	kD *= 1.0 - metallic;

	vec3 diffuse = textureCube(u_env_map, N).rgb * albedo;

	const float MAX_REFLECTION_LOD = 2.0;
	// vec3 vector_view = u_view_pos - (CC_MVMatrix * v_position).xyz;
	// // Magic number 80 is threshold for "blur" reflection to optimize
	// const float view_threshold = 80.0 * 80.0;
	// if(dot(vector_view, vector_view) > view_threshold || u_roughness > 0.5){
	// 	prefilteredColor = textureCube(u_env_map, normalize(reflect(-V, N)).xyz).rgb;
	// }
	// else{
		// prefilteredColor = texCubemapLod(normalize(reflect(-V, N)).xyz, u_roughness * MAX_REFLECTION_LOD).rgb;
	vec3 prefilteredColor = textureCubeLod(u_irradiance_map, u_env_map, normalize(r).xyz, roughness * MAX_REFLECTION_LOD).rgb;
	// }

	vec2 brdf = texture2D(u_brdf_lut, vec2(NdotV, roughness)).rg;
	vec3 specular = prefilteredColor * (F * brdf.x + brdf.y);

	vec3 ambient = (diffuse * kD + specular) * ao * (max(dot(N, vec3(0.0, 1.0, 0.0)), 0.0) * 0.3 + 0.7) ;
	vec3 color = ambient + Lo;
	// vec3 color = Lo;

	color = color / (color + vec3(1.0));
	color = pow(color, vec3(1.0/2.2));


	gl_FragColor = vec4(color, 1.0) * u_color;
}