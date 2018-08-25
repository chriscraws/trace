float sdSphere( vec3 p, float s )
{
    return length(p)-s;
}

vec3 chand() {
  vec3 p = getLocation();

  float t = time / 2.0;

  vec3 color = hsv2rgb(vec3(
    0.33,
    1.0,
  snoise_1(vec3(
    p.x + time / 20.0,
    p.y,
    p.z - time / 13.0
  ))* 0.5
  ));
  
  return color;
}

float pdf(float x) {
  return pow(2.71828, -x * x / 2.0);

}
vec3 getChandelier() {
  vec3 p = getLocation() - getOffset();
  bool bottom = false;

  float freq = random(p.xz) / 2.0;

  float y = p.y + 1.0;
  y /= 2.0;

  bottom = y < 0.001;

  y -= 100.0 * random(p.xz + 400.0) + (1.0 + 5.0 * random(p.xz + 500.0)) * time * 0.05;

  float b = fract(y / (5.0 + 0.8 * freq));

  float i = step(0.5, b);
  b = pdf((b - 0.5) * 60.0);
  
  b *= pdf((p.y - snoise_1(vec3(p.xz, time / 50.0))) * 50.0);

  return hsv2rgb(vec3(
    1.0 / 6.0,
    0.9,
    b  
  ));
}

vec3 getGroundTube() {
  vec3 color = vec3(0.5 + 0.5 * cnoise_0(vec3(getLocation().xz / 1.2, time / 5.0)));
  color.rb *= 0.2;
  return color;
}

void main() {
  vec3 p = getLocation();
  vec3 off = getOffset();

  vec3 ground = getGroundTube();
  vec3 chandelier = max(getChandelier(), chand());

  gl_FragColor = vec4(
                      mix(ground, chandelier, step(0.0, 1.0 - length(off))),
                      1.0
                      );
}
