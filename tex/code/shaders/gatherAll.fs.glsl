#version 330 core

out vec4 color;

uniform sampler2D TEX_C; uniform sampler2D TEX_Z;
uniform sampler2D TEX_VL; uniform sampler2D TEX_NM;

uniform int K; uniform int S;

void main() {
    vec2  X       = gl_FragCoord.xy;
    ivec2 iX      = ivec2(X);
    ivec2 iXByk   = iX / K;

    vec2 vnX        = texelFetch(TEX_NM, iXByk, 0).xy;
    vec2 vX         = texelFetch(TEX_VL, iX, 0).xy;   
    vec3 cX         = texelFetch(TEX_C,  iX, 0).rgb;  
    float zX        = readDepth(iX);


    if(length(vnX) < 0.005){
            color = texelFetch(TEX_C, iX, 0); /* No BLUR */
            return;
    }

    float lv        = clamp(length(vX), 1.0, K);

    float weight    = 1.0 / lv; 
    vec3  sum       = cX * weight;
    float j         = rand(X) - 0.5;

    for(int i = 0; i < S; i++){
        if(i == (S - 1)/2) continue;
        
        float t  = mix(-1.0, 1.0, (float(i)+1.0+j) / float(S+1));
        vec2 Y   = floor(X + vnX * t + vec2(0.5));
        ivec2 iY = ivec2(Y);

        float zY = readDepth(iY);
        vec2  vY = texelFetch(TEX_VL, iY, 0).xy;

        float f  = softDepthCompare(zX, zY);
        float b  = softDepthCompare(zY, zX);

        float aY = f * cone(Y, X, vY) +
                   b * cone(X, Y, vX) +
                   cylinder(Y, X, vY) * cylinder(X, Y, vX) * 2.0;

        vec3 cY  = texelFetch(TEX_C, iY, 0).rgb;

        weight += aY; sum += aY * cY;
    }

    color = vec4(sum / weight, 1.0);
};
