uniform sampler2D texture;

void main()
{
    vec4 color = texture2D(texture, gl_TexCoord[0].xy);

    // delikatne przyciemnienie
    color.rgb *= 0.5;

    gl_FragColor = color;
}