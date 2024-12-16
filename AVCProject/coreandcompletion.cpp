#include <stdio.h> 
#include <algorithm>
#include "images.h" 
#include <unistd.h>
#include <iostream>
#include <queue>

//==================================================================================================================================//
//====================================================== FIELDS FOR THE ROBOT ======================================================//
//==================================================================================================================================//

bool isCentred();
std::queue<char> dirs;  // sequence of turns for challenge maze
int logic;              // current logic pattern used for the robot
int padding = 25;       // the padding amount for the robot sensors
bool ahead;     // if there is a path ahead or not
bool left;      // if there is a path to the left or not
bool right;     // if there is a path to the right or not
int numOfExits; // current number of exits

/** @brief Update sim and set sleep timer here  */
void updateSim(){
    update_sim(200); // (sleep time ms)
}

//==================================================================================================================================//
//===================================================== SIMPLE ROBOT MOVEMENTS =====================================================//
//==================================================================================================================================//

/** 
 * @brief Sets the robot motors and updates the sim 
 * 
 * @param left the speed of the left motor
 * @param right the speed of the right motor
 */
void setAndUpdateRobot(int left, int right){
    set_motors(left, right);
    updateSim();   
}

/** 
 * @brief Move the robot straight by the specified speed and updates the sim 
 * 
 * @param speed the speed of the robot
 */
void goStraight(int speed){
    setAndUpdateRobot(speed, speed);
}

/** @brief turn the robot left and updates the sim */
void turnLeft(){
     goStraight(40);
    goStraight(40);
    goStraight(40);
    set_motors(-20,20);
    updateSim();
    set_motors(-20,20);
    updateSim();
   }

    


/** @brief turn the robot right and updates the sim */
void turnRight(){
    goStraight(40);
    goStraight(40);
    goStraight(40);
    set_motors(20,-20);
    updateSim();
    set_motors(20,-20);
    updateSim();
   }


/** @brief turn the robot around and updates the sim */
void turnAround(){
    for (int i = 0; i < 15; i++) 
        goStraight(10);
    while (! isCentred())    
        setAndUpdateRobot(-5, 5);
}

//==================================================================================================================================//
//========================================================= SENSOR METHODS =========================================================//
//==================================================================================================================================//

/**
 * @brief looks for marker in the current frame
 * 
 * @param color 1 for red, 2 for blue, 3 for green
 * @return 1 if marker is present in the frame, else 0
 */
int findMarker(int color){
    float target;
    int row = camera_image.height/2;
    for (int col = 0; col < camera_image.width; col++) {
        Pixel pix = get_camera_pixel(row,col);
        switch (color){
            case 1: target = pix.r; break;
            case 2: target = pix.b; break;
            case 3: target = pix.g; break;
            default: return 0;
        }
        if (target == 255) {
            goStraight(40);
            return 1;
        }
    }
    return 0;
}

/**
 * @brief Detects if any path is present on a vertical line
 * 
 * @param startRow starting row of the scan line (top)
 * @param col the col to be scanned
 * @param endRow ending row of the scan line (bottom)
 * @return true if there is a path, false otherwise
 */
bool detectRangeVer(int startRow, int col, int endRow){
    for (int row = startRow; row < endRow; row++) {
        Pixel p = get_camera_pixel(row,col);
        if (p.luma < 30 || p.r == 255 || p.g == 255 || p.b == 255) 
            return true;
    }
    return false;
}

/**
 * @brief Detects if any path is present on a horizontal line
 * 
 * @param row the row to be scanned
 * @param startCol starting position of the scan line (left)
 * @param endCol  ending position of the scan line (right)
 * @return true if a path is detected, false otherwise
 */
bool detectRangeHor(int row, int startCol, int endCol){
    for (int col = startCol; col < endCol; col++) {
        Pixel p = get_camera_pixel(row,col);
        if (p.luma < 30 || p.r == 255 || p.g == 255 || p.b == 255) 
            return true;
    }
    return false;
}

/**
 * @brief Looks for if there is a path in the direction given
 * 
 * @param option 1: straight ahead, 2: left side, 3: right side
 * @return true if there is a path, false otherwise
 */
bool pathSensors (int option){
    switch (option) {
        case 1:  return detectRangeHor(0, padding, camera_image.width - padding);
        case 2:  return detectRangeVer(padding, 0, camera_image.height - padding);
        case 3:  return detectRangeVer(padding, camera_image.width-1, camera_image.height - padding);
        default: throw "Invalid option";
    }
}

/**
 * @brief Detects if there is a path in the two front corners, covering the blind spots of the main sensor
 * 
 * @return true if there is a path, false otherwise
 */
bool cornerSensor(){
    if (detectRangeVer(0, 0, padding))  return true;
    if (detectRangeHor(0, 0, padding))  return true;
    if (detectRangeVer(0, camera_image.width-1, padding)) return true;
    if (detectRangeHor(0, camera_image.width-padding, camera_image.width)) return true;
    return false;
}

/**
 * @brief See if the robot as aligned to the line or not
 * 
 * @return true if the middle (horizontally) detected the line, false otherwise
 */
bool isCentred(){
    return detectRangeHor(0, camera_image.width/2 - 5, camera_image.width/2 + 5);
}

/** @brief updates the number of exits and the boolean values for each of the exits */
void updatePaths(){
    numOfExits = 0;
    numOfExits = 0;
    if ((ahead = pathSensors(1)) == true) numOfExits++; 
    if ((left  = pathSensors(2)) == true) numOfExits++; 
    if ((right = pathSensors(3)) == true) numOfExits++;
}

/**
 * @brief Find the error value to determine how much to stare to keep on the route
 * 
 * @return double - Error value
 */
