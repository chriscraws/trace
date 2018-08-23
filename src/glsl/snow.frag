
float pdf(float x) {
  return pow(2.71828, -x * x / 2.0);

}
vec3 getChandelier() {
  vec3 p = getLocation() - getOffset();
  bool bottom = false;

  float freq = random(p.xz);

  float y = p.y + 1.0;
  y /= 2.0;

  bottom = y < 0.001;

  y += (1.0 + 0.5 * random(p.xz + 100.0)) * time * 0.03;

  float b = fract(y / (0.35 + freq * 0.8));

  float i = step(0.5, b);
  float newb = pow(pdf((b - 0.5) * 50.0), 1.0);

  if (bottom) {
    b = mix(newb, (1.0 - b) * 2.0, i);
  } else {
    b = newb;
  }

  return vec3(b);
}

vec3 getGroundTube() {
  vec3 color = vec3(0.5 + 0.5 * cnoise_0(vec3(getLocation().xz / 1.2, time / 5.0)));
  color.rg *= 0.7;
  return color;
}

void main() {
  vec3 p = getLocation();
  vec3 off = getOffset();

  vec3 ground = getGroundTube();
  vec3 chandelier = getChandelier();

  gl_FragColor = vec4(
                      mix(ground, chandelier, step(0.0, 1.0 - length(off))),
                      1.0
                      );
}
