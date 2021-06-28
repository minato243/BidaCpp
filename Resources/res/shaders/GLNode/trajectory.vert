attribute vec3 aPos;

varying float vCoord;

void main() {
	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
	vCoord = aPos.x;
}
