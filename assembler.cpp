#include <bits/stdc++.h>
using namespace std;

const int dictSize = 256;

void printError(int line, string err){
    printf("Error at line %d: %s \n", line, err.c_str());
}

//load map from file
map<string, string> fetchMap(string fileName){
    string key, value;
    map<string,string> m;
    ifstream FILE("resources/"+fileName+".txt");
   
    while(FILE>>key>>value) m[key] = value;
    FILE.close();
    
    return m;
}

//global maps
map<string, string> OPPCODE = fetchMap("oppcode");
map <string, string> REG = fetchMap("regMap");
map<string, string> HEXMAP = fetchMap("hexHelper");

//hex to bin converter
string hex2bin(string hex){
    string hexcode = hex.substr(2, hex.size());
    string bin = "";
    
    for(int i = 0; i<hexcode.size(); i++){
        string key = "";
        key += hexcode[i];
        bin += HEXMAP[key];
        cout << HEXMAP[key] << "\n";
        cout << key << "\n";
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
    }
    if(rs[rs.size() -1] != ','){
        printError(l, "',' missing after REG#2");
        flag = false;
        sc = 1;
    }   
    if(rt[0] != '$'){
        printError(l, "'$' missing in REG#3");
        flag = false;
        t = 1;
    }
    if(rd[0] != '$'){
        printError(l, "'$' missing in REG#1");
        flag = false;
        d = 1;
    }
    if(rd[rd.size() -1] != ','){
        printError(l, "',' missing after REG#1");
        flag = false;
        dc = 1;
    }

    if(s == 0 && sc == 0) 
        if(REG.find(rs.substr(0, rs.size()-1)) == REG.end()) 
            {printError(l, "undefined value for REG#2"); flag = false;}
    if(s == 0 && sc == 1) 
        if(REG.find(rs.substr(0, rs.size())) == REG.end()) 
            {printError(l, "undefined value for REG#2"); flag = false;}

    if(t == 0) 
        if(REG.find(rt) == REG.end()) 
            {printError(l, "undefined value for REG#3"); flag = false;} 
    
    if(d == 0 && dc == 0) 
        if(REG.find(rd.substr(0, rd.size()-1)) == REG.end()) 
            {printError(l, "undefined value for REG#1"); flag = false;}

    if(d == 0 && dc == 1) 
        if(REG.find(rd.substr(0, rd.size()-1)) == REG.end()) 
            {printError(l, "undefined value for REG#1"); flag = false;}

    return flag;
}

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
        }
        if(rt[rt.size() -1] != ','){
            printError(l, "',' missing after REG#1");
            flag = false;
            t = 1;
        }
        if(check.str().substr(1, check.str().size()) == check1.str()){
            printError(l, "offset value not given");
            flag = false;
            nooff = 1;
        }
        if(value > 3 && nooff == 0){
            printError(l, "Invalid offset value");
            flag = false;
        }
        if(rs[0] != '('){
            printError(l, "'(' missing after offset");
            flag = false;
            o = 1;
        }
        if(o == 1){
            if(rs[0] != '$'){
                printError(l, "'$' missing in REG#2");
                flag = false;
                s = 1;
            }
        }else{
            if(rs[1] != '$'){
                printError(l, "'$' missing in REG#2");
                flag = false;
                s = 1;
            }
        }
        if(rs[rs.size() -1] != ')'){
            printError(l, "')' missing after REG#2");
            flag = false;
            c = 1;
        }

        if(t == 0) 
            if(REG.find(rt.substr(0, rt.size() - 1)) == REG.end()) 
                {printError(l, "undefined value for REG#1"); flag = false;}
    
        if(s == 0 && o == 0 && c == 0) 
            if(REG.find(rs.substr(1, rs.size() - 2)) == REG.end()) 
                {cout << '1' << endl; printError(l, "undefined value for REG#2"); flag = false;}

        if(s == 0 && o == 1 && c == 0)
            if(REG.find(rs.substr(0, rs.size() - 1)) == REG.end()) 
                {cout << '2' << endl; printError(l, "undefined value for REG#2"); flag = false;}

        if(s == 0 && o == 0 && c == 1)
            if(REG.find(rs.substr(1, rs.size())) == REG.end()) 
                {cout << '3' << endl; printError(l, "undefined value for REG#2"); flag = false;}
            
        if(s == 0 && o == 1 && c == 1)
            if(REG.find(rs) == REG.end()) 
                {cout << '4' << endl; printError(l, "undefined value for REG#2"); flag = false;}   
    }

    else if(opp == "lui"){
        ss >> rs;
        ss >> offset;

        if(rs[0] != '$') {
            printError(l, "'$' missing in REG#1");
            flag = false;
            s = 1;
        }
        if(rs[rs.size() -1] != ','){
            printError(l, "',' missing after REG#1");
            flag = false;
            s = 1;
        }

        if(offset.size() == 0){
            printError(l, "offset value is not given");
            flag = false;
            v = 1;
        }
        else if(offset[0] != '0' || offset[1] != 'x'){
            printError(l, "offset is not in hexadecimal");
            flag = false;
            v = 1;
        }

        if(s == 0) 
            if(REG.find(rs.substr(0, rs.size() - 1)) == REG.end()) 
                {printError(l, "Undefined value for REG#1"); flag = false;}
        
        if(v == 0)
            if(HEXMAP.find(offset.substr(2, offset.size())) == HEXMAP.end())
                {printError(l, "not supported mem address"); flag = false;}
        
    }

    else{
        ss >> rs;
        ss >> rt;
        ss >> offset;

        if(rs[0] != '$'){
            printError(l, "'$' missing in REG#1");
            flag = false;
            s = 1;
        }
        if(rs[rs.size() -1] != ','){
            printError(l, "',' missing after REG#1");
            flag = false;
            s = 1;
        }
        if(rt[0] != '$'){
            printError(l, "'$' missing in REG#2");
            flag = false;
            t = 1;
        }
        if(rt[rt.size() -1] != ','){
            printError(l, "',' missing after REG#2");
            flag = false;
            t = 1;
        }

        if(offset.size() == 0){
            printError(l, "immediate value not given");
            flag = false;
        }else{
            if(stoi(offset) > 3){
                printError(l, "immediate value is not supported");
                flag = false;
            }
        }

        if(s == 0) 
            if(REG.find(rs.substr(0, rs.size()-1)) == REG.end()) 
                {printError(l, "Undefined value for REG#1"); flag = false;}

        if(t == 0) 
            if(REG.find(rt.substr(0, rt.size()-1)) == REG.end()) 
                {printError(l, "Undefined value for REG#2"); flag = false;}
    }

    return flag;
}

