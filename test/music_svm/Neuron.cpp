/**
 * Neuron.h - a class that takes music features and determines if they correspond to a beat
 * @author Tony Gaitatzis tonyg@tonygaitatzis.com
 * @date 2015-09-15
 */

#include "Neuron.h"

Neuron::Neuron(int numDataPoints) {
	this->numDataPoints = numDataPoints;
	this->dataPoints = 
	// initialize the neuron
}

Neuron::~Neuron() {
	// free variables
	free(this->dataPoints);
	this->dataPoints = null;
}