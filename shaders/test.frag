precision mediump float;
uniform vec4 color;
uniform float time;
uniform sampler2D spos;

void main() {
  vec2 uv = gl_FragCoord.xy / vec2(40.0, 89.0);
  gl_FragColor = vec4(vec3(texture2D(spos, uv).y), 1.0);
}
