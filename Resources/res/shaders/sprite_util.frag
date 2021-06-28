#ifdef GL_ES
precision mediump float;
#endif

varying vec2 v_uv;

uniform float u_grayscale;
uniform float u_opacity;
uniform float u_circle;
uniform float u_width;
uniform float u_height;
uniform float u_radius;
uniform float u_highlight;

void main(){
    vec4 color = texture2D(CC_Texture0, v_uv);
    if(u_grayscale > 0.5){
        float grayColor = dot(color.rgb, vec3(0.299, 0.587, 0.113));
        color = vec4(vec3(grayColor), color.a);
    }
    if(u_circle > 0.5){
        vec2 p = v_uv * vec2(u_width, u_height);
        vec2 center = vec2(u_width/2.0, u_height/2.0);
        float d = length(p - center);
        if(d > u_radius + 2.0){
            color = vec4(0.0, 0.0, 0.0, 0.0);
            color.a = 0.0;
        }
        else if(d >= u_radius){
            color *= clamp((2.0 - (d - u_radius))/2.0, 0.0, 1.0);
        }
    }
    if(u_highlight > 0.5){
        color.rgb *= 1.5;
        if(color.r > 1.0) color.r = 1.0;
        if(color.g > 1.0) color.g = 1.0;
        if(color.b > 1.0) color.b = 1.0;
    }
    color.a *= u_opacity;
    gl_FragColor = color;
}