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
     if((i_sample>44100)&&(i_sample<88200)) {frequency = 9000;}
     if((i_sample>88200)&&(i_sample<132300)) {frequency = 6000;}
     if((i_sample>132300)&&(i_sample<176400)) {frequency = 9000;}
     if((i_sample>176400)&&(i_sample<220500)) {frequency = 6000;}
     if((i_sample>220500)&&(i_sample<264600)) {frequency = 9000;}
     if((i_sample>264600)&&(i_sample<308700)) {frequency = 6000;}
     if((i_sample>308700)&&(i_sample<352800)) {frequency = 9000;}
     if((i_sample>352800)&&(i_sample<396900)) {frequency = 6000;}
     if((i_sample>396100)&&(i_sample<441000)) {frequency = 9000;}
 }      
   
   MakeWavFromInt("siren.wav",sample_rate, waveform, n_samples); //file name can be changed but keep extension .wav 
   
   delete(waveform); 

   return 0; 
}

