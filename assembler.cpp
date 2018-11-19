#include <bits/stdc++.h>
using namespace std;

const int dictSize = 256;


map<string, string> fetchMap(string fileName){
    string key, value;
    map<string,string> m;
    ifstream FILE("resources/"+fileName+".txt");
   
    while(FILE>>key>>value) m[key] = value;
    FILE.close();
    
    return m;
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
            cout << outputData[i] << "\n";
            FILE << outputData[i] << "\n";
        }
        FILE.close();
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
        cout << regNum << endl;
        if(regNum <= 3){
            string binary = bitset<2>(regNum).to_string();
            result += " ";
            result += binary;
            cout << binary << endl;
        }
        else cout << "Unallocated Register!" << endl; 
    }
    return result;
}

string iFormat(vector<string> ins){
    string result = "";

    if()

    return result;
}

//process the instructions
vector<string> generateMachineCode(vector<string> insJar){
    vector<string> machineCodeJar;
    map<string, string> OPPCODE = fetchMap("oppcode");

    for(unsigned insNum = 0; insNum < insJar.size(); insNum++){
        string machineCode = "";
        vector<string> explodedIns = explodeLine(insJar[insNum], " ,$");
        string oppCode = OPPCODE[explodedIns[0]];
        
        cout << oppCode << endl;
        
        machineCode += oppCode;
        machineCode += oppCode[0] == '0' ? rFromat(explodedIns) : iFormat(explodedIns);
        machineCodeJar.push_back(machineCode);
    }

    return machineCodeJar;
}

int main(){
    string fileName;

    cin >> fileName;
    vector<string> inputData = fetchFromFile(fileName);
    vector<string> outputData = generateMachineCode(inputData);
    writeIntoFile(outputData, fileName);
}