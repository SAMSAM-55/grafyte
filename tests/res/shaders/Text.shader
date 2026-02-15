#shader vertex
#version 330 core

layout(location = 0) in vec4 a_PosUV; 
// xy = position
// zw = uv

out vec2 v_UV;

uniform mat4 u_MVP;

void main()
{
    v_UV = a_PosUV.zw;
    gl_Position = u_MVP * vec4(a_PosUV.xy, 0.0, 1.0);
}

#shader fragment
#version 330 core

in vec2 v_UV;

uniform sampler2D u_TextTexture;
uniform vec4 u_TextColor;

out vec4 color;

void main()
{
    float alpha = texture(u_TextTexture, v_UV).r;
    color = vec4(u_TextColor.rgb, alpha * u_TextColor.a);
}
