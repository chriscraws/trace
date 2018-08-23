void main() {
  float pi = getIndex();
  vec3 p = getLocation();

  float d = length(p);
  float t = time;

  float wave = 0.5 + 0.5 * sin(4.0 * d - t * 1.3);
  vec3 color = hsv2rgb(vec3(fract(d / 1.5 + t / 10.0), 1.0, wave));
  gl_FragColor = vec4(color, 1.0);
}
