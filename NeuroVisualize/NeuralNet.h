
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
		float** Inputs;
		float** Targets;
	}Patterns;
	struct
	{
		struct  //Input - Hidden Weights
		{
			float **DeltaWeight, **Weight; //[input][hidden]
			struct  { uint Icount, Hcount; }Count;
		}IH;
		struct  // Hidden - Output Weights
		{
			float **DeltaWeight, **Weight; //[output][hidden]
			struct  { uint Hcount, Ocount; }Count;
		}HO;
	}Weights;
	struct 
	{
		float eta, alpha, smallwt;
	}Settings;
}NeuralNet;
//-



//-Methods

//Setup Alapértelmezett 1 bemenetes, 1 kimenetes. xy Hidden nodes. (minimum of 1)
void NeuralNet_Construct(NeuralNet* _nnet, uint _hiddenCount, float _eta, float _alpha, float _smallwt, NeuralNet_WH_type _whtype);
void NeuralNet_Destruct(NeuralNet* _nnet);

//Adds a new training pair to Training vector.
void NeuralNet_addTrainingPoint(NeuralNet* _nnet, float input, float output);

//Visszatér az utolsó epoch error - értékével.
float NeuralNet_doEpoch(NeuralNet* _nnet, uint _times);

//Visszatér a jelenlegi Súlyok neurális kimenetével.
float NeuralNet_getOutputActivation(NeuralNet* _nnet, float _input);

//-

#endif
