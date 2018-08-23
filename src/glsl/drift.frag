

vec3 getGroundTube() {
  vec3 p = getLocation();
  p *= 3.0;
  float r = snoise_2(vec4(p, time / 5.0));
  float b = 0.8 + 0.2 * r;
  b *= smoothstep(0.0, 5.0, time);
  vec3 color = hsv2rgb(vec3((9.0 / 12.0) + r * 0.15, 1.0, b));
  return color;
}

vec3 getChandelier() {
  vec3 p = getLocation();

  float t = time / 2.0;
  p *= rotation3dY(t);
  p *= rotation3dX(t / 2.0);
  p *= rotation3dZ(t / 3.0);

  p /= time > 1.0 ? 1.0 : bounceOut(time);


  p.y += 0.25 * sin(time / 2.0);

  float d = sdBox(p, vec3(0.35));
  float b = smoothstep(0.2, 0.0, d);
  vec3 color = hsv2rgb(vec3(0.12 * sin(time / 2.0) + 0.85 - 1.2 * length(d), 1.0, b));
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
