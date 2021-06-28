#ifdef GL_ES
precision mediump float;
#endif

varying vec2 v_uv;

uniform float u_sampleNum;
uniform vec2 u_velocity;
uniform float u_texWidth;

void main(){
    vec4 color = texture2D(CC_Texture0, v_uv);

    // vec2 v = normalize(u_velocity) / u_texWidth;
    vec2 v = vec2(sin(CC_Time[1]*2.0), 0.0) / u_texWidth;
    vec2 vv = v;
    float weight = 0.5;
    for(int i=1;i<u_sampleNum;i++, vv += v){
        weight /= 2.0;
        color += texture2D(CC_Texture0, v_uv + vv);
    }
    color /= u_sampleNum;
    gl_FragColor = color;
}