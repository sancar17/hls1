#include <stdio.h>
#include <cstring>
#define inputWidth  16//6
#define KernelSize 3//4
#define num_chnl_op 384//3
#define num_chnl_ip 384//2
#define NumKernels 384//3
#define outDim (inputWidth-KernelSize+1)


typedef struct TwoD_struct_input {
    float X_c[inputWidth][inputWidth];
} TwoD_IPT;

typedef struct TwoD_struct_kernel {
    float W_c[KernelSize][KernelSize];
} TwoD_filterT;

typedef struct TwoD_struct_wt {

    TwoD_filterT ith_filter[num_chnl_ip];

} TwoD_wtT;

typedef struct TwoD_struct_out {
    float Z_out[outDim][outDim];

}TwoD_outT;

typedef struct TwoDTile_IPT_tile{
	float tileData[KernelSize][inputWidth];
}TwoDTile_IPT;

void EntryConv(TwoD_IPT X[num_chnl_ip],  TwoD_wtT W[num_chnl_op], TwoD_outT Z[num_chnl_op],  float b[num_chnl_op] );
