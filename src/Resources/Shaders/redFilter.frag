uniform sampler2D texture;

void main()
{
    vec4 originalColor = texture2D(texture, gl_TexCoord[0].xy);
    
    vec4 tintColor = vec4(1.0, 0.0, 0.0, 1.0); // Czysta czerwieñ

    float mixStrength = 0.15;

    vec4 finalColor = mix(originalColor, tintColor, mixStrength);
  
    finalColor.a = originalColor.a;
    gl_FragColor = finalColor;
}
