#version 120

varying vec2 UV;
uniform sampler2D myTextureSampler;

void main(void)
{
    /* texture flipping: */
    vec2 temp;
    temp.x = UV.x;
    temp.y = 1.0 - UV.y;
    gl_FragColor = texture2D(myTextureSampler, temp);

    /* No texture flipping:
    gl_FragColor = texture2D(myTextureSampler, UV);
    */
}
