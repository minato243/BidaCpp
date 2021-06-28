attribute vec4 a_position;
attribute vec2 a_texCoord;

uniform float u_width;
uniform float u_height;

#ifdef GL_ES
varying mediump vec4 v_posPos;
#else
varying vec4 v_posPos;
#endif
varying vec2 v_uv;

const float FXAA_SUBPIX_SHIFT = 1.0/4.0;

void main()
{
    gl_Position = CC_PMatrix * a_position;
    v_uv = a_texCoord;
    vec2 rcpFrame = vec2(1.0/u_width, 1.0/u_height);
    v_posPos.xy = a_texCoord.xy;
    v_posPos.zw = a_texCoord.xy - (rcpFrame * (0.5 + FXAA_SUBPIX_SHIFT));
}
