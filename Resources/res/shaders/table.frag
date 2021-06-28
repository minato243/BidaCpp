varying vec2 v_texture_coord;
varying vec3 v_normal;
varying vec4 v_position;

uniform vec3 u_view_pos;
uniform vec3 u_light_color;
uniform vec3 u_light_pos;
uniform float u_shineDamper;
uniform float u_specular;
uniform float u_intensity;
uniform float u_ambient;
uniform vec4 u_color;

vec3 limit3 (vec3 v) {
    if (v.x > 1.0) v.x = 1.0;
    if (v.y > 1.0) v.y = 1.0;
    if (v.z > 1.0) v.z = 1.0;
    return v;
}
vec4 limit4 (vec4 v) {
    if (v.r > 1.0) v.r = 1.0;
    if (v.g > 1.0) v.g = 1.0;
    if (v.b > 1.0) v.b = 1.0;
    if (v.a > 1.0) v.a = 1.0;
    return v;
}

void main (void) {
    vec4 finalColor = texture2D(CC_Texture0, v_texture_coord);    

    // light
    vec3 surfaceNormal = v_normal;
    vec3 toCameraVector = u_view_pos - v_position.xyz;

    vec3 lightDirection = u_light_pos - v_position.xyz;
    vec3 unitNormal = normalize(surfaceNormal);
    vec3 unitLightVector = normalize(lightDirection);

    // tinh vector phan chieu anh sang bang reflect vector anh sang va vector normal
    vec3 unitVectorToCamera = normalize(toCameraVector);
    vec3 reflectedLightDirection = normalize(reflect(unitLightVector, unitNormal));

    // tinh phan chieu = nhan vo huong vector huong camera va vector phan chieu anh sang
    float specularFactor = dot(unitVectorToCamera, reflectedLightDirection);
    specularFactor = max(specularFactor, 0.0);
    float dampedFactor = pow(specularFactor, u_shineDamper);
    vec4 v_finalSpecular = dampedFactor*vec4(u_light_color,1.0)*u_specular;

    // tinh diffuse tu normal va vector huong sang
    float nDot1 = dot(unitNormal, -unitLightVector);
    float brightness = max(nDot1, u_ambient) * u_intensity;
    vec3 v_diffuse = brightness * u_light_color * u_color.rgb / u_color.rgb;

    gl_FragColor = vec4(v_diffuse, 1.0) * finalColor + v_finalSpecular;
    // gl_FragColor = vec4(limit4(gl_FragColor).rgb, 1.0);
    // gl_FragColor = vec4(limit4(finalColor).rgb, 1.0) * u_color;
    // gl_FragColor = vec4(limit3(gl_FragColor), 1.0) * u_color;
}
