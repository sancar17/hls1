// #include "cnn.h"
// #include "conv.h"

// namespace ml {

// CNN::CNN(std::vector<CNN_layer_struct> in_layers)
// {
// 	uint32_t insize = 0;
// 	layers = in_layers;
// 	// The tricky part is allocating the proper tensors
// 	for(int i = 0; i < layers.size(); i++){
// 		CNN_layer_struct & lay = layers[i];
// 		// lay->X is the input lay->Z is the output
// 		switch(lay.type){
// 			case Layer_Type::ReLU: case Layer_Type::Softmax:
// 				lay.output_size[0] = layers[i - 1].output_size[0];
// 				lay.output_size[1] = layers[i - 1].output_size[1];
// 				lay.output_size[2] = layers[i - 1].output_size[2];
// 				if(lay.in_place)
// 					lay.Z = layers[i - 1].Z;
// 				else
// 					lay.Z = new Tensor(layers[i-1].output_size[0],lay.output_size[1],lay.output_size[2]);
// 				break;
// 			case Layer_Type::Pool:
// 				lay.Z = new Tensor(lay.output_size[0],lay.output_size[1],lay.output_size[2]);
// 				break;
// 			case Layer_Type::Conv:
// 				lay.Z = new Tensor(lay.output_size[0],lay.output_size[1],lay.output_size[2]);
// 				lay.W = new Tensor[lay.output_size[0]]();
// 				for(int i =0 ; i < lay.output_size[0]; i++){
// 					lay.W[i].allocate(lay.input_channels,lay.kernel_width,lay.kernel_width);
// 				}
// 				lay.B = new Tensor(1,1,lay.output_size[0]);
// 				break;
// 			case Layer_Type::Linear:
// 				insize = layers[i-1].output_size[0] * layers[i-1].output_size[1] * layers[i-1].output_size[2];
// 				lay.Z = new Tensor(1,1,lay.output_size[2]);
// 				lay.W = new Tensor(1,lay.output_size[2],insize);
// 				lay.B = new Tensor(1,1,lay.output_size[2]);
// 				break;
// 			default:
// 				throw std::runtime_error("Layer not implemented !\n");
// 		}
// 	}
// }



// CNN::~CNN()
// {
// 	for(int i = 0; i < layers.size(); i++){
// 		CNN_layer_struct & lay = layers[i];
// 		// lay->X is the input lay->Z is the output
// 		switch(lay.type){
// 			case Layer_Type::ReLU:
// 				if(!(lay.in_place))
// 					delete lay.Z;
// 				break;
// 			case Layer_Type::Softmax:
// 			case Layer_Type::Pool:
// 				delete lay.Z;
// 				break;
// 			case Layer_Type::Conv:
// 				delete lay.Z;
// 				delete [] lay.W;
// 				delete lay.B;
// 				break;
// 			case Layer_Type::Linear:
// 				delete lay.Z;
// 				delete lay.W;
// 				delete lay.B;
// 				break;
// 			default:
// 				printf("Rogue unimplemented layer found during deallocation !\n");
// 		}
// 	}
// }



// /* Implement Inference here !*/
// Tensor * CNN::inference(Tensor * input)
// {
// 	Tensor * X = input;
// 	//printf("%s", "a \n");

// 	for(int i = 0; i < layers.size(); i++){
// 		//printf("%d", i);
// 		CNN_layer_struct & lay = layers[i];
// 		auto start = mtick();
// 		switch(lay.type){
			
// 			case Layer_Type::ReLU:
// 				switch(lay.in_place){
					
				 
// 					case true:
// 						//printf("%s", "relu in place \n");
// 						ReLU(X, X);
// 						runtime[2] += mtock(start);
// 						break;
// 					case false:
// 						//printf("%s", "relu not in place \n");
// 						ReLU(X, lay.Z);
// 						runtime[2] += mtock(start);
// 						break;
					
					
// 				}
// 				break;

// 			case Layer_Type::Softmax:
				
				 
// 				//printf("%s", "softmax \n");
// 				Softmax(X, lay.Z);

// 				runtime[4] += mtock(start);
				
// 				break;

// 			case Layer_Type::Pool:
// 				//printf("%s", "pool \n");

// 				maxPool(X, lay.Z);

