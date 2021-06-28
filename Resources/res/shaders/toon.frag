#ifdef GL_ES
precision mediump float;
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
uniform float u_glossiness;
uniform float u_rimAmount;
uniform vec3 u_ambient;
uniform vec3 u_view_pos;
uniform vec4 u_color;

const float PI = 3.14159265359;

float smooth(float position, float range){
    if(position < 0.0) return 0.0;
    if(position > range) return 1.0;

    float x = position/range;

    return x < 0.5 ? 4.0 * x * x * x : 1.0 - pow(-2.0 * x + 2.0, 3.0) / 2.0;
}

// val < low return 0
// val > high return 1
// return range inside [low/high]
float smoothstep(float low, float high, float val){
    return smooth(val-low, high - low);
}

void main(){
    // gl_FragColor = vec4(1.0,0.0,0.0,1.0) * u_color;
    // return;
    vec3 albedo = texture2D(CC_Texture0, v_texture_coord).rgb;
    // albedo = vec3(0.00, 0.65, 0.01);
    albedo = pow(albedo, vec3(2.2));
    vec3 N = normalize(v_normal);
    vec3 V = normalize(u_view_pos - v_position);

    vec3 L = normalize(u_light_pos_0 - v_position);
    float NoL = max(dot(L, N), 0.0);
    NoL = smoothstep(0.0, 0.5, NoL);

    vec3 ambient = u_ambient;

    vec3 H = normalize(L + V);
    float NoH = max(dot(N, H), 0.0);
    float glossiness = u_glossiness;
    float specularIntensity = pow(NoH * 1.0, glossiness * glossiness);
    specularIntensity = smoothstep(0.005, 0.01, specularIntensity) * 0.7 + 0.3;

    float rimDot = pow(NoL, 0.01) * (1.0 - dot(V, N));
    float rimAmount = u_rimAmount;
    rimDot = smoothstep(rimAmount - 0.05, rimAmount + 0.05, rimDot);

    vec3 color = albedo * (NoL + ambient + specularIntensity + rimDot) * u_light_color;

	color = color / (color + vec3(1.0));
	color = pow(color, vec3(1.0/2.2));

    gl_FragColor = vec4(color, 1.0) * u_color;
}