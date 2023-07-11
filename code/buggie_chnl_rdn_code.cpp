#include "conv.h"
#include <stdint.h>

// Function to convert float to fixed point
int32_t float_to_fixed(float x) {
    return (int32_t)(x * (1 << 16));
}

// Function to convert fixed point to float
float fixed_to_float(int32_t x) {
    return (float)x / (1 << 16);
}

// Function to multiply two fixed point numbers
int32_t fixed_multiply(int32_t x, int32_t y) {
    int64_t result = (int64_t)x * (int64_t)y;
    return (int32_t)(result >> 16);
}

void conv2d_hls( TwoDTile_IPT x[],  TwoD_filterT Kernel_i[], int32_t z[],  int32_t b, int32_t acc[])
{
	unsigned short int numTilesCols = inputWidth - KernelSize +1;


Loop1: for (unsigned short int tileCol = 0; tileCol < numTilesCols; tileCol++)
		{
			//TODO: z[tileCol] needs to be reset after all channels
			//z[tileCol] = 0;

		//for (size_t i = 0; i <  chnl_rdn_factor; i++)
		//{
subLoop1:for (size_t c = 0; c < num_chnl_ip/chnl_rdn_factor; c++)
		{

	/** TODO: uncomment below pragma, temporarily commented for sample test*/
#pragma HLS PIPELINE II=96
subLoop2:	for (size_t p = 0; p < KernelSize; p++)
    		{
//#pragma HLS UNROLL factor=KernelSize
subLoop3:        		for (size_t q = tileCol; q < (tileCol+KernelSize); q++)
        		{
//#pragma HLS UNROLL factor=KernelSize
#pragma HLS array_partition complete

					acc[(q-tileCol) +  (KernelSize*p) ] = fixed_multiply(float_to_fixed(x[c].tileData[p][q]), float_to_fixed(Kernel_i[c].W_c[p][q-tileCol]));
        		}
			}
	subLoop11:	for(size_t m = 0; m < KernelSize*KernelSize; m++)
			{
#pragma HLS PIPELINE II=1
#pragma HLS array_partition complete
				z[tileCol] += acc[m];
			}
		}
		//}


	}

	//for (size_t j = 0; j < outDim; j++)
	//{
		//z[j] += b;
		//printf("z: %f\n", fixed_to_float(z[j]));
	//}
}

void EntryConv(TwoD_IPT X[num_chnl_ip],  TwoD_wtT W[num_chnl_op], TwoD_outT Z[num_chnl_op],  float b[num_chnl_op] )
{
#pragma HLS interface m_axi port=X,W depth=390
#pragma HLS interface m_axi port=Z depth=390
#pragma HLS interface m_axi port=b depth=390
#pragma HLS interface s_axilite port=return
#pragma HLS interface s_axilite port=X,W
#pragma HLS interface s_axilite port=Z
	TwoDTile_IPT x_row[num_chnl_ip/chnl_rdn_factor];
	TwoD_wtT w_set[num_chnl_op];
	int32_t z_[outDim] = {0};
	int32_t acc[KernelSize*KernelSize] = {0};

	for (size_t readImgRow = 0; readImgRow < (inputWidth -KernelSize +1); readImgRow++)
	{
		//reset z_[] for bigTile and acc[] for bigTile
		//z_[outDim] = {0};
		//acc[KernelSize*KernelSize] = {0};
		for (size_t op = 0; op < num_chnl_op; op++)
		{
			z_[outDim] = {0};
			acc[KernelSize*KernelSize] = {0};

		for (size_t i = 0; i <  chnl_rdn_factor; i++)
		{
			for( size_t j = 0; j < num_chnl_ip/chnl_rdn_factor; j++)
			{
				for(size_t row=0; row < KernelSize; row++)
				{
					for(size_t col=0; col<inputWidth; col++)
					{
						x_row[j].tileData[row][col] = X[( i* chnl_rdn_factor ) + j ].X_c[row+readImgRow][col];
					}
				}

				//10 wt
				for(size_t row=0; row<KernelSize; row++){
					for(size_t col=0; col<KernelSize; col++){

							w_set[0].ith_filter[j].W_c[row][col] = W[op].ith_filter[ ( i* chnl_rdn_factor ) + j].W_c[row][col];
					}
				}

			}
			//conv
			conv2d_hls(x_row, w_set[0].ith_filter, z_ , float_to_fixed(b[op]), acc);
			for( int u = 0; u < outDim; u++)
			{

				Z[op].Z_out[readImgRow][u] += fixed_to_float (z_[u]);
			}
		}

		for( int u = 0; u < outDim; u++)
		{
			Z[op].Z_out[readImgRow][u] += float_to_fixed (b[op]);
			printf("z: %f\n", ( Z[op].Z_out[readImgRow][u] ));
		}

		}
	}
}
