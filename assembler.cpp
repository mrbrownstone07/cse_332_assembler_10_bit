#include <bits/stdc++.h>
using namespace std;

const int dictSize = 256;
int errCount = 0;

//load map from file
map<string, string> fetchMap(string fileName){
    string key, value;
    map<string,string> m;
    ifstream FILE("resources/"+fileName+".txt");
   
    while(FILE>>key>>value) m[key] = value;
    FILE.close();
    
    return m;
}

//string compare ignore case
bool scmp_ig_case(const string& a, const string& b)
{
    return std::equal(a.begin(), a.end(),
                      b.begin(), b.end(),
                      [](char a, char b) {
                          return tolower(a) == tolower(b);
                      });
}

//global maps
map <string, string> OPPCODE = fetchMap("oppcode");
map <string, string> REG = fetchMap("regMap");
map <string, string> HEXMAP = fetchMap("hexHelper");
map <string, string> BIN_TO_HEX_MAP = fetchMap("BinToHex");
vector <string> binForHex;

//normal error message printer
void printError(int line, string err){
    if(line == 0) printf("Error: %s \n", err.c_str());
    else printf("Error at line %d: %s \n", line, err.c_str());
}

//fatal error message printer
void fatalError(int line, string err){
    if(line == 0) printf("Fatal error: %s \n", err.c_str());
    else printf("Fatal error at line %d: %s \n", line, err.c_str());
}

//hex to bin converter
string hex2bin(string hex){
    string hexcode = hex.substr(2, hex.size());
    string bin = "";
    
    for(int i = 0; i<hexcode.size(); i++){
        string key = "";
        key += hexcode[i];
        bin += HEXMAP[key];
        //std::cout << HEXMAP[key] << "\n";
        //std::cout << key << "\n";
    }
    
    return bin;
}

