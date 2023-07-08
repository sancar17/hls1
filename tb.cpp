#include <cstdio>
#include "conv.h"
#include <cstdlib>
#include <time.h>
#include <cmath>



#if 0
void convSW(float * x , float * w, float * z)
{
	for(int i = 0 ; i < ZM; i++){
		z[i] = 0;
		for(int j = 0; j< M; j++){
			z[i] += x[i + j] * w[j];
		}
	}
}
#endif






float X_c1[6][6] = {{0.8338, 0.7331, 0.1000, 0.0066, 0.6978, 0.3602},
         {0.1160, 0.7591, 0.2229, 0.5592, 0.7698, 0.5718},
         {0.9811, 0.7313, 0.6109, 0.3295, 0.2870, 0.8275},
         {0.8892, 0.3335, 0.1430, 0.2656, 0.6015, 0.2496},
         {0.7898, 0.2642, 0.8453, 0.7625, 0.5416, 0.7610},
         {0.2369, 0.2527, 0.7824, 0.0062, 0.2599, 0.2738}};

float X_c2[6][6] =   {{0.8671, 0.4562, 0.9880, 0.2685, 0.3802, 0.9617},
                    {0.0964, 0.5339, 0.6119, 0.1945, 0.4482, 0.6303},
                    {0.4495, 0.7026, 0.1514, 0.7907, 0.9677, 0.7058},
                    {0.7377, 0.1865, 0.6883, 0.5104, 0.9320, 0.8761},
                    {0.5663, 0.2681, 0.4568, 0.2736, 0.4610, 0.6378},
                    {0.1463, 0.9184, 0.7258, 0.1479, 0.4591, 0.3726}};

float filter11[4][4] = {{ 0.0189,  0.1436, -0.0166,  0.0813},
                       { 0.1415, -0.0450, -0.1069, -0.0353},
                       {-0.0368, -0.0900, -0.1355, -0.1176},
                       { 0.0823,  0.1657, -0.0419,  0.0325}};


float filter12[4][4] = {{-0.1457,  0.1455,  0.0073, -0.0699},
                      { 0.1143,  0.0721, -0.1370, -0.1525},
                      { 0.0727,  0.0669, -0.1113,  0.0931},
                      { 0.1302, -0.1425,  0.0716, -0.0465}};


float filter21[4][4] = {{ 0.0900,  0.1541, -0.0786,  0.0410},
                {-0.0267,  0.0911,  0.1181,  0.0144 },
                {-0.0384, -0.0370,  0.0829, -0.0172 },
                {-0.1476, -0.1344, -0.0383,  0.1080 }};

float filter22[4][4] =  {{ 0.1024, -0.1539, -0.0587,  0.0820},
                         { 0.1598, -0.1367, -0.1263, -0.1219},
                         {-0.0061,  0.0651, -0.1176,  0.1488},
                         { 0.0541,  0.1236, -0.0738,  0.0719}};
    
    
    float filter31[4][4] =    {{-0.0056,  0.0497, -0.1183, -0.1584},
          {-0.1657,  0.1138,  0.0189,  0.0983},
          { 0.1626, -0.0770,  0.0636, -0.0190},
          { 0.0865,  0.1564, -0.0294, -0.0549}};

    
    
    float filter32[4][4] =  {{-0.1090, -0.0785, -0.0962, -0.0180},
          { 0.1006, -0.1592, -0.0006,  0.0779},
          { 0.1444, -0.0049, -0.0058,  0.1632},
          {-0.0040,  0.0338, -0.0396, -0.0982}};

 

float b[3] = {0.0517, -0.0497, -0.1375} ;


       float zc1[3][3] = {{ 0.1139,  0.0917, -0.3517},
         { 0.3261,  0.0718,  0.0579},
         {-0.0750, -0.1473, -0.0375}};

        float zc2[3][3] = {{ 0.0743,  0.1085,  0.1758},
         {-0.1205,  0.0558, -0.1705},
         { 0.0792,  0.1033, -0.1464}};

        float zc3[3][3] = {{ 0.0787, -0.1685, -0.1651},
         {-0.0581, -0.0650, -0.1303},
         {-0.0671, -0.1451, -0.1179}};








int main(){
//printf("starting entryCOnv");
	int ntests = 1;
	TwoD_outT *z = new   TwoD_outT[ num_chnl_op* ntests];
	TwoD_outT *ref = new TwoD_outT[ num_chnl_op* ntests];
	TwoD_IPT *x = new TwoD_IPT[ num_chnl_ip* ntests];
	TwoD_wtT *w = new TwoD_wtT[num_chnl_op* ntests];
	float b_true[num_chnl_op] = {0}; 
    
    

        memcpy( x[0].X_c, X_c1, sizeof(X_c1));
        memcpy( x[1].X_c, X_c2, sizeof(X_c2));

        memcpy( w[0].ith_filter[0].W_c, filter11, sizeof(filter11));
        
        memcpy( w[0].ith_filter[1].W_c, filter12, sizeof(filter12));
        
        memcpy( w[1].ith_filter[0].W_c, filter21, sizeof(filter21));
        memcpy( w[1].ith_filter[1].W_c, filter22, sizeof(filter22));

        memcpy( w[2].ith_filter[0].W_c, filter31, sizeof(filter31));
        memcpy( w[2].ith_filter[1].W_c, filter32, sizeof(filter32));


        memcpy( b_true, b, sizeof(b_true));

        memcpy( ref[0].Z_out, zc1, sizeof(zc1));
        memcpy( ref[1].Z_out, zc2, sizeof(zc2));
        memcpy( ref[2].Z_out, zc3, sizeof(zc3));
	//printf("starting entryCOnv");
	EntryConv(x , w , z, b_true);
	int i = 0;
	#if 0 
	for(int n =0; n < num_chnl_op ; n++){
        for (size_t i = 0; i < outDim; i++)
        {
		    for(int j = 0 ; j < outDim; j++){

		    	if(fabs(ref[n].Z_out[i][j] - z[n].Z_out[i][j]) > 1e-4){
		    		printf("Error %d: z:%f != ref:%f \n",i,z[n].Z_out[i][j],ref[n].Z_out[i][j]);
		    		return 1;
		    	}
            }
		}
	}
#endif
	//printf("All values are equal!\n");
	delete [] ref;
	delete [] x;
	delete [] w;
	delete [] z;
	return 0;
}
