#include "Uniforms.glsl"
#include "Samplers.glsl"
#include "Transform.glsl"

#if defined(DIFFMAP) || defined(ALPHAMAP)
    varying vec2 vTexCoord;
#endif
#ifdef VERTEXCOLOR
    varying vec4 vColor;
#endif


void main()
{
    vec4 diffColor = cMatDiffColor;

    #ifdef VERTEXCOLOR
        diffColor *= vColor;
    #endif
    //¶¥µãÉ«
    #if (!defined(DIFFMAP)) && (!defined(ALPHAMAP))
        gl_FragColor = diffColor;
    #endif
    //Âþ·´ÉäÌùÍ¼
    #ifdef DIFFMAP
        vec4 diffInput = texture2D(sDiffMap, vTexCoord);
        #ifdef ALPHAMASK
            if (diffInput.a < 0.5)
                discard;
        #endif
        gl_FragColor = diffColor * diffInput;
    #endif
    //alphaÌùÍ¼
    #ifdef ALPHAMAP
        #ifdef GL3
            float alphaInput = texture2D(sDiffMap, vTexCoord).r;
        #else
            float alphaInput = texture2D(sDiffMap, vTexCoord).a;
        #endif
        gl_FragColor = vec4(diffColor.rgb, diffColor.a * alphaInput);
    #endif
}
