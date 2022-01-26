uniform sampler2D Texture_0;
uniform sampler2D Texture_1;
uniform float Delta1f_Tex_S;
varying vec2 Tex2f_0;

void main(void)
{
    vec4 texture_0_color = texture2D(Texture_0, Tex2f_0.st + vec2(Delta1f_Tex_S, 0.0));
    vec4 texture_1_color = texture2D(Texture_1, Tex2f_0.st);
    float mix_alpha = texture_0_color.a + texture_1_color.a - texture_0_color.a * texture_1_color.a;
    vec3 mix_rgb = (texture_0_color.a * (1 - texture_1_color.a) * texture_0_color.rgb + texture_1_color.a * texture_1_color.rgb) / mix_alpha;
    gl_FragColor = vec4(mix_rgb, mix_alpha);
}
