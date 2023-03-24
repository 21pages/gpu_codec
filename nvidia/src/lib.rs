#![allow(non_upper_case_globals)]
#![allow(non_camel_case_types)]
#![allow(non_snake_case)]

include!(concat!(env!("OUT_DIR"), "/nvidia_ffi.rs"));

use common::{
    DataFormat::*, DecodeCalls, EncodeCalls, HWDeviceType::*, InnerDecodeContext,
    InnerEncodeContext,
};

pub fn encode_calls() -> EncodeCalls {
    EncodeCalls {
        new: nvidia_new_encoder,
        encode: nvidia_encode,
        destroy: nvidia_destroy_encoder,
    }
}

pub fn decode_calls() -> DecodeCalls {
    DecodeCalls {
        new: nvidia_new_decoder,
        decode: nvidia_decode,
        destroy: nvidia_destroy_decoder,
    }
}

pub fn possible_support_encoders() -> Vec<InnerEncodeContext> {
    if unsafe { nvidia_encode_driver_support() } != 0 {
        return vec![];
    }
    let devices = vec![CUDA];
    let codecs = vec![H264, HEVC];
    let mut v = vec![];
    for device in devices.iter() {
        for codec in codecs.iter() {
            v.push(InnerEncodeContext {
                device: device.clone(),
                codec: codec.clone(),
            });
        }
    }
    v
}

pub fn possible_support_decoders() -> Vec<InnerDecodeContext> {
    if unsafe { nvidia_encode_driver_support() } != 0 {
        return vec![];
    }
    let devices = vec![CUDA];
    let codecs = vec![H264, HEVC];
    let mut v = vec![];
    for device in devices.iter() {
        for codec in codecs.iter() {
            v.push(InnerDecodeContext {
                device: device.clone(),
                codec: codec.clone(),
            });
        }
    }
    v
}
