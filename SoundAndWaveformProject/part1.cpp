#include <iostream>
#include <math.h>
#include "wav.hpp" // makes wave

using namespace std;

int main () {
	int sample_rate = 44100; // samples per second, (44,100 is _ music sample rate)
	double duration = 5.0; // duration of the wave (in seconds)
	int n_samples = sample_rate*duration; // total number of samples for the whole duration
	double dt = 1.0/sample_rate; // time between samples (delta t)
	int frequency = 6000; // pitch of sound
	int volume = 5000; // volume the user hears
	
	int* waveform = new int[n_samples]; // array creation
	
	for (int i_sample = 0 ; i_sample < n_samples ; i_sample++) {
		waveform[i_sample] = volume*sin(2*M_PI * frequency * i_sample *dt); // produce wave
		
	}
	MakeWavFromInt ("tone1.wav",sample_rate, waveform, n_samples);
	
	delete(waveform);
	
	return 0;
}
