precision mediump float;
uniform vec4 color;
uniform float time;
uniform sampler2D pos;

float v(float t) {
  return 0.5 + 0.5 * sin(t);
}

void main() {
  vec2 uv = gl_FragCoord.xy / vec2(40.0, 89.0);
  float y = texture2D(pos, uv).y;
  gl_FragColor = vec4(y * vec3(v(time)), 1.0);
}
