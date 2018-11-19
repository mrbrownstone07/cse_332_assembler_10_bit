#include <iostream>
#include <string>
#include <fstream>
#include <vector>
using namespace std;

const int dictSize = 256;


//determine oppcode
string getOppCode(string arg){

    //r 
    if(arg == "add")  return "0000";
    if(arg == "sub")  return "0001";
    if(arg == "and")  return "0010";
    if(arg == "or")   return "0011";
    if(arg == "xor")  return "0100";
    if(arg == "not")  return "0101";
    if(arg == "nor")  return "0110";
    if(arg == "")    return "0111";

    //i
    if(arg == "addi") return "1000";
    if(arg == "subi") return "1001";
    if(arg == "shl")  return "1010";
    if(arg == "shr")  return "1011";
    if(arg == "lw")   return "1100";
    if(arg == "sw")   return "1101";
    if(arg == "lui")  return "1110";
    if(arg == "ori")  return "1111";

    else return "error";
}

//r format instruction
string rFromat(vector<string> ins){
    for(unsigned i = 1; 1<ins.size(); i++){
        vector<string> reg = explodeLine(ins[i], "r");
    }
}


//fetch lines from input file
vector<string> fetchFromFile(string fileName){
    ifstream FILE(fileName+".txt");
    vector<string> fileData;
    string buffer;
    
    if(FILE.is_open()) while(getline(FILE, buffer)) fileData.push_back(buffer);
    else cout << fileName+".txt not found!";
    return fileData;    
}

//split instruction line
vector<string> explodeLine(const string &s, const string &del)
{
    static bool dict[dictSize] = { false};
    vector<string> res;
    string token("");

    for (int i = 0; i < del.size(); ++i) dict[del[i]] = true;
    
    for (auto &i : s) {
        if (dict[i]) {
            if (!token.empty()) {
                res.push_back(token);
                token.clear();
            }           
        }
        else token += i;
    }
    if (!token.empty()) res.push_back(token);
    
    return res;
}

//process the instructions
void instructionProcessor(vector<string> insJar){
    for(unsigned insNum = 0; insNum < insJar.size(); insNum++){
        vector<string> assembly;
        vector<string> explodedIns = explodeLine(insJar[insNum], " ,$");
        
        string oppCode = getOppCode(explodedIns[0]);
        assembly.push_back(oppCode);
        cout << oppCode << " ";

        if(oppCode[0] == '0') {

        }
    }
}



int main(){
    string fileName;

    cin >> fileName;
    vector<string> inputData = fetchFromFile(fileName);
    instructionProcessor(inputData);
}