/*
 * 
 */ 

#include "image_pr4.h" 
#include <thread>
#include <chrono>
#include <cmath>
//#include "display_scene.html"


int count = -1;
int prevTime = 0;
int sunCenter = 0;
int x;
int y;
int red = 0;

int xc;
int yc;
int r;
int e;

int crossx;
int cossy;
double angle;

double a11,a12,a13,a21,a22,a23,a31,a32,a33,b1,b2,b3;










// you can use that
struct Orbit {
	// logged position and time
	std::vector<int> x;
	std::vector<int> y;
	std::vector<int> t;
	int xc,yc,r;  // center and radius
	int x_sunrise,y_sunrise;
	double omega = 0.1;
} orbit;










void locateSun(int time){
//void locateSun(){
	
	int count = -1;
	
	for(int row = 0; row<image.height; ++row){
		for(int col = 0; col<image.width; ++col){
			
			int r = (int)get_pixel(image, row, col, 0);
			int g = get_pixel(image, row, col, 1);
			int b = get_pixel(image, row, col, 2);
			
			if(r / ((b + g /2) + 1) > 2 ) {    
				
				
				y = row;
				count++;
				
				orbit.x.push_back(x);
				orbit.y.push_back(y);
				orbit.t.push_back(count);
				x = col;
				
				if(count == sunCenter){
					std::cout<<"X: "<<x<<" Y: "<<y<<std::endl;
					}
				}
			}
		}
		//int current = rPixelsNum/(time+1);
		//std::cout<<"Pixel number: "<<rPixelsNum<<std::endl;
		sunCenter = count/2;
		std::cout<<sunCenter<<std::endl;
		
		
	}
	
double D[3][3] = {
    {a11,a12,a13},
    {a21,a22,a23},
    {a31,a32,a33}
};
double Dx[3][3] = {
    {b1,a12,a13},
    {b2,a22,a23},
    {b3,a32,a33}
};
double Dy[3][3] = {
    {a11,b1,a13},
    {a21,b2,a23},
    {a31,b3,a33}
};
double Dr[3][3] = {
    {a11,a12,b1},
    {a21,a22,b2},
    {a31,a33,b3}
};

//Orbit caluclator
void calulateOrbit(int time){
//void calulateOrbit(){
	a11 = orbit.x.size();
    a12 =0;
    for(unsigned int i =0; i < orbit.t.size(); i++){
        a13 +=cos(orbit.omega*orbit.t.at(i));
    }
    a21 = 0;
    a22 = orbit.x.size();
    for(unsigned int i =0; i<orbit.t.size(); i++){
        a23 += sin(orbit.omega*orbit.t.at(i));

    }
    for(unsigned int i =0; i<orbit.t.size();i++){
        a31+= cos(orbit.omega*orbit.t.at(i));
    }
    for(unsigned int i =0; i<orbit.t.size();i++){
        a32 += sin(orbit.omega*orbit.t.at(i));
    }
    double a33 = orbit.t.size();
    for(unsigned int i =0; i<orbit.x.size(); i++){
        b1 += orbit.x.at(i);
    }
    for(unsigned int i =0; i < orbit.t.size(); i++){
        b2 += orbit.y.at(i);
    }
    for (unsigned int i =0; i<orbit.x.size(); i++){
        b3 += orbit.x.at(i) * cos(orbit.omega*orbit.t.at(i) + (orbit.y.at(i) * sin(orbit.omega*orbit.t.at(i))));
    }

    double det = a11 *(a22 * a33 - a23 * a32) - a12* (a21 * a33 - a23 * a31) - a13 *(a21 * a32 - a22 * a31);

    double detx = b1 *(a22 * a33 - a23 * a32) - a12* (b2 * a33 - a23 * b3) - a13 *(b2 * a32 - a22 * b3);

    double dety = a11 *(b2 * a33 - a23 * b3) - b1* (a21 * a33 - a23 * a31) - a13 *(a21 * b3 - b2 * a31);

    double detr = a11 *(a22 * b3 - b2 * a32) - a12* (a21 * b3 - b2 * a31) - b1 *(a21 * a32 - a22 * a31);



    orbit.xc = detx/det;
    orbit.yc = dety/det;
    orbit.r = detr/det;

	
	//double xSun = orbit.xc + orbit.r * cos(orbit.omega*time);
	//double ySun = orbit.yc + orbit.r * sin(orbit.omega*time);
	}


