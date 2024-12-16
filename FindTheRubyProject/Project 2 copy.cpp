#include "video_proc.hpp"
int main(){
    // this is folder with images. Do not forget / at the end of this string
    std::string folder = "/home/arthur/ENGR101/2022/Project2/Images/Core/";
    int nFrames = 20;
    // read 20 image files from specified folder
    for ( int iFrame = 0; iFrame < nFrames ; iFrame++){
      get_image_file(folder,iFrame);
      int totred = 0;
      int totint = 0;
      double redness = 0;
      for ( int row =0 ; row < image.height ; row++)  {	
	    for ( int column = 0; column < image.width ; column++) {
		  totred = totred + (int)get_pixel(row,column,0); // get redd
		  totint = totint + (int)get_pixel(row,column,3); // get luminocity
		  redness = (float)totred/(3.0*(float)totint);
	    }	  
	  }  
       cout<<"Total red ="<<totred;
       cout<<"Total int ="<< totint;
       cout<<" Redness ="<<redness<<endl;
       // if you want image modified when displayed in browser - do it before "save_bmp"

       save_bmp_file("m1.bmp");	   // saved 
       // wait 0.5 sec
       std::this_thread::sleep_for(std::chrono::milliseconds(500));
     }
    return 0; 
}



