#ifdef COMPILEPS
uniform sampler2D sDiffMap;
uniform samplerCube sDiffCubeMap;
uniform sampler2D sNormalMap;
uniform sampler2D sSpecMap;
uniform sampler2D sEmissiveMap;
uniform sampler2D sEnvMap;
uniform samplerCube sEnvCubeMap;
uniform sampler2D sLightRampMap;
uniform sampler2D sLightSpotMap;
uniform samplerCube sLightCubeMap;
#ifndef GL_ES
    uniform sampler3D sVolumeMap;
    uniform sampler2D sAlbedoBuffer;
    uniform sampler2D sNormalBuffer;
    uniform sampler2D sDepthBuffer;
    uniform sampler2D sLightBuffer;
    uniform sampler2DShadow sShadowMap;
    uniform samplerCube sFaceSelectCubeMap;
    uniform samplerCube sIndirectionCubeMap;
    uniform samplerCube sZoneCubeMap;
    uniform sampler3D sZoneVolumeMap;
#else
    uniform sampler2D sShadowMap;
#endif

#ifdef GL3
#define texture2D texture
#define texture2DProj textureProj
#define texture3D texture
#define textureCube texture
#define texture2DLod textureLod
#define texture2DLodOffset textureLodOffset
#endif

vec3 DecodeNormal(vec4 normalInput)
{
    #ifdef PACKEDNORMAL
        vec3 normal;
        normal.xy = normalInput.ag * 2.0 - 1.0;
        normal.z = sqrt(max(1.0 - dot(normal.xy, normal.xy), 0.0));
        return normal;
    #else
        return normalize(normalInput.rgb * 2.0 - 1.0);
    #endif
}

//把一个depth编码到RGB中
vec3 EncodeDepth(float depth)
{
    #ifndef GL3
        //针对OpenGL3 废掉gl_FragData
        vec3 ret;
        depth *= 255.0;
        ret.x = floor(depth);
        depth = (depth - ret.x) * 255.0;
        ret.y = floor(depth);
        ret.z = (depth - ret.y);
        ret.xy *= 1.0 / 255.0;
        return ret;
    #else
        // OpenGL3 废掉gl_FragData, gl_FragColor,可以使用不同的MRT格式，不需要这样编码
        return vec3(depth, 0.0, 0.0);
    #endif
}
//从一个RGB中解码深度信息
float DecodeDepth(vec3 depth)
{
    #ifndef GL3
        const vec3 dotValues = vec3(1.0, 1.0 / 255.0, 1.0 / (255.0 * 255.0));
        return dot(depth, dotValues);
    #else
       // OpenGL3 可以使用不同的MRT格式，不需要这样编码
        return depth.r;
    #endif
}

float ReconstructDepth(float hwDepth)
{
    return dot(vec2(hwDepth, cDepthReconstruct.y / (hwDepth - cDepthReconstruct.x)), cDepthReconstruct.zw);
}
#endif
