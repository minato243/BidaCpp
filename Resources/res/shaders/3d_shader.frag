varying vec2 v_texture_coord;

uniform vec4 u_color;

void main(void)
{
	// gl_FragColor = vec4(v_normal, 1.0);
	gl_FragColor = texture2D(CC_Texture0, v_texture_coord) * u_color;
}
