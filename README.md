# hls-softmax-cordic

- **ap_fixed<24,8>** FW 16 Resolution 2^-16 = 0.0000152587890625 - Data type is used for internal calculations to ensure sufficient precision.
  

## List of Fixed-Point Data Types Used

- ap_fixed<8,7> FW 1 -> Resolution 2^-1 = 0.5 
  - Just works for iteration 1
  - Does not work for this implementation because the range is too small for hyperbolic functions.
  - Initially, it does not work with the max reduced value because 0.347 cannot be represented with this data type.
  - It will loss representation in LN(2) and INV_LN2 constants. 

- ap_fixed<10,7> FW 3 -> Resolution 2^-3 = 0.125 
  - Just works for iterations 1 to 3

- ap_fixed<12,7> FW 5 -> Resolution 2^-5 = 0.03125 - Just works for iterations 1 to 5

- ap_fixed<14,7> FW 7 -> Resolution 2^-7 = 0.0078125 - Just works for iterations 1 to 7

- ap_fixed<16,7> FW 9 -> Resolution 2^-9 = 0.001953125 - Just works for iterations 1 to 9

- ap_fixed<18,7> FW 11 -> Resolution 2^-11 = 0.00048828125 - Just works for iterations 1 to 11

- ap_fixed<20,7> FW 13 -> Resolution 2^-13 = 0.0001220703125 - Just works for iterations 1 to 13

- ap_fixed<24,7> FW 17 -> Resolution 2^-17 = 7.62939453125e-05 - Just works for iterations 1 to 17

## CORDIC Hiperbolic Angles Table (atanh(2^-i)):
iterations | Angle (radians) | 
------------------------------
1          | 0.5493061443340549
2          | 0.25541281188299536
3          | 0.12565721414045302
4          | 0.06258157147700301
5          | 0.031260178490666998
6          | 0.015626271752052288
7          | 0.0078126589515404865
8          | 0.003906269868396825
9          | 0.001953127483532549
10         | 0.0009765628104410351
11         | 0.00048828128880511277
12         | 0.0002441406298506382
13         | 0.0001220703131063298
14         | 0.0000610351563257915
15         | 0.0000305175781344738
16         | 0.00001525878906131576
17         | 0.00000762939453110197
18         | 0.0000038146972656065
# Important Notes for CORDIC Implementation

## CORDIC Constants

LN(2) = 0.6931471805599453 
INV_LN2 = 1.4426950408889634 
MAX_REDUCED = 0.347 

## Range Expansion