int getErrorValue(){
    int midline[camera_image.width];   // array of 0 and 1, 1 if black(path), else 0 
    int row = camera_image.height-5;  
    std::fill(midline, midline + camera_image.width, 0);
    for (int col = 0; col < camera_image.width; col++) {
        if (get_camera_pixel(row,col,3) < 30) 
            midline[col] = 1;
    }

    // find and return the error value based on the midline
    int error = 0;
    int midValue = camera_image.width / 2;
    for (int i = 0; i <camera_image.width; i++)
        error += midline[i] * (i - midValue);
    return error;
}

//==================================================================================================================================//
//======================================================== DECISION METHODS ========================================================//
//==================================================================================================================================//

/**
 * @brief Used to adjust the steering input of the robot following a line
 * 
 * @param baseSpeed - base speed of the robot
 * @param kp - limit of the steering input
 */
void adjustMovement(int baseSpeed, double kp){
    int error = getErrorValue();
    double deviation =  (error * kp);
    double left = baseSpeed + deviation;
    double right = baseSpeed - deviation;

    std::cout<<" error="<<error<<"  left="<<left<<"  right="<<right<<std::endl;

    set_motors(left, right);
}

/**
 * @brief Main sensors detected dead end, use corner sensors to see if there is a path, 
 * if not, turn around, otherwise adjust the steering input
 */
void deadEnd(){
    if (cornerSensor()){
        std::cout<<"Path: Corner detected"<<std::endl;
        adjustMovement(10, 0.03);
    }else{
        std::cout<<"Path: Dead End"<<std::endl;
        turnAround();
    }
}

/**
 * @brief used to turn into intersections
 * 
 * @param ahead true if we have path straight ahead, false otherwise
 * @param left true if we have path to the left, false otherwise
 * @param right true if we have path to the right, false otherwise
 */
void decideIntersection(bool ahead, bool left, bool right){
    // current solution: hug the left wall
    // input robot movement accordingly
    if (left){
        std::cout<<"Turn left"<<std::endl;
        turnLeft();
    }else if (ahead){
        std::cout<<"Go straight"<<std::endl;
        adjustMovement(10, 0.03);
    }else{
        std::cout<<"Turn right"<<std::endl;
        turnRight();
    }
}

//==================================================================================================================================//
//========================================================= LOGIC BODY =========================================================//
//==================================================================================================================================//

/** @brief just handling single path with movements */
void runCore(){
    adjustMovement(15, 0.01);
}

/** @brief After red marker, handle straight path with intersection */
void runCompletion(){
    updatePaths();

    switch (numOfExits) {
        case 0:  // no path ahead, turn around
            deadEnd();
            return;
        case 1:  // only one path ahead, turn into it
            std::cout<<"Path: 1 path only"<<std::endl;
            decideIntersection(ahead, left, right);
            return;
        default:  // 2 or more path ahead, determine which direction to go
            std::cout<<"Path: 2+ paths, intersection"<<std::endl;
            decideIntersection(ahead, left, right);
            return;
    }
}

/** @brief After blue marker, find green marker hidden in maze */
void runChallenge(){
    updatePaths();

    switch (numOfExits) {
        case 0:  // no path ahead, turn around
            deadEnd();
            return;
        case 1:  // only one path ahead, turn into it
            std::cout<<"Path: 1 path only"<<std::endl;
            decideIntersection(ahead, left, right);
            return;
        default:  // 2 or more path ahead, determine which direction to go
            std::cout<<"Path: 2+ paths, intersection"<<std::endl;
            char c = dirs.front();
            dirs.pop();
            printf("Challenge turn: %c\n Next turn: %c\n", c, dirs.front());
            switch (c){
                case 'R':
                    turnRight();
                    return;
                case 'L':
                    turnLeft();
                    return;
                default:
                    //decideIntersection(ahead, left, right);
                    return;
            }
    }
}

/** @brief Fireworks! */
void runCelebration(){
    set_motors(0,0);
    printf("WE GOT TO THE END!!!\n WOHOO~~! WOHOO~~! WOHOO~~!\n");
}

//==================================================================================================================================//
//=========================================================== MAIN LOGIC ===========================================================//
//==================================================================================================================================//

/** @brief Initialises the starting position and variables for the robot */
void initialise(){
    std::cout<<"start simulation..."<<std::endl;
    init(2,100,0); // arguments - initial robot position, x,y,angle 
	//init(110,160,15*3.14159/180.0); // start of core
	//init(1880,600,90*3.14159/180.0); // start of completion
	//init(800,680,90*3.14159/180.0); // start of challenge
	//init(1400,690,180*3.14159/180.0); // start of midway through completion
    logic = 1;
    dirs.push('R'); dirs.push('R'); dirs.push('L'); dirs.push('L'); dirs.push('L');
    goStraight(10);
}

/**
 * @brief Main logic framwork
 * @return current logic state, 1 for core, 2 for completion, 3 for challenge
 */
/**
 * @brief Main logic framwork
 * @return current logic state, 1 for core, 2 for completion, 3 for challenge
 */
void runSim(){
    logic += findMarker(logic);  // if marker is found, update the logic state and enter next stage
    printf("logic: %d\n", logic);
    switch (logic){
        case 1: runCore(); break;
        case 2: runCompletion(); break;
        case 3: runChallenge(); break;
        case 4: runCelebration(); break;
        default: throw new std::exception();
    }
    updateSim();   
}

/** @brief Loops the main logic forever */
int main(){        
    initialise();
    for ( int time = 0 ; time < 1000; time++){ // change upper limit or replace with while(1)to run forever 
        printf("\ntime: %d  ", time);
        runSim();
    }
    return 0;
}
