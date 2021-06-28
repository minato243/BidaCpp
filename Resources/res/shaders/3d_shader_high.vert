attribute vec4 a_position;
attribute vec3 a_normal;
attribute vec2 a_texCoord;

varying vec4 v_position;
varying vec2 v_texture_coord;
varying vec3 v_normal;

void main(void)
{
    gl_Position = CC_MVPMatrix * a_position;
    v_texture_coord = a_texCoord;
    v_texture_coord.y = 1.0 - v_texture_coord.y;
    v_position = CC_MVMatrix * a_position;
    v_normal = CC_NormalMatrix * a_normal;
}
