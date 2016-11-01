#if 0
//
// Generated by Microsoft (R) HLSL Shader Compiler 6.3.9600.16384
//
//
// Buffer Definitions: 
//
// cbuffer OBJECT
// {
//
//   row_major float4x4 worldMatrix;    // Offset:    0 Size:    64
//   row_major float4x4 viewMatrix;     // Offset:   64 Size:    64
//   row_major float4x4 projectionMatrix;// Offset:  128 Size:    64
//   float ScreenHeight;                // Offset:  192 Size:     4 [unused]
//   float3 pad;                        // Offset:  196 Size:    12 [unused]
//
// }
//
//
// Resource Bindings:
//
// Name                                 Type  Format         Dim Slot Elements
// ------------------------------ ---------- ------- ----------- ---- --------
// OBJECT                            cbuffer      NA          NA    0        1
//
//
//
// Input signature:
//
// Name                 Index   Mask Register SysValue  Format   Used
// -------------------- ----- ------ -------- -------- ------- ------
// POSITION                 0   xyzw        0     NONE   float   xyz 
// UV                       0   xyz         1     NONE   float       
// NORMALS                  0   xyz         2     NONE   float       
//
//
// Output signature:
//
// Name                 Index   Mask Register SysValue  Format   Used
// -------------------- ----- ------ -------- -------- ------- ------
// SV_POSITION              0   xyzw        0      POS   float   xyzw
// UV                       0   xyz         1     NONE   float   xyz 
// NORMAL                   0   xyz         2     NONE   float   xyz 
//
gs_4_0
dcl_constantbuffer cb0[12], immediateIndexed
dcl_input v[1][0].xyzw
dcl_input v[1][1].xyz
dcl_input v[1][2].xyz
dcl_temps 4
dcl_inputprimitive point 
dcl_outputtopology trianglestrip 
dcl_output_siv o0.xyzw, position
dcl_output o1.xyz
dcl_output o2.xyz
dcl_maxout 4
add r0.xyzw, l(-2.000000, 2.000000, 2.000000, -2.000000), v[0][0].xyxy
mul r1.xyzw, r0.yyyy, cb0[1].xyzw
mad r2.xyzw, r0.xxxx, cb0[0].xyzw, r1.xyzw
mad r1.xyzw, r0.zzzz, cb0[0].xyzw, r1.xyzw
mad r1.xyzw, v[0][0].zzzz, cb0[2].xyzw, r1.xyzw
add r1.xyzw, r1.xyzw, cb0[3].xyzw
mad r2.xyzw, v[0][0].zzzz, cb0[2].xyzw, r2.xyzw
add r2.xyzw, r2.xyzw, cb0[3].xyzw
mul r3.xyzw, r2.yyyy, cb0[5].xyzw
mad r3.xyzw, r2.xxxx, cb0[4].xyzw, r3.xyzw
mad r3.xyzw, r2.zzzz, cb0[6].xyzw, r3.xyzw
mad r2.xyzw, r2.wwww, cb0[7].xyzw, r3.xyzw
mul r3.xyzw, r2.yyyy, cb0[9].xyzw
mad r3.xyzw, r2.xxxx, cb0[8].xyzw, r3.xyzw
mad r3.xyzw, r2.zzzz, cb0[10].xyzw, r3.xyzw
mad r2.xyzw, r2.wwww, cb0[11].xyzw, r3.xyzw
mov o0.xyzw, r2.xyzw
mov o1.xyz, l(0,0,0,0)
mul r3.xyz, r2.yyyy, cb0[1].xyzx
mad r2.xyw, r2.xxxx, cb0[0].xyxz, r3.xyxz
mad r2.xyz, r2.zzzz, cb0[2].xyzx, r2.xywx
mov o2.xyz, r2.xyzx
emit 
mul r2.xyzw, r1.yyyy, cb0[5].xyzw
mad r2.xyzw, r1.xxxx, cb0[4].xyzw, r2.xyzw
mad r2.xyzw, r1.zzzz, cb0[6].xyzw, r2.xyzw
mad r1.xyzw, r1.wwww, cb0[7].xyzw, r2.xyzw
mul r2.xyzw, r1.yyyy, cb0[9].xyzw
mad r2.xyzw, r1.xxxx, cb0[8].xyzw, r2.xyzw
mad r2.xyzw, r1.zzzz, cb0[10].xyzw, r2.xyzw
mad r1.xyzw, r1.wwww, cb0[11].xyzw, r2.xyzw
mov o0.xyzw, r1.xyzw
mov o1.xyz, l(1.000000,0,0,0)
mul r2.xyz, r1.yyyy, cb0[1].xyzx
mad r1.xyw, r1.xxxx, cb0[0].xyxz, r2.xyxz
mad r1.xyz, r1.zzzz, cb0[2].xyzx, r1.xywx
mov o2.xyz, r1.xyzx
emit 
mul r1.xyzw, r0.wwww, cb0[1].xyzw
mad r2.xyzw, r0.xxxx, cb0[0].xyzw, r1.xyzw
mad r0.xyzw, r0.zzzz, cb0[0].xyzw, r1.xyzw
mad r0.xyzw, v[0][0].zzzz, cb0[2].xyzw, r0.xyzw
add r0.xyzw, r0.xyzw, cb0[3].xyzw
mad r1.xyzw, v[0][0].zzzz, cb0[2].xyzw, r2.xyzw
add r1.xyzw, r1.xyzw, cb0[3].xyzw
mul r2.xyzw, r1.yyyy, cb0[5].xyzw
mad r2.xyzw, r1.xxxx, cb0[4].xyzw, r2.xyzw
mad r2.xyzw, r1.zzzz, cb0[6].xyzw, r2.xyzw
mad r1.xyzw, r1.wwww, cb0[7].xyzw, r2.xyzw
mul r2.xyzw, r1.yyyy, cb0[9].xyzw
mad r2.xyzw, r1.xxxx, cb0[8].xyzw, r2.xyzw
mad r2.xyzw, r1.zzzz, cb0[10].xyzw, r2.xyzw
mad r1.xyzw, r1.wwww, cb0[11].xyzw, r2.xyzw
mov o0.xyzw, r1.xyzw
mov o1.xyz, l(0,1.000000,0,0)
mul r2.xyz, r1.yyyy, cb0[1].xyzx
mad r1.xyw, r1.xxxx, cb0[0].xyxz, r2.xyxz
mad r1.xyz, r1.zzzz, cb0[2].xyzx, r1.xywx
mov o2.xyz, r1.xyzx
emit 
mul r1.xyzw, r0.yyyy, cb0[5].xyzw
mad r1.xyzw, r0.xxxx, cb0[4].xyzw, r1.xyzw
mad r1.xyzw, r0.zzzz, cb0[6].xyzw, r1.xyzw
mad r0.xyzw, r0.wwww, cb0[7].xyzw, r1.xyzw
mul r1.xyzw, r0.yyyy, cb0[9].xyzw
mad r1.xyzw, r0.xxxx, cb0[8].xyzw, r1.xyzw
mad r1.xyzw, r0.zzzz, cb0[10].xyzw, r1.xyzw
mad r0.xyzw, r0.wwww, cb0[11].xyzw, r1.xyzw
mov o0.xyzw, r0.xyzw
mov o1.xyz, l(1.000000,1.000000,0,0)
mul r1.xyz, r0.yyyy, cb0[1].xyzx
mad r0.xyw, r0.xxxx, cb0[0].xyxz, r1.xyxz
mad r0.xyz, r0.zzzz, cb0[2].xyzx, r0.xywx
mov o2.xyz, r0.xyzx
emit 
ret 
// Approximately 76 instruction slots used
#endif

