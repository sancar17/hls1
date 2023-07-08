#include "conv.h"

float* conv2d_hls( TwoDTile_IPT x[],  TwoD_filterT Kernel_i[], TwoD_outT z[],  float b )
{
	float z_bigTile_per_Opchnl[outDim] ={0};
	unsigned short int numTilesCols = inputWidth - KernelSize +1;

	for (unsigned short int tileCol = 0; tileCol < numTilesCols; tileCol++) 
	{
		for (size_t c = 0; c < num_chnl_ip; c++)
		{
			for (size_t p = 0; p < KernelSize; p++)
    		{
        		for (size_t q = tileCol; q < (tileCol+KernelSize); q++)
        		{
					z_bigTile_per_Opchnl[tileCol]  += x[c].tileData[p][q] * Kernel_i[c].W_c[p][q-tileCol];
				}
			}
		}
	}

	for (size_t j = 0; j < outDim; j++)
	{
		z_bigTile_per_Opchnl[j] += b;
		printf("z_bigTile_per_Opchnl: %f\n", z_bigTile_per_Opchnl[j]);
	}
	return z_bigTile_per_Opchnl;

}

  


void EntryConv(TwoD_IPT X[num_chnl_ip],  TwoD_wtT W[num_chnl_op], TwoD_outT Z[num_chnl_op],  float b[num_chnl_op] )
{
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
			float *currZ_bigTile = conv2d_hls(x_row, w_set[0].ith_filter, Z , b[i]);
			//TODO: send result to processor here	
		}
	}
}
