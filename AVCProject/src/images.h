// following functions are needed for ppm generation:
// - draw circle(x,y,R,r,g,b) (sun)
// - draw background
// - draw object (duck). Object ia another ppm image
#include <cmath>
#include <iostream>
#include <sstream> 
#include <fstream>
#include <string>
#include <vector>
#include <optional>
#include <algorithm>
#include <thread>
#include <chrono>
#include "robot.h"

#define sign(x) ((x > 0) ? 1 : ((x < 0) ? -1 : 0))

struct RGB{
	unsigned char r;
	unsigned char g;
	unsigned char b;
};



struct Pixel{
	int x,y;
	// red, green, blue, luminosity
	unsigned char r,g,b,luma;
};



struct Segment{
	Point2Di p1,p2;
	RGB rgb;
	int width;
};



struct ImagePPM{
	// pixels
	int width, height;
	int n_bytes;
	char* data;
	std::vector<Pixel> pixs; 
    int init_image(int w,int h, ImagePPM& image);
	
} table_orig , table_image, camera_image;



// create new ppm image
int init_images(int w_table,int h_table,Robot r){
	table_orig.width = w_table;
	table_orig.height = h_table;
    table_orig.n_bytes =  table_orig.width*table_orig.height*3;
    table_orig.data = new char[table_orig.n_bytes];
    table_orig.pixs.reserve(table_orig.width*table_orig.height);

	table_image.width = w_table;
	table_image.height = h_table;
    table_image.n_bytes =  table_image.width*table_image.height*3;
    table_image.data = new char[table_image.n_bytes];
    table_image.pixs.reserve(table_image.width*table_image.height);
    
    camera_image.width = r.fovWidth;
	camera_image.height = r.fovLength;
    camera_image.n_bytes =  camera_image.width*camera_image.height*3;
    camera_image.data = new char[camera_image.n_bytes];
    camera_image.pixs.reserve(camera_image.width*camera_image.height);
    
    return 0;
}

Pixel get_pixel( ImagePPM image,int row,int col){
	if ((row < 0 ) || (row > image.height) )  {
        std::cout<<"row is "<<row<<" and out of range\n";
        throw "Out_of_range";
    }
    if ( (col< 0) || (col > image.width))    {
        std::cout<<"column is "<<col<<" and out of range\n";
    }
	return image.pixs[image.width*row + col];
}

Pixel get_camera_pixel(int row,int col){
	int address =camera_image.width*row*3 + col*3;  // standard ppm
    //int address = image. image.width*row*3 + col*3;  // standard ppm
	if ((row < 0 ) || (row >camera_image.height) )  {
        std::cout<<"row is "<<row<<" and out of range\n";
        throw "Out_of_range";
    }
    if ( (col< 0) || (col > camera_image.width))    {
        std::cout<<"column is "<<col<<" and out of range\n";
    }
    unsigned char r = camera_image.data[address];
    unsigned char g = camera_image.data[address+1];
    unsigned char b = camera_image.data[address+2];
    Pixel p;
    p.r = r; p.g = g; p.b = b;
	int totalLum = p.r+p.g+p.b;
	p.luma = (unsigned char)(totalLum/3);
    return p;
	//return camera_image.pixs[camera_image.width*row + col];
}

// returns color component (color==0 -red,color==1-green,color==2-blue
// color == 3 - luminosity
// for pixel located at (row,column)
unsigned char get_pixel( ImagePPM image,int row,int col, int color){
    // calculate address in 1D array of pixels
    int address = image.width*row*3 + col*3;  // standard ppm
    
    //int address = image.width*row*4 + col*4;
    if ((row < 0 ) || (row > image.height) )  {
        std::cout<<"row is out of range\n";
        return -1;
    }
    if ( (col< 0) || (col > image.width))    {
        std::cout<<"column is out of range\n";
        return -1;
    }
    unsigned char r = image.data[address];
    unsigned char g = image.data[address+1];
    unsigned char b = image.data[address+2];
	//std::cout<<"GP "<<(int)r<<" "<<(int)g<<" "<<(int)b<<std::endl;		          
    if (color==0) { return r;}
    if (color==1) { return g;}
    if (color==2) { return b;}
    if (color==3)    {
		int yi = r+g+b;
        unsigned char y = yi/3;
		//std::cout<<"GP y="<<(int)y<<std::endl;		          
        return y;
    }
    printf("Color encoding wrong: 0-red, 1-green,2-blue,3 - luminosity\n");
    return -2; //error
}

