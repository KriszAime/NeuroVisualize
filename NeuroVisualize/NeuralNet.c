#include "NeuralNet.h"
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <math.h>


NeuralNet* NeuralNet_create(uint _InputCount, uint _hiddenCount, uint _OutputCount, double _eta, double _alpha, double _smallwt, double _bias)
{
	NeuralNet* _nnet = (NeuralNet*)malloc(sizeof(NeuralNet));
	if (_nnet == NULL) return NULL;
	memset(_nnet, 0x00u, sizeof(NeuralNet)); //all 0

	//Settings
	_nnet->Settings.alpha = _alpha;
	_nnet->Settings.eta = _eta;
	_nnet->Settings.smallwt = _smallwt;
	_nnet->Settings.bias = _bias;
	//

	//IOs
	_nnet->Settings.NeuronCount.Icount = _InputCount;
	_nnet->Settings.NeuronCount.Hcount = _hiddenCount;
	_nnet->Settings.NeuronCount.Ocount = _OutputCount;
	//

	//Weights
	_nnet->Weights.IH.Weight = (double**)calloc(_InputCount, sizeof(double*)); //error can happen if null pointer
	for (uint i = 0; i < _InputCount; i++) 
	{
		*(_nnet->Weights.IH.Weight + i) = (double*)calloc(_hiddenCount, sizeof(double));
		for (uint h = 0; h < _hiddenCount; h++)
			_nnet->Weights.IH.Weight[i][h] = 2. * (double)(((double)rand() / ((double)RAND_MAX + 1)) - .5) * _smallwt;
	}


	_nnet->Weights.HO.Weight = (double**)calloc(_OutputCount, sizeof(double*)); //error can happen if null pointer
	for (uint i = 0; i < _OutputCount; i++)
	{
		*(_nnet->Weights.HO.Weight + i) = (double*)calloc(_hiddenCount, sizeof(double));
		for (uint h = 0; h < _hiddenCount; h++)
			 _nnet->Weights.HO.Weight[i][h] = 2. * (double)(((double)rand() / ((double)RAND_MAX + 1)) - .5) * _smallwt;
	}
	//

	//UPoints
	_nnet->Points.Sum.UHidden = calloc(_hiddenCount, sizeof(double));
	_nnet->Points.Sum.UOutput = calloc(_OutputCount, sizeof(double));
	_nnet->Points.Values.UHidden = calloc(_hiddenCount, sizeof(double));
	//

	//Deltas
	_nnet->Deltas.Hidden = (double*)calloc(_hiddenCount, sizeof(double));
	_nnet->Deltas.Output = (double*)calloc(_OutputCount, sizeof(double));
	_nnet->Deltas.SumOutputWeight = (double*)calloc(_hiddenCount, sizeof(double));

	_nnet->Deltas.WeightIH = (double**)calloc(_InputCount, sizeof(double*)); //error can happen if null pointer
	for (uint i = 0; i < _InputCount; i++) *(_nnet->Deltas.WeightIH + i) = (double*)calloc(_hiddenCount, sizeof(double));

	_nnet->Deltas.WeightHO = (double**)calloc(_OutputCount, sizeof(double*)); //error can happen if null pointer
	for (uint i = 0; i < _OutputCount; i++) *(_nnet->Deltas.WeightHO + i) = (double*)calloc(_hiddenCount, sizeof(double));
	//

	return _nnet;
}

