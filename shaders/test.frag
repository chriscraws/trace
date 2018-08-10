precision mediump float;
uniform vec4 color;
uniform float time;
uniform sampler2D spos;

void main() {
  vec2 uv = gl_FragCoord.xy / vec2(89.0, 40.0);
  vec3 tex = texture2D(spos, uv).rgb;
  gl_FragColor = vec4(tex, 1.0);
}
