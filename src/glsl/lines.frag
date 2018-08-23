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

    p.y += 0.3 * sin(time / 5.0);

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
        fract(r / 15.0),
        1.0,
        (0.5 + 0.5 * random(vec2(5.0, r))) * b
    ));

    return color;
}

vec3 chandelier() {
    float t = time / 40.0;
    vec3 color = vec3(0.0);

    const float c = 5.0;
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
    float t = time / 40.0 * 5.0;

    float h = snoise_0(vec2(10.0, floor(t)));
    float h2 = snoise_0(vec2(10.0, floor(t) + 1.0));

    vec2 p = getLocation().xz;
    float t2 = time / 5.0;
    vec3 c = hsv2rgb(vec3(
        h + 0.05 * snoise_1(vec3(p, t2)),
        1.0,
        (0.7 + 0.2 * snoise_1(vec3(p, t2)))
    ));
    vec3 c2 = hsv2rgb(vec3(
        h2 + 0.05 * snoise_1(vec3(p, t2)),
        1.0,
        (0.7 + 0.2 * snoise_1(vec3(p, t2)))
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