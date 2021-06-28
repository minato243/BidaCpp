#ifdef GL_ES
precision mediump float;
#endif

varying float vCoord;

void main() {
	gl_FragColor = vec4(0.0, 1.0, 0.0, 1.0 - min(1.0, abs(vCoord) * 10));
}
