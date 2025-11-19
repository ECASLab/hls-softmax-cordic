#ifndef CORDIC_EXP_HLS_H
#define CORDIC_EXP_HLS_H

#include <ap_fixed.h> // <W: Ancho Total, I: Bits Enteros(sign)>
#include <ap_int.h>   // <N: Enteros>

// Config. de Precision
typedef ap_fixed<24,8> fixed_t; // Q8.16
typedef ap_uint<4> iter_t; // Contador: Rango 0-15

// Constantes
namespace CORDICConstants {
    const fixed_t LN2 = 0.693147180559945; // Reduccion de Rango -  0.693145752
    const fixex_t INV_LN2 = 1.442695040888963; // - 1.442687988
    const fixed_t MAX_REDUCED = 0.347; // - 0.346984863
    const iter_t MAX_ITERATIONS = 10;

        // Tabla de Ang. HB: ANGLES[k-1] = atanh(2^-k)
    const fixed_t ANGLES[12] = {
        0.5493061443340549,   // k=1
        0.25541281188299536,  // k=2
        0.12565721414045303,  // k=3
        0.06258157147700301,  // k=4
        0.031260178490666565, // k=5
        0.015626271262875565, // k=6
        0.007812661760296865, // k=7
        0.003906269176061193, // k=8
        0.001953127835361916, // k=9
        0.0009765619494331953,// k=10
        0.00048828121119489827,// k=11
        0.00024414062014936177 // k=12
    };

}; //

// Estado Interno
struct CORDICState
{
    fixed_t X, // cosh(z) 
            Y, // sinh(z)
            Z; // Ang. Residual: Conv. a 0

    //Constr: Init (X,Y,Z) = (1,0,0)
    CORDICState() : X(1.0), Y(0.0), Z(0.0) {} 
};

// Movimiento - CORDIC
inline void cordic_hyperbolic_step(CORDICState& s, int k, const fixed_t& angle) {
#pragma HLS INLINE
    
    // 1. Det. Direccion de Rotacion
    const bool positive = (s.Z >= fixed_t(0));

    // 2. Cal. Desplazamiento (2^k)
    const fixed_t x_shift = s.X >> k;
    const fixed_t y_shift = s.Y >> k;

    // 3. Act. Estado segun Sign(Z)
    if(positive) {
        // a. Rotar Positivo
        s.X = z.X + y_shift;
        s.Y = s.Y + x_shift;
        s.Z = s.Z - angle; // Red. Z
    } else {
        // b. Rotar Negativo
        s.X = s.X - y_shift;
        s.Y = s.Y - x_shift;
        s.Z = s.Z + angle; // Incr. Z
    }
}

// Nucleo - CORDIC
inline CORDICState cordic_hyperbolic_core(fixed_t z_input) {
#pragma HLS INLINE off

    // 1. Init. Estado: (X,Y,Z) = (1, 0, z_input)
    CORDICState s;
    s.Z = z_input;

    // 2. Sec. de Iter. CORDIC
    cordic_hyperbolic_step(s, 1, CORDICConstants::ANGLES[0]); // k=1
    cordic_hyperbolic_step(s, 2, CORDICConstants::ANGLES[1]); // k=2
    cordic_hyperbolic_step(s, 3, CORDICConstants::ANGLES[2]); // k=3
    cordic_hyperbolic_step(s, 4, CORDICConstants::ANGLES[3]); // k=4
    cordic_hyperbolic_step(s, 4, CORDICConstants::ANGLES[3]); // k=4 Rep
    cordic_hyperbolic_step(s, 5, CORDICConstants::ANGLES[4]); // k=5
    cordic_hyperbolic_step(s, 6, CORDICConstants::ANGLES[5]); // k=6
    cordic_hyperbolic_step(s, 7, CORDICConstants::ANGLES[6]); // k=7
    cordic_hyperbolic_step(s, 8, CORDICConstants::ANGLES[7]); // k=8
    cordic_hyperbolic_step(s, 9, CORDICConstants::ANGLES[8]); // k=9


    // 3. Ret. Estado Final: s.X = cosh(z) s.Y = sinh(z)
    return s;

}

// Fun. Principal e^x
inline float cordic_exp_hls(float x_input) {
#pragma HLS INLINE off

    // Conversion y Entrada (float -> fixed_t)
    fixed_t x = x_input;

    // 1. Saturacion
    if (x > fixed_t(8.0)) x = fixed_t(8.0); // Sat. Superior
    if (x < fixed_t(-8.0)) x = fixed_t(-8.0); // Sat. Inferior

    // 2. Valor Abs 
    fixed_t abs_x;
    if (x >= fixed_t(0))
    {
        abs_x = x; // x positivo -> Mantener
    
    } else 
    {
        abs_x = -x; // x negativo -> Negar 
    }
    
    // 3. Reduccion de Rango: e^x = 2^n * e^(x')
    ap_int<6> n = 0; 
    fixed_t x_prime = x; // Residuo Reducido

    // Solo reducir si x es grande
    if (abs_x > CORDICConstants::MAX_REDUCED) 
    {
        fixed_t half = fixed_t(0.5); // Redondeo

        // Calc. n = round(x / LN2)
        fixed_t scaled = x * CORDICConstants::INV_LN2;
        
        // a. Redondeo
        if (scaled >= fixed_t(0))
        {
            // Para Num +
            n = static_cast<int>(scaled + half); // Redondea hacia Arriba
        } else
        {
            // Para Num -
            n = static_cast<int>(scaled - half); // Redondea hacia Abajo
        }

        // b. Extrac. de Residuo
        x_prime = x - fixed_t(n) * CORDICConstants::LN2;
    }
    
        

        

            

            

        

        

    // 4. Iter. CORDIC HB

    // 5. Reconstruccion: e^(x') = cosh(x') + sinh(x')

    // 6. Restauracion: e^x = e^(x') * 2^n 

    // Conversion y Salida (fixed_t -> float)

}


#endif // CORDIC_EXP_HLS_H