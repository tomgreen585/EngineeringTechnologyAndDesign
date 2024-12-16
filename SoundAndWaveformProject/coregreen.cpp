#include <iostream> // input-output library 
#include <math.h>  // library for sin function 
#include "wav.hpp" // make sure to include this helper library 
// " " instead of <> means that library is in the same folder as your program 
 
using namespace std; 
 
int main() { 
   int sample_rate = 44100; // samples per second, select value which provides good quality sound
   double duration = 10.0; // how long [seconds] it will sound 
   int n_samples = sample_rate*duration; // if sound is "duration" seconds long and there are "sample_rate" samples per second 
   double dt = 1.0/(sample_rate); // time between samples 
   int frequency= 6000;// pitch of the sound 
   int volume= 5000;// 5000 is enough volume
   
   int* waveform = new int[n_samples]; // creates the array 
 

   for ( int i_sample = 0; i_sample < n_samples ; i_sample++){
       waveform[i_sample] = volume*sin(2*M_PI * frequency * i_sample *dt);// 
 }      
   
   MakeWavFromInt("tone1.wav",sample_rate, waveform, n_samples); //file name can be changed but keep extension .wav 
   
   delete(waveform); 

   return 0; 
}
