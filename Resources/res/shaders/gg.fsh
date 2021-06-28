#ifdef GL_ES
precision mediump float;
#endif

varying vec2 v_texCoord;

uniform vec3 u_color;
// uniform vec2 resolution;
// uniform float blurRadius;
// uniform float sampleNum;

vec3 blur(vec2, vec2, float);

vec3 screen(vec3, vec3);

void main(void)
{
    vec3 color = texture2D(CC_Texture0, v_texCoord);
    vec2 res = vec2(1920.0, 1080.0);
    vec3 col1 = blur(v_texCoord, res, 2.0); //* v_fragmentColor.rgb;
    vec3 col2 = blur(v_texCoord, 0.5*res, 4.0); //* v_fragmentColor.rgb;
    vec3 col3 = blur(v_texCoord, 0.25*res, 8.0); //* v_fragmentColor.rgb;
    // float val = (col.x + col.y + col.z) / 3.0;


    gl_FragColor = vec4(screen(screen(screen(col1, color), col2), col3)*u_ccolor, 1.0);
}

vec3 screen(vec3 a, vec3 b){
    return vec3(1.0) - (vec3(1.0) - a)*(vec3(1.0) - b);
}
vec3 blur(vec2 p, vec2 resolution, float r)
{
    float threshold = 0.5;
    vec3 col = vec3(0.0);
    vec2 unit = 1.0 / resolution.xy;
    
    float sampleStep = r / 10.0;
    
    float count = 0.0;
    
    for(float x = -r; x < r; x += sampleStep)
    {
        for(float y = -r; y < r; y += sampleStep)
        {
            vec3 color = texture2D(CC_Texture0, p + vec2(x * unit.x, y * unit.y)).rgb;
            float weight = (r - abs(x)) * (r - abs(y));
            float brightness = dot(color, vec3(0.2126, 0.7152, 0.0722));
            if(brightness > threshold){
                col += color * weight;
            }
            count += weight;
        }
    }
    
    return col / count;
}