void NeuralNet_destroy(NeuralNet* _nnet)
{
	if (_nnet == NULL) return;
	//-Weights
	//IH
	for (uint i = 0; i < _nnet->Settings.NeuronCount.Icount; i++)
		free((_nnet->Weights.IH.Weight[i]));
	free(_nnet->Weights.IH.Weight);
	//HO
	for (uint o = 0; o < _nnet->Settings.NeuronCount.Ocount; o++)
		free((_nnet->Weights.HO.Weight[o]));
	free(_nnet->Weights.HO.Weight);
	//-

	//-Points
	free(_nnet->Points.Sum.UHidden);
	free(_nnet->Points.Sum.UOutput);
	free(_nnet->Points.Values.UHidden);
	//Values
	for (uint p = 0; p < _nnet->Patterns.PatternCount; p++) {
		free(_nnet->Points.Values.Hidden[p]);
		free(_nnet->Points.Values.Output[p]);
	}
	if(_nnet->Points.Values.Hidden != NULL)free(_nnet->Points.Values.Hidden);
	if(_nnet->Points.Values.Output != NULL)free(_nnet->Points.Values.Output);
	//Sums
	for (uint p = 0; p < _nnet->Patterns.PatternCount; p++) {
		free(_nnet->Points.Sum.Hidden[p]);
		free(_nnet->Points.Sum.Output[p]);
	}
	if(_nnet->Points.Sum.Hidden != NULL)free(_nnet->Points.Sum.Hidden);
	if(_nnet->Points.Sum.Output != NULL)free(_nnet->Points.Sum.Output);
	//
	//-

	//-Deltas
	free(_nnet->Deltas.Hidden);
	free(_nnet->Deltas.Output);
	free(_nnet->Deltas.SumOutputWeight);
	//wIH
	for (uint i = 0; i < _nnet->Settings.NeuronCount.Icount; i++)
		free((_nnet->Deltas.WeightIH[i]));
	free(_nnet->Deltas.WeightIH);
	//wHO
	for (uint o = 0; o < _nnet->Settings.NeuronCount.Ocount; o++)
		free((_nnet->Deltas.WeightHO[o]));
	free(_nnet->Deltas.WeightHO);
	//-
	
	//-Patterns
	for (uint p = 0; p < _nnet->Patterns.PatternCount; p++) {
		free(_nnet->Patterns.Inputs[p]);
		free(_nnet->Patterns.Targets[p]);
	}
	if(_nnet->Patterns.Inputs != NULL)free(_nnet->Patterns.Inputs);
	if(_nnet->Patterns.Targets != NULL)free(_nnet->Patterns.Targets);
	if(_nnet->Patterns.IndexArray != NULL)free(_nnet->Patterns.IndexArray);
	//-
	memset(_nnet, 0x00u, sizeof(NeuralNet)); //all 0
	return;
}

void NeuralNet_addTrainingPattern(NeuralNet* _nnet, double* inputs, double* outputs)
{
	if (_nnet == NULL, inputs == NULL, outputs == NULL) return;

	//create and assign new input pattern
	_nnet->Patterns.Inputs = (double**)realloc(_nnet->Patterns.Inputs, sizeof(double*) * (_nnet->Patterns.PatternCount + 1));
	_nnet->Patterns.Inputs[_nnet->Patterns.PatternCount] = (double*)calloc(_nnet->Settings.NeuronCount.Icount, sizeof(double));
	for (uint i = 0; i < _nnet->Settings.NeuronCount.Icount; i++)
		_nnet->Patterns.Inputs[_nnet->Patterns.PatternCount][i] = inputs[i];
	//

	//create and assign new output pattern
	_nnet->Patterns.Targets = (double**)realloc(_nnet->Patterns.Targets, sizeof(double*) * (_nnet->Patterns.PatternCount + 1));
	_nnet->Patterns.Targets[_nnet->Patterns.PatternCount] = (double*)calloc(_nnet->Settings.NeuronCount.Ocount, sizeof(double));
	for (uint i = 0; i < _nnet->Settings.NeuronCount.Ocount; i++)
		_nnet->Patterns.Targets[_nnet->Patterns.PatternCount][i] = outputs[i];
	//

	//Points - add Value slots
	_nnet->Points.Values.Hidden = (double**)realloc(_nnet->Points.Values.Hidden, sizeof(double*) * (_nnet->Patterns.PatternCount + 1));
	_nnet->Points.Values.Hidden[_nnet->Patterns.PatternCount] = (double*)calloc(_nnet->Settings.NeuronCount.Hcount, sizeof(double));
	_nnet->Points.Values.Output = (double**)realloc(_nnet->Points.Values.Output, sizeof(double*) * (_nnet->Patterns.PatternCount + 1));
	_nnet->Points.Values.Output[_nnet->Patterns.PatternCount] = (double*)calloc(_nnet->Settings.NeuronCount.Ocount, sizeof(double));

	//Points - add Sum slots
	_nnet->Points.Sum.Hidden = (double**)realloc(_nnet->Points.Sum.Hidden, sizeof(double*) * (_nnet->Patterns.PatternCount + 1));
	_nnet->Points.Sum.Hidden[_nnet->Patterns.PatternCount] = (double*)calloc(_nnet->Settings.NeuronCount.Hcount, sizeof(double));
	_nnet->Points.Sum.Output = (double**)realloc(_nnet->Points.Sum.Output, sizeof(double*) * (_nnet->Patterns.PatternCount + 1));
	_nnet->Points.Sum.Output[_nnet->Patterns.PatternCount] = (double*)calloc(_nnet->Settings.NeuronCount.Ocount, sizeof(double));
	//-

	//- add slot to index array
	_nnet->Patterns.IndexArray = (uint*)realloc(_nnet->Patterns.IndexArray, sizeof(uint) * (_nnet->Patterns.PatternCount + 1));
	//-

	_nnet->Patterns.PatternCount++;
	return;
}

