uniform sampler2D Texture_0;
varying vec2 Tex2f_0;

void main(void)
{
    gl_FragColor = texture2D(Texture_0, Tex2f_0.st);
}
