#pragma glslify: import('./header.glsl')
#pragma glslify: box = require('glsl-sdf-box')

uniform vec4 color;

void main() {
  float pi = (gl_FragCoord.y * 40.0) +  gl_FragCoord.x;
  vec2 uv = (gl_FragCoord.xy + 0.5) / vec2(40.0, 89.0); 
  vec3 p = vec3(texture2D(spos, uv).xyz);
  p -= 0.5;
  p *= 2.0;

  vec3 white = vec3(1.0);
  vec3 black = vec3(0.0);

  float d = box(p, vec3(0.25));
  float blur = 0.2;
  float mixamt = smoothstep(0.0, 0.1, d);
  vec3 color = mix(white, black, mixamt);

  gl_FragColor = vec4(color, 1.0);
}
