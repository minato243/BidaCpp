#ifdef GL_ES
#ifdef GL_FRAGMENT_PRECISION_HIGH
precision highp float;
#endif
#endif

// #define BALLSHADOW
// Values from vertex shader
// position: model position
varying vec4 v_position;
varying vec2 v_texture_coord;
varying vec3 v_normal;


uniform vec3 u_light_color;

uniform vec3 u_light_pos_0;
uniform vec3 u_light_pos_1;
uniform vec3 u_light_pos_2;
uniform vec3 u_light_pos_3;
uniform vec3 u_light_pos_4;
uniform vec3 u_light_pos_5;

uniform int u_light_count;
uniform vec3 u_view_pos;
uniform vec4 u_color;

#ifdef BALLSHADOW
uniform float u_ball_radius;
uniform vec3 ballPos[18];
uniform float ballEnabled[18];
#endif

uniform sampler2D u_mrao_map;
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
#ifdef BALLSHADOW
float distanceToLine(vec3 p, vec3 start, vec3 end){
	vec3 u = normalize(end - start);
	vec3 ap = p - start;
	vec3 project = start + dot(u, ap) * u;
	return length(p - project);
}
#endif

vec3 CalculatePlaneIntersection(vec3 viewPosition, vec3 reflection, vec3 lightDir, vec3 rectangleCenter){
	return viewPosition + reflection * (dot(lightDir, viewPosition - rectangleCenter)/dot(-reflection, lightDir));
}

void main(void)
{
	vec3 position = v_position.xyz;
	vec3 albedo = texture2D(CC_Texture0, v_texture_coord).rgb;
	albedo = pow(albedo, vec3(2.2));
	float metallic = texture2D(u_mrao_map, v_texture_coord).r;
	float roughness = texture2D(u_mrao_map, v_texture_coord).g;
	float ao = texture2D(u_mrao_map, v_texture_coord).b;

	vec3 V = normalize(u_view_pos - position);
	// for matte feeling, use cheap functions
	// #ifndef GL_ES
	// const float a = 2.5; 
	// const float c = 2.5;
	vec3 N = normalize(v_normal);
	// #else
	// const float a = 2.5; 
	// const float c = 2.5;
	
	// float xx = a/c*abs(fract((position.x + position.z)) - 0.5);
	// float zz = a/c*abs(fract((position.z - position.x)) - 0.5);
	// // vec3 N = normalize(v_normal);
	// vec3 N = normalize(vec3(xx*zz, -xx*zz, -1.0)); 
	// if(dot(N, v_normal) < 0.0) N *= -1.0;
	
	// float kNormal = dot(u_view_pos - (CC_MVMatrix * v_position).xyz, u_view_pos - (CC_MVMatrix * v_position).xyz);
	// float b = 150.0;
	// kNormal /= b * b;
	// kNormal = clamp(kNormal, 0.0, 1.0);
	// N = normalize((1.0-kNormal) * N + kNormal*normalize(v_normal));
	// // N = normalize(v_normal);
	// #endif

	float NdotV = max(dot(N, V), 0.0001);
	vec3 r = reflect(-V, N);

	// Reflectance at normal incidence; if dia-electric (plastic) use F0 = 0.04
	// If it's metal, use the albedo color 
	vec3 F0 = vec3(0.04);
	F0 = mix(F0, albedo, metallic);

	vec3 Lo = vec3(0.0);
	
	// u_light_count = 1;
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
		
		#define POINTLIGHT 
		// cur_light = vec3(0,200,0);
		vec3 light = cur_light - position;
		#ifdef POINTLIGHT
		vec3 L = normalize(light);

		float distance = length(light)/4000.0;
		distance = min(distance, 40.0);
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
		float distance = length(closestPoint)/600.0;
		#endif

		#ifdef RECTLIGHT
		vec3 right = vec3(1.0, 0.0, 0.0);
		vec3 up = vec3(0.0, 0.0, 1.0);
		const float width = 20.0;
		const float height = 80.0;
		const float luminosity = 3.0;

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
		float dist2 = dot(position - cur_light, position - cur_light)/(2020.0*2020.0);

		// clamp in (0.02, 1.0) to make the light source not disappear when too far (quick trick in this scenario)
		// float falloff = 1.0 - clamp(dist2, 0.3, 1.0 * 0.98);
		float falloff = 1.0/dist2;
		
		// vec3 c = vec3((nearest2DPoint.y + width) / (2.0*width));
		// if(c.x > 1.0) c = vec3(1.0, 0.0, 0.0);
		// if(c.x < 0.0) c = vec3(0.0, 0.0, 1.0);
		// gl_FragColor = vec4(vec3(falloff), 1.0);
		// return;
		vec3 L = normalize(nearestPoint - position);
		#endif

		vec3 H = normalize(V + L);
		#ifndef RECTLIGHT
		float attenuation = 1.0/distance/distance;
		vec3 radiance = u_light_color * attenuation;
		#endif


		vec3 F = fresnelSchlick(max(dot(H, V), 0.001), F0);


		float NDF = DistributionGGX(N, H, roughness);
		float G = GeometrySmith(N, V, L, roughness);

		vec3 numerator = NDF * G * F;
		float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0);
		vec3 specular = numerator / max(denominator, 0.001);


		vec3 kS = F;
		vec3 kD = vec3(1.0) - kS;
		kD *= 1.0 - metallic;

		#ifdef BALLSHADOW
		const float epsilon = 0.2;
		const float shadowWeight = 0.3;
		const float revWeight = 1-shadowWeight;
		float thisao = 1.0;
		for(int i=0;i<18;i++){
			if(ballEnabled[i] < 0.5) continue;
			// gl_FragColor = vec4(vec3(distanceToLine(ballPos[i], cur_light, position)/100.0/u_ball_radius), 1.0);
			// return;
			float d = distanceToLine(ballPos[i], cur_light, position);
			float k = sqrt(dot(ballPos[i] - position, ballPos[i] - position) - d * d);
			d = d - u_ball_radius;
			k = clamp(revWeight*d/(k * epsilon), 0.0, revWeight);
			// gl_FragColor = vec4(vec3(k), 1.0);
			thisao *= shadowWeight + k;
			// return;
			if(thisao < shadowWeight){ 
				thisao = shadowWeight;
				break;
			}
		}

		ao *= thisao;
		#endif
		
		#if defined TUBELIGHT || defined RECTLIGHT
		float NdotL = NoL;
		#ifdef RECTLIGHT
		// float specularAmount = dot(r, vec3(0.0, 1.0, 0.0));
		// if(specularAmount > 0.0){
		// 	float specFactor = 1.0 - clamp(length(nearest2DPoint - intersectPlanePoint) * pow((1.0-u_roughness), 2.0) * 32.0, 0.0, 1.0);
		// 	specular = specular * specFactor * specularAmount * NoL;
		// }
		#endif
		#else
		float NdotL = max(dot(N, L), 0.0);
		#endif

		#ifdef RECTLIGHT
		Lo += (specular + kD * albedo / PI) * u_light_color * falloff * NoL * luminosity;
		#else
		Lo += (kD * albedo / PI + specular) * radiance * NdotL;
		#endif
	}

	vec3 ambient = vec3(0.03) * albedo * ao;
	// vec3 ambient = (diffuse*kD + specular) * ao;
	// vec3 color = ambient + Lo * max(ao, 0.5);
	vec3 color = ambient * Lo;

	color = color / (color + vec3(1.0));
	color = pow(color, vec3(1.0/2.2));

	gl_FragColor = vec4(color, 1.0) * u_color;
}