// returns color component (color==0 -red,color==1-green,color==2-blue
// color == 3 - luminocity
// for pixel located at (row,column)
unsigned char get_camera_pixel( int row,int col, int color){
    // calculate address in 1D array of pixels
    int address = camera_image.width*row*3 + col*3;  // standard ppm
    //int address = image.width*row*4 + col*4;
    if ((row < 0 ) || (row > camera_image.height) )  {
        std::cout<<"row is out of range\n";
        return -1;
    }
    if ( (col< 0) || (col > camera_image.width))    {
        std::cout<<"column is out of range\n";
        return -1;
    }
    unsigned char r = camera_image.data[address];
    unsigned char g = camera_image.data[address+1];
    unsigned char b = camera_image.data[address+2];
	//std::cout<<"GP "<<(int)r<<" "<<(int)g<<" "<<(int)b<<std::endl;		          
    if (color==0) { return r;}
    if (color==1) { return g;}
    if (color==2) { return b;}
    if (color==3)    {
		int yi = r+g+b;
        unsigned char y = yi/3;
		//std::cout<<"GP y="<<(int)y<<std::endl;		          
        return y;
    }
    printf("Color encoding wrong: 0-red, 1-green,2-blue,3 - luminosity\n");
    return -2; //error
}


int set_pixel(ImagePPM& image, int row, int col, unsigned char red,unsigned char green,unsigned char blue){
    //int address = image.width*row*4 + col*4; // RPI
    int address = image.width*row*3 + col*3;  // standard ppm
    if ((address < 0) || (address>image.width*image.height*3)){
        printf("SetPixel(): wrong x,y coordinates\n");
        printf(" row= %d col=%d address=%d\n",row,col,address);
        printf(" width= %d height=%d\n",image.width,image.height);
        return -1;
    }
    image.data[address] = red;
    image.data[address+1] = green;
    image.data[address+2] = blue;
    return 0;
}


int SavePPMFile(std::string filename, ImagePPM& image){
	std::ofstream fOut;
	fOut.open(filename,std::ofstream::binary);
	if (!fOut.is_open()){ std::cout<<"no file"; return -1;}
	fOut<<"P6"<<std::endl;
	fOut<<(int)image.width<<" "<<(int)image.height<<" 255"<<std::endl;
	
	fOut.write(image.data,image.n_bytes);
	flush(fOut);
	fOut.close();
	
	return 0;
}

int save_bmp_file(std::string filename, ImagePPM& image){
	//Stream & out;
	std::ofstream fOut;
	fOut.open(filename,std::ofstream::binary);
	if (!fOut.is_open()){ std::cout<<"no file"; return -1;}
	
	uint32_t w = image.width, h = image.height;
    uint32_t pad = w * -3 & 3;
    uint32_t total = 54 + 3*w*h + pad*h;
    uint32_t head[13] = {total, 0, 54, 40, w, h, (24<<16)|1};
    //char const* rgb = (char const*)img.rgb.data();

    fOut.write("BM",2);
    fOut.write((char*)head, 52);
    char* rgb = new char[3];
    for (int row = image.height-1 ; row >= 0; row--)
    for (int col = 0 ; col < image.width ; col++){
		int address = image.width*row*3 + col*3;
		rgb[0] = image.data[address+2];;
		rgb[1] = image.data[address+1];;
		rgb[2] = image.data[address];;
		fOut.write(rgb,3); 
	}
    fOut.write(image.data,image.n_bytes);
    fOut.flush();	
    return 1;
}

