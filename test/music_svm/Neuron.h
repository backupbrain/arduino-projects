/**
 * Neuron.h - a class that takes music features and determines if they correspond to a beat
 * @author Tony Gaitatzis tonyg@tonygaitatzis.com
 * @date 2015-09-15
 */

#ifndef Neuron_h
#define Neuron_h

class Neuron {
	public:
		Neuron();
		~Neuron();
		bool onBeat;
		int numDataPoints;
		float* dataPoints;
	private:


};

#endif