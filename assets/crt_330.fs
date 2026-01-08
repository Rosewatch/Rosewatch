#version 330

in vec2 fragTexCoord;
out vec4 finalColor;

uniform sampler2D texture0;
uniform float time;
uniform vec2 resolution = vec2(1280, 720);
uniform vec4 colDiffuse;

uniform float scanlineStrength = 0.3f; // 0.0 - 1.0
uniform float glowStrength     = 0.25f; // 0.0 - 1.0
uniform float noiseStrength    = 0.075f; // 0.0 - 1.0
uniform float warpStrength     = 0.015f; // 0.0 - 1.0

const float samples = 5.0;          // Pixels per axis; higher = bigger glow, worse performance
const float quality = 2.0;          // Defines size factor: Lower = smaller glow, better quality

float rand(vec2 co) {
    return fract(sin(dot(co, vec2(12.9898, 78.233))) * 43758.5453);
}

void main() {
    // --- Bloom --
    vec4 sum = vec4(0);
    vec2 sizeFactor = vec2(1)/resolution*quality;

    // Texel color fetching from texture sampler
    vec4 source = texture(texture0, fragTexCoord);

    const int range = 2;            // should be = (samples - 1)/2;

    for (int x = -range; x <= range; x++) {
        for (int y = -range; y <= range; y++) {
            sum += texture2D(texture0, fragTexCoord + vec2(x, y)*sizeFactor);
        }
    }

    // --- Subtle CRT curvature (UI-safe) ---
    vec2 uv = fragTexCoord * 2.0 - 1.0; // to -1..1
    float r2 = uv.x * uv.x + uv.y * uv.y;

    // Barrel distortion (very gentle)
    uv += uv * r2 * warpStrength;

    // Back to 0..1
    uv = uv * 0.5 + 0.5;

    // Prevent edge sampling artifacts
    uv = clamp(uv, 0.0, 1.0);

    vec3 color = (((sum/(samples*samples)) + source)*colDiffuse).rgb;

    // Scanlines
    float scanline = sin(uv.y * resolution.y * 3.141592);
    scanline = mix(1.0, scanline, scanlineStrength);
    color *= scanline;

    // Noise
    float noise = rand(uv * resolution + time);
    color += (noise - 0.5) * noiseStrength;

    // Glow / phosphor persistence
    color += color * glowStrength;

    // Vignette
    float dist = distance(uv, vec2(0.5));
    color *= smoothstep(0.85, 0.45, dist);

    finalColor = vec4(color, 1.0);
}
