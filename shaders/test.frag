precision mediump float;
uniform vec4 color;
uniform float time;
uniform sampler2D spos;

void main() {
  vec2 uv = (gl_FragCoord.xy + 0.5) / vec2(40.0, 89.0); 
  vec3 p = vec3(texture2D(spos, uv).xyz);
  p -= 0.5;
  p *= 2.0;

  vec3 white = vec3(1.0);
  vec3 black = vec3(0.0);

  float blur = 0.2;
  float size = 0.2 + 0.1 * sin(time);
  vec3 color = mix(white, black, smoothstep(size, size + blur, length(p)));

  gl_FragColor = vec4(color, 1.0);
}
