
float pdf(float x) {
  return pow(2.71828, -x * x / 2.0);

}
vec3 getChandelier() {
  vec3 p = getLocation() - getOffset();
  bool bottom = false;

  float freq = 0.2 + random(p.xz);

  float y = p.y + 1.0;
  y /= 2.0;

  bottom = y < 0.001;

  y += time * 7.5;

  float b = fract(random(p.xz + 1000.0) + y / (1.5 + freq * 0.8));

  float i = step(0.5, b);
  float newb = pow(pdf((b - 0.5) * 20.0), 1.0);
  vec3 color = vec3(0.5, 0.5, 1.0) * 0.7;
  if (bottom) {
    b = mix(newb, (1.0 - b) * 2.0, i);
    b = pow(b, 6.0);
    color = vec3(0.8, 0.8, 1.0) * 0.6;
  } else {
    b = newb;
  }

  float t = time / 800.0;
  b += smoothstep(-1.0 + t, -1.1 + t, p.y);
  return color * b;
}

vec3 getGroundTube() {
  vec3 color = vec3(0.5 + 0.5 * cnoise_0(vec3(getLocation().xz / 1.2, time / 5.0)));
  color.rg *= 0.7;
  return color;
}

void main() {
  vec3 p = getLocation() - getOffset();
  vec3 off = getOffset();

  vec3 ground = getGroundTube();
  vec3 chandelier = getChandelier();

  // clouds
  chandelier *= smoothstep(0.8, 0.4, p.y);
  chandelier += (0.25 + 0.3 * snoise_1(vec3(p.x + time / 20.0, p.yz / 2.0))) * smoothstep(0.2, 0.8, p.y - 0.4 * snoise_1(vec3(p.xz, time / 20.0)));

  // lightning
  float t = pow(fract(-(time - 5.0) / 20.0), 100.0) * random(vec2(time / 100.0, 0.0));
  ground += t;
  chandelier += t;
  

  gl_FragColor = vec4(
                      mix(ground, chandelier, step(0.0, 1.0 - length(off))),
                      1.0
                      );
}