double NeuralNet_doEpoch(NeuralNet* _nnet, uint _times)
{
	double Error = 1.0;
	uint patternIndex;
	if (_nnet == NULL) return Error;
	for (uint epoch = 0; epoch < _times; epoch++)
	{
		//-randomize order of training patterns
		for (uint i = 0; i < _nnet->Patterns.PatternCount; i++)
		{
			_nnet->Patterns.IndexArray[i] = i;
		}
		for (uint i = 0; i < _nnet->Patterns.PatternCount - 1; i++)
		{
			uint tmp0 = i + rand() / (RAND_MAX / (_nnet->Patterns.PatternCount - i) + 1);
			uint tmp1 = _nnet->Patterns.IndexArray[tmp0];
			_nnet->Patterns.IndexArray[tmp0] = _nnet->Patterns.IndexArray[i];
			_nnet->Patterns.IndexArray[i] = tmp1;
		}
		//-

		Error = 0.0;
		//-repeat for all the training patterns
		for (uint ip = 0; ip < _nnet->Patterns.PatternCount; ip++)
		{
			patternIndex = _nnet->Patterns.IndexArray[ip];
			
			//compute hidden unit activations
			for (uint h = 0; h < _nnet->Settings.NeuronCount.Hcount; h++)
			{
				_nnet->Points.Sum.Hidden[patternIndex][h] = _nnet->Settings.bias;
				for (uint i = 0; i < _nnet->Settings.NeuronCount.Icount; i++)
					_nnet->Points.Sum.Hidden[patternIndex][h] += _nnet->Patterns.Inputs[patternIndex][i] * _nnet->Weights.IH.Weight[i][h];
				_nnet->Points.Values.Hidden[patternIndex][h] = 1. / (1. + exp(-_nnet->Points.Sum.Hidden[patternIndex][h]));
			}
			//

			//compute output unit activations and errors
			for (uint o = 0; o < _nnet->Settings.NeuronCount.Ocount; o++)
			{
				_nnet->Points.Sum.Output[patternIndex][o] = _nnet->Settings.bias;
				for (uint h = 0; h < _nnet->Settings.NeuronCount.Hcount; h++)
					_nnet->Points.Sum.Output[patternIndex][o] += _nnet->Points.Values.Hidden[patternIndex][h] * _nnet->Weights.HO.Weight[o][h];
				_nnet->Points.Values.Output[patternIndex][o] = 1. / (1. + exp(-_nnet->Points.Sum.Output[patternIndex][o])); //Sigmoidal Outputs
				
				Error += 0.5 * pow((_nnet->Patterns.Targets[patternIndex][o] - _nnet->Points.Values.Output[patternIndex][o]), 2.); //SSE
				_nnet->Deltas.Output[o] = (_nnet->Patterns.Targets[patternIndex][o] - _nnet->Points.Values.Output[patternIndex][o]) 
					* _nnet->Points.Values.Output[patternIndex][o] * (1. - _nnet->Points.Values.Output[patternIndex][o]); //Sigmoidal Outputs, SSE
			}
			//
			
			//'back-propagate' errors to hidden layer
			for (uint h = 0; h < _nnet->Settings.NeuronCount.Hcount; h++)
			{
				_nnet->Deltas.SumOutputWeight[h] = 0.0;
				for (uint o = 0; o < _nnet->Settings.NeuronCount.Ocount; o++)
					_nnet->Deltas.SumOutputWeight[h] += _nnet->Weights.HO.Weight[o][h] * _nnet->Deltas.Output[o];
				_nnet->Deltas.Hidden[h] = _nnet->Deltas.SumOutputWeight[h] * _nnet->Points.Values.Hidden[patternIndex][h] * (1. - _nnet->Points.Values.Hidden[patternIndex][h]);
			}
			//update weights WeightIH
			for (uint h = 0; h < _nnet->Settings.NeuronCount.Hcount; h++)
			{
				for (uint i = 0; i < _nnet->Settings.NeuronCount.Icount; i++)
				{
					_nnet->Deltas.WeightIH[i][h] = _nnet->Settings.eta * _nnet->Patterns.Inputs[patternIndex][i] *
						_nnet->Deltas.Hidden[h] + _nnet->Settings.alpha * _nnet->Deltas.WeightIH[i][h];
					_nnet->Weights.IH.Weight[i][h] += _nnet->Deltas.WeightIH[i][h];
				}
			}
			//update weights WeightHO
			for (uint o = 0; o < _nnet->Settings.NeuronCount.Ocount; o++)
			{
				for (uint h = 0; h < _nnet->Settings.NeuronCount.Hcount; h++)
				{
					_nnet->Deltas.WeightHO[o][h] = _nnet->Settings.eta * _nnet->Points.Values.Hidden[patternIndex][h] *
						_nnet->Deltas.Output[o] + _nnet->Settings.alpha * _nnet->Deltas.WeightHO[o][h];
					_nnet->Weights.HO.Weight[o][h] += _nnet->Deltas.WeightHO[o][h];
				}
			}
			//-
		}
		//-

	}

	return Error;
}

