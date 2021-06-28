attribute vec4 a_position;
attribute vec2 a_texCoord;

varying vec2 v_texture_coord;
varying vec3 v_normal;

uniform vec4 u_quat;
uniform vec4 u_quat_reverse;

vec4 quat_mult(vec4 p, vec4 q) {
	return vec4(
		p.w * q.x + p.x * q.w + p.y * q.z - p.z * q.y,
		p.w * q.y - p.x * q.z + p.y * q.w + p.z * q.x,
		p.w * q.z + p.x * q.y - p.y * q.x + p.z * q.w,
		p.w * q.w - p.x * q.x - p.y * q.y - p.z * q.z
	);
}

// vec4 quat_reverse(vec4 q) {
// 	float square = q.w * q.w + q.x * q.x + q.y * q.y + q.z * q.z;
// 	return vec4(-q.x / square, -q.y / square, -q.z / square, q.w / square);
// }

void main(void)
{
    gl_Position = CC_MVPMatrix * a_position;
    v_texture_coord = a_texCoord;
    v_texture_coord.y = 1.0 - v_texture_coord.y;
    // vec4 quat_rev = quat_reverse(u_quat);
    vec4 r = quat_mult(quat_mult(u_quat, a_position), u_quat_reverse);
    v_normal = r.xyz;
}
