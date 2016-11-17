#version 410

uniform vec3 light[6];       // The position of the light in eye space.
uniform mat4 mvm;
uniform vec4 eye;

in vec4 v_Position;       // Interpolated position for this fragment.

in vec4 v_Normal;         // Interpolated normal for this fragment.

out vec4 fragmentColor;

// The entry point for our fragment shader.
void main()
{

    // Transform the vertex into eye space.
    vec4 modelViewVertex = mvm * v_Position;

    mat4 normalMatrix = transpose(inverse(mvm));

    // Transform the normal's orientation into eye space.
    vec4 n = normalMatrix * v_Normal;

    vec4 v = normalize(eye - modelViewVertex);

    vec4 l = vec4(0.8, 0.8, 0.8, 1.0);  // intensidade RGB da LUZ
    vec4 kd = vec4(0.0, 0.0, 1.0, 1.0); // cor difusa                       COR DO OBJETO
    vec4 ks = vec4(1.0, 1.0, 1.0, 1.0); // cor especular
    int coef = 50;                //  coeficiente especular

    vec4 la = vec4(0.2, 0.2, 0.2, .0); // intensidade RGB da LUZ AMBIENTE

    vec4 DIFUSE   = vec4( (l.x * kd.x),(l.y * kd.y),(l.z * kd.z),(l.w * kd.w) );
    vec4 SPECULAR = vec4( (l.x*ks.x), (l.y*ks.y), (l.z*ks.z), (l.w * ks.w) );

    vec4 AMBIENTE = vec4( (la.x * kd.x), (la.y * kd.y), (la.z * kd.z), (la.w * kd.w) );

    for(int i = 0; i < 6; i++)
    {
        vec4 L = normalize(vec4(light[i].x, light[i].y, light[i].z, 1.0) - modelViewVertex);
        vec4 r = 2*dot(n,L)*(n - L);
        vec4 DIFUSE_DOT   = max(dot(n,L), 0.0) * DIFUSE;
        vec4 SPECULAR_DOT = pow(max(dot(r,v),0.0) , coef) * SPECULAR;

        AMBIENTE += DIFUSE_DOT + SPECULAR_DOT;
    }

    fragmentColor = AMBIENTE;
}