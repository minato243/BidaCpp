#ifdef GL_ES
precision mediump float;
#endif

varying vec2 v_uv;
uniform float u_grayscale;

void main(){
    vec4 color = texture2D(CC_Texture0, v_uv);
    if(u_grayscale > 0.5){
        float grayColor = dot(color.rgb, vec3(0.299, 0.587, 0.113));
        color = vec4(vec3(grayColor), color.a);
    }
    gl_FragColor = color;
}