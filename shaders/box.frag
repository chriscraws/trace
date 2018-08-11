precision mediump float;
#define GLSLIFY 1
uniform float time;
uniform sampler2D spos;

// Originally sourced from:
// http://iquilezles.org/www/articles/distfunctions/distfunctions.htm

float sdBox(vec3 position, vec3 dimensions) {
  vec3 d = abs(position) - dimensions;

  return min(max(d.x, max(d.y,d.z)), 0.0) + length(max(d, 0.0));
}

uniform vec4 color;

void main() {
  float pi = (gl_FragCoord.y * 40.0) +  gl_FragCoord.x;
  vec2 uv = (gl_FragCoord.xy + 0.5) / vec2(40.0, 89.0); 
  vec3 p = vec3(texture2D(spos, uv).xyz);
  //p -= 0.5;
  //p *= 2.0;

  vec3 white = vec3(1.0);
  vec3 black = vec3(0.0);

  float d = sdBox(p, vec3(0.05));
  float blur = 0.2;
  //float mixamt = smoothstep(0.0, 10.0, abs(mod(time * 100.0, 36.0 * 67.0) - pi));
  float mixamt = step(0.5, length(p));
  vec3 color = mix(white, black, mixamt);

  gl_FragColor = vec4(p, 1.0);
}
