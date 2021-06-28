#ifdef GL_ES
precision mediump float;
#endif

varying vec2 v_uv;

uniform float u_distortRatio;
uniform float u_minOpacity;
uniform vec3 u_color;
uniform float u_realHeight;
uniform float u_texHeight;
uniform float u_timeSpeed;
uniform float u_colorPart;
uniform float u_time;
uniform float u_compressCenter;
uniform sampler2D tex2;

const float maxOpacity = 0.75;
void main(){
    vec2 uv = v_uv;
    float curHeight = v_uv.y * u_realHeight;
    uv.y = fract(curHeight/8.0);
    uv.y += fract(u_time * u_timeSpeed);
    if(uv.y > 1.0) uv.y -= 1.0;

    // float compress = minDistort + (u_distortRatio - minDistort)*v_uv.y;
    float compress = 1.0;
    float neck = 30.0; // no more distort after <neck> px
    if(neck > u_realHeight){
        compress = u_distortRatio + (1.0 - v_uv.y) * (1.0 - v_uv.y) * (1.0 - u_distortRatio);
    }
    else if(v_uv.y > neck / u_realHeight){
        compress = u_distortRatio * (1.0 - (v_uv.y - neck / u_realHeight)*0.9);
    }
    else{
        float part = v_uv.y * u_realHeight / neck;
        compress = u_distortRatio + (1.0 - part) * (1.0 - part) * (1.0 - u_distortRatio);
    }
    // uv.x = (uv.x - (1.0-compress)*0.5) / compress;
    // map from [compressCenter - compress/2, compressCenter + compress/2] to [0, 1];
    float compressCenter = u_compressCenter;
    if(compressCenter < compress*0.5){
        compressCenter = compress*0.5;
    }
    if(compressCenter > 1.0 - compress*0.5){
        compressCenter = 1.0 - compress*0.5;
    }
    uv.x = (uv.x - compressCenter + compress*0.5)/compress;

    vec4 color;
    if(v_uv.x < compressCenter - compress*0.5 || v_uv.x > compressCenter  + compress*0.5){
        color = vec4(0.0);
    } 
    else {
        color = (u_colorPart) * texture2D(CC_Texture0, uv) + (1.0 - u_colorPart) * texture2D(tex2, uv);
    }

    float opacity;
    if(v_uv.y > 0.5){
        opacity = maxOpacity * max(0.0, (0.78 - v_uv.y)/0.28);
    }
    else if(u_realHeight < 16.0) {
        opacity =(maxOpacity - u_minOpacity) * v_uv.y / 0.5 + u_minOpacity;
    }
    else {
        opacity = (maxOpacity - u_minOpacity) * min(1.0, (v_uv.y * u_realHeight)/8.0) + u_minOpacity;
    }
    color *= opacity;
    gl_FragColor = color * vec4(u_color, 1.0);
}