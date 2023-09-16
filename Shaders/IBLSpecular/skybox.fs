#version 330 core
in vec3 TexCoords;

uniform samplerCube skybox_texture;

out vec4 FragColor;

void main()
{
    vec3 color = textureLod(skybox_texture, TexCoords, 2.0).rgb;

    FragColor = vec4(color, 1.0);
}