//split instruction line
vector<string> explodeLine(const string &s, const string &del){
    static bool dict[dictSize] = {false};
    vector<string> res;
    string token("");

    for (int i = 0; i < del.size(); ++i) dict[del[i]] = true;
    
    for (auto &i : s){
        if (dict[i]){
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

//R format validity checker
bool checkValRformat(stringstream& ss, int l){
    string rs, rt, rd;
    bool flag = true, s = 0, t = 0, d = 0, sc = 0, dc = 0;
    
    ss >> rd;
    ss >> rs;
    ss >> rt;

    if(rs[0] != '$') {
        printError(l, "'$' missing in REG#2");
        flag = false;
        s = 1;
        errCount++;
    }
    if(rs[rs.size() -1] != ','){
        printError(l, "',' missing after REG#2");
        flag = false;
        sc = 1;
        errCount++;
    }   
    if(rt[0] != '$'){
        printError(l, "'$' missing in REG#3");
        flag = false;
        t = 1;
        errCount++;
    }
    if(rd[0] != '$'){
        printError(l, "'$' missing in REG#1");
        flag = false;
        d = 1;
        errCount++;
    }
    if(rd[rd.size() -1] != ','){
        printError(l, "',' missing after REG#1");
        flag = false;
        dc = 1;
        errCount++;
    }

    if(s == 0 && sc == 0) 
        if(REG.find(rs.substr(0, rs.size()-1)) == REG.end()) 
            {printError(l, "undefined value for REG#2"); flag = false; errCount++;}
    if(s == 0 && sc == 1) 
        if(REG.find(rs.substr(0, rs.size())) == REG.end()) 
            {printError(l, "undefined value for REG#2"); flag = false; errCount++;}

    if(t == 0) 
        if(REG.find(rt) == REG.end()) 
            {printError(l, "undefined value for REG#3"); flag = false; errCount++;} 
    
    if(d == 0 && dc == 0) 
        if(REG.find(rd.substr(0, rd.size()-1)) == REG.end()) 
            {printError(l, "undefined value for REG#1"); flag = false; errCount++;}

    if(d == 0 && dc == 1) 
        if(REG.find(rd.substr(0, rd.size())) == REG.end()) 
            {printError(l, "undefined value for REG#1"); flag = false; errCount++;}

    return flag;
}

//I format validity checker
bool checkValIformat(string opp, stringstream& ss, int l){
    string rd, rs, rt, offset, rsc1, rsc2, buff;
    stringstream check, check1;
    int value = -1;
    bool t = 0, s = 0, d = 0, o = 0, c = 0, v = 0, nooff = 0, flag = true;

    if(opp == "lw" || opp == "sw"){
        ss >> rt;
        check << ss.rdbuf();
        check >> value;
        check1 << check.rdbuf();
        check1 >> rs;

        if(rt[0] != '$'){
            printError(l, "'$' missing in REG#1");
            flag = false;
            t = 1;
            errCount++;
        }
        if(rt[rt.size() -1] != ','){
            printError(l, "',' missing after REG#1");
            flag = false;
            t = 1;
            errCount++;
        }
        if(check.str().substr(1, check.str().size()) == check1.str()){
            printError(l, "offset value not given");
            flag = false;
            nooff = 1;
            errCount++;
        }
        if(value > 3 && nooff == 0){
            printError(l, "Invalid offset value");
            flag = false;
            errCount++;
        }
        if(rs[0] != '('){
            printError(l, "'(' missing after offset");
            flag = false;
            o = 1;
            errCount++;
        }
        if(o == 1){
            if(rs[0] != '$'){
                printError(l, "'$' missing in REG#2");
                flag = false;
                s = 1;
                errCount++;
            }
        }else{
            if(rs[1] != '$'){
                printError(l, "'$' missing in REG#2");
                flag = false;
                s = 1;
                errCount++;
            }
        }
        if(rs[rs.size() -1] != ')'){
            printError(l, "')' missing after REG#2");
            flag = false;
            c = 1;
            errCount++;
        }

        if(t == 0) 
            if(REG.find(rt.substr(0, rt.size() - 1)) == REG.end()) 
                {printError(l, "undefined value for REG#1"); flag = false; errCount++;}
    
        if(s == 0 && o == 0 && c == 0) 
            if(REG.find(rs.substr(1, rs.size() - 2)) == REG.end()) 
                {printError(l, "undefined value for REG#2"); flag = false; errCount++;}

        if(s == 0 && o == 1 && c == 0)
            if(REG.find(rs.substr(0, rs.size() - 1)) == REG.end()) 
                {printError(l, "undefined value for REG#2"); flag = false; errCount++;}

        if(s == 0 && o == 0 && c == 1)
            if(REG.find(rs.substr(1, rs.size())) == REG.end()) 
                {printError(l, "undefined value for REG#2"); flag = false; errCount++;}
            
        if(s == 0 && o == 1 && c == 1)
            if(REG.find(rs) == REG.end()) 
                {printError(l, "undefined value for REG#2"); flag = false; errCount++;}   
    }

    else if(opp == "lui"){
        ss >> rs;
        ss >> offset;

        if(rs[0] != '$') {
            printError(l, "'$' missing in REG#1");
            flag = false;
            s = 1;
            errCount++;
        }
        if(rs[rs.size() -1] != ','){
            printError(l, "',' missing after REG#1");
            flag = false;
            s = 1;
            errCount++;
        }

        if(offset.size() == 0){
            printError(l, "offset value is not given");
            flag = false;
            v = 1;
            errCount++;
        }
        else if(offset[0] != '0' || offset[1] != 'x'){
            printError(l, "offset is not in hexadecimal");
            flag = false;
            v = 1;
            errCount++;
        }

        if(s == 0) 
            if(REG.find(rs.substr(0, rs.size() - 1)) == REG.end()) 
                {printError(l, "Undefined value for REG#1"); flag = false; errCount++;}
        
        if(v == 0)
            if(HEXMAP.find(offset.substr(2, offset.size())) == HEXMAP.end())
                {printError(l, "not supported mem address"); flag = false; errCount++;}
        
    }

    else{
        ss >> rs;
        ss >> rt;
        ss >> offset;

        if(rs[0] != '$'){
            printError(l, "'$' missing in REG#1");
            flag = false;
            s = 1;
            errCount++;
        }
        if(rs[rs.size() -1] != ','){
            printError(l, "',' missing after REG#1");
            flag = false;
            s = 1;
            errCount++;
        }
        if(rt[0] != '$'){
            printError(l, "'$' missing in REG#2");
            flag = false;
            t = 1;
            errCount++;
        }
        if(rt[rt.size() -1] != ','){
            printError(l, "',' missing after REG#2");
            flag = false;
            t = 1;
            errCount++;
        }

        if(offset.size() == 0){
            printError(l, "immediate value not given");
            flag = false;
            errCount++;
        }else{
            if(stoi(offset) > 3){
                printError(l, "immediate value is not supported");
                flag = false;
                errCount++;
            }
        }

        if(s == 0) 
            if(REG.find(rs.substr(0, rs.size()-1)) == REG.end()) 
                {printError(l, "Undefined value for REG#1"); flag = false; errCount++;}

        if(t == 0) 
            if(REG.find(rt.substr(0, rt.size()-1)) == REG.end()) 
                {printError(l, "Undefined value for REG#2"); flag = false; errCount++;}
    }

    return flag;
}

//main checker
bool checkValidity(string ins, int lineNum){
    stringstream ss;
    string opp, c;
    if(ins.size() == 0) printError(lineNum, "empty instruction !");
    else{
        ss << ins;
        ss >> opp;
        if(OPPCODE.find(opp) == OPPCODE.end()){
            fatalError(lineNum, "undefined oppcode");
            return false;
            errCount++;
        }
        else c = OPPCODE[opp];
        return c[0]=='1' ? checkValIformat(opp,ss,lineNum) : checkValRformat(ss,lineNum);
    }
    return false;
}

//fetch lines from input file
vector<string> fetchFromFile(string fileName){
    ifstream FILE(fileName+".txt");
    vector<string> fileData, emp;
    string buffer;
    int line = 0, run = 0;
    if(FILE.is_open()){
        std::cout << "checking errors: \n\n";
        while(line++, getline(FILE, buffer)) {
            bool isVal = checkValidity(buffer, line);
            if(isVal) {
                printf("**sucessful instruction: %s **\n\n", buffer.c_str());
                fileData.push_back(buffer);
            }
            else {
                printf("**could not run instruction: %s **\n\n", buffer.c_str());
                run = 1;    
            }
        }
        FILE.close();
        std::cout << "errors found: " << errCount << endl;
        if(run == 1) {
            std::cout << "please fix the errors!" << "\n";
            return emp;
        }
    } 
    else std::cout << fileName+".txt not found!\n";
    
    return fileData;    
}

//write into file
void writeIntoFile(vector<string>outputData, string fileName){
    ofstream FILE(fileName+".mc.txt");

    if(FILE.is_open()){
        FILE << "v2.0 raw" << "\n";
        for(unsigned i = 0; i < outputData.size(); i++){
            //std::cout << outputData[i] << "\n";
            FILE << outputData[i] << "\n";
        }
        FILE.close();
    }
    else std::cout << "Failed to open!\n" << endl;

    printf("output file: %s.mc.txt\n", fileName.c_str());
}

//r format instruction
string rFromat(vector<string> ins){
    int formattingArr[3] = {2,3,1};
    string result = "";
    
    for(unsigned i = 0; i<3; i++){
        int regNum = stoi(ins[formattingArr[i]].substr(1, ins[formattingArr[i]].size()));
        
        if(regNum <= 3){
            string binary = bitset<2>(regNum).to_string();
            result += " ";
            result += binary;

        }
        else std::cout << "Unallocated Register!" << endl; 
    }
    return result;
}

//i format
string iFormat(vector<string> ins){
    string result = " ";

    if(ins[0] == "lw" || ins[0] == "sw"){
        int rs = stoi(ins[1].substr(1, ins[1].size()));
        int offset = stoi(ins[2].substr(0, ins[2].size() -1));
        int rt = stoi(ins[3].substr(1, ins[3].size()-1));
        
        if(offset > 3) std::cout << "Offset value is greater than possible \n";
        
        string offsetBin = bitset<2>(offset).to_string(); 
        string rsBin = bitset<2>(rs).to_string();
        string rtBin = bitset<2>(rt).to_string();

        result += rtBin + " " + rsBin + " " + offsetBin;
    }
    else if(ins[0] == "lui"){
        int rs = stoi(ins[1].substr(1, ins[1].size()));  
        
        string rsBin = bitset<2>(rs).to_string(); 
        string memLoc = hex2bin(ins[2]);

        result += rsBin + " " + memLoc;
    }
    else{
        int rt = stoi(ins[1].substr(1, ins[1].size()));
        int rs = stoi(ins[2].substr(1, ins[2].size()));
        int val = stoi(ins[3]);

        string rsBin = bitset<2>(rs).to_string();
        string rtBin = bitset<2>(rt).to_string();
        string valBin = bitset<2>(val).to_string();

        result += rsBin + " " + rtBin + " " +valBin;
    }

    return result;
}

//process the instructions
vector<string> generateMachineCode(vector<string> insJar){
    vector<string> machineCodeJar;
    for(unsigned insNum = 0; insNum < insJar.size(); insNum++){
        string machineCode = "";
        vector<string> explodedIns = explodeLine(insJar[insNum], " ,$");
        string oppCode = OPPCODE[explodedIns[0]];
        //std::cout << insJar[insNum] << endl;
        //std::cout << oppCode << endl;
 
        machineCode += oppCode;
        machineCode += oppCode[0] == '0' ? rFromat(explodedIns) : iFormat(explodedIns);
        //std::cout << machineCode << endl;
        machineCodeJar.push_back(machineCode);
    }

    return machineCodeJar;
}

bool promt(){
    string opp;

    std::cout << "do you want to run .txt: ";
    cin >> opp;

    return (scmp_ig_case(opp, "yes") || scmp_ig_case(opp, "y")) == 0? 0 : 1;
    
}

string binToHex(string bin){
    string hex = "";
    for(int i = 0; i < bin.length(); i+=4){
        string buffer = bin.substr(i, 4);
        hex += BIN_TO_HEX_MAP[buffer];
    }
    return hex;
}

int main(){
    

    if(promt() == true){
        string fileName;

        std::cout << "please enter the file name: ";
        cin >> fileName;
    
        vector<string> inputData = fetchFromFile(fileName);
        vector<string> outputData = generateMachineCode(inputData);
        if(outputData.size()>0) writeIntoFile(outputData, fileName);
    }else{
        cout << "10 bit assembler\n"; 
        string cmd;
        vector<string> outputJar;

        cout << "->> ";
        cin.get();
        while(getline(cin, cmd) && !scmp_ig_case(cmd, "quit")){
            
            if(checkValidity(cmd, 0)){
                vector<string> inputdata;
                inputdata.push_back(cmd);
                string hex = "00";
                vector<string> outputData = generateMachineCode(inputdata);
                for(unsigned i = 0; i < outputData.size(); i++){
                    cout << outputData[i] << "\n";
                    //outputJar.push_back(outputData[i]);
                    hex += outputData[i];
                    string::iterator end_pos = std::remove(hex.begin(), hex.end(), ' ');
                    hex.erase(end_pos, hex.end());
                    string hexOut = binToHex(hex);
                    outputJar.push_back(hexOut);
                    cout << hexOut;
                }
                cout << "\n";
            }               
            cout << "->> ";
        }

        string fileName;
        cout << "save as: ";
        cin >> fileName;
        if(outputJar.size() > 0) writeIntoFile(outputJar, fileName);

    }
}
 