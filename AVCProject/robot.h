#include "geom.h"
#define DEBUG 0

struct Robot{
	Point2Di pos = {100,100};  // current position
	double azimuth = 0.0;    // robot angle
	
	double vLeft  = 0.0;
	double vRight = 0.0; //current speed of left and right motors
	
	double dv = 2.0; //inertia
	
	Rect body_current; 
	Rect body_templ;    // 
	Rect camera_current; 
	Rect camera_templ;    // 
	
	Point2Di cent = {0,0}; // center of rotation
	
	double wheelRadius = 50.0;
	double width = 50.0;   // distance between wheels
	double length = 100.0;
	double cameraForward = 100;  // camera tilt
	int fovWidth = 100;     //size of camera FieldOfView
	int fovLength = 80;
	unsigned char *image; // current camera image
	int nImage;   // size(bytes) of camera image
	
	void init(int x0,int y0,double az);
    void move();
	
	
} robot;

void Robot::init(int x0,int y0,double az){
	pos.x = x0;
	pos.y = y0;
	azimuth = az;
	vLeft = 0.0;
	vRight = 0.0;
	
	body_templ.points[0].x =  - length/2;
	body_templ.points[0].y =  - width/2;
	
	body_templ.points[1].x = length/2;
	body_templ.points[1].y = - width/2;
	
	body_templ.points[2].x = length/2;
	body_templ.points[2].y =  width/2;
	
	body_templ.points[3].x = - length/2;
	body_templ.points[3].y = width/2;
	
	Rect r0;
	r0.points[0].x = -fovLength/2;
	r0.points[0].y = -fovWidth/2;
	 
	r0.points[1].x = fovLength/2;
	r0.points[1].y = -fovWidth/2;
	
	r0.points[2].x = fovLength/2;
	r0.points[2].y = fovWidth/2;

	r0.points[3].x = -fovLength/2;
	r0.points[3].y = fovWidth/2;
	camera_templ = r0.rotate_move({0,0},{(int)cameraForward,0},0.0);
}

void Robot::move(){
	
	//if (checkCollisions()==1){
	//	return;
	//}
	
	//float l_left;   // left wheel movement
    //float l_right;  // right wheel movement
    //float d_l_r;
    //float s_l_r;
    //float r_turn;    // radius of turn
    //float df_turn;   // degree of turn
    
    if (DEBUG){
		std::cout<<" inside moveRobot()"<<std::endl;
		std::cout<<"     vLeft = "<< robot.vLeft<<" vRright = "<<robot.vRight<<std::endl;
		std::cout<<"     before: x = "<< robot.pos.x<<" y = "<<robot.pos.y;
		std::cout<<"     azimuth="<<180*robot.azimuth/3.14159<<std::endl;
       
    }
    // calculate wheel tip movements- lenght of the arc: 
    double l_left =  robot.wheelRadius * robot.vLeft * M_PI/180.0; 
    double l_right = robot.wheelRadius * robot.vRight * M_PI/180.0;
    if (DEBUG) std::cout<<" l_left = "<< l_left<<" l_right = "<<l_right<<std::endl;
    // differences in wheel movements
    double d_l_r = l_left - l_right;
   	// d_l_r = l_left - l_right;
   	// s_l_r = l_left + l_right;
     
    if ( abs(d_l_r) <= 0.001 ) {
		// move in straight line
		//r_turn = 1000.0;
		if(DEBUG) std::cout<<"  moving straight"<<std::endl;
		robot.pos.x = robot.pos.x + l_left*cos(robot.azimuth);
		robot.pos.y = robot.pos.y + l_right*sin(robot.azimuth);
    } else  {
		// robot is rotating
		if(DEBUG) {std::cout<<"   robot turning"<<std::endl;}
		double df_turn = (l_left - l_right) / robot.width;         //radians	
		double r_turn = l_right/df_turn;  //can be negative
		
		double u0x = sin(robot.azimuth);
		double u0y = -cos(robot.azimuth);
		double u1x = sin(robot.azimuth + df_turn);
		double u1y = -cos(robot.azimuth  + df_turn);

		double icc_x = robot.pos.x - (r_turn+robot.width/2)*u0x;
		double icc_y = robot.pos.y - (r_turn+robot.width/2)*u0y;   
		if(DEBUG) std::cout<<" w="<<robot.width<<"    r_turn ="<<r_turn<<" df_turn="<<180.0*df_turn/3.14159<<std::endl;   
		if(DEBUG) std::cout<<"   icc_x="<<icc_x<<"  icc_y="<<icc_y<<std::endl;
		
		robot.pos.x = icc_x + (r_turn+robot.width/2.0)*u1x;
		robot.pos.y = icc_y + (r_turn+robot.width/2.0)*u1y;	
		robot.azimuth = robot.azimuth + df_turn; 
    }
    if (DEBUG){
		std::cout<<"      after: x = "<< robot.pos.x<<" y = "<<robot.pos.y;
		std::cout<<"      azimuth="<<180.0*robot.azimuth/3.14159<<std::endl;
		int debug_stop;
		std::cin>>debug_stop;
    }
    body_current = body_templ.rotate_move(cent,pos,azimuth);
    
    //}
}
