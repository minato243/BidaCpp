#ifdef GL_ES
precision mediump float;
#endif

varying vec2 v_texCoord;
varying vec3 v_LoSpecular;
varying vec3 v_LoDiffuse;
varying vec3 v_ambientDiffuse;
varying vec3 v_ambientSpecular;

uniform vec4 u_color;

void main(){
    vec3 albedo = texture2D(CC_Texture0, v_texCoord).rgb;
    vec3 Lo = v_LoSpecular + v_LoDiffuse*albedo;
    vec3 ambient = v_ambientSpecular + v_ambientDiffuse * albedo;
    
	// Last color = environmentColor value + lightSourceColor value
	vec3 color = ambient + Lo;

	// Gamma correction to prevent color being clamp from (higher than 1) to 1 => which cause incorrect result
	color = color / (color + vec3(1.0));
	color = pow(color, vec3(1.0/2.2));

    gl_FragColor = vec4(color, 1.0) * u_color;
}