int OpenPPMFile(std::string filename,ImagePPM& image){
	//std::cout<<"Opening single ppm file"<<std::endl;
	
	std::ifstream fin;
	fin.open(filename);
	if (!fin.is_open()){ std::cout<<"no file"; return -1;}
    // get first line - contains P6
    std::string fd1;
    getline(fin,fd1);
    //std::cout<<fd1<<std::endl; 
    // get secondline
    getline(fin,fd1);
    //std::cout<<fd1<<std::endl; 
    int depth;
    sscanf( fd1.c_str(),"%d %d %d\n",&image.width ,&image.height,&depth );
        
    image.n_bytes =  image.width*image.height*3;
    image.data = new char[image.n_bytes];
    fin.read(image.data, image.n_bytes);
    fin.close();
    return 0;
}



// fill whole image
int draw_background(ImagePPM& image){
	int rr = 70;
	for (int col = 0 ; col < image.width ; col++)
	for (int row = 0 ; row < image.height; row++){
		set_pixel(image, row , col, 180 + rand() % rr, 180 +rand() % rr, 180 +rand() % rr);
	} 
	return 0;
}

// draw circle
int draw_circle(ImagePPM& image, Point2Di c, int R, char r,char g, char b){
	for (int col = c.x-R ; col<c.x+R ; col++)
	for (int row = c.y-R; row < c.y+R ; row++){
		int dx = col-c.x;
		int dy = row -c.y;
		if ((row>0) && (row < image.height ) &&
		    (col>0) && (col <image.width ) &&
		    ( dx*dx + dy*dy<R*R)) set_pixel(image,row,col,r,g,b);
	}
	return 0;
}


int draw_square(ImagePPM& image, int xc,int yc, int W, char r, char g, char b){
	int xTop = xc - W/2;
	int yTop = yc - W/2;
	int xBot = xc + W/2;
	int yBot = yc + W/2;
	for (int x = xTop ; x < xBot; x++) {
	  	if ((x>=0)&&(x<image.width)&&(yTop>=0)&&(yTop<image.height))
	     	set_pixel(image,yTop,x,r,g,b);
    }
	for (int x = xTop ; x < xBot; x++) {
		if ((x>=0)&&(x<image.width)&&(yBot>=0)&&(yBot<image.height))
        	set_pixel(image,yBot,x,r,g,b);
	}  
	for (int y = yBot ; y > yTop; y--){
	  	if ((xTop>=0)&&(xTop<image.width)&&(y>=0)&&(y<image.height))
		 	set_pixel(image,y,xTop,r,g,b);
	}
	for (int y = yBot ; y > yTop; y--) {
	  	if ((xBot>=0)&&(xBot<image.width)&&(y>=0)&&(y<image.height))
			set_pixel(image,y,xBot,r,g,b);
	}
	return 0;
}

/**
 * @brief check the position of point C relative to the midline of point A>B, 
 * 			<code>true</code> if C is on the right of AB, <code>false</code> otherwise
 * 
 * @param pa point A
 * @param pb point B
 * @param pc point C, point to be evaluated
 * @return <code>true</code> if C is on the right of AB, <code>false</code> otherwise
 */
bool which_side(Point2Di pa, Point2Di pb, Point2Di pc){
	return ((pb.x-pa.x)*(pc.y-pa.y)-(pb.y-pa.y)*(pc.x-pa.x)>0);
}

