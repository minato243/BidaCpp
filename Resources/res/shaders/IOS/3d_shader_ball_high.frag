#if (GL_ES && GL_FRAGMENT_PRECISION_HIGH)
precision mediump float;
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


// uniform sampler2D u_albedo_map;
uniform samplerCube u_irradiance_map;
// uniform sampler2D u_irradiance_map_1;
// uniform sampler2D u_irradiance_map_2;
// uniform sampler2D u_irradiance_map_3;
// uniform sampler2D u_irradiance_map_4;
// uniform sampler2D u_irradiance_map_5;
// uniform sampler2D u_irradiance_map_6;

uniform samplerCube u_env_map;
uniform sampler2D u_brdf_lut;
// uniform vec3 u_albedo;
uniform float u_metallic;
uniform float u_roughness;
uniform float u_ao;

uniform float u_grayscale;

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

vec3 hdrTreatment(vec3 color){
	if(color.r + color.g + color.b > 2.0){
		color = pow(color, vec3(2.2));
	}
	return color;
}

// Tex4 always the (blurred) convolution map
vec4 textureCubeLod(samplerCube tex, samplerCube tex2, vec3 uvw, float lod){
	vec3 color1 = textureCube(tex, uvw).rgb;
	vec3 color2 = textureCube(tex2, uvw).rgb;
	// color1 = (color1 + vec3(1.0))/color1;
	// color2 = (color2 + vec3(1.0))/color2;
	// color1 = hdrTreatment(color1);
	// color2 = hdrTreatment(color2);
	float frac = fract(lod) * fract(lod);
	if(lod < 1.0/3.0){
		return vec4(color1, 1.0);
	}
	if(lod < 2.0/3.0)
		return vec4(mix(color1, color2, frac), 1.0);
	return vec4(color2,1.0);
}

vec4 grayscale(vec4 color){
	float g = 0.3 * color.r + 0.59 * color.g + 0.11 * color.b;
	return vec4(g, g, g, color.a);
}

void main(void)
{
	vec3 position = v_position.xyz;
	vec3 albedo = texture2D(CC_Texture0, v_texture_coord).rgb;
	// albedo = vec3(0.0, 0.0, 0.0);
	vec3 N = normalize(v_normal);
	vec3 V = normalize(u_view_pos - v_position.xyz);

	float metallic = u_metallic;
	float roughness = u_roughness;
	float ao = u_ao;

	albedo = pow(albedo, vec3(2.2));
	
	if(u_grayscale > 0.5){
		albedo = grayscale(vec4(albedo, 1.0)).rgb;
	}

	float NdotV = max(dot(N, V), 0.001);

	vec3 r = reflect(-V, N);

	// Reflectance at normal incidence; if dia-electric (plastic) use F0 = 0.04
	// If it's metal, use the albedo color 
	vec3 F0 = vec3(0.04);
	F0 = mix(F0, albedo, metallic);

	// vec3 Lo = vec3((dot(N, vec3(0.0, 1.0, 0.0)) * 0.99));
	// if(Lo.r > 0) Lo *= 0.1/0.99;
	vec3 Lo = vec3(0.0);
	vec3 lightAreak = vec3(0.0);

	for(int i = 0; i < u_light_count; i = i + 1)
	{
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

		// square distance for performance
		// float distance2 = dot(light/300.0, light/300.0);
		
		vec3 light = cur_light - position;
		const float lightRadius = 40.0;
		vec3 centerToRay = (dot(light, r) * r) - light;
		vec3 closestPoint = light + centerToRay*clamp(lightRadius / length(centerToRay), 0.0, 1.0);
		vec3 L = normalize(closestPoint);

		// square distance for performance
		float distance = length(closestPoint)/130.0;

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

		vec3 fresnel = vec3(max(0.0, min(1.0, 0.63 + 0.15 * pow(1.0 + NdotV, 2.3))));

		specular = clamp(specular, 0.0, 2.5);
		Lo += mix(u_light_color * NdotL, (kD * albedo / PI + specular) * radiance * NdotL, fresnel);

		lightAreak += dot(N, L);
	}

	// IBL: Calculate diffuse and specular lightning by surround environment.
	vec3 F = fresnelSchlickRoughness(max(dot(N, V), 0.001), F0, roughness);
	// vec3 F = vec3(1.0);
	vec3 kS = F;
	vec3 kD = vec3(1.0) - kS;
	kD *= 1.0 - metallic;

	vec3 diffuse = (vec3(0.45) + 0.55 * textureCube(u_env_map, N).rgb) * albedo;

	vec3 prefilteredColor = textureCubeLod(u_irradiance_map, u_env_map, normalize(r).xyz, roughness).rgb;

	vec2 brdf = texture2D(u_brdf_lut, vec2(NdotV, roughness)).rg;
	float brdfD = 0.7;
	vec3 specular = prefilteredColor * ((F * brdf.x + vec3(brdf.y)) * brdfD + vec3(1.0 - brdfD));

	vec3 ambient = (diffuse * kD + specular) * ao;
	// vec3 ambient = (diffuse * kD + specular ) * ao;
	vec3 color = ambient + Lo;
	// vec3 color = ambient;


	// color = color / (color + vec3(1.0));
	float fLightCount = float(u_light_count);
	color = pow(color, vec3(1.0/2.2));
	color *= 0.9 * (lightAreak + fLightCount) / (2.0 * fLightCount) + 0.1;

	gl_FragColor = vec4(color, 1.0) * u_color;
}