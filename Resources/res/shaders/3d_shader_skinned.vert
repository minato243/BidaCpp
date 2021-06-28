attribute vec3 a_position;

attribute vec3 a_normal;
attribute vec2 a_texCoord;
attribute vec4 a_blendWeight;
attribute vec4 a_blendIndex;


const int SKINNING_JOINT_COUNT = 60;
// Uniforms
uniform vec4 u_matrixPalette[SKINNING_JOINT_COUNT * 3];

// Varyings
varying vec4 v_position;
varying vec2 v_texture_coord;
varying vec3 v_normal;

varying vec3 v_light_pos_0;
varying vec3 v_light_pos_1;
varying vec3 v_light_pos_2;
varying vec3 v_light_pos_3;
varying vec3 v_light_pos_4;
varying vec3 v_light_pos_5;
varying vec3 v_V;
varying float v_distance_0;
varying float v_distance_1;
varying float v_distance_2;
varying float v_distance_3;
varying float v_distance_4;
varying float v_distance_5;

uniform vec3 u_view_pos;
uniform vec3 u_light_pos_0;
uniform vec3 u_light_pos_1;
uniform vec3 u_light_pos_2;
uniform vec3 u_light_pos_3;
uniform vec3 u_light_pos_4;
uniform vec3 u_light_pos_5;

vec3 calcVectorL(vec3 r, vec3 cur_light, vec3 position, out float distance){
	vec3 light = cur_light - position;

	const float lightRadius = 40.0;
	vec3 centerToRay = (dot(light, r) * r) - light;
	vec3 closestPoint = light + centerToRay*clamp(lightRadius / length(centerToRay), 0.0, 1.0);
	vec3 L = normalize(closestPoint);

	// square distance for performance
	// float distance2 = dot(light/300.0, light/300.0);
	distance = length(closestPoint);
    return L;
}

void getPositionAndNormal(out vec4 position, out vec3 normal)
{
    float blendWeight = a_blendWeight[0];

    int matrixIndex = int (a_blendIndex[0]) * 3;
    vec4 matrixPalette1 = u_matrixPalette[matrixIndex] * blendWeight;
    vec4 matrixPalette2 = u_matrixPalette[matrixIndex + 1] * blendWeight;
    vec4 matrixPalette3 = u_matrixPalette[matrixIndex + 2] * blendWeight;


    blendWeight = a_blendWeight[1];
    if (blendWeight > 0.0)
    {
        matrixIndex = int(a_blendIndex[1]) * 3;
        matrixPalette1 += u_matrixPalette[matrixIndex] * blendWeight;
        matrixPalette2 += u_matrixPalette[matrixIndex + 1] * blendWeight;
        matrixPalette3 += u_matrixPalette[matrixIndex + 2] * blendWeight;

        blendWeight = a_blendWeight[2];
        if (blendWeight > 0.0)
        {
            matrixIndex = int(a_blendIndex[2]) * 3;
            matrixPalette1 += u_matrixPalette[matrixIndex] * blendWeight;
            matrixPalette2 += u_matrixPalette[matrixIndex + 1] * blendWeight;
            matrixPalette3 += u_matrixPalette[matrixIndex + 2] * blendWeight;

            blendWeight = a_blendWeight[3];
            if (blendWeight > 0.0)
            {
                matrixIndex = int(a_blendIndex[3]) * 3;
                matrixPalette1 += u_matrixPalette[matrixIndex] * blendWeight;
                matrixPalette2 += u_matrixPalette[matrixIndex + 1] * blendWeight;
                matrixPalette3 += u_matrixPalette[matrixIndex + 2] * blendWeight;
            }
        }
    }

    vec4 p = vec4(a_position, 1.0);
    position.x = dot(p, matrixPalette1);
    position.y = dot(p, matrixPalette2);
    position.z = dot(p, matrixPalette3);
    position.w = p.w;

    vec4 n = vec4(a_normal, 0.0);
    normal.x = dot(n, matrixPalette1);
    normal.y = dot(n, matrixPalette2);
    normal.z = dot(n, matrixPalette3);
}

void main()
{
    vec4 position;
    vec3 normal;
    getPositionAndNormal(position, normal);
    gl_Position = CC_MVPMatrix * position;
    
    v_texture_coord = a_texCoord;
    v_texture_coord.y = 1.0 - v_texture_coord.y;
    v_position = CC_MVMatrix * position;
    v_normal = CC_NormalMatrix * normal;
    
    v_V = normalize(u_view_pos - v_position.xyz);

	vec3 r = reflect(-v_V, normalize(v_normal));

    v_light_pos_0 = calcVectorL(r, u_light_pos_0, v_position.xyz, v_distance_0);
    v_light_pos_1 = calcVectorL(r, u_light_pos_1, v_position.xyz, v_distance_1);
    v_light_pos_2 = calcVectorL(r, u_light_pos_2, v_position.xyz, v_distance_2);
    v_light_pos_3 = calcVectorL(r, u_light_pos_3, v_position.xyz, v_distance_3);
    v_light_pos_4 = calcVectorL(r, u_light_pos_4, v_position.xyz, v_distance_4);
    v_light_pos_5 = calcVectorL(r, u_light_pos_5, v_position.xyz, v_distance_5);
}