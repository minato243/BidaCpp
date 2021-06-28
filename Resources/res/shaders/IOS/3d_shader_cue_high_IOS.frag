#if (GL_ES && GL_FRAGMENT_PRECISION_HIGH)
precision highp float;
#endif

// #define NOIBL

// Values from vertex shader
// position: world position
// texCoord: nothing special, texture coordianate of cur pixel
// v_normal: nothing special, normal vector at cur pixel
varying vec2 v_texture_coord;
varying vec3 v_normal;
varying vec4 v_position;
uniform vec3 u_light_color;

// Up to 6 light source
uniform vec3 u_light_pos_0;
uniform vec3 u_light_pos_1;
uniform vec3 u_light_pos_2;
uniform vec3 u_light_pos_3;
uniform vec3 u_light_pos_4;
uniform vec3 u_light_pos_5;

// Total lights count
uniform int u_light_count;
// DEPRECATE uniform float u_lod_optimize;
// Camera position (world position)
uniform vec3 u_view_pos;
// Color attribute to mute cocos warnings
uniform vec4 u_color;
uniform float u_opacity;

// Decrease sampler uniforms count
// Albedo can be store in CC_Texture0
// Metallic-Roughness-AO => MRAO map (RGB channels for 3 map)

// uniform sampler2D u_metallic_map;
// uniform sampler2D u_roughness_map;
// uniform sampler2D u_albedo_map;
// uniform sampler2D u_ao_map;
uniform sampler2D u_mrao_map;

// Irradiance map for reflection (so we need cubemap) - Use HDR images
// 2nd one is for mip level (as I couldn't use the LoD level in OpenGL 1.1, please fix this if you can because this take a lot of bandwidth)
uniform samplerCube u_irradiance_map;
// uniform samplerCube u_irradiance_map_2;
// A pretty useless uniform but i put here anyway, decrease this to decrease the max level of mipmap (but current levels = 2 so there is not much meaning)
uniform float u_lod_level;

// Environment map (More like blurry version of irradiance map - use to calc Environment lightning)
uniform samplerCube u_env_map;

// A BRDF LookUpTexture for calculating environment lightning
uniform sampler2D u_brdf_lut;

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
		color = pow(color, vec3(1.0/5.2));
	}
	return color;
}

int mod(int a, int b){
	return a - ((a/b) * b);
}

// Tex4 always the (blurred) convolution map
vec4 textureCubeLod(samplerCube tex, samplerCube tex2, vec3 uvw, float lod){
	vec3 color1 = textureCube(tex, uvw).rgb;
	vec3 color2 = textureCube(tex2, uvw).rgb;
	// color1 = (color1 + vec3(1.0))/color1;
	// color2 = (color2 + vec3(1.0))/color2;
	color1 = hdrTreatment(color1);
	color2 = hdrTreatment(color2);
	float frac = fract(lod) * fract(lod);
	if(lod < 1.0/3.0){
		return vec4(color1, 1.0);
	}
	if(lod < 2.0/3.0)
		return vec4(mix(color1, color2, frac),1.0);
	return vec4(color2,1.0);
}

void main(void)
{
	int posx = int(gl_FragCoord.x);
	int posy = int(gl_FragCoord.y);

	float thresholdMatrix[16];
	thresholdMatrix[0] = 1.0/17.0;
	thresholdMatrix[1] = 9.0/17.0;
	thresholdMatrix[2] = 3.0/17.0;
	thresholdMatrix[3] = 11.0/17.0;
	thresholdMatrix[4] = 13.0/17.0;
	thresholdMatrix[5] = 5.0/17.0;
	thresholdMatrix[6] = 15.0/17.0;
	thresholdMatrix[7] = 7.0/17.0;
	thresholdMatrix[8] = 4.0/17.0;
	thresholdMatrix[9] = 12.0/17.0;
	thresholdMatrix[10] = 2.0/17.0;
	thresholdMatrix[11] = 10.0/17.0;
	thresholdMatrix[12] = 16.0/17.0;
	thresholdMatrix[13] = 8.0/17.0;
	thresholdMatrix[14] = 14.0/17.0;
	thresholdMatrix[15] = 6.0/17.0;
	if(u_opacity < thresholdMatrix[mod(posx, 4) * 4 + mod(posy, 4)]){
		discard;
	}

	vec3 position = v_position.xyz;
	// Get the value from maps
	vec3 albedo = texture2D(CC_Texture0, v_texture_coord).rgb;
	// albedo = vec3(pow(albedo.r, 2.2), pow(albedo.g, 2.2), pow(albedo.b, 2.2));
	float metallic = texture2D(u_mrao_map, v_texture_coord).r;
	float roughness = texture2D(u_mrao_map, v_texture_coord).g;
	float ao = texture2D(u_mrao_map, v_texture_coord).b;

	// Normalize some vectors
	vec3 N = normalize(v_normal);
	vec3 V = normalize(u_view_pos - v_position.xyz);
	// NdotV = cos of (N, V). cos < 0 means the camera can't see this anyway so clamp to zero
	float NdotV = max(dot(N, V), 0.000001);
	// reflection vector
	vec3 r = reflect(-V, N);

	// Reflectance at normal incidence; if dia-electric (plastic) use F0 = 0.04
	// If it's metal, use the albedo color 
	vec3 F0 = vec3(0.04);
	F0 = mix(F0, albedo, metallic);

	// Irradiance term from lightsource, init as zero
	vec3 Lo = vec3(0.0);
	
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

		vec3 light = cur_light - position;
		const float lightRadius = 40.0;
		vec3 centerToRay = (dot(light, r) * r) - light;
		vec3 closestPoint = light + centerToRay*clamp(lightRadius / length(centerToRay), 0.0, 1.0);
		vec3 L = normalize(closestPoint);
		// square distance for performance
		// float distance2 = dot(light/300.0, light/300.0);
		float distance = length(closestPoint)/170.0;

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


		Lo += (kD * albedo / PI + specular) * radiance * NdotL;
	}
	
	// IBL: Calculate diffuse and specular lightning by surround environment.
	vec3 F = fresnelSchlickRoughness(max(dot(N, V), 0.001), F0, roughness);
	// vec3 F = fresnelSchlickRoughness(NdotV, F0, u_roughness);
	vec3 kS = F;
	vec3 kD = vec3(1.0) - kS;
	kD *= 1.0 - metallic;

	vec3 diffuse = (textureCube(u_env_map, N).rgb * 0.4 + 0.6) * albedo;

	vec3 prefilteredColor = textureCubeLod(u_irradiance_map, u_env_map, normalize(r).xyz, roughness).rgb;

	vec2 brdf = texture2D(u_brdf_lut, vec2(NdotV, roughness)).rg;
	float brdfD = 0.6;
	vec3 specular = prefilteredColor * ((F * brdf.x + vec3(brdf.y)) * brdfD + vec3(1.0 - brdfD));

	vec3 ambient = (diffuse * kD + specular) * ao;
	// vec3 ambient = (diffuse * kD + specular ) * u_ao;
	vec3 color = ambient + Lo;
	// vec3 color = ambient;

	// color = color / (color + vec3(1.0));
	// color = pow(color, vec3(1.0/2.2));

	gl_FragColor = vec4(color, 1.0) * u_color;
}
