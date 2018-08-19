
void main() {
  float pi = getIndex();
  vec3 p = getLocation() - getOffset();

  float t = time / 70.0;


  vec3 sunp = p;
  sunp.y += fract(t) * 2.0 * 1.45 - 1.45;
  float d = length(sunp);
  float b = smoothstep(0.6, 0.4, d);
  vec3 suncolor = vec3(b);
  suncolor *= 1.0 - step(1.0, mod(t, 2.0));
  suncolor *= 1.0 - step(2.0, mod(t, 4.0));
  suncolor *= step(0.01, 1.0 - length(getOffset()));


  vec3 skycolor;
  vec3 skyp = p;
  skyp.y -= 1.0;
  float tscale = 3.14159;
  float yscale = 0.7;
  float off = 0.7;
  skycolor.r = sin(yscale * skyp.y + t * tscale);
  skycolor.g = 0.7 * sin(yscale * skyp.y + off + t * tscale);
  skycolor.b = sin(yscale * skyp.y + 2.0 * off + t * tscale);
  skycolor *= max(
    1.0 - smoothstep(1.4, 1.6, mod(t, 3.8)),
    smoothstep(3.5, 3.8, mod(t, 3.8)));
  skycolor *= 0.8;

  vec3 starcolor;
  float r = random(vec2(random(p.xy), p.z));
  float prob = mix(0.93, 0.6, step(0.1, length(getOffset())));
  if (r > prob) {
    starcolor = vec3(r * (.85 * sin(t * (r * 70.0) + 720.0 * r) + 0.75));
  }
  starcolor *= 1.0 - pow(sin(2.0 * PI * (t / 3.8 + 0.125)) + 0.1, 3.0) / pow(1.1, 3.0);

  gl_FragColor = vec4(max(starcolor, max(suncolor, skycolor)), 1.0);
}
