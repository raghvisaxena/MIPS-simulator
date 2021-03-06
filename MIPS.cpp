#include<iostream>
#include<string>
#include<vector>
#include<bitset>
#include<fstream>
using namespace std;
#define ADDU 1
#define SUBU 3
#define ADD 0
#define SUB 2
#define AND 4
#define OR  5
#define NOR 7
#define MUL 24
#define DIV 26
#define MULU 25
#define DIVU 27
#define XOR 6
#define MemSize 65536 // actual memory size should be 2^32, but for this project, for the space reasons,
//we keep it as 65536, the memory is still 32-bit addressable.

class RF
{
public:
    bitset<32> ReadData1, ReadData2;
    RF()
    {
        Registers.resize(32);
        Registers[0] = bitset<32> (0);
    }

    void ReadWrite(bitset<5> RdReg1, bitset<5> RdReg2, bitset<5> WrtReg, bitset<32> WrtData, int WrtEnable)
    {
        // implement the function by you.
            
            ReadData1 = Registers[RdReg1.to_ulong()];   // perform read operation
            ReadData2 = Registers[RdReg2.to_ulong()];

         if(WrtEnable == 1){    
           
            Registers[WrtReg.to_ulong()] = WrtData;    // perform write operation
        }
    }

    void OutputRF()
    {
        ofstream rfout;
        rfout.open("RFresult.txt",std::ios_base::app);
        if (rfout.is_open())
        {
            rfout<<"A state of RF:"<<endl;
            for (int j = 0; j<32; j++) 
            {
                rfout << Registers[j]<<endl;
            }
        }
        else cout<<"Unable to open file";
        rfout.close();
    }
private:
    vector<bitset<32>>Registers;

};

class ALU
{
public:
    bitset<32> ALUresult;
    bitset<32> ALUOperation (bitset<3> ALUOP, bitset<32> oprand1, bitset<32> oprand2)
    {    // implement the ALU operations.
        unsigned long result;
        
        if(ALUOP.to_string() == "001"){
            result = oprand1.to_ulong() + oprand2.to_ulong(); // addu operation
        }
        else if(ALUOP.to_string() == "000"){
            result = oprand1.to_ulong() + oprand2.to_ulong(); // add operation
        }
        else if(ALUOP.to_string() == "010"){
            result = oprand1.to_ulong() - oprand2.to_ulong(); // sub operation
        }
        else if(ALUOP.to_string() == "011"){
            result = oprand1.to_ulong() - oprand2.to_ulong(); // subu operation
        }
        else if(ALUOP.to_string() == "100"){
            result = oprand1.to_ulong() & oprand2.to_ulong(); // and operation
        }
        else if(ALUOP.to_string() == "101"){
            result = oprand1.to_ulong() | oprand2.to_ulong(); // or operation
        }
        else if(ALUOP.to_string() == "111"){
            result = ~(oprand1.to_ulong() | oprand2.to_ulong()); // nor operation
        }
        else if(ALUOP.to_string() == "110"){
            result = ~(oprand1.to_ulong() ^ oprand2.to_ulong()); // xor operation
        }
        
        bitset<32> res((int)result);

        ALUresult = res;
        return ALUresult;
    }

    bitset<32> ALUresults;
    bitset<32> ALUOperations (bitset<6> ALUOP, bitset<32> oprand1, bitset<32> oprand2)
    {    // implement the ALU operations.
        unsigned long result;
        cout<<"ALUOP: "<<ALUOP<<endl; 

        if(ALUOP.to_string() == "011000"){
            result = oprand1.to_ulong() * oprand2.to_ulong(); // mul operation
        }
        else if(ALUOP.to_string() == "011001"){
            result = oprand1.to_ulong() * oprand2.to_ulong(); // mulu operation
        }
        else if(ALUOP.to_string() == "011010"){
            result = oprand1.to_ulong() / oprand2.to_ulong(); // div operation
        }
        else if(ALUOP.to_string() == "011011"){
            result = oprand1.to_ulong() / oprand2.to_ulong(); // divu operation
        }
         
        bitset<32> res((int)result);

        ALUresults = res;
        return ALUresults;
    }
};

