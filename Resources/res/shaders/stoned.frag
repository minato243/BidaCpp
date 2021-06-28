#ifdef GL_ES
varying mediump vec2 v_texture_coord;
#else
varying vec2 v_texture_coord;
#endif

uniform vec4 u_color;
//uniform sampler2D u_sampler0;

void main(void)
{
    vec4 texColor = texture2D(CC_Texture0, v_texture_coord) * u_color;
	float mono = 0.30;
	mono = (texColor.r * 0.2101) + (texColor.g * 0.7101) + (texColor.b * 0.0701);
	texColor.rgb = vec3(mono, mono, mono);
	//vec4 stoneTex = texture2D(u_sampler0, TextureCoordOut);
	gl_FragColor = texColor;
}

