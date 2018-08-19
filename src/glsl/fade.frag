void main() {
  vec3 color = mix(getA(), getB(), step(0.5, tmix));
  color *= abs(tmix - 0.5) * 2.0;
  gl_FragColor = vec4(color, 1.0);
}
