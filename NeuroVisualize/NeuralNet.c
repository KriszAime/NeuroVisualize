#include "NeuralNet.h"
#include <stdio.h>
#include <stdlib.h>


void NeuralNet_Construct(NeuralNet* _nnet, uint _hiddenCount, float _eta, float _alpha, float _smallwt, NeuralNet_WH_type _whtype)
{
	uint _InputCount = 1, _OutputCount = 1; //const Mivel ebben a tesztben egyelõre csak 1 be és kimenetes háló fog csak futni.
	_nnet->Settings.alpha = _alpha;
	_nnet->Settings.eta = _eta;
	_nnet->Settings.smallwt = _smallwt;

	_nnet->Weights.IH.Count.Icount = _InputCount;
	_nnet->Weights.IH.Count.Hcount = _hiddenCount;
	_nnet->Weights.IH.Weight = (float**)calloc(_InputCount, sizeof(float*)); //error can happen if null pointer
	for (uint i = 0; i < _InputCount; i++) (*_nnet->Weights.IH.Weight) = (float*)calloc(_hiddenCount, sizeof(float));
	
	_nnet->Weights.HO.Count.Hcount = _hiddenCount;
	_nnet->Weights.HO.Count.Ocount = _OutputCount;
	_nnet->Weights.HO.Weight = (float**)calloc(_OutputCount, sizeof(float*)); //error can happen if null pointer
	for (uint i = 0; i < _OutputCount; i++) (*_nnet->Weights.HO.Weight) = (float*)calloc(_hiddenCount, sizeof(float));

	return;
}
