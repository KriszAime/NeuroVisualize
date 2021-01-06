
#ifndef _header_NeuralNet
#define _header_NeuralNet

//-
typedef unsigned int uint;
typedef unsigned long ulong;
typedef unsigned char byte;
//-


//-Structs
typedef	struct NeuralNet
{
	struct
	{
		uint* IndexArray;
		double** Inputs; //[pattern][input]
		double** Targets; //[pattern][target]
		uint PatternCount;
	}Patterns;
	struct
	{
		struct  //Input - Hidden Weights
		{
			double** Weight; //[input][hidden]
		}IH;
		struct  // Hidden - Output Weights
		{
			double** Weight; //[output][hidden]
		}HO;
	}Weights;
	struct 
	{
		struct
		{
			double** Hidden; //[pattern][hidden]
			double** Output; //[pattern][output]
			double* UHidden; //[hidden]
			double* UOutput; //[output]
		}Sum;
		struct 
		{
			double** Hidden; //[pattern][hidden]
			double** Output; //[pattern][output]
			double* UHidden; //[hidden]
		}Values;
	}Points;
	struct
	{
		double* Hidden; //[hidden]
		double* Output; //[output]
		double* SumOutputWeight; //[hidden]
		double** WeightIH; //[input][hidden]
		double** WeightHO; //[output][hidden]
	}Deltas; //
	struct 
	{
		double eta; //learning rate
		double alpha; //learning momentum
		double smallwt, bias;
		struct { uint Icount, Hcount, Ocount; }NeuronCount;
	}Settings;
}NeuralNet;
//-



//-Methods

//Setup Network & nodes. (minimum of 1)
NeuralNet* NeuralNet_create(uint _InputCount, uint _hiddenCount, uint _OutputCount, double _eta, double _alpha, double _smallwt, double _bias);
void NeuralNet_destroy(NeuralNet* _nnet);

/*Adds a new training pair to Training vector.
* input & output values copied internally
*/
void NeuralNet_addTrainingPattern(NeuralNet* _nnet, double* inputs, double* outputs);

//Visszatér az utolsó epoch error - értékével.
double NeuralNet_doEpoch(NeuralNet* _nnet, uint _times);

/*Vissza írja a jelenlegi Súlyok neurális kimenetét.
* _inputs & _outputs should be matched with NN IO count.
* _output will be updated with NN Activation values.
*/
void NeuralNet_getOutputActivation(NeuralNet* _nnet, double* _inputs, double *_outputs);


/*
	Reinitializes, all weights with random values.
*/
void NeuralNet_reRandomizeWeights(NeuralNet* _nnet);

//-

#endif