class INSMem
{
public:
    bitset<32> Instruction;
    INSMem()
    {       IMem.resize(MemSize);
        ifstream imem;
        string line;
        int i=0;
        imem.open("imem.txt");
        if (imem.is_open())
        {
            while (getline(imem,line))
            {
                bitset<8> b1(line);
                IMem[i] = b1;
                //IMem[i] = bitset<8>(line);
                i++;
            }
        }
        else cout<<"Unable to open file";
        imem.close();
    }

    bitset<32> ReadMemory (bitset<32> ReadAddress)
    {    // implement by you. (Read the byte at the ReadAddress and the following three byte).
        unsigned long count = ReadAddress.to_ulong();
        int index = (int)count;
        string inst;
        for(int i=0;i<4;i++){

            inst += IMem[index].to_string();
            index++;
        }
        bitset<32> Inst(inst);
        Instruction = Inst;

        return Instruction;
    }
private:
    vector<bitset<8> > IMem;

};


class DataMem
{
public:
    bitset<32> readdata;
    DataMem()
    {
        DMem.resize(MemSize);
        ifstream dmem;
        string line;
        int i=0;
        dmem.open("dmem.txt");
        if (dmem.is_open())
        {
            while (getline(dmem,line))
            {
                bitset<8> b2(line);
                DMem[i] = b2;
                //DMem[i] = bitset<8>(line);
                i++;
            }
        }
        else cout<<"Unable to open file";
        dmem.close();
    }

    bitset<32> MemoryAccess (bitset<32> Address, bitset<32> WriteData, bitset<1> readmem, bitset<1> writemem)
    {
        unsigned long count = Address.to_ulong();
        int index = (int) count;
        if((readmem.to_ulong() == 1) && (writemem.to_ulong() == 0)) {

            string data;
            for (int i = 0; i < 4; i++) {

                data += DMem[index].to_string();
                index++;
            }
            bitset<32> Data(data);
            readdata = Data;
        }

        else if((readmem.to_ulong() == 0) && (writemem.to_ulong() == 1)){
                                                                           // perform write operation
            for(int i= 0;i<32;i=i+8){
                string result = WriteData.to_string().substr(i,8);
                bitset<8> finalResult(result);
                DMem[index] =  finalResult;
                index++;
            }
        }
        return readdata;
    }

    void OutputDataMem()
    {
        ofstream dmemout;
        dmemout.open("dmemresult.txt");
        if (dmemout.is_open())
        {
            for (int j = 0; j< 1000; j++)
            {
                dmemout << DMem[j]<<endl;
            }
        }
        else cout<<"Unable to open file";
        dmemout.close();
    }
private:
    vector<bitset<8> > DMem;

};


string TypeofInstruction(string opcode){
    if(opcode == "000000") return "R";
    else if(opcode == "000010") return "J";
    else if(opcode == "111111") return "halt";
    else if( opcode == "001001") return "addiu";
    else if(opcode == "000100") return "beq";
    else if(opcode == "100011") return "lw";
    else if(opcode == "101011") return "sw";
    else return "Invalid Opcode";
}

string TypeofFunction(bitset<32> instruction){
    string aluOp = instruction.to_string().substr(29,3);
    if(aluOp == "001") return "addu";
    else if (aluOp == "011") return "subu";
    else if (aluOp == "100") return "and";
    else if (aluOp == "101") return "or";
    else if (aluOp == "111") return "nor";
    else if (aluOp == "000") return "add";
    else if (aluOp == "010") return "sub"; 
    else if (aluOp == "110") return "xor"; 
    else return "Invalid AluOP";
}
string TypeofFunctions(bitset<32> instruction){
    string aluOp = instruction.to_string().substr(26,6);
    
    if(aluOp == "011000") return "mul";
    else if (aluOp == "011001") return "mulu";
    else if (aluOp == "011010") return "div";
    else if (aluOp == "011011") return "divu";
    else return "Invalid AluOP";
}

vector<bitset<5> > OperandsAddr_R(bitset<32> instruction){
    vector<bitset<5> > result(3);
    string rs = instruction.to_string().substr(6,5);
    string rt = instruction.to_string().substr(11,5);
    string rd = instruction.to_string().substr(16,5);
    bitset<5> Rd(rd);
    bitset<5> Rs(rs);
    bitset<5> Rt(rt);

    result[0] = Rs;
    result[1] = Rt;
    result[2] = Rd;

    return result;
}

