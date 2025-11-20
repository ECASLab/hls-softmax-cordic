#ifndef __CONFIG_H__
#define __CONFIG_H__

// Inc Standard y HLS
#include <stdint.h>     // uint32_t, uint64_t
#include <ap_int.h>     // ap_int<N> , ap_uint<N>
#include <ap_fixed.h>   // ap_fixed <W,I>
#include <hls_stream.h> // Streams/FIFOs para DATAFLOW (hls::stream<T>)

// Config. del Ancho del BUS AXI
#ifndef BUS
static constexpr int kBusWidth = 512; // Ancho por defecto (bits)
#else
static constexpr int kBusWidth = BUS // Override con -DBUS = N
#endif // !BUS

// 

//
#ifdef USE_FLOAT32
// Opt 1: F32 (IEEE 754)
static constexpr int kDataWidth = 32;

#elif defined(USE_FLOAT16)
// Opt 2: F16 (Half Precision)
static constexpr int kDataWidth = 16;

#elif defined(USE_FIXED16)
// Opt 3: FP16 (Q6.10)
static constexpr int kFxPDataWidth = 16;
statis constexpr int kFxPDataInt = 6;
static constexpr int kDataWidth = kFxPDataWidth;
using DataT = ap_fixed<kFxPDataWidth, kFxPDataInt>;


#else
// Opt 4: FP8 (Q4.4)
static constexpr int kFxPDataWidth = 8; // Total Bits
static constexpr int kFxPDataInt = 4;   // Bits Enteros
static constexpr int kDataWidth = kFxPDataWidth;
using DataT = ap_fixed<kFxPDataWidth, kFxPDataInt>;

#endif 