const BYTE Geo_GS[] =
{
     68,  88,  66,  67,   0,   6, 
    192, 229,  87,  55, 103,  88, 
    235, 161,  69, 131, 246,  76, 
    103, 168,   1,   0,   0,   0, 
    136,  13,   0,   0,   5,   0, 
      0,   0,  52,   0,   0,   0, 
    176,   1,   0,   0,  28,   2, 
      0,   0, 140,   2,   0,   0, 
     12,  13,   0,   0,  82,  68, 
     69,  70, 116,   1,   0,   0, 
      1,   0,   0,   0,  68,   0, 
      0,   0,   1,   0,   0,   0, 
     28,   0,   0,   0,   0,   4, 
     83,  71,   0,   1,   0,   0, 
     64,   1,   0,   0,  60,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   1,   0, 
      0,   0,   1,   0,   0,   0, 
     79,  66,  74,  69,  67,  84, 
      0, 171,  60,   0,   0,   0, 
      5,   0,   0,   0,  92,   0, 
      0,   0, 208,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0, 212,   0,   0,   0, 
      0,   0,   0,   0,  64,   0, 
      0,   0,   2,   0,   0,   0, 
    224,   0,   0,   0,   0,   0, 
      0,   0, 240,   0,   0,   0, 
     64,   0,   0,   0,  64,   0, 
      0,   0,   2,   0,   0,   0, 
    224,   0,   0,   0,   0,   0, 
      0,   0, 251,   0,   0,   0, 
    128,   0,   0,   0,  64,   0, 
      0,   0,   2,   0,   0,   0, 
    224,   0,   0,   0,   0,   0, 
      0,   0,  12,   1,   0,   0, 
    192,   0,   0,   0,   4,   0, 
      0,   0,   0,   0,   0,   0, 
     28,   1,   0,   0,   0,   0, 
      0,   0,  44,   1,   0,   0, 
    196,   0,   0,   0,  12,   0, 
      0,   0,   0,   0,   0,   0, 
     48,   1,   0,   0,   0,   0, 
      0,   0, 119, 111, 114, 108, 
    100,  77,  97, 116, 114, 105, 
    120,   0,   2,   0,   3,   0, 
      4,   0,   4,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
    118, 105, 101, 119,  77,  97, 
    116, 114, 105, 120,   0, 112, 
    114, 111, 106, 101,  99, 116, 
    105, 111, 110,  77,  97, 116, 
    114, 105, 120,   0,  83,  99, 
    114, 101, 101, 110,  72, 101, 
    105, 103, 104, 116,   0, 171, 
    171, 171,   0,   0,   3,   0, 
      1,   0,   1,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
    112,  97, 100,   0,   1,   0, 
      3,   0,   1,   0,   3,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,  77, 105,  99, 114, 
    111, 115, 111, 102, 116,  32, 
     40,  82,  41,  32,  72,  76, 
     83,  76,  32,  83, 104,  97, 
    100, 101, 114,  32,  67, 111, 
    109, 112, 105, 108, 101, 114, 
     32,  54,  46,  51,  46,  57, 
     54,  48,  48,  46,  49,  54, 
     51,  56,  52,   0, 171, 171, 
     73,  83,  71,  78, 100,   0, 
      0,   0,   3,   0,   0,   0, 
      8,   0,   0,   0,  80,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   3,   0, 
      0,   0,   0,   0,   0,   0, 
     15,   7,   0,   0,  89,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   3,   0, 
      0,   0,   1,   0,   0,   0, 
      7,   0,   0,   0,  92,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   3,   0, 
      0,   0,   2,   0,   0,   0, 
      7,   0,   0,   0,  80,  79, 
     83,  73,  84,  73,  79,  78, 
      0,  85,  86,   0,  78,  79, 
     82,  77,  65,  76,  83,   0, 
     79,  83,  71,  78, 104,   0, 
      0,   0,   3,   0,   0,   0, 
      8,   0,   0,   0,  80,   0, 
      0,   0,   0,   0,   0,   0, 
      1,   0,   0,   0,   3,   0, 
      0,   0,   0,   0,   0,   0, 
     15,   0,   0,   0,  92,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   3,   0, 
      0,   0,   1,   0,   0,   0, 
      7,   8,   0,   0,  95,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   3,   0, 
      0,   0,   2,   0,   0,   0, 
      7,   8,   0,   0,  83,  86, 
     95,  80,  79,  83,  73,  84, 
     73,  79,  78,   0,  85,  86, 
      0,  78,  79,  82,  77,  65, 
     76,   0, 171, 171,  83,  72, 
     68,  82, 120,  10,   0,   0, 
     64,   0,   2,   0, 158,   2, 
      0,   0,  89,   0,   0,   4, 
     70, 142,  32,   0,   0,   0, 
      0,   0,  12,   0,   0,   0, 
     95,   0,   0,   4, 242,  16, 
     32,   0,   1,   0,   0,   0, 
      0,   0,   0,   0,  95,   0, 
      0,   4, 114,  16,  32,   0, 
      1,   0,   0,   0,   1,   0, 
      0,   0,  95,   0,   0,   4, 
    114,  16,  32,   0,   1,   0, 
      0,   0,   2,   0,   0,   0, 
    104,   0,   0,   2,   4,   0, 
      0,   0,  93,   8,   0,   1, 
     92,  40,   0,   1, 103,   0, 
      0,   4, 242,  32,  16,   0, 
      0,   0,   0,   0,   1,   0, 
      0,   0, 101,   0,   0,   3, 
    114,  32,  16,   0,   1,   0, 
      0,   0, 101,   0,   0,   3, 
    114,  32,  16,   0,   2,   0, 
      0,   0,  94,   0,   0,   2, 
      4,   0,   0,   0,   0,   0, 
      0,  11, 242,   0,  16,   0, 
      0,   0,   0,   0,   2,  64, 
      0,   0,   0,   0,   0, 192, 
      0,   0,   0,  64,   0,   0, 
      0,  64,   0,   0,   0, 192, 
     70,  20,  32,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
     56,   0,   0,   8, 242,   0, 
     16,   0,   1,   0,   0,   0, 
     86,   5,  16,   0,   0,   0, 
      0,   0,  70, 142,  32,   0, 
      0,   0,   0,   0,   1,   0, 
      0,   0,  50,   0,   0,  10, 
    242,   0,  16,   0,   2,   0, 
      0,   0,   6,   0,  16,   0, 
      0,   0,   0,   0,  70, 142, 
     32,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,  70,  14, 
     16,   0,   1,   0,   0,   0, 
     50,   0,   0,  10, 242,   0, 
     16,   0,   1,   0,   0,   0, 
    166,  10,  16,   0,   0,   0, 
      0,   0,  70, 142,  32,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,  70,  14,  16,   0, 
      1,   0,   0,   0,  50,   0, 
      0,  11, 242,   0,  16,   0, 
      1,   0,   0,   0, 166,  26, 
     32,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,  70, 142, 
     32,   0,   0,   0,   0,   0, 
      2,   0,   0,   0,  70,  14, 
     16,   0,   1,   0,   0,   0, 
      0,   0,   0,   8, 242,   0, 
     16,   0,   1,   0,   0,   0, 
     70,  14,  16,   0,   1,   0, 
      0,   0,  70, 142,  32,   0, 
      0,   0,   0,   0,   3,   0, 
      0,   0,  50,   0,   0,  11, 
    242,   0,  16,   0,   2,   0, 
      0,   0, 166,  26,  32,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,  70, 142,  32,   0, 
      0,   0,   0,   0,   2,   0, 
      0,   0,  70,  14,  16,   0, 
      2,   0,   0,   0,   0,   0, 
      0,   8, 242,   0,  16,   0, 
      2,   0,   0,   0,  70,  14, 
     16,   0,   2,   0,   0,   0, 
     70, 142,  32,   0,   0,   0, 
      0,   0,   3,   0,   0,   0, 
     56,   0,   0,   8, 242,   0, 
     16,   0,   3,   0,   0,   0, 
     86,   5,  16,   0,   2,   0, 
      0,   0,  70, 142,  32,   0, 
      0,   0,   0,   0,   5,   0, 
      0,   0,  50,   0,   0,  10, 
    242,   0,  16,   0,   3,   0, 
      0,   0,   6,   0,  16,   0, 
      2,   0,   0,   0,  70, 142, 
     32,   0,   0,   0,   0,   0, 
      4,   0,   0,   0,  70,  14, 
     16,   0,   3,   0,   0,   0, 
     50,   0,   0,  10, 242,   0, 
     16,   0,   3,   0,   0,   0, 
    166,  10,  16,   0,   2,   0, 
      0,   0,  70, 142,  32,   0, 
      0,   0,   0,   0,   6,   0, 
      0,   0,  70,  14,  16,   0, 
      3,   0,   0,   0,  50,   0, 
      0,  10, 242,   0,  16,   0, 
      2,   0,   0,   0, 246,  15, 
     16,   0,   2,   0,   0,   0, 
     70, 142,  32,   0,   0,   0, 
      0,   0,   7,   0,   0,   0, 
     70,  14,  16,   0,   3,   0, 
      0,   0,  56,   0,   0,   8, 
    242,   0,  16,   0,   3,   0, 
      0,   0,  86,   5,  16,   0, 
      2,   0,   0,   0,  70, 142, 
     32,   0,   0,   0,   0,   0, 
      9,   0,   0,   0,  50,   0, 
      0,  10, 242,   0,  16,   0, 
      3,   0,   0,   0,   6,   0, 
     16,   0,   2,   0,   0,   0, 
     70, 142,  32,   0,   0,   0, 
      0,   0,   8,   0,   0,   0, 
     70,  14,  16,   0,   3,   0, 
      0,   0,  50,   0,   0,  10, 
    242,   0,  16,   0,   3,   0, 
      0,   0, 166,  10,  16,   0, 
      2,   0,   0,   0,  70, 142, 
     32,   0,   0,   0,   0,   0, 
     10,   0,   0,   0,  70,  14, 
     16,   0,   3,   0,   0,   0, 
     50,   0,   0,  10, 242,   0, 
     16,   0,   2,   0,   0,   0, 
    246,  15,  16,   0,   2,   0, 
      0,   0,  70, 142,  32,   0, 
      0,   0,   0,   0,  11,   0, 
      0,   0,  70,  14,  16,   0, 
      3,   0,   0,   0,  54,   0, 
      0,   5, 242,  32,  16,   0, 
      0,   0,   0,   0,  70,  14, 
     16,   0,   2,   0,   0,   0, 
     54,   0,   0,   8, 114,  32, 
     16,   0,   1,   0,   0,   0, 
      2,  64,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,  56,   0,   0,   8, 
    114,   0,  16,   0,   3,   0, 
      0,   0,  86,   5,  16,   0, 
      2,   0,   0,   0,  70, 130, 
     32,   0,   0,   0,   0,   0, 
      1,   0,   0,   0,  50,   0, 
      0,  10, 178,   0,  16,   0, 
      2,   0,   0,   0,   6,   0, 
     16,   0,   2,   0,   0,   0, 
     70, 136,  32,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
     70,   8,  16,   0,   3,   0, 
      0,   0,  50,   0,   0,  10, 
    114,   0,  16,   0,   2,   0, 
      0,   0, 166,  10,  16,   0, 
      2,   0,   0,   0,  70, 130, 
     32,   0,   0,   0,   0,   0, 
      2,   0,   0,   0,  70,   3, 
     16,   0,   2,   0,   0,   0, 
     54,   0,   0,   5, 114,  32, 
     16,   0,   2,   0,   0,   0, 
     70,   2,  16,   0,   2,   0, 
      0,   0,  19,   0,   0,   1, 
     56,   0,   0,   8, 242,   0, 
     16,   0,   2,   0,   0,   0, 
     86,   5,  16,   0,   1,   0, 
      0,   0,  70, 142,  32,   0, 
      0,   0,   0,   0,   5,   0, 
      0,   0,  50,   0,   0,  10, 
    242,   0,  16,   0,   2,   0, 
      0,   0,   6,   0,  16,   0, 
      1,   0,   0,   0,  70, 142, 
     32,   0,   0,   0,   0,   0, 
      4,   0,   0,   0,  70,  14, 
     16,   0,   2,   0,   0,   0, 
     50,   0,   0,  10, 242,   0, 
     16,   0,   2,   0,   0,   0, 
    166,  10,  16,   0,   1,   0, 
      0,   0,  70, 142,  32,   0, 
      0,   0,   0,   0,   6,   0, 
      0,   0,  70,  14,  16,   0, 
      2,   0,   0,   0,  50,   0, 
      0,  10, 242,   0,  16,   0, 
      1,   0,   0,   0, 246,  15, 
     16,   0,   1,   0,   0,   0, 
     70, 142,  32,   0,   0,   0, 
      0,   0,   7,   0,   0,   0, 
     70,  14,  16,   0,   2,   0, 
      0,   0,  56,   0,   0,   8, 
    242,   0,  16,   0,   2,   0, 
      0,   0,  86,   5,  16,   0, 
      1,   0,   0,   0,  70, 142, 
     32,   0,   0,   0,   0,   0, 
      9,   0,   0,   0,  50,   0, 
      0,  10, 242,   0,  16,   0, 
      2,   0,   0,   0,   6,   0, 
     16,   0,   1,   0,   0,   0, 
     70, 142,  32,   0,   0,   0, 
      0,   0,   8,   0,   0,   0, 
     70,  14,  16,   0,   2,   0, 
      0,   0,  50,   0,   0,  10, 
    242,   0,  16,   0,   2,   0, 
      0,   0, 166,  10,  16,   0, 
      1,   0,   0,   0,  70, 142, 
     32,   0,   0,   0,   0,   0, 
     10,   0,   0,   0,  70,  14, 
     16,   0,   2,   0,   0,   0, 
     50,   0,   0,  10, 242,   0, 
     16,   0,   1,   0,   0,   0, 
    246,  15,  16,   0,   1,   0, 
      0,   0,  70, 142,  32,   0, 
      0,   0,   0,   0,  11,   0, 
      0,   0,  70,  14,  16,   0, 
      2,   0,   0,   0,  54,   0, 
      0,   5, 242,  32,  16,   0, 
      0,   0,   0,   0,  70,  14, 
     16,   0,   1,   0,   0,   0, 
     54,   0,   0,   8, 114,  32, 
     16,   0,   1,   0,   0,   0, 
      2,  64,   0,   0,   0,   0, 
    128,  63,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,  56,   0,   0,   8, 
    114,   0,  16,   0,   2,   0, 
      0,   0,  86,   5,  16,   0, 
      1,   0,   0,   0,  70, 130, 
     32,   0,   0,   0,   0,   0, 
      1,   0,   0,   0,  50,   0, 
      0,  10, 178,   0,  16,   0, 
      1,   0,   0,   0,   6,   0, 
     16,   0,   1,   0,   0,   0, 
     70, 136,  32,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
     70,   8,  16,   0,   2,   0, 
      0,   0,  50,   0,   0,  10, 
    114,   0,  16,   0,   1,   0, 
      0,   0, 166,  10,  16,   0, 
      1,   0,   0,   0,  70, 130, 
     32,   0,   0,   0,   0,   0, 
      2,   0,   0,   0,  70,   3, 
     16,   0,   1,   0,   0,   0, 
     54,   0,   0,   5, 114,  32, 
     16,   0,   2,   0,   0,   0, 
     70,   2,  16,   0,   1,   0, 
      0,   0,  19,   0,   0,   1, 
     56,   0,   0,   8, 242,   0, 
     16,   0,   1,   0,   0,   0, 
    246,  15,  16,   0,   0,   0, 
      0,   0,  70, 142,  32,   0, 
      0,   0,   0,   0,   1,   0, 
      0,   0,  50,   0,   0,  10, 
    242,   0,  16,   0,   2,   0, 
      0,   0,   6,   0,  16,   0, 
      0,   0,   0,   0,  70, 142, 
     32,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,  70,  14, 
     16,   0,   1,   0,   0,   0, 
     50,   0,   0,  10, 242,   0, 
     16,   0,   0,   0,   0,   0, 
    166,  10,  16,   0,   0,   0, 
      0,   0,  70, 142,  32,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,  70,  14,  16,   0, 
      1,   0,   0,   0,  50,   0, 
      0,  11, 242,   0,  16,   0, 
      0,   0,   0,   0, 166,  26, 
     32,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,  70, 142, 
     32,   0,   0,   0,   0,   0, 
      2,   0,   0,   0,  70,  14, 
     16,   0,   0,   0,   0,   0, 
      0,   0,   0,   8, 242,   0, 
     16,   0,   0,   0,   0,   0, 
     70,  14,  16,   0,   0,   0, 
      0,   0,  70, 142,  32,   0, 
      0,   0,   0,   0,   3,   0, 
      0,   0,  50,   0,   0,  11, 
    242,   0,  16,   0,   1,   0, 
      0,   0, 166,  26,  32,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,  70, 142,  32,   0, 
      0,   0,   0,   0,   2,   0, 
      0,   0,  70,  14,  16,   0, 
      2,   0,   0,   0,   0,   0, 
      0,   8, 242,   0,  16,   0, 
      1,   0,   0,   0,  70,  14, 
     16,   0,   1,   0,   0,   0, 
     70, 142,  32,   0,   0,   0, 
      0,   0,   3,   0,   0,   0, 
     56,   0,   0,   8, 242,   0, 
     16,   0,   2,   0,   0,   0, 
     86,   5,  16,   0,   1,   0, 
      0,   0,  70, 142,  32,   0, 
      0,   0,   0,   0,   5,   0, 
      0,   0,  50,   0,   0,  10, 
    242,   0,  16,   0,   2,   0, 
      0,   0,   6,   0,  16,   0, 
      1,   0,   0,   0,  70, 142, 
     32,   0,   0,   0,   0,   0, 
      4,   0,   0,   0,  70,  14, 
     16,   0,   2,   0,   0,   0, 
     50,   0,   0,  10, 242,   0, 
     16,   0,   2,   0,   0,   0, 
    166,  10,  16,   0,   1,   0, 
      0,   0,  70, 142,  32,   0, 
      0,   0,   0,   0,   6,   0, 
      0,   0,  70,  14,  16,   0, 
      2,   0,   0,   0,  50,   0, 
      0,  10, 242,   0,  16,   0, 
      1,   0,   0,   0, 246,  15, 
     16,   0,   1,   0,   0,   0, 
     70, 142,  32,   0,   0,   0, 
      0,   0,   7,   0,   0,   0, 
     70,  14,  16,   0,   2,   0, 
      0,   0,  56,   0,   0,   8, 
    242,   0,  16,   0,   2,   0, 
      0,   0,  86,   5,  16,   0, 
      1,   0,   0,   0,  70, 142, 
     32,   0,   0,   0,   0,   0, 
      9,   0,   0,   0,  50,   0, 
      0,  10, 242,   0,  16,   0, 
      2,   0,   0,   0,   6,   0, 
     16,   0,   1,   0,   0,   0, 
     70, 142,  32,   0,   0,   0, 
      0,   0,   8,   0,   0,   0, 
     70,  14,  16,   0,   2,   0, 
      0,   0,  50,   0,   0,  10, 
    242,   0,  16,   0,   2,   0, 
      0,   0, 166,  10,  16,   0, 
      1,   0,   0,   0,  70, 142, 
     32,   0,   0,   0,   0,   0, 
     10,   0,   0,   0,  70,  14, 
     16,   0,   2,   0,   0,   0, 
     50,   0,   0,  10, 242,   0, 
     16,   0,   1,   0,   0,   0, 
    246,  15,  16,   0,   1,   0, 
      0,   0,  70, 142,  32,   0, 
      0,   0,   0,   0,  11,   0, 
      0,   0,  70,  14,  16,   0, 
      2,   0,   0,   0,  54,   0, 
      0,   5, 242,  32,  16,   0, 
      0,   0,   0,   0,  70,  14, 
     16,   0,   1,   0,   0,   0, 
     54,   0,   0,   8, 114,  32, 
     16,   0,   1,   0,   0,   0, 
      2,  64,   0,   0,   0,   0, 
      0,   0,   0,   0, 128,  63, 
      0,   0,   0,   0,   0,   0, 
      0,   0,  56,   0,   0,   8, 
    114,   0,  16,   0,   2,   0, 
      0,   0,  86,   5,  16,   0, 
      1,   0,   0,   0,  70, 130, 
     32,   0,   0,   0,   0,   0, 
      1,   0,   0,   0,  50,   0, 
      0,  10, 178,   0,  16,   0, 
      1,   0,   0,   0,   6,   0, 
     16,   0,   1,   0,   0,   0, 
     70, 136,  32,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
     70,   8,  16,   0,   2,   0, 
      0,   0,  50,   0,   0,  10, 
    114,   0,  16,   0,   1,   0, 
      0,   0, 166,  10,  16,   0, 
      1,   0,   0,   0,  70, 130, 
     32,   0,   0,   0,   0,   0, 
      2,   0,   0,   0,  70,   3, 
     16,   0,   1,   0,   0,   0, 
     54,   0,   0,   5, 114,  32, 
     16,   0,   2,   0,   0,   0, 
     70,   2,  16,   0,   1,   0, 
      0,   0,  19,   0,   0,   1, 
     56,   0,   0,   8, 242,   0, 
     16,   0,   1,   0,   0,   0, 
     86,   5,  16,   0,   0,   0, 
      0,   0,  70, 142,  32,   0, 
      0,   0,   0,   0,   5,   0, 
      0,   0,  50,   0,   0,  10, 
    242,   0,  16,   0,   1,   0, 
      0,   0,   6,   0,  16,   0, 
      0,   0,   0,   0,  70, 142, 
     32,   0,   0,   0,   0,   0, 
      4,   0,   0,   0,  70,  14, 
     16,   0,   1,   0,   0,   0, 
     50,   0,   0,  10, 242,   0, 
     16,   0,   1,   0,   0,   0, 
    166,  10,  16,   0,   0,   0, 
      0,   0,  70, 142,  32,   0, 
      0,   0,   0,   0,   6,   0, 
      0,   0,  70,  14,  16,   0, 
      1,   0,   0,   0,  50,   0, 
      0,  10, 242,   0,  16,   0, 
      0,   0,   0,   0, 246,  15, 
     16,   0,   0,   0,   0,   0, 
     70, 142,  32,   0,   0,   0, 
      0,   0,   7,   0,   0,   0, 
     70,  14,  16,   0,   1,   0, 
      0,   0,  56,   0,   0,   8, 
    242,   0,  16,   0,   1,   0, 
      0,   0,  86,   5,  16,   0, 
      0,   0,   0,   0,  70, 142, 
     32,   0,   0,   0,   0,   0, 
      9,   0,   0,   0,  50,   0, 
      0,  10, 242,   0,  16,   0, 
      1,   0,   0,   0,   6,   0, 
     16,   0,   0,   0,   0,   0, 
     70, 142,  32,   0,   0,   0, 
      0,   0,   8,   0,   0,   0, 
     70,  14,  16,   0,   1,   0, 
      0,   0,  50,   0,   0,  10, 
    242,   0,  16,   0,   1,   0, 
      0,   0, 166,  10,  16,   0, 
      0,   0,   0,   0,  70, 142, 
     32,   0,   0,   0,   0,   0, 
     10,   0,   0,   0,  70,  14, 
     16,   0,   1,   0,   0,   0, 
     50,   0,   0,  10, 242,   0, 
     16,   0,   0,   0,   0,   0, 
    246,  15,  16,   0,   0,   0, 
      0,   0,  70, 142,  32,   0, 
      0,   0,   0,   0,  11,   0, 
      0,   0,  70,  14,  16,   0, 
      1,   0,   0,   0,  54,   0, 
      0,   5, 242,  32,  16,   0, 
      0,   0,   0,   0,  70,  14, 
     16,   0,   0,   0,   0,   0, 
     54,   0,   0,   8, 114,  32, 
     16,   0,   1,   0,   0,   0, 
      2,  64,   0,   0,   0,   0, 
    128,  63,   0,   0, 128,  63, 
      0,   0,   0,   0,   0,   0, 
      0,   0,  56,   0,   0,   8, 
    114,   0,  16,   0,   1,   0, 
      0,   0,  86,   5,  16,   0, 
      0,   0,   0,   0,  70, 130, 
     32,   0,   0,   0,   0,   0, 
      1,   0,   0,   0,  50,   0, 
      0,  10, 178,   0,  16,   0, 
      0,   0,   0,   0,   6,   0, 
     16,   0,   0,   0,   0,   0, 
     70, 136,  32,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
     70,   8,  16,   0,   1,   0, 
      0,   0,  50,   0,   0,  10, 
    114,   0,  16,   0,   0,   0, 
      0,   0, 166,  10,  16,   0, 
      0,   0,   0,   0,  70, 130, 
     32,   0,   0,   0,   0,   0, 
      2,   0,   0,   0,  70,   3, 
     16,   0,   0,   0,   0,   0, 
     54,   0,   0,   5, 114,  32, 
     16,   0,   2,   0,   0,   0, 
     70,   2,  16,   0,   0,   0, 
      0,   0,  19,   0,   0,   1, 
     62,   0,   0,   1,  83,  84, 
     65,  84, 116,   0,   0,   0, 
     76,   0,   0,   0,   4,   0, 
      0,   0,   0,   0,   0,   0, 
      6,   0,   0,   0,  59,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   1,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   4,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   1,   0,   0,   0, 
      5,   0,   0,   0,   4,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0
};
