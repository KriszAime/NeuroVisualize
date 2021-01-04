
#ifndef _header_NeuralNet
#define _header_NeuralNet

//-
typedef unsigned int uint;
typedef unsigned long ulong;
typedef unsigned char byte;
//-

//-
typedef enum NeuralNet_WH_type
{
	NNtypeRandom,
	NNtypeConstant
}NeuralNet_WH_type;
//-

//-Structs
typedef	struct NeuralNet
{
	struct
	{
		float** Inputs; //[pattern][input]
		float** Targets; //[pattern][target]
		uint PatternCount;
	}Patterns;
	struct
	{
		struct  //Input - Hidden Weights
		{
			float** Weight; //[input][hidden]
		}IH;
		struct  // Hidden - Output Weights
		{
			float** Weight; //[output][hidden]
		}HO;
	}Weights;
	struct 
	{
		struct
		{
			float** Hidden; //[pattern][hidden]
			float** Output; //[pattern][output]
		}Sum;
		struct 
		{
			float** Hidden; //[pattern][hidden]
			float** Output; //[pattern][output]
		}Values;
	}Points;
	struct
	{
		float* Hidden; //[hidden]
		float* Output; //[output]
		float* SumOutputWeight; //[hidden]
		float** WeightIH; //[input][hidden]
		float** WeightHO; //[output][hidden]
	}Deltas; //
	struct 
	{
		float eta, alpha, smallwt;
		struct { uint Icount, Hcount, Ocount; }NeuronCount;
	}Settings;
}NeuralNet;
//-



//-Methods

//Setup Network & nodes. (minimum of 1)
void NeuralNet_Construct(NeuralNet* _nnet, uint _InputCount, uint _hiddenCount, uint _OutputCount, float _eta, float _alpha, float _smallwt, NeuralNet_WH_type _whtype);
void NeuralNet_Destruct(NeuralNet* _nnet);

/*Adds a new training pair to Training vector.
* input & output values copied internally
*/
void NeuralNet_addTrainingPattern(NeuralNet* _nnet, float* inputs, float* outputs);

//Visszatér az utolsó epoch error - értékével.
float NeuralNet_doEpoch(NeuralNet* _nnet, uint _times);

//Visszatér a jelenlegi Súlyok neurális kimenetével.
float* NeuralNet_getOutputActivation(NeuralNet* _nnet, float* _inputs);

//-

#endif
