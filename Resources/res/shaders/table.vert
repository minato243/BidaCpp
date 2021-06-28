attribute vec4 a_position;
attribute vec2 a_texCoord;
attribute vec3 a_normal;

attribute vec4 a_blendWeight;
attribute vec4 a_blendIndex;
attribute vec3 a_tangent;
attribute vec3 a_binormal;

varying vec2 v_texture_coord;
varying vec3 v_normal;
varying vec4 v_position;

//uniform mat4 u_ViewMatrix;

void main(void) {
    gl_Position = CC_MVPMatrix * a_position;
    v_position = CC_MVMatrix * a_position;
    v_texture_coord = a_texCoord;
    v_texture_coord.y = 1.0 - v_texture_coord.y;
    v_normal = CC_NormalMatrix * a_normal;
}
