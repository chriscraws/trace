float sdCapsule( vec3 p, vec3 a, vec3 b, float r )
{
    vec3 pa = p - a, ba = b - a;
    float h = clamp( dot(pa,ba)/dot(ba,ba), 0.0, 1.0 );
    return length( pa - ba*h ) - r;
}

vec3 line(
    float t,
    float r
) {
    vec3 p = getLocation() - getOffset();

    p *= rotation3dY(time / 20.0);
    p *= rotation3dZ(time / 15.0);
    p *= rotation3dX(time / 30.0);

    vec3 start = vec3(
        0.7 * (2.0 * random(vec2(0.0, r)) - 1.0),
        -1.2 * (2.0 * random(vec2(1.0, r)) - 1.0),
        -0.7 * (2.0 * random(vec2(2.0, r)) - 1.0)
    );

    vec3 direction = normalize(-start);

    start += vec3(
        (2.0 * random(vec2(6.0, r)) - 1.0),
        (2.0 * random(vec2(7.0, r)) - 1.0),
        (2.0 * random(vec2(8.0, r)) - 1.0)
    ) * 0.45;

    float dist = distance(
        start,
        min(
            vec3(0.7, 1.2, 0.7),
            max(
                vec3(-0.7, -1.2, -0.7),
                start + direction * 100000.0
            )
        )
    );

    float d = sdCapsule(
        p,
        start + direction * sineInOut(clamp(t * 2.0 - 1.0, 0.0, 1.0)) * dist,
        start + direction * sineInOut(clamp(t * 2.0, 0.0, 1.0)) * dist, // end
        0.0 // radius
    );
    float b = smoothstep(0.2, 0.05, d);

    vec3 color = hsv2rgb(vec3(
        random(vec2(3.0, r)),
        0.8 + 0.2 * pow(random(vec2(4.0, r)), 3.0),
        (0.5 + 0.5 * random(vec2(5.0, r))) * b
    ));

    return color;
}

vec3 chandelier() {
    float t = time / 70.0;
    vec3 color = vec3(0.0);

    const float c = 4.0;
    float n = 1.0 / c;

    for (float i = 0.0; i < c; i++) {
        float off = i * n;
        float r = floor(t + i * 10000.0 - off);
        color += line(
            (
                0.5 * clamp(fract(t - off), 0.0, n) / n +
                0.5 * clamp(fract(t - off) - (c - 1.0) * n, 0.0, n) / n
            ),
            r
        );
    }

    return color;
}

vec3 ground() {
    float t = time / 70.0 * 4.0;

    float h = random(vec2(10.0, floor(t)));
    float h2 = random(vec2(10.0, floor(t) + 1.0));

    vec3 c = hsv2rgb(vec3(
        h,
        1.0,
        0.8
    ));
    vec3 c2 = hsv2rgb(vec3(
        h2,
        1.0,
        0.8
    ));

    return mix(c, c2, pow(fract(t), 20.0));
}

void main() {
    vec3 ground = ground();
    vec3 chandelier = chandelier();

    gl_FragColor = vec4(
                        mix(ground, chandelier, step(0.0, 1.0 - length(getOffset()))),
                        1.0
                        );
}