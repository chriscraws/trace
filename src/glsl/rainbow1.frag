void main() {
  float pi = getIndex();
  vec3 p = getLocation();

  float v = 0.2 + 0.8 * cnoise_2(vec4(p * 0.8, time / 2.0));
  float h = cnoise_2(vec4(p + 1000.0, time / 3.0));

  vec3 color = hsv2rgb(vec3(h, 1.0, v));

  gl_FragColor = vec4(color, 1.0);
}
