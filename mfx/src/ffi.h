#ifndef AMF_FFI_H
#define AMF_FFI_H

#include "../../common/src/callback.h"
#include <stdbool.h>

int mfx_driver_support();

void *mfx_new_encoder(void *handle, int64_t luid, int32_t api,
                      int32_t dataFormat, int32_t width, int32_t height,
                      int32_t kbs, int32_t framerate, int32_t gop,
                      int32_t q_min, int32_t q_max);

int mfx_encode(void *encoder, void *tex, EncodeCallback callback, void *obj);

int mfx_destroy_encoder(void *encoder);

void *mfx_new_decoder(void *device, int64_t luid, int32_t api,
                      int32_t dataFormat, bool outputSharedHandle);

int mfx_decode(void *decoder, uint8_t *data, int len, DecodeCallback callback,
               void *obj);

int mfx_destroy_decoder(void *decoder);

int mfx_test_encode(void *outDescs, int32_t maxDescNum, int32_t *outDescNum,
                    int32_t api, int32_t dataFormat, int32_t width,
                    int32_t height, int32_t kbs, int32_t framerate, int32_t gop,
                    int32_t q_min, int32_t q_max);

int mfx_test_decode(void *outDescs, int32_t maxDescNum, int32_t *outDescNum,
                    int32_t api, int32_t dataFormat, bool outputSharedHandle,
                    uint8_t *data, int32_t length);

int mfx_set_bitrate(void *encoder, int32_t kbs);

int mfx_set_framerate(void *encoder, int32_t framerate);

#endif // AMF_FFI_H