

vec3 chandelier(vec3 p, float t, float c, float blur) {
  float s = qinticInOut(min(t, 1.0));

  float d = max(
      sdBox( p, vec3(0.5,1.0,0.5) * s ),
      -sdBox( p, vec3(0.4,2.0,0.4) * s  )
  );
  d = max(
      d,
      -sdBox( p, vec3(0.4,0.93,0.6) * s  )
  );
  d = max(
      d,
      -sdBox( p, vec3(0.6,0.93,0.4) * s  )
  );
  float b = smoothstep(blur, 0.0, d);
  return hsv2rgb(vec3(
    fract(c),
    1.0, 
     b
  ));
}

vec3 top(vec3 p, float blur) {
    float t = time / 8.0;
    return max(
        chandelier(p, fract(t) * 2.0, random(vec2(floor(t), 0.0)), blur) * 
        (1.0 - clamp(0.0, 1.0, (fract(t) * 2.0 - 1.0))) *
        sineIn(clamp(0.0, 1.0, (fract(t) * 2.0))),

        chandelier(p, fract(t + 0.5) * 2.0, random(vec2(floor(t + .5), 1.0)), blur) * 
        (1.0 - clamp(0.0, 1.0, (fract(t + 0.5) * 2.0 - 1.0))) *
        sineIn(clamp(0.0, 1.0, (fract(t + 0.5) * 2.0)))
    );
}

void main() {
    float t = time / 8.0;
    vec3 chandelier = top(getLocation() - getOffset(), 0.2);
    vec3 ground = top(vec3(0.5, 1.0, 0.5), 0.8);

    gl_FragColor = vec4(
                        mix(ground, chandelier, step(0.0, 1.0 - length(getOffset()))),
                        1.0
                        );
}