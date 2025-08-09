#version 330

in vec3 fragPosition;
in vec2 fragTexCoord;
in vec3 fragNormal;
in vec3 worldPosition;

uniform sampler2D texture0;
uniform vec4 colDiffuse;

out vec4 finalColor;

// Simple noise function for texture generation
float noise(vec2 p) {
    return fract(sin(dot(p, vec2(127.1, 311.7))) * 43758.5453);
}

float fbm(vec2 p) {
    float value = 0.0;
    float amplitude = 0.5;
    float frequency = 1.0;
    
    for (int i = 0; i < 4; i++) {
        value += amplitude * noise(p * frequency);
        amplitude *= 0.5;
        frequency *= 2.0;
    }
    
    return value;
}

// Generate procedural texture based on tile ID and position
vec3 generateTileTexture(float tileId, vec2 uv) {
    vec3 color = vec3(0.5);
    
    // Different patterns based on tile ID
    if (tileId < 2.0) { // Grass-like tile (tile 1)
        float grass = fbm(uv * 8.0 + tileId);
        color = mix(vec3(0.2, 0.6, 0.1), vec3(0.4, 0.8, 0.2), grass);
        
        // Add some variation
        float variation = noise(uv * 16.0 + tileId);
        color = mix(color, vec3(0.15, 0.5, 0.08), variation * 0.3);
    } else if (tileId < 250.0) { // Land tiles
        float pattern = fbm(uv * 6.0 + tileId * 0.1);
        
        // Rock/stone pattern
        color = mix(vec3(0.4, 0.35, 0.3), vec3(0.6, 0.55, 0.5), pattern);
        
        // Add some detail
        float detail = noise(uv * 32.0 + tileId);
        color = mix(color, color * 1.2, detail * 0.2);
    } else { // Water tiles (>= 250)
        float wave = sin(uv.x * 16.0 + tileId) * sin(uv.y * 16.0 + tileId * 0.5);
        color = mix(vec3(0.1, 0.3, 0.7), vec3(0.2, 0.5, 0.9), wave * 0.5 + 0.5);
        
        // Water ripples
        float ripple = fbm(uv * 12.0 + tileId * 0.2);
        color = mix(color, color * 1.3, ripple * 0.3);
    }
    
    return color;
}

void main()
{
    vec3 normal = normalize(fragNormal);
    
    // Get tile ID from texture atlas (assuming R channel stores tile ID)
    float tileId = texture(texture0, fragTexCoord).r * 255.0;
    
    // Calculate triplanar mapping weights
    vec3 blendWeights = abs(normal);
    blendWeights = blendWeights / (blendWeights.x + blendWeights.y + blendWeights.z);
    
    // Scale UV coordinates to respect 4x4 tile size
    float tileScale = 0.25; // 1/4 for 4x4 tiles
    
    // Calculate UVs for each plane
    vec2 uvX = worldPosition.yz * tileScale;
    vec2 uvY = worldPosition.xz * tileScale;
    vec2 uvZ = worldPosition.xy * tileScale;
    
    // Generate textures for each plane
    vec3 colorX = generateTileTexture(tileId, uvX);
    vec3 colorY = generateTileTexture(tileId, uvY);
    vec3 colorZ = generateTileTexture(tileId, uvZ);
    
    // Blend the three planes based on normal
    vec3 finalTexture = colorX * blendWeights.x + 
                       colorY * blendWeights.y + 
                       colorZ * blendWeights.z;
    
    // Simple lighting
    vec3 lightDir = normalize(vec3(0.5, 1.0, 0.3));
    float NdotL = max(dot(normal, lightDir), 0.0);
    float lighting = 0.3 + 0.7 * NdotL; // Ambient + diffuse
    
    finalColor = vec4(finalTexture * lighting * colDiffuse.rgb, colDiffuse.a);
}