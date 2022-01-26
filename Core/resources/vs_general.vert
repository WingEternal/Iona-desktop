attribute vec4 Pos4f;
attribute vec2 TexCoord2f_0;
uniform mat4 Transform;
varying vec2 Tex2f_0;

void main(void)
{
    gl_Position = Transform * Pos4f;
    Tex2f_0 = TexCoord2f_0;
}
