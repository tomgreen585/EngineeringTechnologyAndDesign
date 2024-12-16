#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

//error statement
void error (std::string msg){
	std::cout<<msg;
	exit(0);
}

std::vector<std::string>code;
std::string b;

//IDENTIFIERS

//states
using namespace std;
vector <string>numbers;
string token;
string state;
string initialState;
//transition
struct Transition{
	std::string input_signal;
	std::string dest_state;};
//state
struct State{
	std::string name;
	std::vector<Transition> transitions;};
//output
struct FSM_out{
	std::vector<State>states;
	std::string accepting_name;
	std::string initial_name;
	State state_in_progress;
	Transition trans_in_progress;} fsm_out;

//WRITTEN STATEMENTS

//start code written
vector<string>start_code(){
	code.push_back(" #include <iostream> \n #include <string> \n using namespace std; \n string state; \n string ipts;\n\nint ProcessChar(char input){ \n int accept = 0;\n");
	return code;}
//end code written
vector<string>end_code(){
	code.push_back(" int main(){ \n cout<<\"enter input string:\"; \n getline(std::cin,ipts); \n cout<<\"Input is \" <<ipts<<\" state is\"<<state<<endl; \n bool string_accepted = 0; \n state = \"S0\"; \n for (char s:ipts){ \n string_accepted = ProcessChar(s);\n }\n cout<<\" accepted = \"<<string_accepted<<endl; \n} ");
	return code;}
//code pull main written
vector<string>write_code(){
	for(State s : fsm_out.states){
		if(s.name != "S0"){ // if state is 0
			b = b+ ("} \n");
		}
		b = b + ("if (state == \""+s.name+"\"){ \n"); //written pull for starting state
		for(Transition t : s.transitions){
			b = b +("if(input == \'"+ t.input_signal+ "\') {state = \""+ t.dest_state + "\";"); //pull wording
			if(t.dest_state == fsm_out.accepting_name){
				b = b + ("ipts = ipts + \"c\";"); //code working statement
			}
			b = b + (" } \n"); //closing bracket
		} 
		if(fsm_out.accepting_name == s.name){
			b = b +("accept = 1;"); //accept statements
		} else{b = b +("accept = 0;\n");}
			
	}
	b = b +(" \n }\n return accept; \n} \n"); //return and ending statements
	code.push_back(b);
	return code;}

//save code method
int SaveCodeFile(std::string fileName){
  std::ofstream codeOutFile(fileName);
  for(unsigned int i = 0; i < code.size(); i++){
     codeOutFile<<code.at(i)<<std::endl;
  }
  codeOutFile.close();
  cout<<"file saved at " + fileName +" selected";
  return 0;
}

void ProcessChar(char c){
	if(token.size() > 2){error("Error: no such state exists");}
	if(state == "s0"){ //S0
		if(c == 'i'){state = "s1"; return;}
		else if(c == 'a'){state = "s3"; return;}
		else if(c == 's'){state = "s5"; return;}
		else{error("Error in state s0");}
	}else if(state == "s1"){ //S1
		if(c == '{'){state = "s2"; token.clear(); return;}
		else{error("Error in state s1");}
	}else if(state == "s2"){ //S2
		if(c == '}'){state = "s0"; fsm_out.initial_name = token; return;}
		else{ token = token + c;}
	}else if(state == "s3"){ //S3
		if(c == '{'){state = "s4"; token.clear();return;}
		else{error("Error in state s3");}
	}else if(state == "s4"){ //S4
		if(c == '}'){state = "s0"; fsm_out.accepting_name = token; return;}
		else{ token = token + c; }
	}else if(state == "s5"){ //S5
		if(c == '{'){state = "s6";  token.clear(); fsm_out.state_in_progress.transitions.clear();  return;}
		else{error("Error in state s5");}
	}else if(state == "s6"){ //S6
		if(c == 'n'){state = "s7"; return;}
		else if(c == 't'){state = "s9"; fsm_out.trans_in_progress.input_signal=""; fsm_out.trans_in_progress.dest_state=""; return; }
		else if(c == '}'){ state = "s0"; fsm_out.states.push_back(fsm_out.state_in_progress); return;}
		error("Error in state s6");
	}else if(state == "s7"){ //S7
		if(c == '{'){state = "s8";token.clear(); fsm_out.state_in_progress.name.clear(); return;}
		error("Error in state s7");
	}else if(state == "s8"){ //S8
		if(c == '}'){state = "s6"; fsm_out.state_in_progress.name = token; return;}
		else{ token = token + c; }
	}else if(state == "s9"){ //S9
		if(c == '{'){state = "s10";token.clear(); return;}
		else{error("Error in state s9");}
	}else if(state == "s10"){ //S10
		if(c == '}'){state = "s6"; fsm_out.state_in_progress.transitions.push_back(fsm_out.trans_in_progress);return;}
		else if(c == 'i'){state = "s11"; return;}
		else if(c == 'd'){state = "s13"; return;}
		error("Error in state s10");
	}else if(state == "s11"){ //S11
		if(c == '{'){state = "s12";token.clear();fsm_out.trans_in_progress.input_signal.clear(); return;}
		else{error("Error in state s11");}
	}else if(state == "s12"){ //S12
		if(c == '}'){state = "s10"; fsm_out.trans_in_progress.input_signal = token;}
		else{token = token + c;}
	}else if(state == "s13"){ //S13
		if(c == '{'){state = "s14";token.clear();fsm_out.trans_in_progress.dest_state.clear();return;}
		else {error("Error in state s13");}
	}else if(state == "s14"){ //S14
		if(c == '}'){state = "s10"; fsm_out.trans_in_progress.dest_state = token;}
		else{ token = token + c;}
	}
	return;

}
	
std::string ReadSpecFile(std::string fileName){
	std::cout<<"Reading specifications file"<<fileName<<std::endl;
	std::string out;
	std::ifstream inputFile(fileName);
	std::string line;
	out = "";
	std::string n1;
	while(getline(inputFile,line)){
	line.erase(std::remove(line.begin(), line.end(),'\r'), line.end());
	line.erase(std::remove(line.begin(), line.end(),'\n'), line.end());
	line.erase(std::remove(line.begin(), line.end(),' '), line.end());
	out.append(line);
	}
	return out;}

int main(){
  
  string inputFile;
  cout<<"Enter input string:";
  getline(std::cin,inputFile);
  state = "s0";
  
  //read file
  for(char s : ReadSpecFile(inputFile)){
    ProcessChar(s);}
  
  //accepting statement
  cout<<"Accepting State: " + fsm_out.accepting_name+"\n";
  //write code
  start_code();
  write_code();
  end_code();
  //save code
  string codeName;
  cout<<"Give file a name: ";
  getline(std::cin, codeName);
  SaveCodeFile(codeName);

}

	
	

		
	































		
		
		
		
		
		
		
		
		
		
		
		
		
		
