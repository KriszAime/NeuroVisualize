#include "NeuralNet.h"
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>


void NeuralNet_Construct(NeuralNet* _nnet, uint _InputCount, uint _hiddenCount, uint _OutputCount, float _eta, float _alpha, float _smallwt, NeuralNet_WH_type _whtype)
{
	if (_nnet == NULL) return;
	memset(_nnet, 0x00u, sizeof(NeuralNet)); //all 0

	//Settings
	_nnet->Settings.alpha = _alpha;
	_nnet->Settings.eta = _eta;
	_nnet->Settings.smallwt = _smallwt;
	//

	//IOs
	_nnet->Settings.NeuronCount.Icount = _InputCount;
	_nnet->Settings.NeuronCount.Hcount = _hiddenCount;
	_nnet->Settings.NeuronCount.Ocount = _OutputCount;
	//

	//Weights
	_nnet->Weights.IH.Weight = (float**)calloc(_InputCount, sizeof(float*)); //error can happen if null pointer
	for (uint i = 0; i < _InputCount; i++) (*_nnet->Weights.IH.Weight) = (float*)calloc(_hiddenCount, sizeof(float));
	
	_nnet->Weights.HO.Weight = (float**)calloc(_OutputCount, sizeof(float*)); //error can happen if null pointer
	for (uint i = 0; i < _OutputCount; i++) (*_nnet->Weights.HO.Weight) = (float*)calloc(_hiddenCount, sizeof(float));
	//

	//Points
	//no pattern
	//
	
	//Deltas
	_nnet->Deltas.Hidden = (float*)calloc(_hiddenCount, sizeof(float));
	_nnet->Deltas.Output = (float*)calloc(_OutputCount, sizeof(float));
	_nnet->Deltas.SumOutputWeight = (float*)calloc(_hiddenCount, sizeof(float));

	_nnet->Deltas.WeightIH = (float**)calloc(_InputCount, sizeof(float*)); //error can happen if null pointer
	for (uint i = 0; i < _InputCount; i++) (*_nnet->Deltas.WeightIH) = (float*)calloc(_hiddenCount, sizeof(float));

	_nnet->Deltas.WeightHO = (float**)calloc(_OutputCount, sizeof(float*)); //error can happen if null pointer
	for (uint i = 0; i < _OutputCount; i++) (*_nnet->Deltas.WeightHO) = (float*)calloc(_hiddenCount, sizeof(float));
	//

	return;
}

void NeuralNet_addTrainingPattern(NeuralNet* _nnet, float* inputs, float* outputs)
{
	if (_nnet == NULL, inputs == NULL, outputs == NULL) return;

	//create and assign new input pattern
	_nnet->Patterns.Inputs = (float**)realloc(_nnet->Patterns.Inputs, sizeof(float*)*(_nnet->Patterns.PatternCount + 1));
	_nnet->Patterns.Inputs[_nnet->Patterns.PatternCount] = (float*)calloc(_nnet->Settings.NeuronCount.Icount, sizeof(float));
	for (uint i = 0; i < _nnet->Settings.NeuronCount.Icount; i++)
		_nnet->Patterns.Inputs[_nnet->Patterns.PatternCount][i] = inputs[i];
	//

	//create and assign new output pattern
	_nnet->Patterns.Targets = (float**)realloc(_nnet->Patterns.Targets, sizeof(float*) * (_nnet->Patterns.PatternCount + 1));
	_nnet->Patterns.Targets[_nnet->Patterns.PatternCount] = (float*)calloc(_nnet->Settings.NeuronCount.Ocount, sizeof(float));
	for (uint i = 0; i < _nnet->Settings.NeuronCount.Ocount; i++)
		_nnet->Patterns.Targets[_nnet->Patterns.PatternCount][i] = outputs[i];
	//


	//Points - add Value slots
	_nnet->Points.Values.Hidden = (float**)realloc(_nnet->Points.Values.Hidden, sizeof(float*) * (_nnet->Patterns.PatternCount + 1));
	_nnet->Points.Values.Hidden[_nnet->Patterns.PatternCount] = (float*)calloc(_nnet->Settings.NeuronCount.Hcount, sizeof(float));
	_nnet->Points.Values.Output = (float**)realloc(_nnet->Points.Values.Output, sizeof(float*) * (_nnet->Patterns.PatternCount + 1));
	_nnet->Points.Values.Output[_nnet->Patterns.PatternCount] = (float*)calloc(_nnet->Settings.NeuronCount.Ocount, sizeof(float));

	//Points - add Sum slots
	_nnet->Points.Sum.Hidden = (float**)realloc(_nnet->Points.Sum.Hidden, sizeof(float*) * (_nnet->Patterns.PatternCount + 1));
	_nnet->Points.Sum.Hidden[_nnet->Patterns.PatternCount] = (float*)calloc(_nnet->Settings.NeuronCount.Hcount, sizeof(float));
	_nnet->Points.Sum.Output = (float**)realloc(_nnet->Points.Sum.Output, sizeof(float*) * (_nnet->Patterns.PatternCount + 1));
	_nnet->Points.Sum.Output[_nnet->Patterns.PatternCount] = (float*)calloc(_nnet->Settings.NeuronCount.Ocount, sizeof(float));
	//-
	_nnet->Patterns.PatternCount++;
	return;
}
