// ConstantBuffer��ҪOpenGL3 ��OpenGL3��֧��UBO��ConstantBuffer�Ƕ�ӦDx�Ľз���

#if !defined(GL3) || !defined(USE_CBUFFERS)

// OpenGL 2 uniforms (��֧��UBO)

//Vertex Shader ����
#ifdef COMPILEVS
          
// Vertex shader uniforms
uniform vec3 cAmbientStartColor;        //������ʼ��ɫ
uniform vec3 cAmbientEndColor;          //����������ɫ
uniform mat3 cBillboardRot;             //�������ת����
uniform vec3 cCameraPos;                //���λ��
uniform mat3 cCameraRot;                //�����ת
uniform float cNearClip;                //����ƽ��
uniform float cFarClip;                 //Զ��ƽ��
uniform vec4 cDepthMode;                //���ģʽ
uniform vec3 cFrustumSize;              //��׵���С
uniform float cDeltaTime;               //����ʱ��
uniform float cElapsedTime;             //����ȥʱ��
uniform vec4 cGBufferOffsets;           //GBufferƫ��
uniform vec3 cLightDir;                 //��Դ����
uniform vec4 cLightPos;                 //��Դλ��
uniform mat4 cModel;                    //model����
uniform mat4 cViewProj;                 //view����
uniform vec4 cUOffset;                  //uƫ��
uniform vec4 cVOffset;                  //vƫ��
uniform mat4 cZone;
#if !defined(GL_ES) || defined(WEBGL)
    uniform mat4 cLightMatrices[4];
#else
    uniform mat4 cLightMatrices[2];
#endif
#ifdef SKINNED
    //��Ƥ�����趨��MAXBONES
    uniform vec4 cSkinMatrices[MAXBONES*3];
#endif
#ifdef NUMVERTEXLIGHTS
    uniform vec4 cVertexLights[4*3];       //��󶥵��Դ��
#endif
#ifdef GL3
    uniform vec4 cClipPlane;                //����ƽ��
#endif
#endif

//PixelShader ����
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

// OpenGL 3 uniforms (֧��UBO)
//Vertex Shader ����
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


//Pixel Shader����
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
