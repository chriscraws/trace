float pdf(float x) {
  return pow(2.71828, -x * x / 2.0);
}

vec3 bubbles() {
    vec3 p = getLocation() - getOffset();
    bool bottom = false;

    float freq = random(p.xz);
    
    float t = time * 2.0;

    p.y += 0.1 * sin(p.x * 3.0 + t);
    p.y += 0.1 * sin(p.z + t / 2.0);

    float y = p.y + 1.0;
    y /= 2.0;

    bottom = y < 0.001;

    y -= time * 0.35;

    float b = fract(y / 5.0 + freq * 10.0);
    b = pdf((b - 0.5) * 500.0);

    b *= smoothstep(0.2, 0.2-0.04, p.y);

    float normalY = ((p.y + 1.0) / 2.0);
    return hsv2rgb(vec3(
        0.67,
        0.2,
        normalY * 2.0 * b
    ));
}

vec3 chandelier() {
    vec3 p = getLocation();
    float t = time * 2.0;

    p.y += 0.1 * sin(p.x * 3.0 + t);
    p.y += 0.1 * sin(p.z + t / 2.0);

    float blur = 0.04;
    float normalY = ((p.y + 1.0) / 2.0);
    vec3 color1 = hsv2rgb(vec3(
        0.67 - 0.12 * normalY,
        1.0,
        normalY * 2.0
    ));
    float shimmer = snoise_2(vec4(p * 1.5, t / 3.0));
    color1.g += 0.3 * shimmer;
    color1.b += 0.2 * shimmer;
    color1.r += 0.08 * shimmer;
    vec3 color2 = vec3(0.0);
    vec3 liquidColor = mix(color1, color2, smoothstep(-blur + 0.2, blur + 0.2, p.y));

    return max(liquidColor, bubbles());
}

vec3 ground() {
    vec3 color = vec3(0.5 + 0.5 * cnoise_0(vec3(getLocation().xz / 1.2, time / 5.0)));
    color.rg *= 0.3;
    return color;
}

void main() {
    vec3 ground = ground();
    vec3 chandelier = chandelier();

    gl_FragColor = vec4(
                        mix(ground, chandelier, step(0.0, 1.0 - length(getOffset()))),
                        1.0
                        );
}