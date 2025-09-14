#version 330 core
uniform int size; // 1024 for example

// You’ll need a function that returns tile ID at (x, z)
int tileAt(int x, int z) {
    // Your procedural logic here, e.g.:
    // return (x + z) % 256;
    // Replace with your actual tilegen
    return (x + z) % 256;
}

out vec4 FragColor;

void main() {
    ivec2 pix = ivec2(gl_FragCoord.xy) - ivec2(0, 0); // pixel coords

    int x = pix.x;
    int zBase = pix.y * 4;

    int t0 = tileAt(x, zBase);
    int t1 = tileAt(x, zBase + 1);
    int t2 = tileAt(x, zBase + 2);
    int t3 = tileAt(x, zBase + 3);

    FragColor = vec4(t0, t1, t2, t3) / 255.0;
}
