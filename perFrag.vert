#version 410

uniform mat4 mvp;      // A constant representing the combined model/view/projection matrix.
uniform mat4 mvm;       // A constant representing the combined model/view matrix.

in vec4 vtx;     // Per-vertex position information we will pass in.       // Per-vertex color information we will pass in.
in vec4 normal;       // Per-vertex normal information we will pass in.

out vec4 v_Position;       // This will be passed into the fragment shader.         // This will be passed into the fragment shader.
out vec4 v_Normal;         // This will be passed into the fragment shader.

// The entry point for our vertex shader.
void main()
{
    // Transform the vertex into eye space.
    v_Position = vtx;

    // Transform the normal's orientation into eye space.
    v_Normal = normal;

    // gl_Position is a special variable used to store the final position.
    // Multiply the vertex by the matrix to get the final point in normalized screen coordinates.
    gl_Position = mvp * vtx;
}