vector<string> OperandsAddr_I(bitset<32> instruction){
    string instr = instruction.to_string();

    string rs = instruction.to_string().substr(6,5);
    string rd = instruction.to_string().substr(11,5);
    string imm = instruction.to_string().substr(16,16);

    vector<string> result(3);
    result[0] = rs;
    result[1] = rd;
    result[2] = imm;

    return result;
}

bitset<32> calc_PC(bitset<32> &programCounter)
{
      programCounter = bitset<32> (programCounter.to_ulong()+4);
      return programCounter;
}

int main()
{
    RF myRF;
    ALU myALU;
    INSMem myInsMem;
    DataMem myDataMem;

    bitset<32> programCounter = bitset<32> (0),instruction;

    string Type_instruction, Type_function, Type_functions;
    bitset<5> Addr_Rs, Addr_Rt, Addr_Rd;
    bitset<32> $Rs,$Rd,$Rt,AluResult;
    bitset<3> addu_op(ADDU), subu_op(SUBU), and_op(AND), or_op(OR), nor_op(NOR),add_op(ADD),sub_op(SUB), xor_op(XOR);
    bitset<6> mul_op(MUL),div_op(DIV),mulu_op(MULU),divu_op(DIVU);
    vector<bitset<5> > RAddresses;
    vector<string> IAddressess(3);
    unsigned long counter=0;
    int flag = 0;

    while (1)
    {
        // Fetch
        instruction = myInsMem.ReadMemory(programCounter);

        if(instruction.to_string() == "11111111111111111111111111111111") break; //halt condition

        // decode(Read RF)

        //execute instruction
        string opcode = instruction.to_string().substr(0,6);  //cout<<"Opcode : "<<opcode<<" : ";

        string Type_instruction = TypeofInstruction(opcode);  //identifying instruction type

        if(Type_instruction == "R"){
            //execute R instruction
            RAddresses = OperandsAddr_R(instruction);
            Addr_Rs = RAddresses[0];
            Addr_Rt = RAddresses[1];
            Addr_Rd = RAddresses[2];

            Type_function = TypeofFunction(instruction);
            Type_functions = TypeofFunctions(instruction);
            string f;

            //Execute
            myRF.ReadWrite(Addr_Rs,Addr_Rt,NULL,NULL,0);
            $Rs = myRF.ReadData1;
            $Rt = myRF.ReadData2;
            cout<<"$rs and $Rt: "<<$Rs<<" "<<$Rt<<endl;

            // ALU operation
            if(Type_function == "addu") {AluResult = myALU.ALUOperation(addu_op, $Rs, $Rt); f=Type_function;}
            else if(Type_function == "add") {AluResult = myALU.ALUOperation(add_op, $Rs, $Rt); f=Type_function;}
            else if(Type_function == "sub") {AluResult = myALU.ALUOperation(subu_op, $Rs, $Rt); f=Type_function;}
            else if(Type_function == "and") {AluResult = myALU.ALUOperation(and_op, $Rs, $Rt); f=Type_function;}
            else if(Type_function == "or") {AluResult = myALU.ALUOperation(or_op, $Rs, $Rt); f=Type_function;}
            else if(Type_function == "nor") {AluResult = myALU.ALUOperation(nor_op, $Rs, $Rt); f=Type_function;}
            else if(Type_function == "subu") {AluResult = myALU.ALUOperation(subu_op, $Rs, $Rt); f=Type_function;}
            else if(Type_function == "xor") {AluResult = myALU.ALUOperation(xor_op, $Rs, $Rt); f=Type_function;}

            if(Type_functions == "mul") {AluResult = myALU.ALUOperations(mul_op, $Rs, $Rt); f=Type_functions;}
            else if(Type_functions == "div") {AluResult = myALU.ALUOperations(div_op, $Rs, $Rt); f=Type_functions;}
            else if(Type_functions == "mulu") {AluResult = myALU.ALUOperations(mulu_op, $Rs, $Rt); f=Type_functions;}
            else if(Type_functions == "divu") {AluResult = myALU.ALUOperations(divu_op, $Rs, $Rt); f=Type_functions;}
            cout<<"ALU RESULT : "<<AluResult<<endl;
            cout<<"Instruction : "<<f<<endl;
            myRF.ReadWrite(NULL, NULL, Addr_Rd, AluResult, 1); //Writing result to write register
            programCounter = calc_PC(programCounter);
        }

        else if(Type_instruction == "lw" || Type_instruction == "sw" || Type_instruction == "addiu" || Type_instruction == "beq")
            {
                IAddressess = OperandsAddr_I(instruction);
                bitset<5> RsIAddr(IAddressess[0]);
                bitset<5> RtIAddr(IAddressess[1]);
                bitset<16> ImmIAddr(IAddressess[2]);
                unsigned long templ = RsIAddr.to_ulong();
                int temp = (int)templ; temp++;
                bitset<5> RsIAddrt(to_string(temp));
                RsIAddr = RsIAddrt;
                myRF.ReadWrite(RsIAddr, NULL, NULL, NULL, 0);
                $Rs = myRF.ReadData1;
                string finalImmStr,finalImmStrs; //complete Imm

                if (ImmIAddr.to_string().at(0) == '0') {
                        finalImmStr = "00000000000000000000000000000000";
                        finalImmStrs = "0000000000000000" + ImmIAddr.to_string();
                } 
                else if (ImmIAddr.to_string().at(0) == '1') {
                    finalImmStr = "1111111111111111" + ImmIAddr.to_string();
                }
                bitset<32> finalImm(finalImmStr);
                bitset<32> finalImms(finalImmStrs);
                string t = "00000000000000000000000000000000";
                bitset<32> taddr(t);
                if(flag==0){
                    //find effective address of memory
                    AluResult = finalImms;
                    flag = 1;
                }
                else{
                    //find effective address of memory 
                    AluResult = finalImm;  
                    flag = 0;
                }
                
            if(Type_instruction == "lw") {
                bitset<32> operand1 = myDataMem.MemoryAccess(AluResult, NULL, 1, 0); //get value from Data Memory
                if(flag == 0){
                    myRF.ReadWrite(NULL, NULL, RsIAddr, operand1, 1); //load value into the register
                }
                else{
                    myRF.ReadWrite(NULL, NULL, RtIAddr, operand1, 1); //load value into the register
                }
                programCounter = calc_PC(programCounter);
            }

            else if(Type_instruction == "sw"){
                myRF.ReadWrite(RtIAddr,NULL,NULL,NULL,0); // get value from register
                $Rd = myRF.ReadData1;
                unsigned long laddr = AluResult.to_ulong();
                int iaddr = (int) laddr + 4;
                bitset<32> aluaddr(iaddr);
                bitset<32> useless = myDataMem.MemoryAccess(aluaddr,$Rd,0,1); //store value to data memory
                programCounter = calc_PC(programCounter);
            }

            else if(Type_instruction == "addiu"){
                AluResult = myALU.ALUOperation(add_op, $Rs, finalImm);
                myRF.ReadWrite(NULL,NULL,Addr_Rt,AluResult,1);
                programCounter = calc_PC(programCounter);
            }

            else if(Type_instruction == "beq"){
                myRF.ReadWrite(NULL, RtIAddr, NULL, NULL, 0);
                $Rt = myRF.ReadData2;
                if($Rs.to_ulong() == $Rt.to_ulong()){
                    //jump according to logic
                    finalImmStr = finalImm.to_string().substr(0,30)+"00";
                    bitset<32> signExtImm(finalImmStr);

                    counter = programCounter.to_ulong();
                    counter = counter + 4 + signExtImm.to_ulong();
                    bitset<32> pc(counter);
                    programCounter = pc;
                }
                else {
                    programCounter = calc_PC(programCounter);
                    }
              }
        }
        else if (Type_instruction == "J"){
            bitset<26> JAddress(instruction.to_string().substr(6,26));
            counter = programCounter.to_ulong();
            counter = counter + 4;
            bitset<32> pc(counter);
            programCounter = pc;

            bitset<4> pc4MSB(programCounter.to_string().substr(0,4));
            string pcStr = pc4MSB.to_string() + JAddress.to_string() + "00";
            bitset<32> newPC(pcStr);
            programCounter = newPC;
        }
        myRF.OutputRF(); // dump RF;
    }
    myDataMem.OutputDataMem(); // dump data mem

    return 0;

}
