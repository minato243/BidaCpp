#ifdef GL_ES
varying mediump vec2 v_texture_coord;
#else
varying vec2 v_texture_coord;
#endif
uniform vec4 u_color;

void main(void)
{
    gl_FragColor = texture2D(CC_Texture0, v_texture_coord) * u_color;
}
