precision mediump float;
uniform vec4 color;
uniform float time;
uniform sampler2D spos;

void main() {
  vec2 uv = gl_FragCoord.xy / vec2(40.0, 89.0); 
  vec3 tex = vec3(texture2D(spos, uv).y);
  gl_FragColor = vec4(tex, 1.0);
}
