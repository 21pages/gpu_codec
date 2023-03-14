#ifndef AMF_FFI_H
#define AMF_FFI_H

#include "common.h"

typedef enum AMF_SURFACE_FORMAT
{
    AMF_SURFACE_UNKNOWN     = 0,
    AMF_SURFACE_NV12,               ///< 1  - planar 4:2:0 Y width x height + packed UV width/2 x height/2 - 8 bit per component
    AMF_SURFACE_YV12,               ///< 2  - planar 4:2:0 Y width x height + V width/2 x height/2 + U width/2 x height/2 - 8 bit per component
    AMF_SURFACE_BGRA,               ///< 3  - packed 4:4:4 - 8 bit per component
    AMF_SURFACE_ARGB,               ///< 4  - packed 4:4:4 - 8 bit per component
    AMF_SURFACE_RGBA,               ///< 5  - packed 4:4:4 - 8 bit per component
    AMF_SURFACE_GRAY8,              ///< 6  - single component - 8 bit
    AMF_SURFACE_YUV420P,            ///< 7  - planar 4:2:0 Y width x height + U width/2 x height/2 + V width/2 x height/2 - 8 bit per component
    AMF_SURFACE_U8V8,               ///< 8  - packed double component - 8 bit per component
    AMF_SURFACE_YUY2,               ///< 9  - packed 4:2:2 Byte 0=8-bit Y'0; Byte 1=8-bit Cb; Byte 2=8-bit Y'1; Byte 3=8-bit Cr
    AMF_SURFACE_P010,               ///< 10 - planar 4:2:0 Y width x height + packed UV width/2 x height/2 - 10 bit per component (16 allocated, upper 10 bits are used)
    AMF_SURFACE_RGBA_F16,           ///< 11 - packed 4:4:4 - 16 bit per component float
    AMF_SURFACE_UYVY,               ///< 12 - packed 4:2:2 the similar to YUY2 but Y and UV swapped: Byte 0=8-bit Cb; Byte 1=8-bit Y'0; Byte 2=8-bit Cr Byte 3=8-bit Y'1; (used the same DX/CL/Vulkan storage as YUY2)
    AMF_SURFACE_R10G10B10A2,        ///< 13 - packed 4:4:4 to 4 bytes, 10 bit per RGB component, 2 bits per A 
    AMF_SURFACE_Y210,               ///< 14 - packed 4:2:2 - Word 0=10-bit Y'0; Word 1=10-bit Cb; Word 2=10-bit Y'1; Word 3=10-bit Cr
    AMF_SURFACE_AYUV,               ///< 15 - packed 4:4:4 - 8 bit per component YUVA
    AMF_SURFACE_Y410,               ///< 16 - packed 4:4:4 - 10 bit per YUV component, 2 bits per A, AVYU 
    AMF_SURFACE_Y416,               ///< 16 - packed 4:4:4 - 16 bit per component 4 bytes, AVYU
    AMF_SURFACE_GRAY32,             ///< 17 - single component - 32 bit
    AMF_SURFACE_P012,               ///< 18 - planar 4:2:0 Y width x height + packed UV width/2 x height/2 - 12 bit per component (16 allocated, upper 12 bits are used)
    AMF_SURFACE_P016,               ///< 19 - planar 4:2:0 Y width x height + packed UV width/2 x height/2 - 16 bit per component (16 allocated, all bits are used)

    AMF_SURFACE_FIRST = AMF_SURFACE_NV12,
    AMF_SURFACE_LAST = AMF_SURFACE_P016
} AMF_SURFACE_FORMAT;

typedef enum AMF_MEMORY_TYPE
{
    AMF_MEMORY_UNKNOWN          = 0,
    AMF_MEMORY_HOST             = 1,
    AMF_MEMORY_DX9              = 2,
    AMF_MEMORY_DX11             = 3,
    AMF_MEMORY_OPENCL           = 4,
    AMF_MEMORY_OPENGL           = 5,
    AMF_MEMORY_XV               = 6,
    AMF_MEMORY_GRALLOC          = 7,
    AMF_MEMORY_COMPUTE_FOR_DX9  = 8, // deprecated, the same as AMF_MEMORY_OPENCL
    AMF_MEMORY_COMPUTE_FOR_DX11 = 9, // deprecated, the same as AMF_MEMORY_OPENCL
    AMF_MEMORY_VULKAN           = 10,
    AMF_MEMORY_DX12             = 11,
} AMF_MEMORY_TYPE;


void* amf_new_encoder(AMF_MEMORY_TYPE memoryType, 
                        AMF_SURFACE_FORMAT surfaceFormat,
                        enum Codec codec,
                        uint32_t width, 
                        uint32_t height);

int amf_encode(void *enc,
                uint8_t *data[MAX_AV_PLANES],
                uint32_t linesize[MAX_AV_PLANES],
                EncodeCallback callback,
                void* obj);

int amf_destroy_encoder(void *enc);

void* amf_new_decoder(AMF_MEMORY_TYPE memoryTypeOut,
                        AMF_SURFACE_FORMAT formatOut,
                        enum Codec codec);

int amf_decode(void *dec,
                uint8_t *data,
                uint32_t length,
                DecodeCallback callback,
                void *obj);

int amf_destroy_decoder(void *dec);

#endif // AMF_FFI_H