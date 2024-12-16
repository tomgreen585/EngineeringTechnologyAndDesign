
#include "images.h" 


int main()
{        
	std::cout<<"start simulation..."<<std::endl;
	//init(1698,1018,90*3.14159/180.0); // start of completion
	//init(1880,640,90*3.14159/180.0);
	//init(110,160,15*3.14159/180.0); // start of core
	
	init(1880,680,90*3.14159/180.0); // start of completion
	
	while(true){
		set_motors(20,16);
		update_sim(200);
  	}
    return 0;
}

