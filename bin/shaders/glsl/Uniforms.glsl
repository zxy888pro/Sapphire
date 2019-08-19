// ConstantBuffer需要OpenGL3 （OpenGL3才支持UBO，ConstantBuffer是对应Dx的叫法）

#if !defined(GL3) || !defined(USE_CBUFFERS)

// OpenGL 2 uniforms (不支持UBO)

//Vertex Shader 部分
#ifdef COMPILEVS
          
// Vertex shader uniforms
uniform vec3 cAmbientStartColor;        //动画开始颜色
uniform vec3 cAmbientEndColor;          //动画结束颜色
uniform mat3 cBillboardRot;             //公告板旋转矩阵
uniform vec3 cCameraPos;                //相机位置
uniform mat3 cCameraRot;                //相机旋转
uniform float cNearClip;                //近裁平面
uniform float cFarClip;                 //远裁平面
uniform vec4 cDepthMode;                //深度模式
uniform vec3 cFrustumSize;              //视椎体大小
uniform float cDeltaTime;               //增量时间
uniform float cElapsedTime;             //已逝去时间
uniform vec4 cGBufferOffsets;           //GBuffer偏移
uniform vec3 cLightDir;                 //光源方向
uniform vec4 cLightPos;                 //光源位置
uniform mat4 cModel;                    //model矩阵
uniform mat4 cViewProj;                 //view矩阵
uniform vec4 cUOffset;                  //u偏移
uniform vec4 cVOffset;                  //v偏移
uniform mat4 cZone;
#if !defined(GL_ES) || defined(WEBGL)
    uniform mat4 cLightMatrices[4];
#else
    uniform mat4 cLightMatrices[2];
#endif
#ifdef SKINNED
    //蒙皮矩阵需定义MAXBONES
    uniform vec4 cSkinMatrices[MAXBONES*3];
#endif
#ifdef NUMVERTEXLIGHTS
    uniform vec4 cVertexLights[4*3];       //最大顶点光源数
#endif
#ifdef GL3
    uniform vec4 cClipPlane;                //剪裁平面
#endif
#endif

//PixelShader 部分
#ifdef COMPILEPS

// Pixel shader uniforms
#ifdef GL_ES
    precision mediump float;
#endif

uniform vec3 cAmbientColor;
uniform vec3 cCameraPosPS;
uniform float cDeltaTimePS;
uniform vec4 cDepthReconstruct;
uniform float cElapsedTimePS;
uniform vec4 cFogParams;
uniform vec3 cFogColor;
uniform vec2 cGBufferInvSize;
uniform vec4 cLightColor;
uniform vec3 cLightDirPS;
uniform vec4 cLightPosPS;
uniform vec4 cMatDiffColor;
uniform vec3 cMatEmissiveColor;
uniform vec3 cMatEnvMapColor;
uniform vec4 cMatSpecColor;
uniform float cNearClipPS;
uniform float cFarClipPS;
uniform vec4 cShadowCubeAdjust;
uniform vec4 cShadowDepthFade;
uniform vec2 cShadowIntensity;
uniform vec2 cShadowMapInvSize;
uniform vec4 cShadowSplits;
uniform mat4 cLightMatricesPS[4];

#endif

#else

// OpenGL 3 uniforms (支持UBO)
//Vertex Shader 部分
#ifdef COMPILEVS

uniform FrameVS
{
    float cDeltaTime;
    float cElapsedTime;
};

uniform CameraVS
{
    vec3 cCameraPos;
    mat3 cCameraRot;
    float cNearClip;
    float cFarClip;
    vec4 cDepthMode;
    vec3 cFrustumSize;
    vec4 cGBufferOffsets;
    mat4 cViewProj;
    vec4 cClipPlane;
};

uniform ZoneVS
{
    vec3 cAmbientStartColor;
    vec3 cAmbientEndColor;
    mat4 cZone;
};

uniform LightVS
{
    vec3 cLightDir;
    vec4 cLightPos;
#ifdef NUMVERTEXLIGHTS
    vec4 cVertexLights[4 * 3];
#else
    mat4 cLightMatrices[4];
#endif
};

#ifndef CUSTOM_MATERIAL_CBUFFER
uniform MaterialVS
{
    vec4 cUOffset;
    vec4 cVOffset;
};
#endif

uniform ObjectVS
{
    mat4 cModel;
#ifdef BILLBOARD
    mat3 cBillboardRot;
#endif
#ifdef SKINNED
    uniform vec4 cSkinMatrices[MAXBONES*3];
#endif
};

#endif


//Pixel Shader部分
#ifdef COMPILEPS

// Pixel shader uniforms
uniform FramePS
{
    float cDeltaTimePS;
    float cElapsedTimePS;
};

uniform CameraPS
{
    vec3 cCameraPosPS;
    vec4 cDepthReconstruct;
    vec2 cGBufferInvSize;
    float cNearClipPS;
    float cFarClipPS;
};

uniform ZonePS
{
    vec3 cAmbientColor;
    vec4 cFogParams;
    vec3 cFogColor;
};

uniform LightPS
{
    vec4 cLightColor;
    vec4 cLightPosPS;
    vec3 cLightDirPS;
    vec4 cShadowCubeAdjust;
    vec4 cShadowDepthFade;
    vec2 cShadowIntensity;
    vec2 cShadowMapInvSize;
    vec4 cShadowSplits;
    mat4 cLightMatricesPS[4];
};

#ifndef CUSTOM_MATERIAL_CBUFFER
uniform MaterialPS
{
    vec4 cMatDiffColor;
    vec3 cMatEmissiveColor;
    vec3 cMatEnvMapColor;
    vec4 cMatSpecColor;
};
#endif

#endif

#endif