int main()
{        
	std::cout<<"start..."<<std::endl;
	
	init(0);//0 - core, 1- completion, 2-challenge
	
	//int x_sun, y_sun; // current position of the sun
	
	//double orbit;
    
    for ( int time = 0 ; time < 950; time++){
       draw_all(time);  // image is ready,
       
       locateSun(time);
       calulateOrbit(time);
       //locateSun();
       //calulateOrbit();
       
       //get_aim(orbit.xc, orbit.yc);
       //double angle = atan2(y - orbit.yc, x - orbit.xc);
       get_aim(xc, yc);
       double angle = atan2(y - yc, x - xc);
       
       move2_aim(angle);
       
       std::cout<<" time="<<time<<std::endl;
       std::cout<<"________________________________________"<<std::endl;
       
       //std::cout<<"xc: "<<orbit.xc<<" || yc: "<<orbit.yc<<std::endl;
       std::cout<<"xc: "<<xc<<" || yc: "<<yc<<std::endl;
       
       std::cout<<"________________________________________"<<std::endl;
       
       
       
       std::this_thread::sleep_for(std::chrono::milliseconds(500));
       
   }
    return 0;
}

























/*#include "image_pr4.h" 
#include <thread>
#include <chrono>
#include <math.h>


using namespace std;
int red = 0;
int x;
int y;

int xc;
int yc;
int r;
int e;

double a11,a12,a13,a21,a22,a23,a31,a32,a33,b1,b2,b3;

int count = -1;
int crossx;
int crossy;
double angle;

struct Orbit {
	// logged position and time
	std::vector<int> x;
	std::vector<int> y;
	std::vector<int> t;
	int xc,yc,r;  // center and radius
	int x_sunrise,y_sunrise;
	double omega = 0.1;
} orbit;


void locateSun() {
	int red;
	int green;
	int blue;
    for (int row = 0; row < image.height; ++row) {
        for (int col = 0; col < image.width; ++col) {
        	red= get_pixel(image, row, col, 0);
			green = get_pixel(image,row,col,1);
			blue = get_pixel(image,row,col,2);
            if(red / ((blue+green /2) + 1) > 2 ) {     
				y = row;
                count++;
                orbit.y.push_back(y);
                orbit.x.push_back(x);
                orbit.t.push_back(count);
				x = col;				
            }
        }
    }
}


double D[3][3] = {
    {a11,a12,a13},
    {a21,a22,a23},
    {a31,a32,a33}
};
double Dx[3][3] = {
    {b1,a12,a13},
    {b2,a22,a23},
    {b3,a32,a33}
};
double Dy[3][3] = {
    {a11,b1,a13},
    {a21,b2,a23},
    {a31,b3,a33}
};
double Dr[3][3] = {
    {a11,a12,b1},
    {a21,a22,b2},
    {a31,a33,b3}
};



void calculateOrbit(){
    a11 = orbit.x.size();
    a12 =0;
    for(int i =0; i < orbit.t.size(); i++){
        a13 +=cos(orbit.omega*orbit.t.at(i));
    }
    a21 = 0;
    a22 = orbit.x.size();
    for(int i =0; i<orbit.t.size(); i++){
        a23 += sin(orbit.omega*orbit.t.at(i));

    }
    for(int i =0; i<orbit.t.size();i++){
        a31+= cos(orbit.omega*orbit.t.at(i));
    }
    for(int i =0; i<orbit.t.size();i++){
        a32 += sin(orbit.omega*orbit.t.at(i));
    }
    double a33 = orbit.t.size();
    for(int i =0; i<orbit.x.size(); i++){
        b1 += orbit.x.at(i);
    }
    for(int i =0; i < orbit.t.size(); i++){
        b2 += orbit.y.at(i);
    }
    for (int i =0; i<orbit.x.size(); i++){
        b3 += orbit.x.at(i) * cos(orbit.omega*orbit.t.at(i) + (orbit.y.at(i) * sin(orbit.omega*orbit.t.at(i))));
    }

    double det = a11 *(a22 * a33 - a23 * a32) - a12* (a21 * a33 - a23 * a31) - a13 *(a21 * a32 - a22 * a31);

    double detx = b1 *(a22 * a33 - a23 * a32) - a12* (b2 * a33 - a23 * b3) - a13 *(b2 * a32 - a22 * b3);

    double dety = a11 *(b2 * a33 - a23 * b3) - b1* (a21 * a33 - a23 * a31) - a13 *(a21 * b3 - b2 * a31);

    double detr = a11 *(a22 * b3 - b2 * a32) - a12* (a21 * b3 - b2 * a31) - b1 *(a21 * a32 - a22 * a31);



    xc = detx/det;
    yc = dety/det;
    r = detr/det;





}

int main()
{        
	std::cout<<"start..."<<std::endl;
	init(1);
	double orbit;
	
    for ( int time = 0 ; time < 950; time++){
       draw_all(time); 
      std::cout<<"Time: " << time << endl;
      
      // your code here
            locateSun();
            calculateOrbit();
            get_aim(xc,yc);
            double angle = atan2(y - yc, x - xc);
            move_aim(angle);

      std::this_thread::sleep_for(std::chrono::milliseconds(500));
   }

    return 0;
}














#include "image_pr4.h"
#include <thread>
#include <chrono>

using namespace std;



struct Orbit {
	std::vector<int> x;
	std::vector<int> y;
	std::vector<int> t;
	
	int xc, yc, r;
	
	int x_sunrise, y_sunrise;
	double omega = 0.1;
}

orbit;

std::vector<std::string> lines;

void SaveCodeFile(std::string fileName){
	std::ofstream codeOutFile(fileName);
	
	for(unsigned int i = 0; i < lines.size(); i++){
		codeOutFile<<lines.at(i)<<std::endl;
	}
	codeOutFile.close();
}

double det2(double one, double two, double three, double four){
	return (one*four) - (two*three);
}

double det3(double one, double two, double three, double four, double five, double six, double seven, double eight, double nine){
	
	double detA = one * det2(five, six, eight, nine);
	double detB = two * det2(four, six, seven, nine);
	double detC = three * det2(four, five, seven, eight);
	
	return detA + (-detB) + detC;
}

int degrees(double n){
	return(3.14159265359 / 180.0) * n;
}

int radians(double n){
	return(n * 3.14159265359) / 180.0;
}

vector<vector<int>> rows;
vector<int> cols;
int count = 0;

void mapper(){
	init(0);
	for(int time = 0; time < 1; time++){
	draw_all(time);
	
	int red = 0;
	int green = 0;
	int blue = 0;
	
	vector<int> mapX;
	vector<int> mapY;
	
	for(int row = 0; row < image.height; row++){
		for(int col = 0; col < image.width; col++){
			
			red = (int)get_pixel(image, row, col, 0);
			
			green = (int)get_pixel(image, row, col, 1);
			
			blue = (int)get_pixel(image, row, col, 2);
			
			if(red>(green*2) && red > (blue*2)){
				mapX.push_back(col);
				mapY.push_back(row);
			}
		}
	}
	
	int initialX = mapX.at(0);
	int initialY = mapY.at(0);
	
	int workingY = initialY;
	
	for(unsigned int i = 0; i < mapX.size(); i++){
		
		if(mapY.at(i) == workingY){
			cols.push_back(mapX.at(i) - initialX);
		} else{
			cols.push_back(mapX.at(i) - initialX);
			rows.push_back(cols);
			cols.clear();
			workingY++;
		}
	}
	
	rows.push_back(cols);
	cols.clear();
	
	for(unsigned int i = 0; i < rows.size(); i++){
		for(unsigned int j = 0; j < rows.at(i).size(); j++){
		}
	}
	
	}
}

void move_aim(double angle){
	std::cout<<" Prev pos x="<<scene.x_aim<<" y="<<scene.y_aim<<std::endl;
	std::cout<<" Angle = "<<angle*180/3.14159<<std::endl;
	scene.x_aim = scene.x_aim + 20.5*cos(angle);
	scene.y_aim = scene.y_aim + 20.5*sin(angle);
	
	std::cout<<" Next pos x="<<scene.x_aim<<" y="<<scene.y_aim<<std::endl;
}

int main(){
	std::cout<<"start..."<<std::endl;
	mapper();
	init(2);
	for(int time = 0; time<200; time++){
		cout<<endl;
		std::cout<<"Time="<<time<<std::endl;
		draw_all(time);
		
		int red = 0;
		int green = 0;
		int blue = 0;
		
		vector<int> mapX;
		vector<int> mapY;
		
		bool there = false;
		int countRed = 0;
		
		for(int row = 0; row<image.height; row++){
			for(int col = 0; col<image.width; col++){
				
			red = (int)get_pixel(image, row, col, 0);
			green = (int)get_pixel(image, row, col, 1);
			blue = (int)get_pixel(image, row, col, 2);
			
			if(red>(green*2) && red > (blue*2)){
				
				int testRed = 0;
				int testGreen = 0;
				int testBlue = 0;
				
				countRed = 0;
				
				for(unsigned int i=0; i< rows.size(); i++){
					for(unsigned int j=0; j < rows.at(i).size(); j++){
						
						
					if((int)(row+i) < image.height && (int)col + rows.at(i).at(j) < image.width && (int)(row + i) > 0 && (int)col + rows.at(i).at(j)>0){
						
						int testX = col + rows.at(i).at(j);
						int testY = row + i;
						
						testRed = (int)get_pixel(image, testY, testX, 0);
						testGreen = (int)get_pixel(image, testY, testX, 1);
						testBlue = (int)get_pixel(image, testY, testX, 2);
						
						if(testRed > (testGreen *2) && testRed > (testBlue *2)){
							countRed += 1;
						}
					}
				}
			}
			
			if(countRed >= (mapX.size()) * 0.90){
				for(unsigned int i = 0; i< rows.size(); i++){
					for(unsigned int j = 0; j < rows.at(i).size(); j++){
						
						mapX.push_back(col+rows.at(i).at(j));
						mapY.push_back(row + i);
					}
				}
				row += 1000;
				col += 1000;
				there = true;
			}	
				
			}
		}
	}
	cout<<"Red Count: "<<countRed<<endl;
	
	if(there){
		cout<<"SUN IS SEEN"<<endl;
		
		double xAvg = 0;
		double yAvg = 0;
		
		for(unsigned int i = 0; i < mapX.size(); i++) xAvg += mapX.at(i);
		for(unsigned int i = 0; i < mapX.size(); i++) yAvg += mapY.at(i);
		
		xAvg = xAvg / mapX.size();
		yAvg = yAvg / mapY.size();
		
		orbit.x.push_back(xAvg);
		orbit.y.push_back(yAvg);
		orbit.t.push_back(time);
		if(orbit.x.size() >= 7){
			
			double a11 = orbit.x.size();
			double a12 = 0;
			double a13 = 0;
			
			for(unsigned int i = 0; i < orbit.t.size(); i++) a13 += cos(orbit.omega * orbit.t.at(i));
			
			double a21 = 0;
			double a22 = orbit.x.size();
			double a23 = 0;
			
			for(unsigned int i = 0; i < orbit.t.size(); i++) a23 += sin(orbit.omega * orbit.t.at(i));
			
			double a31 = 0;
			double a32 = orbit.x.size();
			double a33 = 0;
			
			for(unsigned int i = 0; i < orbit.t.size(); i++) a31 += cos(orbit.omega * orbit.t.at(i));
			for(unsigned int i = 0; i < orbit.t.size(); i++) a32 += sin(orbit.omega * orbit.t.at(i));
			
			double b1 = 0;
			double b2 = 0;
			double b3 = 0;
			
			for(unsigned int i = 0; i < orbit.x.size(); i++) b1 += orbit.x.at(i);
			for(unsigned int i = 0; i < orbit.y.size(); i++) b2 += orbit.y.at(i);
			
			for(unsigned int i = 0; i < orbit.t.size(); i++){
				b3 += orbit.x.at(i) * cos(orbit.omega * orbit.t.at(i));
				b3 += orbit.y.at(i) * sin(orbit.omega * orbit.t.at(i));
			}
			
			double detA = det3(a11, a12, a13, a21, a22, a23, a31, a32, a33);
			double detA1 = det3(b1, a12, a13, b2, a22, a23, b3, a32, a33);
			double detA2 = det3(a11, b1, a13, a21, b2, a23, a31, b3, a33);
			double detA3 = det3(a11, a12, b1, a21, a22, b2, a31, a32, b3);
			
			orbit.xc = detA1 / detA;
			orbit.yc = detA2 / detA;
			orbit.r = detA3 / detA;
			
			//double x0 = orbit.xc + (orbit.r * cos(orbit.omega * (time - 1)));
			//double x1 = orbit.xc + (orbit.r * cos(orbit.omega * (time + 1)));
			//double y0 = orbit.yc + (orbit.r * sin(orbit.omega * (time - 1)));
			//double y1 = orbit.yc + (orbit.r * sin(orbit.omega * (time + 1)));
			
			int x_sun = orbit.xc + (orbit.r * cos(orbit.omega * (time)));
			int y_sun = orbit.yc + (orbit.r * sin(orbit.omega * (time)));
			
			int x_panel = 0;
			int y_panel = 0;
			
			get_aim(x_panel, y_panel);
			
			
			double angle = 1.0 * atan2((y_sun - y_panel), (x_sun - x_panel));
			
			//double test = (int)degrees(angle);
			
			move_aim(angle);
			
		}
		else{
			scene.x_aim = xAvg;
			scene.y_aim = yAvg;
		}
	}
	else{ 
		if(orbit.x.size() >= 7){
			if(scene.y_aim >= image.height){
				
				
				scene.x_aim = orbit.xc - sqrt(pow(orbit.r,2) - pow((image.height - orbit.yc), 2));
				scene.y_aim = image.height;
			}
			else{
				scene.x_aim = orbit.xc + (orbit.r * cos(orbit.omega * time));
				scene.y_aim = orbit.yc + (orbit.r * sin(orbit.omega * time));
		}
}
}
}
}*/

