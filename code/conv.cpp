#include "conv.h"

void conv2d( TwoD_IPT X[num_chnl_ip], TwoD_wtT W[num_chnl_op], TwoD_outT Z[num_chnl_op], float b[num_chnl_op] )
{



	
    TwoD_wtT filters[num_chnl_op] = {0};
    int Zc = 0, Zm = 0, Zn = 0;

    Zc = num_chnl_op;
    Zn = inputWidth - kernelSize + 1;
    Zm = inputWidth - kernelSize + 1;
    int N = num_chnl_op; // number of filters
    int j = 0, k = 0;

    TwoD_wtT currFilter = {0};

    for (size_t i = 0; i < Zc; i++)
    {
        for (size_t j = 0; j < Zn; j++)
        {
            for (size_t k = 0; k < Zm; k++)
            {
                /** TODO: floating point to fixed point */
                Z[i].Z_out[j][k] = 0.f;
            }
        }
    }


    for (size_t fp = 0; fp < N; fp++ )
    {
        memcpy(&filters[fp], &W[fp], sizeof(TwoD_wtT));

        j = 0;
        k = 0;
        do
        {
            do
            {
                for (int c = 0; c < num_chnl_ip; c++)
                {
                    for (int p = 0; p < kernelSize; p++)
                    {
                        for (int q = 0; q < kernelSize; q++)
                        {
                            Z[fp].Z_out[j][k] += (X[c].X_c[j + p][k + q]) * filters[fp].ith_filter[c].W_c[p][q] ;
                        }
                    }
                }
                j++;
            } while (j < Zn);
            j = 0;
            k++;
        } while (k < Zm);
    }


    for (size_t i = 0; i < Zc; i++)
    {
        for (size_t j = 0; j < Zn; j++)
        {
            for (size_t k = 0; k < Zm; k++)
            {
                Z[i].Z_out[j][k] += b[i];
            }
        }
    }
}


void EntryConv(TwoD_IPT X[num_chnl_ip],  TwoD_wtT W[num_chnl_op], TwoD_outT Z[num_chnl_op],  float b[num_chnl_op])
{
	#pragma HLS interface m_axi port=X,W depth=10
	#pragma HLS interface m_axi port=Z depth=10
	#pragma HLS interface s_axilite port=return
	#pragma HLS interface s_axilite port=X,W
	#pragma HLS interface s_axilite port=Z
	conv2d(X,W,Z,b);
}