int draw_segment(ImagePPM& image,Segment s){
	// bounding box
	//int x_right,y_left,y_right;
	double angle = atan2(s.p2.y-s.p1.y,s.p2.x-s.p1.x);
	//double w2 = s.width*sqrt(2.0)/2.0;
	// corner points
	Point2Di p1 = { (int)( s.p1.x + s.width*(-cos(angle) + sin(angle))),
	                (int)( s.p1.y + s.width*(-sin(angle) - cos(angle) ))};
	Point2Di p2 = { (int)( s.p1.x + s.width*(-cos(angle) - sin(angle))),
	                (int)( s.p1.y + s.width*(-sin(angle) + cos(angle) )) };
	                
	Point2Di p3 = { (int)(s.p2.x + s.width*(cos(angle) + sin(angle))),
	                (int)(s.p2.y + s.width*(sin(angle) - cos(angle) ) )};
	Point2Di p4 = { (int)(s.p2.x + s.width*(cos(angle) - sin(angle)) ),
	                (int)(s.p2.y + s.width*(sin(angle) + cos(angle) ))};
	/*
	std::cout<<"p1.x="<<s.p1.x<<" p1.y="<<s.p1.y<<std::endl;
	std::cout<<"p2.x="<<s.p2.x<<" p2.y="<<s.p2.y<<" w="<<s.width<<std::endl;
	std::cout<<" angle="<<angle<<std::endl;
	std::cout<<" x1="<<p1.x<<" y1="<<p1.y<<std::endl;
	std::cout<<" x2="<<p2.x<<" y2="<<p2.y<<std::endl;
	std::cout<<" x3="<<p3.x<<" y3="<<p3.y<<std::endl;
	std::cout<<" x4="<<p4.x<<" y4="<<p4.y<<std::endl;
	*/
	int x_left = std::min({p1.x,p2.x,p3.x,p4.x});
	int x_right = std::max({p1.x,p2.x,p3.x,p4.x});
	int y_top = std::min({p1.y,p2.y,p3.y,p4.y});
	int y_bottom = std::max({p1.y,p2.y,p3.y,p4.y});
	for (int x = x_left;x<x_right;x++)
	for (int y = y_top; y<y_bottom ; y++){
		Point2Di p={x,y};
		if (which_side(p1,p2,p)==which_side(p1,p2,p3))
		if (which_side(p2,p4,p)==which_side(p2,p4,p1))
		if (which_side(p4,p3,p)==which_side(p4,p3,p1))
		if (which_side(p3,p1,p)==which_side(p3,p1,p2)) {
			set_pixel(image,y,x,s.rgb.r,s.rgb.g,s.rgb.b);
	   	}
	}
	
	return 0;
}


int draw_line(ImagePPM& image,Point2Di p1,Point2Di p2){
	//	std::cout<<"draw line:"<<p1.x<<" "<<p1.y<<" "<<p2.x<<" "<<p2.y<<std::endl;
	float x1 = p1.x; float y1 = p1.y;
	float x2 = p2.x; float y2 = p2.y;
	      // Bresenham's line algorithm
	const bool steep = (fabs(y2 - y1) > fabs(x2 - x1));
	if(steep){
		std::swap(x1, y1);
		std::swap(x2, y2);
	}
	
	if(x1 > x2) {
		std::swap(x1, x2);
		std::swap(y1, y2);
	}
	
	const float dx = x2 - x1;
	const float dy = fabs(y2 - y1);
	
	float error = dx / 2.0f;
	const int yStep = (y1 < y2) ? 1 : -1;
	int y = (int)y1;
	
	const int maxX = (int)x2;
	
	for(int x=(int)x1; x<=maxX; x++){
		if(steep) {
			set_pixel(image,x,y,10,10,10);//SetPixel(y,x, color);
		} else{
			set_pixel(image,y,x,10,10,10);//SetPixel(x,y, color);
		}
	
		error -= dy;
		if(error < 0){
			y += yStep;
			error += dx;
		}
	}
	
    return 0;
}

int draw_rectangle(ImagePPM& image, Rect r){
	draw_line(image,r.points[0],r.points[1]);
	draw_line(image,r.points[1],r.points[2]);
	draw_line(image,r.points[2],r.points[3]);
	draw_line(image,r.points[3],r.points[0]);
	return 0;
}

int draw_rectangle(ImagePPM& image, Rect r,int red, int gr, int bl){
	double dxu = r.points[1].x - r.points[0].x;
	double dxv = r.points[3].x - r.points[0].x;
	double dyu = r.points[1].y - r.points[0].y;
	double dyv = r.points[3].y - r.points[0].y;
	
	for (double u = 0 ; u<1.0;  u = u+0.01){
	    for (double v = 0 ; v<1.0; v = v+0.01){
			int x = r.points[0].x + u*dxu + v*dxv;
			int y = r.points[0].y + u*dyu + v*dyv;
			set_pixel(image,y,x,red,gr,bl);
		}
	}
	//draw_line(image,r.points[0],r.points[1]);
	//draw_line(image,r.points[1],r.points[2]);
	//draw_line(image,r.points[2],r.points[3]);
	//draw_line(image,r.points[3],r.points[0]);
	return 0;
}

