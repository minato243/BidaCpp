#if GL_ES
precision mediump float;
#endif

varying vec3 v_Lo;
varying vec3 v_specular;
varying vec3 v_diffuse;
varying vec2 v_texture_coord;
varying vec3 v_normal;
varying vec4 v_position;
varying vec3 v_colorMultiplier;

uniform vec3 u_view_pos;
uniform vec4 u_color;
uniform float u_metallic;
uniform float u_roughness;
uniform float u_ao;

uniform float u_grayscale;
uniform float u_opacity;

vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness){
	return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(1.0 - cosTheta, 5.0);
}

vec4 grayscale(vec4 color){
	float g = 0.3 * color.r + 0.59 * color.g + 0.11 * color.b;
	return vec4(g, g, g, color.a);
}

int mod(int a, int b){
	return a - ((a/b) * b);
}

void main(){
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

    float ao = u_ao;
    float metallic = u_metallic;
    float roughness = u_roughness;
    vec3 albedo = texture2D(CC_Texture0, v_texture_coord).rgb;
	albedo = pow(albedo, vec3(2.2));
	
	if(u_grayscale > 0.5){
		albedo = grayscale(vec4(albedo, 1.0)).rgb;
	}

	vec3 N = normalize(v_normal);
	vec3 V = normalize(u_view_pos - v_position.xyz);

	vec3 F0 = vec3(0.04);
	F0 = mix(F0, albedo, metallic);
	// IBL: Calculate diffuse and specular lightning by surround environment.
	vec3 F = fresnelSchlickRoughness(max(dot(N, V), 0.0001), F0, roughness);
	// vec3 F = vec3(1.0);
	vec3 kS = F;
	vec3 kD = vec3(1.0) - kS;
	kD *= 1.0 - metallic;

    vec3 diffuse = v_diffuse * albedo;
    vec3 ambient = (diffuse * kD + v_specular) * ao;
    vec3 color = ambient + v_Lo;
	color = pow(color, vec3(1.0/2.2));
    color *= v_colorMultiplier;

    gl_FragColor = vec4(color,1.0) * u_color;
}