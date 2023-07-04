#include <cstdlib>
#include <time.h>
#include <cmath>
#include <cstring>
#include <cstdio>

#define inputWidth  6
#define kernelSize 4
#define num_chnl_op 3
#define num_chnl_ip 2

#define outDim (inputWidth-kernelSize+1)

typedef struct TwoD_input {
    float X_c[inputWidth][inputWidth];
} TwoD_IPT;

typedef struct TwoD_filter {
    float W_c[kernelSize][kernelSize];
} TwoD_filterT;

typedef struct TwoD_struct_wt {

    TwoD_filterT ith_filter[num_chnl_ip];

} TwoD_wtT;

typedef struct TwoD_struct_output {
    float Z_out[outDim][outDim];

}TwoD_outT;

void EntryConv( TwoD_IPT X[num_chnl_ip], TwoD_wtT W[num_chnl_op], TwoD_outT Z[num_chnl_op], float b[num_chnl_op]);
void conv2d( TwoD_IPT X[num_chnl_ip], TwoD_wtT W[num_chnl_op], TwoD_outT Z[num_chnl_op], float b[num_chnl_op] );

