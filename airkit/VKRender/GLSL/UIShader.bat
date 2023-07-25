glslc.exe UIShader.glsl.vert -o UIShaderVert.spv
glslc.exe UIShader.glsl.frag -o UIShaderFrag.spv
E:\Tools\restool.exe -i UIShaderVert.spv UIShaderFrag.spv -o UIShader.h