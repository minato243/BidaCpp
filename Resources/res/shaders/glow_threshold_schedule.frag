#ifdef GL_ES
precision mediump float;
#endif

uniform float u_glowSpeed;
uniform vec4 u_color;
uniform float u_isGlow;
varying vec2 v_uv;

void main(){
    if(u_isGlow > 0.5){
        float sine = abs(sin(CC_Time[1]/2.0*u_glowSpeed));
        vec4 color = texture2D(CC_Texture0, v_uv);
        const float glowMax = 1.4;
        if(sine > 1.0 / glowMax){
            color.rgb = glowMax * sine * color.rgb;

            clamp(color.r, 0.0, 1.0);
            clamp(color.g, 0.0, 1.0);
            clamp(color.b, 0.0, 1.0);
        }
        gl_FragColor = color;
    }
    else{
        gl_FragColor = texture2D(CC_Texture0, v_uv);
    }

}