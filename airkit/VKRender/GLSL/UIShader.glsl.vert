#version 450 core
layout(location = 0) in vec3 vPos;
layout(location = 1) in vec3 vUVW;
layout(location = 2) in vec4 vClr;
layout(push_constant) uniform uPushConstant {mat4 mvp;} cv;

out gl_PerVertex { vec4 gl_Position; };
layout(location = 0) out struct { vec3 Pos;vec4 Color; vec3 UV; } Out;

void main()
{
    gl_Position = cv.mvp* vec4(vPos.xy, 0.0, 1.0);
    Out.Color = vClr;
    Out.UV = vUVW;
    Out.Pos==vPos;
}