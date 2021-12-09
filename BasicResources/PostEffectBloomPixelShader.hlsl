#include "PostEffectBloomHeader.hlsli"



//float4 Bluer(Texture2D<float4> tex, SamplerState smp, float2 uv, float dx, float dy)
//{
//    float4 ret = float4(0, 0, 0, 0);
    
    
//    ret += bkweights[0] * tex.Sample(smp, uv);
    
//    for (int i = 1; i < 3; ++i)
//    {
//        //ret += bkweights[i >> 2][i % 4] * tex.Sample(smp, uv + float2(i * dx, i * dy));
//        //ret += bkweights[i >> 2][i % 4] * tex.Sample(smp, uv + float2(-i * dx, -i * dy));
//        ret += bkweights[i >> 2][i % 4] * tex.Sample(smp, uv + float2(i * dx, 0));
//        ret += bkweights[i >> 2][i % 4] * tex.Sample(smp, uv + float2(-i * dx, 0));
//    }

//    return ret;
//}

//float4 HorizontalBluer(Texture2D<float4> tex, SamplerState smp, float2 uv, float dx, float dy)
//{
//    float4 ret = float4(0, 0, 0, 0);
    
    
//    ret += bkweights[0] * tex.Sample(smp, uv);
    
//    for (int i = 1; i < 3; ++i)
//    {
//        //ret += bkweights[i >> 2][i % 4] * tex.Sample(smp, uv + float2(i * dx, i * dy));
//        //ret += bkweights[i >> 2][i % 4] * tex.Sample(smp, uv + float2(-i * dx, -i * dy));
//        ret += bkweights[i >> 2][i % 4] * tex.Sample(smp, uv + float2(0, i * dx));
//        ret += bkweights[i >> 2][i % 4] * tex.Sample(smp, uv + float2(0, -i * dx));
//    }

//    return ret;
//}

float4 VerticalBlur(Texture2D<float4> tex, float2 uv, float dy, float filletrRadius, float bluerScale)
{
    float sum = 0;
    float wsum = 0;

    for (float y = -filletrRadius; y <= filletrRadius; y += 1.0f)
    {
        float4 sample = tex.Sample(smp, uv + float2(0, dy * y));

        float r = y * bluerScale;
        float w = exp(-r * r);


        sum += sample * w;
        wsum += w;
    }

    if (wsum > 0.0f)
    {
        sum /= wsum;
    }

    return sum;
}

float4 HorizontalBlur(Texture2D<float4> tex, float2 uv, float dx, float filletrRadius, float bluerScale)
{
    float sum = 0;
    float wsum = 0;

    for (float x = -filletrRadius; x <= filletrRadius; x += 1.0f)
    {
        float4 sample = tex.Sample(smp, uv + float2(dx * x, 0));

        float r = x * bluerScale;
        float w = exp(-r * r);


        sum += sample * w;
        wsum += w;
    }

    if (wsum > 0.0f)
    {
        sum /= wsum;
    }

    return sum;
}


float4 VerticalBokehPS(Output input) : SV_TARGET
{
    float w, h, miplevels;
    tex.GetDimensions(0, w, h, miplevels);
    float dx = 1.0 / w;
    float dy = 1.0 / h;
    
    float4 bloomAccum = float4(0, 0, 0, 0);
    float2 uvSize = float2(1, 1);
    float2 uvOfst = float2(0, 0);
        
    bloomAccum += VerticalBlur(texShrinkHighLum0, input.uv, dy, 8, 0.112f);
           
    bloomAccum += VerticalBlur(texShrinkHighLum1, input.uv, dy, 8, 0.112f);
    
    bloomAccum += VerticalBlur(texShrinkHighLum2, input.uv, dy, 8, 0.112f);
    
    bloomAccum += VerticalBlur(texShrinkHighLum3, input.uv, dy, 8, 0.112f);
	
    //Get5x5GaussianBlur(texHighLum, smp, input.uv, dx, dy, float4(uvOfst, uvOfst + uvSize)) +
    //saturate(bloomAccum);
                      
    return tex.Sample(smp, input.uv) + saturate(bloomAccum);

}

float getBrightness(float3 color)
{
    return max(color.r, max(color.g, color.b));
}



//�@�u���[
float4 BlurPS(Output input) : SV_Target
{
    float w, h, miplevels;
    tex.GetDimensions(0, w, h, miplevels);
    
    float dx = 1.0f / w;
    float dy = 1.0f / h;
    float2 uvSize = float2(0.5f, 0.5f);
    float2 uvOfst = float2(0, 0);
    
    float4 ret = float4(0, 0, 0, 0);
    
    
    return HorizontalBlur(texHighLum, input.uv, dx, 8, 0.112f);
}

float4 main(Output input) : SV_Target
{
    float w, h, level;
    tex.GetDimensions(0, w, h, level);
    
    float dx = 1.0f / w;
    float dy = 1.0f / h;
    
    float4 col = tex.Sample(smp, input.uv);

    float brightness = getBrightness(col.rgb);
    
    float contribution = max(0, brightness - 0.8f);
    contribution /= max(brightness, 0.00001);
        
    return col * contribution;
}