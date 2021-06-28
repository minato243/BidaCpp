#ifdef GL_ES
precision mediump float;
#endif

varying vec2 v_uv;

uniform float u_sampleNum;
// uniform float u_texWidth;

// void main(){
//     vec4 color = texture2D(CC_Texture0, v_uv);
//     vec2 blurPower = pow(vec2(2.0 * (v_uv - vec2(0.5))), vec2(2.0)) * (sin(CC_Time[1]) + 1.0) / 5.0;

//     if(v_uv.x > 0.5) blurPower.x *= -1.0;
//     if(v_uv.y > 0.5) blurPower.y *= -1.0;

//     vec2 v = blurPower / (u_sampleNum * 2.0);
//     vec2 vv = v;
//     // float weight = 1.0;
//     for(int i=1;i<u_sampleNum;i++, vv += v){
//         // weight /= 2.0;
//         vec2 uv = v_uv + vv;
//         color += texture2D(CC_Texture0, uv);
//     }
//     color /= u_sampleNum;
//     gl_FragColor = color;
// }

// #ifdef GL_ES
// precision mediump float;
// #endif

// #extension GL_OES_standard_derivatives : enable

// uniform float time;
// uniform vec2 mouse;
// uniform vec2 resolution;
const float PI = 3.1415927;

void main(void){
    vec2 resolution = vec2(1136.0, 640.0);
    vec2 p = (gl_FragCoord.xy * 2.0 - resolution) / min(resolution.x, resolution.y);
    vec3 color1 = vec3(0.0, 0.3, 0.5);
    vec3 color2 = vec3(0.5, 0.0, 0.3);

    float f = 0.0;
    float g = 0.0;
    float h = 0.0;
    float PI = 3.14159265;
    for(float i = 0.0; i < u_sampleNum; i++){
        float s = sin(CC_Time[1] + i * PI / 20.0) * 0.8;
        float c = cos(CC_Time[1] + i * PI / 20.0) * 0.8;
        float d = abs(p.x + c);
        float e = abs(p.y + s);
        f += 0.001 / d;
        g += 0.001 / e;
        h += 0.00003 / (d * e);
    }


    gl_FragColor = vec4(f * color1 + g * color2 + vec3(h), 1.0);
}
