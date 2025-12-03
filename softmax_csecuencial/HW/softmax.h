#ifndef __SOFTMAX_H__
#define __SOFTMAX_H__

#include "config.h" // Configuracion del Proyecto

// Dimensiones de la Matriz ===============
#ifndef M_COLS
static constexpr int kCols = 4096; // Numero de Columnas por defecto
#else
static constexpr int kCols = M_COLS; // Override con -DM_COLS = N
#endif // !M_COLS

#ifndef M_ROWS
static constexpr int kRows = 4096; // Numero de Filas por defecto
#else
static constexpr int kRows = M_ROWS; // Override con -DM_ROWS = N
#endif // !M_ROWS
// ========================================

// Calc. Tamaño Max.
static constexpr uint64_t kTotalMaxSize = kCols * kRows / kPackets; // Maximo Total de Elementos

extern "C" {

// Funcion Softmax: Implementacion Secuencial
    void softmax(RawDataT *in1, RawDataT *out, uint64_t size);

} // extern "C"


#endif // __SOFTMAX_H__
