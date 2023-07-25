#version 450 core
layout(location = 0) out vec4 fColor;
layout(set=0, binding=0) uniform sampler2D sTexture;
layout(location = 0) in struct { vec4 Pos;vec4 Color; vec3 UV; } In;
void main()
{
    //没有纹理
    if(In.Pos.z==0){
        fColor=In.Color;
    }else{
        fColor = In.Color * texture(sTexture, In.UV.st);
    }
    
}