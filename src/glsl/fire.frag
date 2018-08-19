float fbm(vec3 p) {
  float f;
  mat3 m = mat3(
    1.6, 0.0, 1.2,
    0.0, 1.0, 0.0,
   -1.2, 0.0, 1.6
  );
  f  = 0.5 * cnoise_1(p); p = m*p;
  f += 0.25 * cnoise_1(p); p = m*p;
  f += 0.125 * cnoise_1(p); p = m*p;
  f += 0.0625 * cnoise_1(p); p = m*p;
  f = 0.5 + 0.5 * f;
  return f;
}



void main() {
  float pi = getIndex();
  vec3 p = getLocation() - getOffset();

  p.y += 1.0;
  p /= 2.0;

  // Adapted from www.shadertoy.com/view/XsXSWS
  // Fires by xbe

  float strength = 7.0;
  float T3 = max(1.5, 1.25 * strength) * time * 0.8;
  p.y -= 0.08;
  float n = fbm(strength * p - vec3(0.0, T3, 0.0));
  float hscale = 0.5;
  float c = 1. - 16. * pow(max(0., length(p * vec3(hscale+p.y*1.5, .75, hscale+p.y*1.5)) - n * max(0., p.y+.25) ) , 1.0);

  float c1 = n * c * (1.5 - pow(1.2 * p.y, 2.));
  c1 = clamp(c1, 0.0, 1.0);

  vec3 col = vec3(1.5*c1, 1.5*c1*c1*c1, c1*c1*c1*c1*c1*c1);

  c = c * (1.0 - pow(p.y, 3.0));
  gl_FragColor = vec4(mix(vec3(0.0), col, c), 1.0);
}
