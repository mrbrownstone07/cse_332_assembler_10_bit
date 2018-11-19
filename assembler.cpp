#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <bitset>
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
    if(arg == "nand") return "0111";

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

//fetch lines from input file
vector<string> fetchFromFile(string fileName){
    ifstream FILE(fileName+".txt");
    vector<string> fileData;
    string buffer;
    
    if(FILE.is_open()){
        while(getline(FILE, buffer)) fileData.push_back(buffer);
        FILE.close();
    } 
    else cout << fileName+".txt not found!";
    return fileData;    
}

//write into file
void writeIntoFile(vector<string>outputData, string fileName){
    ofstream FILE(fileName+".mc.txt");

    if(FILE.is_open()){
        for(unsigned i = 0; i < outputData.size(); i++){
            FILE << outputData[i] << "\n";
        }
    }
    else cout << "Failed to open!" << endl;
}

//r format instruction
string rFromat(vector<string> ins){
    int formattingArr[3] = {2,3,1};
    string result = "";
    
    for(unsigned i = 0; i<3; i++){
        string reg = ins[formattingArr[i]];
        string buff = reg.substr(1, reg.size());
        int regNum = stoi(buff);

        if(regNum <= 3){
            string binary = bitset<2>(regNum).to_string();
            result += " ";
            result += binary;
        }
        else cout << "Unallocated Register!" << endl; 
    }
    return result;
}

string iFormat(vector<string> ins){
    return "rs rt immediate";
}

//process the instructions
void generateMachineCode(vector<string> insJar){
    vector<string> machineCodeJar;

    for(unsigned insNum = 0; insNum < insJar.size(); insNum++){
        string machineCode = "";
        vector<string> explodedIns = explodeLine(insJar[insNum], " ,$");
        string oppCode = getOppCode(explodedIns[0]);

        machineCode += oppCode;
        machineCode += oppCode[0] == '0' ? rFromat(explodedIns) : iFormat(explodedIns);
        machineCodeJar.push_back(machineCode);
    }

    writeIntoFile(machineCodeJar, "ass");
}



int main(){
    string fileName;

    cin >> fileName;
    vector<string> inputData = fetchFromFile(fileName);
    generateMachineCode(inputData);
}