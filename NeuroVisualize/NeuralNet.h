
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
	random,
	constant
}NeuralNet_WH_type;
//-

//-Structs
typedef	struct NeuralNet
{
	int tmp;
	struct Layers
	{
		struct Input
		{
			int a;
		};
		struct Hidden
		{
			int b;
		};
		struct Output
		{
			int c;
		};
	};
	
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