// 				runtime[1] += mtock(start);
// 				break;

// 			case Layer_Type::Conv:
// 				if(lay.pad!=0){
// 				//printf("%s", "will perform padding\n");
// 				X = padTensor(X, lay.pad);
// 				//printf("%s", "performed\n");
// 				}
// 				//printf("%s", "conv \n");
// 				//CIRCULAR CONV
// 				conv2d(X, lay.W, lay.B, lay.Z);

// 				// //WINOGRAD TRANSFORMATION    
// 				Tensor * W_wino = winoWeights(lay.W, lay.Z->size[0]);    
// 				//printf("kernel_size: %d\n", lay.kernel_width);//// print kernel_size
// 				convWinograd(X, W_wino, lay.B, lay.Z,  lay.W->size[2]); 

// 				runtime[3] += mtock(start);

// 				break;

// 			case Layer_Type::Linear:
// 				//printf("%s", "linear \n");

// 				Linear(X, lay.W, lay.B, lay.Z);

// 				runtime[0] += mtock(start);


// 				break;

// 			default:
// 				throw std::runtime_error("Layer not implemented !\n");
// 		}

// 		X = lay.Z;
// 	}

// 	//printf("%s", "return \n");

// 	return X;
// }

// }

#include "cnn.h"
#include "conv.h"