// pictures in the scene
// white (rgb>250) is left  with background color
struct Object {
	ImagePPM object_image;
	int load_from_file(std::string fn);
	double vx,vy;
	int x0,y0;
};

int Object::load_from_file(std::string fn){
	OpenPPMFile(fn,object_image);
	
	return 0;
}

void insert_object(ImagePPM& im, Object ob, int x, int y){
	for (int objrow = 0 ; objrow < ob.object_image.height; objrow++){
		for (int objcol = 0 ; objcol < ob.object_image.width; objcol++){
			unsigned char r = get_pixel(ob.object_image,objrow,objcol, 0);
			unsigned char g = get_pixel(ob.object_image,objrow,objcol, 1);
			unsigned char b = get_pixel(ob.object_image,objrow,objcol, 2);
			if (!((r>250)&&(g>250)&&(b>250))) // not perfect white
			set_pixel(im, y+objrow, x+objcol, r,g,b);
		}
	}
}



struct Scene{
	RGB background;
	std::vector<Object> things;
	std::vector<Segment> segments; // tape line
	int x_sun,y_sun,r_sun;
	int time;
	int x_aim,y_aim;
	double omega;
	std::ofstream log_file;
} scene;

//produces original image
void draw_table_orig(){
	draw_background(table_orig);
    for (Object thing:scene.things){
	   if (thing.object_image.n_bytes>0)
	       insert_object(table_orig,thing ,thing.x0, thing.y0);
	}	
	for (Segment seg:scene.segments){
		draw_segment(table_orig,seg); 
	}
	save_bmp_file("table.bmp", table_orig);
 
}

int set_scene(){

    //scene.log_file.open ("log.txt");
    //if(level>0){
		// add clouds
	  Object thing;
	  //scene.things.push_back(thing);
	  /*
      thing.load_from_file("cloud1.ppm"); // 
	  thing.x0 = 100; thing.y0 = 570; 
	  thing.vx = 10; thing.vy = 0; 
	  scene.things.push_back(thing);
	 // if (level == 2){ // add ducks
	        thing.load_from_file("duck3.ppm");
	        thing.x0 = 100; thing.y0 = 800; 
	        thing.vx = 40; thing.vy = 0; 
	        scene.things.push_back(thing);
	        thing.load_from_file("duck3.ppm");
	        thing.x0 = 200; thing.y0 = 700; 
	        thing.vx = -10; thing.vy = 0; 
	        scene.things.push_back(thing);
	        thing.load_from_file("duck3.ppm");
	        thing.x0 = 200; thing.y0 = 400; 
	        thing.vx = 50; thing.vy = 0; 
	        scene.things.push_back(thing);
	    
      //}
    //}
 */
    Segment seg;
    
    std::ifstream fin;
	fin.open("maze.txt");
	//fin.open("maze_pid_tune.txt");
	if (!fin.is_open()){
		std::cout<<"Can not open maze file"<<std::endl;
		exit(1); 
	}
	std::string in_line;
	while (!fin.eof()) {
		std::getline(fin,in_line);
		std::cout<<in_line<<std::endl;
		if (in_line.length()>1){
			std::stringstream str_strm(in_line);
			std::string token;
			std::vector<std::string> tokenized;
			tokenized.clear();
			while(std::getline(str_strm, token, ',')) {
				tokenized.push_back(token);
			}
			if (tokenized.at(0)!="#"){ // not comment
				if ((tokenized.at(0)=="s")&&(tokenized.size()==9)){ //segment
					seg.p1.x = stoi(tokenized.at(1));
					seg.p1.y = stoi(tokenized.at(2));
					seg.p2.x = stoi(tokenized.at(3)); 
					seg.p2.y = stoi(tokenized.at(4));
					seg.width = stoi(tokenized.at(5));
					seg.rgb.r = stoi(tokenized.at(6));
					seg.rgb.g = stoi(tokenized.at(7));
					seg.rgb.b = stoi(tokenized.at(8));
					scene.segments.push_back(seg);
					std::cout<<"   segment added"<<std::endl;
				}
			}      
		}
    }
   	//int stop;
   	// std::cin>>stop;
    std::cout<<"Scene initialized.."<<std::endl;
    draw_table_orig();
    return 0;
}


