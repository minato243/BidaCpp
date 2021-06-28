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
	return mix(textureCube(tex, uvw), textureCube(tex2, uvw), fract(lod/2.0));
}

void main(void)
{
	vec3 albedo = texture2D(CC_Texture0, v_texture_coord).rgb;
	// vec3 albedo = vec3(0.57, 1.00, 0.52);
	albedo = pow(albedo, vec3(2.2));
	// albedo = vec3(pow(albedo.r, 2.2), pow(albedo.g, 2.2), pow(albedo.b, 2.2));
	vec3 N = normalize(v_normal);
	vec3 V = normalize(u_view_pos - v_position.xyz);
	float NdotV = max(dot(N, V), 0.001);

	vec3 r = reflect(-V, N);

	// Reflectance at normal incidence; if dia-electric (plastic) use F0 = 0.04
	// If it's metal, use the albedo color 
	vec3 F0 = vec3(0.04);
	F0 = mix(F0, albedo, u_metallic);

	vec3 Lo = vec3(0.0);
	vec3 position = v_position.xyz;
	
	// // u_light_count = 0;
	for(int i = 0 ; i < u_light_count ; i++){
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
		#define RECTLIGHT 

		#ifdef RECTLIGHT
		// Too far, use TUBE LIGHT
		if(length(u_view_pos - v_position.xyz) > 150.0){
			const float tubeLength = 60.0;
			const float tubeRadius = 5.0;
			const vec3 tubeDir = vec3(0.0,0.0,1.0);
			vec3 l0 = light + tubeDir*tubeLength;
			vec3 l1 = light - tubeDir*tubeLength;
			float distL0 = length(l0)/600.0;
			float distL1 = length(l1)/600.0;
			vec3 nl0 = normalize(l0); 
			vec3 nl1 = normalize(l1);

			float NoL0 = dot(nl0, N)/(2.0*distL0);
			float NoL1 = dot(nl1, N)/(2.0*distL1);
			float NoL = (2.0*clamp(NoL0 + NoL1, 0.0, 1.0))/(distL0 * distL1 + dot(nl0, nl1) + 2.0);
			// gl_FragColor = vec4(vec3(NoL), 1.0);
			// return;
			vec3 Ldist = l1 - l0;
			float RoLd = dot(r,Ldist);
			float distLd = length(Ldist);
			float t = (dot(r,l0) * RoLd - dot(l0,Ldist))/(distLd * distLd - RoLd * RoLd);

			vec3 closestPoint = l0 + Ldist*clamp(t, 0.0, 1.0);
			vec3 centerToRay = dot(closestPoint, r) * r - closestPoint;
			closestPoint = closestPoint + centerToRay * clamp(tubeRadius/length(centerToRay), 0.0, 1.0);
			vec3 L = normalize(closestPoint);
			float distance = length(closestPoint)/400.0;
			vec3 H = normalize(V + L);
			
			float attenuation = 1.0/distance/distance;
			vec3 radiance = u_light_color * attenuation;
			
			vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);


			float NDF = DistributionGGX(N, H, u_roughness);
			float G = GeometrySmith(N, V, L, u_roughness);

			vec3 numerator = NDF * G * F;
			float denominator = 2.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0);
			vec3 specular = numerator / max(denominator, 0.001);

			vec3 kS = F;
			vec3 kD = vec3(1.0) - kS;
			kD *= 1.0 - u_metallic;
			float NdotL = NoL;
			specular = clamp(specular, 0.0, 1.5);
	
			Lo += (kD * albedo / PI + specular) * radiance * NdotL;
		}
		else{
			vec3 right = vec3(1.0, 0.0, 0.0);
			vec3 up = vec3(0.0, 0.0, 1.0);
			const float width = 20.0;
			const float height = 80.0;
			const float luminosity = 4.0;

			vec3 lightPos1 = cur_light + right * width + up * height;
			vec3 lightPos2 = cur_light - right * width + up * height;
			vec3 lightPos3 = cur_light - right * width - up * height;
			vec3 lightPos4 = cur_light + right * width - up * height;

			vec3 v0 = lightPos1 - position;
			vec3 v1 = lightPos2 - position;
			vec3 v2 = lightPos3 - position;
			vec3 v3 = lightPos4 - position;

			vec3 n0 = normalize(cross(v0, v1));
			vec3 n1 = normalize(cross(v1, v2));
			vec3 n2 = normalize(cross(v2, v3));
			vec3 n3 = normalize(cross(v3, v0));
			float g0 = acos(dot(-n0, n1));
			float g1 = acos(dot(-n1, n2));
			float g2 = acos(dot(-n2, n3));
			float g3 = acos(dot(-n3, n0));

			float solidAngle = g0 + g1 + g2 + g3 - 2.0 * PI;
			// Diffuse N dot L
			float NoL = 0.2 * (
				clamp(dot(normalize(v0), N), 0.0, 1.0) +  
				clamp(dot(normalize(v1), N), 0.0, 1.0) +  
				clamp(dot(normalize(v2), N), 0.0, 1.0) +  
				clamp(dot(normalize(v3), N), 0.0, 1.0) +  
				clamp(dot(normalize(cur_light - position), N), 0.0, 1.0)
				);

			// Nearest point => representative point
			vec3 intersectPoint = CalculatePlaneIntersection(position, r, vec3(0.0, 1.0, 0.0), cur_light);
			vec3 intersectionVector = intersectPoint - cur_light;
			vec2 intersectPlanePoint = vec2(dot(intersectionVector, right), dot(intersectionVector, up));
			vec2 nearest2DPoint = vec2(clamp(intersectPlanePoint.x, -width, width), clamp(intersectPlanePoint.y, -height, height));

			vec3 nearestPoint = cur_light + (right * nearest2DPoint.x + up * nearest2DPoint.y);
			// float dist2 = dot(position - cur_light, position - cur_light)/(320.0*320.0);
			float dist2 = dot(position - cur_light, position - cur_light)/(220.0*220.0);

			float falloff = 1.0/dist2;

			vec3 L = normalize(nearestPoint - position);
			vec3 H = normalize(V + L);

			vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);

			float NDF = DistributionGGX(N, H, u_roughness);
			float G = GeometrySmith(N, V, L, u_roughness);

			vec3 numerator = NDF * G * F;
			float denominator = 2.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0);
			vec3 specular = numerator / max(denominator, 0.001);


			vec3 kS = F;
			vec3 kD = vec3(1.0) - kS;
			kD *= 1.0 - u_metallic;

			specular = clamp(specular, 0.0, 1.5);
			Lo += (specular + kD * albedo / PI) * u_light_color * falloff * NoL * luminosity;
		}

		#else
		#ifdef POINTLIGHT
		vec3 L = normalize(light);

		float distance = length(light)/400.0;
		#endif
		
		#ifdef SPHERELIGHT
		const float lightRadius = 20.0;
		vec3 centerToRay = (dot(light, r) * r) - light;
		vec3 closestPoint = light + centerToRay*clamp(lightRadius / length(centerToRay), 0.0, 1.0);
		vec3 L = normalize(closestPoint);

		// square distance for performance
		// float distance2 = dot(light/300.0, light/300.0);
		float distance = length(closestPoint)/300.0;
		#endif

		#ifdef TUBELIGHT
		const float tubeLength = 60.0;
		const float tubeRadius = 5.0;
		const vec3 tubeDir = vec3(0.0,0.0,1.0);
		vec3 l0 = light + tubeDir*tubeLength;
		vec3 l1 = light - tubeDir*tubeLength;
		float distL0 = length(l0)/600.0;
		float distL1 = length(l1)/600.0;
		vec3 nl0 = normalize(l0); 
		vec3 nl1 = normalize(l1);

		float NoL0 = dot(nl0, N)/(2.0*distL0);
		float NoL1 = dot(nl1, N)/(2.0*distL1);
		float NoL = (2.0*clamp(NoL0 + NoL1, 0.0, 1.0))/(distL0 * distL1 + dot(nl0, nl1) + 2.0);
		// gl_FragColor = vec4(vec3(NoL), 1.0);
		// return;
		vec3 Ldist = l1 - l0;
		float RoLd = dot(r,Ldist);
		float distLd = length(Ldist);
		float t = (dot(r,l0) * RoLd - dot(l0,Ldist))/(distLd * distLd - RoLd * RoLd);

		vec3 closestPoint = l0 + Ldist*clamp(t, 0.0, 1.0);
		vec3 centerToRay = dot(closestPoint, r) * r - closestPoint;
		closestPoint = closestPoint + centerToRay * clamp(tubeRadius/length(centerToRay), 0.0, 1.0);
		vec3 L = normalize(closestPoint);
		float distance = length(closestPoint)/400.0;
		#endif

		vec3 H = normalize(V + L);
		float attenuation = 1.0/distance/distance;
		vec3 radiance = u_light_color * attenuation;

		vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);


		float NDF = DistributionGGX(N, H, u_roughness);
		float G = GeometrySmith(N, V, L, u_roughness);

		vec3 numerator = NDF * G * F;
		float denominator = 2.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0);
		vec3 specular = numerator / max(denominator, 0.001);


		vec3 kS = F;
		vec3 kD = vec3(1.0) - kS;
		kD *= 1.0 - u_metallic;

		#if defined TUBELIGHT
		float NdotL = NoL;
		#else
		float NdotL = max(dot(N, L), 0.0);
		#endif

		specular = clamp(specular, 0.0, 1.5);

		Lo += (kD * albedo / PI + specular) * radiance * NdotL;
		#endif
	}

	#ifndef NOIBL
	// IBL: Calculate diffuse and specular lightning by surround environment.
	vec3 F = fresnelSchlickRoughness(max(dot(N, V), 0.001), F0, u_roughness);
	// vec3 F = fresnelSchlickRoughness(NdotV, F0, u_roughness);
	vec3 kS = F;
	vec3 kD = 1.0 - kS;
	kD *= 1.0 - u_metallic;

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
	vec3 prefilteredColor = textureCubeLod(u_irradiance_map, u_env_map, normalize(r).xyz, u_roughness * MAX_REFLECTION_LOD).rgb;
	// }

	vec2 brdf = texture2D(u_brdf_lut, vec2(NdotV, u_roughness)).rg;
	vec3 specular = prefilteredColor * (F * brdf.x + brdf.y);

	vec3 ambient = (diffuse * kD + specular) * u_ao * (max(dot(N, vec3(0.0, 1.0, 0.0)), 0.0) * 0.3 + 0.7) ;
	#else
	vec3 ambient = vec3(0.06) * albedo * u_ao;
	#endif
	vec3 color = ambient + Lo;
	// vec3 color = Lo;

	color = color / (color + vec3(1.0));
	color = pow(color, vec3(1.0/2.2));


	gl_FragColor = vec4(color, 1.0) * u_color;
}