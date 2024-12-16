#include "video_proc.hpp"
int main(){
    std::string folder = "/Users/tomgreen/Desktop/greenthom/Images/Completion/Theft/";
    int nFrames = 20;
    for ( int iFrame = 0; iFrame < nFrames ; iFrame++){
      get_image_file(folder,iFrame);
      int totred = 0;
      int totgreen = 0;
      int totblue = 0;
      int totint = 0;
      double redness = 0;
      for ( int row =0 ; row < image.height ; row++)  {	
	    for ( int column = 0; column < image.width ; column++) {
		  totred = totred + (int)get_pixel(row,column,0); // get redd
		  totblue = totblue + (int)get_pixel(row,column,2); // get blue
		  totgreen = totgreen + (int)get_pixel(row,column,1); // get green
		  totint = totint + (int)get_pixel(row,column,3); // get luminocity
		  redness = (float)totred/(float)totgreen;
	    }	  
	  }
       
   if(redness>1){
	   cout<<"Ruby is present"<<endl;
	  }
	  else{
		 cout<<"Ruby is stolen"<<endl; 
	  }
		
       cout<<"Total red ="<<totred;
       cout<<"Total blue ="<<totblue;
       cout<<"Total green ="<<totgreen;
       cout<<"Total int ="<< totint;
       cout<<" Redness ="<<redness<<endl;
			
       save_bmp_file("m1.bmp");
       std::this_thread::sleep_for(std::chrono::milliseconds(500));
     }
    return 0; 
}



