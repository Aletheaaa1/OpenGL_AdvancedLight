#version 330 core
in vec3 TexCoords;

uniform samplerCube skybox_texture;

out vec4 FragColor;

void main()
{
    vec3 color = texture(skybox_texture, TexCoords).rgb;

    FragColor = vec4(color, 1.0);
}