attribute vec4 a_position;
attribute vec3 a_normal;
uniform float OutlineWidth;
uniform vec3 u_view_pos;

varying float v_alpha;

void main(void)
{
    vec4 pos = CC_MVPMatrix * a_position;
    vec4 oldPos = CC_MVPMatrix * a_position;
    vec4 normalproj = CC_MVPMatrix * vec4(a_normal, 0);
    normalproj = normalize(normalproj);
    pos.xy += normalproj.xy * (OutlineWidth * (pos.z * 0.5 + 0.5));

    v_alpha = max(1.0 - distance(oldPos.xy, pos.xy) / 2.0, 0.0);

    gl_Position = pos;
}
