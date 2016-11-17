#version 410

out vec4 fragmentColor;

in vec4 v_Color;

void main(void)
{
    fragmentColor = v_Color;
}
