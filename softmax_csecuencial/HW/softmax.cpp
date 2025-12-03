#include "hls_math.h" // HLS Math Library
#include "cordic_hls_exp.h" // CORDIC Hyperbolic Exp
#include "softmax.h" // Softmax Function


extern "C" {
    void softmax(RawDataT *in1, RawDataT *out, uint64_t size_elems) {

#pragma HLS INTERFACE m_axi port=in1 offset=slave bundle=gmem0
#pragma HLS INTERFACE m_axi port=out offset=slave bundle=gmem1
#pragma HLS INTERFACE s_axilite port=size_elems
#pragma HLS INTERFACE s_axilite port=return

if (size_elems == 0) return;

if ((size_elems % kPackets) != 0) return;

const uint64_t size_words = size_elems / kPackets; // Num. de Palabras (RawDataT) que se leeran/escribiran.


AccT sum; 
GET_NUMBER(sum) = 0.0f;

// A. Calc. de Exp. y Acu. de la Suma Global
pass1_words:
        for (uint64_t w = 0; w < size_words; ++w) {
#pragma HLS PIPELINE II=1
#pragma HLS LOOP_TRIPCOUNT min=1 max=kTotalMaxSize avg=kTotalMaxSize

            RawDataT raw_in = in1[w]; // Lee una palabra de entrada completa desde AXI
            RawDataT raw_exp = 0; // Almacena los exponentes calculados

            AccT local_sum;
            GET_NUMBER(local_sum) = 0.0f;

            pass1_unpack:
                for (int p = 0; p < kPackets; ++p) {
#pragma HLS UNROLL
                    const int offlow = p * kDataWidth;
                    const int offhigh = offlow + kDataWidth - 1;

                    // 1. Extrae el dato empaquetado
                    AccT x;
                    GET_RAW(x) = raw_in(offhigh, offlow);

                    // 2. Calcula e^x usando CORDIC Hyperbolic (en float)
                    float ex = cordic_exp_hls(GET_NUMBER(x));

                    // 3. Guarda exponente en un AccT
                    AccT ex_val;
                    GET_NUMBER(ex_val) = ex;

                    // 4. Acumula en la suma local
                    GET_NUMBER(local_sum) += ex;

                    // 5. Reempaqueta exponente en la palabra de salida
                    raw_exp(offhigh, offlow) = GET_RAW(ex_val);
                }
            // 6. Escribe los exponentes calc. como buffer temporal
            out[w] = raw_exp;

            // 7. Acumula la suma local en la suma global
            GET_NUMBER(sum) += GET_NUMBER(local_sum);
        }

        // 8. Calc. Factor de Normalizacion: scale = 1 / sum
        AccT scale;
        GET_NUMBER(scale) = hls::divide(1.0f, GET_NUMBER(sum));

// B. Calc. de Exp. y Acu. de la Suma Global

        




    }
}