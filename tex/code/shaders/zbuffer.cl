void triangle(
         const float4 *pts, global float *zbuffer,
         global float2 *velocity, global uchar4 *image,
         uint w, uint h, uchar4 color,
         float16 MVP, float16 iMVP, float16 MVP_old
) {
    float4 t0 = pts[0]; float4 t1 = pts[1]; float4 t2 = pts[2];

    if (t0.y==t1.y && t0.y==t2.y) return;
    
    if (t0.y>t1.y) _swap_float4(&t0, &t1);
    if (t0.y>t2.y) _swap_float4(&t0, &t2);
    if (t1.y>t2.y) _swap_float4(&t1, &t2);

  
    int total_height = t2.y-t0.y;
    for (int i=0; i < total_height; i++) {
        bool second_half = i>t1.y-t0.y || t1.y==t0.y;
        int segment_height = second_half ? t2.y-t1.y : t1.y-t0.y;
        float alpha = (float)i/total_height;
        float beta  = (float)(i-(second_half ? t1.y-t0.y : 0))/segment_height;
        float4 A =               t0 + (float4)(t2-t0)*alpha;
        float4 B = second_half ? t1 + (float4)(t2-t1)*beta : t0 + (float4)(t1-t0)*beta;

        if (A.x>B.x) _swap_float4(&A, &B);
        for (int j=A.x; j <= B.x; j++) {
            float phi = B.x==A.x ? 1. : (float)(j-A.x)/(float)(B.x-A.x);
            float4 P = (float4)(A) + (float4)(B-A)*phi;

            if (P.x >= w || P.y >= h || P.x < 0 || P.y < 0) continue;

            uint x = j; uint y = t0.y + i; 
            uint pos = (uint)(x) + (uint)(y) * w;
            
            if (zbuffer[pos] < P.z && P.z < -10.f) {
                zbuffer[pos] = P.z;
                image[pos] = color;
                float4 oldP = MVP_old * IMVP * P;
                velocity[pos] = (float2)(oldP.x - P.x, oldP.y - P.y);
            }
        }
    }
}