// pick part of the image which is inside camera POV
// (rectangle robot.camera_current) 
void pick_camera(Robot robot){
	// position of top-left corner of FOV is robot.camera_current.points[1]
	robot.camera_current = robot.camera_templ.rotate_move(robot.cent,
                                       robot.pos,-robot.azimuth);
                                       
	for (int cam_row = 0; cam_row < robot.fovLength; cam_row++){
	    double u = (double)cam_row/(double)robot.fovLength;
	    double xa = robot.camera_current.points[1].x + u *
		      (robot.camera_current.points[0].x- robot.camera_current.points[1].x);
	    double ya = robot.camera_current.points[1].y + u *
		      (robot.camera_current.points[0].y- robot.camera_current.points[1].y);
		      
	    double xb = robot.camera_current.points[2].x + u *
		      (robot.camera_current.points[3].x- robot.camera_current.points[2].x);
	    double yb = robot.camera_current.points[2].y + u *
		      (robot.camera_current.points[3].y- robot.camera_current.points[2].y);
		      
	    //draw_circle(table_image,{xa,ya},2,10,10,10);	
	    //draw_circle(table_image,{xb,yb},2,10,10,10);	
	    for (int cam_col = 0 ; cam_col < robot.fovWidth ; cam_col++){
			set_pixel(camera_image,cam_row,cam_col,255,255,255);
			double v = (double)cam_col/(double)robot.fovWidth;
		    double x = xa + v*(xb-xa); 
		    double y = ya + v*(yb-ya); 
		    // unsigned char get_pixel( ImagePPM image,int row,int col, int color)
		    unsigned char r = get_pixel(table_image,(int)round(y),(int)round(x),0);
		    unsigned char g = get_pixel(table_image,(int)round(y),(int)round(x),1);
		    unsigned char b = get_pixel(table_image,(int)round(y),(int)round(x),2);
		    set_pixel(camera_image,cam_row,cam_col,r,g,b);
		}      
	}
}


// copy original image 
void copy_orig_to_current(){
	for (int i = 0; i<table_orig.n_bytes;i++)
	  table_image.data[i]=table_orig.data[i];
}	

// draw rectangles - robot body and camera Field_Of_View
void draw_robot(Robot& robot){
	robot.body_current = robot.body_templ.rotate_move(robot.cent,
										robot.pos,-robot.azimuth);
	robot.camera_current = robot.camera_templ.rotate_move(robot.cent,
										robot.pos,-robot.azimuth);
		
	draw_rectangle(table_image,robot.body_current,255,0,0); 
	draw_rectangle(table_image,robot.camera_current); 
}

int update_sim(int delay){
	robot.move();
	copy_orig_to_current();
	// grab robot camera view
	pick_camera(robot);
	draw_robot(robot); 	  
	save_bmp_file("m1.bmp", table_image);
	save_bmp_file("m2.bmp", camera_image);
	std::this_thread::sleep_for(std::chrono::milliseconds(delay));
	return 0;
}



// level not really needed , initial robot position
// is good for debugging
void init(int rx0,int ry0,double az){
	robot.init(rx0,ry0,az);
	if (init_images(2200 , 1650,robot) == 0){
        set_scene();
    } else {std::cout<<" Fail to reserve image memory"<<std::endl;}
   
}

/*INTERFACE FUNCTIONS, SAME AS RPI*/

int set_motors(int vl,int vr){
	double inertia = 0.6;
	robot.vLeft = robot.vLeft + (vl -robot.vLeft)*inertia;
	robot.vRight = robot.vRight + (vr -robot.vRight)*inertia;

	
	if (robot.vLeft > 40.0) robot.vLeft = 40.0;
	if (robot.vLeft < -40.0) robot.vLeft = -40.0;
	if (robot.vRight > 40.0) robot.vRight = 40.0;
	if (robot.vRight<-40.0) robot.vRight = -40.0;
	
	//robot.vLeft = vl;
	//robot.vRight = vr;
	std::cout<<" set_motors :"<<robot.vRight<<" "<<robot.vLeft<<std::endl;
	//int deb;
	//std::cin>>deb;
	return 0;
}
