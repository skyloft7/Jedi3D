#version 330
in vec2 f_texcoord;
uniform sampler2D texture_sampler;

void main()
{
    gl_FragColor = texture(texture_sampler, f_texcoord);
}