bool checkValidity(string ins, int lineNum){
    stringstream ss;
    string opp, c;
    if(ins.size() == 0) printError(lineNum, "empty instruction !");
    else{
        ss << ins;
        ss >> opp;
        if(OPPCODE.find(opp) == OPPCODE.end()){
            printError(lineNum, "undefined oppcode");
            return false;
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
        while(line++, getline(FILE, buffer)) {
            bool isVal = checkValidity(buffer, line);
            if(isVal) {
                printf("**sucessful instruction: %s **\n\n", buffer.c_str());
                fileData.push_back(buffer);
                run = 1;
            }
            else {
                printf("**could not run instruction: %s **\n\n", buffer.c_str());
                run = 0;    
            }
        }
        FILE.close();
    } 
    else cout << fileName+".txt not found!";
    if(run == 0) {
        cout << "please fix the errors!" << "\n";
        return emp;
    }
    return fileData;    
}

//write into file
void writeIntoFile(vector<string>outputData, string fileName){
    ofstream FILE(fileName+".mc.txt");

    if(FILE.is_open()){
        for(unsigned i = 0; i < outputData.size(); i++){
            //cout << outputData[i] << "\n";
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
        int regNum = stoi(ins[formattingArr[i]].substr(1, ins[formattingArr[i]].size()));
        
        if(regNum <= 3){
            string binary = bitset<2>(regNum).to_string();
            result += " ";
            result += binary;
        }
        else cout << "Unallocated Register!" << endl; 
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
        
        if(offset > 3) cout << "Offset value is greater than possible \n";
        
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
        cout << insJar[insNum] << endl;
        //cout << oppCode << endl;
        
        machineCode += oppCode;
        machineCode += oppCode[0] == '0' ? rFromat(explodedIns) : iFormat(explodedIns);
        cout << machineCode << endl;
        machineCodeJar.push_back(machineCode);
    }

    return machineCodeJar;
}

int main(){
    string fileName;

    cin >> fileName;
    vector<string> inputData = fetchFromFile(fileName);
    vector<string> outputData = generateMachineCode(inputData);
    if(outputData.size()>0) writeIntoFile(outputData, fileName); 
}