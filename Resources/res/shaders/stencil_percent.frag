#ifdef GL_ES
precision mediump float;
#endif

varying vec2 v_uv;

uniform vec4 u_color;
uniform float u_percent;

void main(){
    vec4 color = texture2D(CC_Texture0, v_uv);
    if(v_uv.x > 0.5){
        color = vec4(1.0, 0.0, 0.0, 1.0);
    }
    gl_FragColor = vec4(color);
}