void NeuralNet_getOutputActivation(NeuralNet* _nnet, double* _inputs, double* _outputs)
{
	//todo null ptr?
	//compute hidden unit activations
	for (uint h = 0; h < _nnet->Settings.NeuronCount.Hcount; h++)
	{
		_nnet->Points.Sum.UHidden[h] = _nnet->Settings.bias;
		for (uint i = 0; i < _nnet->Settings.NeuronCount.Icount; i++)
			_nnet->Points.Sum.UHidden[h] += _inputs[i] * _nnet->Weights.IH.Weight[i][h];
		_nnet->Points.Values.UHidden[h] = 1.0f / (1.0f + expf(-_nnet->Points.Sum.UHidden[h]));
	}
	//
	//compute output unit activations
	for (uint o = 0; o < _nnet->Settings.NeuronCount.Ocount; o++)
	{
		_nnet->Points.Sum.UOutput[o] = _nnet->Settings.bias;
		for (uint h = 0; h < _nnet->Settings.NeuronCount.Hcount; h++)
			_nnet->Points.Sum.UOutput[o] += _nnet->Points.Values.UHidden[h] * _nnet->Weights.HO.Weight[o][h];
		_outputs[o] = 1.0f / (1.0f + expf(-_nnet->Points.Sum.UOutput[o])); //Sigmoidal Outputs
	}
	//
	return;
}

void NeuralNet_reRandomizeWeights(NeuralNet* _nnet)
{
	if ((_nnet == NULL) || (_nnet->Weights.IH.Weight == NULL) || (_nnet->Weights.HO.Weight == NULL)) return;
	
	//HI
	for (uint i = 0; i < _nnet->Settings.NeuronCount.Icount; i++)
		for (uint h = 0; h < _nnet->Settings.NeuronCount.Hcount; h++)
			_nnet->Weights.IH.Weight[i][h] = 2. * (double)(((double)rand() / ((double)RAND_MAX + 1)) - .5) * _nnet->Settings.smallwt;
	//HO
	for (uint o = 0; o < _nnet->Settings.NeuronCount.Ocount; o++)
		for (uint h = 0; h < _nnet->Settings.NeuronCount.Hcount; h++)
			_nnet->Weights.HO.Weight[o][h] = 2. * (double)(((double)rand() / ((double)RAND_MAX + 1)) - .5) * _nnet->Settings.smallwt;
	//
	return;
}
