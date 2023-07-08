#include "conv.h"

float* conv2d_hls( TwoDTile_IPT x[],  TwoD_filterT Kernel_i[], float *z,  float b )
{
	unsigned short int numTilesCols = inputWidth - KernelSize +1;

	for (unsigned short int tileCol = 0; tileCol < numTilesCols; tileCol++)
	{
		for (size_t c = 0; c < num_chnl_ip; c++)
		{
			for (size_t p = 0; p < KernelSize; p++)
    		{
        		for (size_t q = tileCol; q < (tileCol+KernelSize); q++)
        		{
					z[tileCol]  += x[c].tileData[p][q] * Kernel_i[c].W_c[p][q-tileCol];
				}
			}
		}
	}

	for (size_t j = 0; j < outDim; j++)
	{
		z[j] += b;
		printf("z_bigTile_per_Opchnl: %f\n", z[j]);
	}
	return z;

}




void EntryConv(TwoD_IPT X[num_chnl_ip],  TwoD_wtT W[num_chnl_op], TwoD_outT Z[num_chnl_op],  float b[num_chnl_op] )
{

	#pragma HLS interface m_axi port=X,W depth=10
	#pragma HLS interface m_axi port=Z depth=10
	#pragma HLS interface s_axilite port=return
	#pragma HLS interface s_axilite port=X,W
	#pragma HLS interface s_axilite port=Z


	TwoDTile_IPT x_row[num_chnl_ip];
	TwoD_wtT w_set[num_chnl_op];

    size_t rowSize = sizeof(float) * inputWidth * KernelSize;

	for (size_t readImgRow = 0; readImgRow < (inputWidth -KernelSize +1); readImgRow++)
	{
		//printf("readImgRow %d", readImgRow);
		for (size_t i = 0; i < num_chnl_ip; i++)
		{
			for(size_t row=0; row < KernelSize; row++){
				for(size_t col=0; col<inputWidth; col++){
					x_row[i].tileData[row][col] = X[i].X_c[row+readImgRow][col];
				}
			}
			//Local BRAM at time t
			//memcpy(x_row[i].tileData, X[i].X_c + readImgRow, rowSize );
		}
		for (size_t i = 0; i < num_chnl_op; i++)
		{

			for (size_t j = 0; j < num_chnl_ip; j++)
			{
				for(size_t row=0; row<KernelSize; row++){
					for(size_t col=0; col<KernelSize; col++){
						w_set[0].ith_filter[j].W_c[row][col] = W[i].ith_filter[j].W_c[row][col];
					}
				}
				//memcpy( w_set[0].ith_filter[j].W_c, W[i].ith_filter[j].W_c,\
					 KernelSize*KernelSize*sizeof(float) );
			}
			float currZ_bigTile[outDim] = {0};
			float *output = conv2d_hls(x_row, w_set[0].ith_filter, currZ_bigTile , b[i]);
			//TODO: send result to processor here
		}
	}
}