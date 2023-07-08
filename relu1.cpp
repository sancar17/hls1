#define outputWidth 3

float *ReLU(float *X, float *Z){

#pragma HLS unroll factor=outputWidth
    for(int i=0; i<outputWidth; i++){
        Z[i] = X[i];

        if(Z[i]<0){
            Z[i] = 0;
        }
    }
}

/*void ReLU(Tensor *X , Tensor *Z)
{ 

    //x ... holds individual feature maps from 3D object X
    //z ... holds
    Tensor x;
    Tensor z;
    
    //Step 1: loading one feature map of 3D object X in
        //Subnote 1: RAM PL is small. Load only 1 feature map in.
        //Subnote 2: no pipelining different feature maps. DRAM only one read port -> only one value per timestep can be read.
        //Subnote 3: if pipelining ... there will be several values read in the same timestep from different units from the same port.
    for(int i=0; i < X ->size[0]; i++){
        //load the the current feature map from X into x.
        for (size_t height = 0; height < X->size[1]; height++)
        {
            for (size_t width = 0; width < X->size[2]; width++)
            {
                x.data[0][height][width] = X->data[i][height][width];
            }
        
        }
        for(int j=0; j < X ->size[1]; j++){
//Step 2: Pipeling the ReLU-computation for the whole row.
    //Subnote 1: feature maps is a grid with rows.
    //Subnote 2: all rows get processed in an overlapped manner via pipeline.
#pragma HLS pipeline II=1
            for(int k=0; k < X->size[2]; k++){
//Step 3: Compute the ReLU for all column elements for current row parallel.
#pragma HLS UNROLL
                float tmp = x.data[0][j][k];
                float val = 0;
                if(tmp > 0){
                    val = tmp;
                }
                //Step 4: Write results to z.
                    //Subnote 1: there can be made only one writing operation to Z at a time.
                    //Subnote 2: UNROLL ... there will be (#column x writing) per operation to Z per unit. More than 1.
                    //Subnote 3: write first to internal z. Element internal RAM. Multiple writings in one timestep possible.
                (z).data[i][j][k] = val;
            }
        }
        //Step 5: Write to Z outside pipeline.
            //Subnote: not more than one writing process per timestep possible.
        for (size_t l = 0; l < X->size[1]; l++)
        {
            for (size_t m = 0; m < X->size[2]; m++)
            {
                (*Z)[i][l][m] = z.data[i][l][m];
            }
            
        }
        
    }
}*/
