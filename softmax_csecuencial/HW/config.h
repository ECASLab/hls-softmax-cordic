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

// Seleccion de Tipo de Data
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

// Calc. de Empaquetamiento
static constexpr int kPackets = kBusWidth / kDataWidth;

// Tipos de Datos
using RawDataT = ap_uint<kBusWidth>;
using StreamT = hls::stream<RawDataT>;

// Tipos de Acumuladores
#ifdef USE_FLOAT32
#define USE_UNION
typedef union {
    uint32_t i; // Representacion como Entero (bits raw)
    float f;    // Representacion como float
} d32;
using AccT = d32;

#elif defined(USE_FLOAT16) || defined(USE_FLOAT8) || defined(USE_FLOAT4)
#define USE_UNION
typedef union {
  uint16_t i;  // Acceso a bits raw
  half f;      // Acceso como half-float
} d16;
using AccT = d16;

#else
using AccT = DataT;
#endif

// Macros de Acceso
#ifdef USE_UNION
#define GET_NUMBER(n) (n).f   // Accede al valor numérico
#define GET_RAW(n) (n).i      // Accede a bits raw
#else
#define GET_NUMBER(n) (n)     // Acceso directo (fixed-point)
#define GET_RAW(n) (n).V      // Bits internos de ap_fixed
#endif

#endif // __CONFIG_H__