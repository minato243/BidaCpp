uniform vec3 OutLineColor;
uniform vec4 u_color;

varying float v_alpha;

void main(void)
{
    gl_FragColor =  vec4(OutLineColor * u_color.rgb, v_alpha);
}