namespace ml {

CNN::CNN(std::vector<CNN_layer_struct> in_layers)
{
	uint32_t insize = 0;
	layers = in_layers;
	// The tricky part is allocating the proper tensors
	for(int i = 0; i < layers.size(); i++){
		CNN_layer_struct & lay = layers[i];
		// lay->X is the input lay->Z is the output
		switch(lay.type){
			case Layer_Type::ReLU: case Layer_Type::Softmax:
				lay.output_size[0] = layers[i - 1].output_size[0];
				lay.output_size[1] = layers[i - 1].output_size[1];
				lay.output_size[2] = layers[i - 1].output_size[2];
				if(lay.in_place)
					lay.Z = layers[i - 1].Z;
				else
					lay.Z = new Tensor(layers[i-1].output_size[0],lay.output_size[1],lay.output_size[2]);
				break;
			case Layer_Type::Pool:
				lay.Z = new Tensor(lay.output_size[0],lay.output_size[1],lay.output_size[2]);
				break;
			case Layer_Type::Conv:
				lay.Z = new Tensor(lay.output_size[0],lay.output_size[1],lay.output_size[2]);
				lay.W = new Tensor[lay.output_size[0]]();
				for(int i =0 ; i < lay.output_size[0]; i++){
					lay.W[i].allocate(lay.input_channels,lay.kernel_width,lay.kernel_width);
				}
				lay.B = new Tensor(1,1,lay.output_size[0]);
				break;
			case Layer_Type::Linear:
				insize = layers[i-1].output_size[0] * layers[i-1].output_size[1] * layers[i-1].output_size[2];
				lay.Z = new Tensor(1,1,lay.output_size[2]);
				lay.W = new Tensor(1,lay.output_size[2],insize);
				lay.B = new Tensor(1,1,lay.output_size[2]);
				break;
			case Layer_Type::FPGAConv:
				lay.Z = new Tensor(num_chnl_op,outDim, outDim );
				lay.W = new Tensor[num_chnl_op]();
				for(int i =0 ; i < num_chnl_op; i++){
					lay.W[i].allocate(num_chnl_ip,KernelSize,KernelSize);
				}
				lay.B = new Tensor(1,1,num_chnl_op);
				break;

			default:
				throw std::runtime_error("Layer not implemented !\n");
		}
	}
}



CNN::~CNN()
{
	for(int i = 0; i < layers.size(); i++){
		CNN_layer_struct & lay = layers[i];
		// lay->X is the input lay->Z is the output
		switch(lay.type){
			case Layer_Type::ReLU:
				if(!(lay.in_place))
					delete lay.Z;
				break;
			case Layer_Type::Softmax:
			case Layer_Type::Pool:
				delete lay.Z;
				break;
			case Layer_Type::Conv:
				delete lay.Z;
				delete [] lay.W;
				delete lay.B;
				break;
			case Layer_Type::Linear:
				delete lay.Z;
				delete lay.W;
				delete lay.B;
				break;
			case Layer_Type::FPGAConv:
				delete lay.Z;
				delete [] lay.W;
				delete lay.B;
				break;

			default:
				printf("Rogue unimplemented layer found during deallocation !\n");
		}
	}
}



/* Implement Inference (classification) here !
 * The input is a tensor of size (1,1,28,28) and the output is a tensor of size (1,1,10)
 * The output is the probability of each class
 * Softmax is the last layer
 * After a convolutional layer, no convolutional layer and no pooling layer can be used
 * The information of the different layers is in the layers vector: type, sequence, configuration
 * Smallnet, mediumNet and bigNet are CNNs that take 3x128x128 images as input and classify them into 100 classes
 * Layer Struct: type, output_size[3] = size of Z, kernel_width = size of W, input_channels( size[0] of previous layers Z ), 
 * pad = size of the zero-padding, bool in_place, Z(output tensor),W (weight tensor array),B (bias tensor)
 * */

Tensor * CNN::inference(Tensor * input)
{
	//medium
	Tensor * X = input;
	/***************************************/
	PYNQ_loadBitstream( (char*) "");
	PYNQ_MMIO_WINDOW led, hls;
	PYNQ_createMMIOWindow( &led, 0x40010000, 8);
	PYNQ_createMMIOWindow( &hls, 0x40000000, 64);
	uint32_t* b_led = (uint32_t*) led.buffer;
	b_led[1] = 0;
	b_led[0] = 3;

	

		
	 
	/***********************************/

	for(int i = 0; i < layers.size(); i++){
		CNN_layer_struct & lay = layers[i];
		
		switch (lay.type) {
    		case Layer_Type::Linear:
			{
				auto start = mtick();
				Linear(X, lay.W, lay.B, lay.Z);
				double time = mtock(start);
				runtime[0] += time;
				//printf("Type: Linear\n");
				break;
			}
    		case Layer_Type::Pool:
			{
				auto start = mtick();
				maxPool(X, lay.Z);
				double time = mtock(start);
				runtime[1] += time;
				//printf("Type: Pool\n");
				break;
			}
    		case Layer_Type::ReLU:
			{
				auto start = mtick();
				ReLU(X, lay.Z);
				double time = mtock(start);
				runtime[2] += time;
				//printf("Type: ReLU\n");
				break;
			}
    		case Layer_Type::Conv:
			{
				 //for basic conv
		
				 auto start = mtick();
				 X = padTensor(X, lay.pad);
				 //Basic conv on Processor: 
				 conv2d(X, lay.W, lay.B, lay.Z);
				 double time = mtock(start);
				 runtime[3] += time;
				 //printf("Type: Conv\n");
				// break;
				
				
				//// // for winograd
				//auto start = mtick();
				//Tensor * W_wino = winoWeights(lay.W, lay.Z->size[0]);
				//// print kernel_size
				////printf("kernel_size: %d\n", lay.kernel_width);
				//convWinograd(X, W_wino, lay.B, lay.Z,  lay.W->size[2]);
				//double time = mtock(start);
				//runtime[3] += time;
				////printf("Type: Conv\n");
				break;
				
				
				
				//FFT
				// auto start = mtick();
				// C_Tensor * W_wino = fftWeights(lay.W, lay.Z->size[0]);
				// convFFT(X, W_wino, lay.B, lay.Z,  lay.W->size[2]);
				// double time = mtock(start);
				// runtime[3] += time;
				// //printf("Type: Conv\n");
				// break;
				

			}
    		case Layer_Type::Softmax:
		    {
				auto start = mtick();
				Softmax(X, lay.Z);
				double time = mtock(start);
				runtime[4] += time;
				//printf("Type: Softmax\n");
				break;
			}
			case Layer_Type::FPGAConv:
				{
					//shared ptr
					tensorToFpgaStructP fpgaInput =  tensorTofpgaStruct(X, lay.W, lay.B);
					//printf(" tensor to fpga struct conv successs\n");
					
					/********************************************PYNQ FPGA CONFIG CONT. ***************************************************/
					 tensorToFpgaStructP fpgaInput =  tensorTofpgaStruct(X, lay.W, lay.B);

					PYNQ_SHARED_MEMORY sm_x, sm_w, sm_z, sm_b;
					PYNQ_allocateSharedMemory( &sm_x, num_chnl_ip*inputWidth*inputWidth*sizeof(float) ); //complete Image flattened
					PYNQ_allocateSharedMemory( &sm_w, num_chnl_ip*num_chnl_op*KernelSize*KernelSize*sizeof(float) ); //Array of Filters flattened 
					PYNQ_allocateSharedMemory( &sm_z, num_chnl_op*outDim*outDim*sizeof(float) ); //Flattened memory for Storing Z result from FPGA output  
					PYNQ_allocateSharedMemory( &sm_b, num_chnl_op*sizeof(float) ); //Flattened memory for Bias  


					float* virt_x = ( float*)sm_x.pointer;
					float* virt_w = (float*)sm_w.pointer;
					float* virt_z = (float*)sm_z.pointer; 
					float* virt_b = (float*)sm_z.pointer; 


					size_t size_x = inputWidth * inputWidth * sizeof(float);

					for (size_t i = 0; i < num_chnl_ip; i++)
					{
					    memcpy(virt_x + (inputWidth * inputWidth * i), fpgaInput->X[i].X_c, size_x);
					}

					size_t size_w = KernelSize * KernelSize * sizeof(float);


					for( size_t c = 0; c < num_chnl_op; c++)
					{
						for (size_t i = 0; i < num_chnl_ip; i++)
						{
						    memcpy( virt_x + (KernelSize * KernelSize * i), fpgaInput->W[c].ith_filter[i].W_c, size_w );
						}
					}

					/** TODO: check copying of Z in FPGA to this shared pointer */

					size_t size_b = num_chnl_op*sizeof(float);
					for (size_t i = 0; i < num_chnl_op; i++)
					{
						memcpy(virt_b + (i), fpgaInput->B, size_b);
					}

					/** Copy the address over axi_lite 
					 *	The shared pointer is given to IP via AXI_lite interface 
					 */	
					memcpy( hls.buffer + /** offset */, &(sm_x.physical_addr), sizeof(size_t) );
					memcpy( hls.buffer + /** offset */, &(sm_w.physical_addr), sizeof(size_t) );
					memcpy( hls.buffer + /** offset */, &(sm_z.physical_addr), sizeof(size_t) );
					memcpy( hls.buffer + /** offset */, &(sm_b.physical_addr), sizeof(size_t) );


     
					/** 
					 * Monitor this variable for idle or not to check if FPGA finished the task 
					 * Also to start the IP running
					 */ 
					uint32_t* hls_ctrl = (uint32_t*) hls.buffer;

					//monitor hls, test wait for hls, refer synthesis report

					 //wait until 3rd b 
					//memcpy(sm_x.virtual,&
					//memcpy(sm_x.virtual,&(fpgaInput->X[0].X_c[0][0]),sizeof(float) );
					 /**************************************PYNQ_FPGA CONFIG ENDS************************************************************/

					auto start = mtick();
					//load bit stream
					//fpgaInput->;
					//pynqHandling( conv.bit);
					//printf("Starting EntryConv\n");
					//EntryConv(fpgaInput->X,  fpgaInput->W, fpgaInput->Z,  fpgaInput->B );
					//printf("EntryConv success\n");

					//Start the Convlution on FPGA
					*ptr_ctrl = 0b1; //setting first bit to start
					
					//Wait until FPGA returns
					while( ! (*ptr_ctrl & 0b100 ) ) {};
					

					double time = mtock(start);
					runtime[5] += time;
					//printf("Starting fpga to tensor  struct conv\n");


					printf("%f\n", virt_z[i]);
					printf("\nFPGA processing finished, exiting...\n");
				
					//TODO: free memory hls.buffer and other #####important#######
					PYNQ_closeMMIOWindow(&led); 

					
					exit(0);


					Tensor* fpgaStructToTensor  = fpgaStructToTensorFunc(   fpgaInput->Z );
					//printf("fpga to tensor  struct conv success\n");

					lay.Z = fpgaStructToTensor;
					break;
				}
			default:
				throw(std::runtime_error("Rogue unimplemented layer found during inference !\n"));
		}
		X = lay.Z;
	}
	return X;
}



}
