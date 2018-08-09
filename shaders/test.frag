uniform vec4 color;
uniform float time;
uniform sampler2D pos;
float v(float t) {
  return 0.5 + 0.5 * sin(t);
}
void main() {
  gl_FragColor = vec4(vec3(v(time)), 1.0);
}
