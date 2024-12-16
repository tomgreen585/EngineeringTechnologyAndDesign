#include <iostream> // input-output library 
#include <math.h>  // library for sin function 
#include "wav.hpp" // make sure to include this helper library 
// " " instead of <> means that library is in the same folder as your program 
 
using namespace std; 
 
int main(){ 
   int sample_rate = 44100; // samples per second, select value which provides good quality sound  
   double duration= 5.0; // how long [seconds] it will sound 
   int n_samples = sample_rate*duration; // if sound is "duration" seconds long and there are "sample_rate" samples per second 
                        // - how many samples are there altogether? What type should this variable be? 
   double dt = 1.0/(sample_rate); // time between samples 
   int* waveform = new int[n_samples]; // creates the array 
  
   double frequency= 1000// pitch of the sound 
   int volume= 6000;// 6000 is loud enough
   //int pi = 3.14159265359
   //pi = pi*2 
   
   for ( int i_sample = 0; i_sample < n_samples ; i_sample++){  
       waveform[i_sample] = volume*sin(2*3.1415)*frequency*(i_sample*dt);// 
       //6.28318530718*frequency*dt
       
    //count an be used here to check values of "waveform"   
   } 
   // generates sound file from waveform array, writes n_samples numbers  
   // into sound wav file 
   // should know sample_rate for sound timing 
   MakeWavFromInt("tone1.wav",sample_rate, waveform, n_samples); //file name can be changed but keep extension .wav 
}
   return 0; 
} 
