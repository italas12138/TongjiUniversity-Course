#include "ObjectCodeGen.h"

void ObjectCodeGen::divideBlock(vector<FuncEnterTable>* funcEnterTable, vector<_Quaternary> interMediateCode)
{
    //放入一个四元式代码结束标志，方便统一处理
    funcEnterTable->push_back({interMediateCode.size(), "end"});

    for(auto iter = funcEnterTable->begin(); (iter + 1) != funcEnterTable->end(); iter++)
    {
        /*遍历每一个函数，以函数为单位，划分每一个函数的基本块*/
        int curFunEnterPoint = iter->first;
        string curFunName = iter->second;

        //用于记录该函数所有的基本块
        vector<Block> blocks;

        //用于记录所有块的入口（四元式编号）
        priority_queue<int, vector<int>, greater<int>> blockEnters;

        /*
            按照三个原则划分基本块：
            1、程序的第一个语句
            2、能由条件转移或无条件转移转移到的语句
            3、紧跟在条件转移后面的语句
        */
       //第一条语句，即函数的入口点，算第一个块
       blockEnters.push(curFunEnterPoint);

       //获取下一个函数的入口点，以便遍历当前函数的所有四元式
       int nextFunEnterPoint = (iter + 1)->first;
       
        //说明该函数函数体空
       if(curFunEnterPoint + 1 == nextFunEnterPoint) continue;
            
       //遍历当前函数的所有四元式
       for(int i = curFunEnterPoint; i != nextFunEnterPoint; i ++)
       {
            _Quaternary q = interMediateCode[i];
            if(q._opt[0] == 'j')
            {
                //条件跳转语句，需要额外把紧跟其后的第一条语句（如果有的话）作为下一个块的入口点
                if(q._opt != "j" && i + 1 < nextFunEnterPoint)
                    blockEnters.push(i + 1);

                //条件转移或无条件转移语句跳转到的语句
                blockEnters.push(stoi(q._rd));
            }
            else if((q._opt == "return" || q._opt == "call") && i + 1 < nextFunEnterPoint)
            {
                //函数调用或是返回语句
                blockEnters.push(i + 1);
            }
       }

        //得到每个基本块的起始入口后，开始设置每一个块的属性
        //包括名称、所包含的四元式以及它的两个出口（如果有的话）
        Block block;

        //映射关系：{上一个块, {当前块的名称，当前块的块号}}
        map<int, pair<string,int>> trans;  

        int enter, lastEnter = blockEnters.top();
        blockEnters.pop();

        bool isFirstBlock = true;

        //针对函数只有一个基本块的情况，压入一个结束标志方便统一处理
        blockEnters.push((iter + 1)->first);
        while(blockEnters.size())
        {
            enter = blockEnters.top();
            blockEnters.pop();

            //这里实际上处理两个跳转到同一出口的情况
            if(enter == lastEnter) continue;

            //将两个入口点之间的所有四元式放入
            for(int i = lastEnter; i != enter; i++)
                block.quaters.push_back(interMediateCode[i]);
            
            if(isFirstBlock)
            {
                block.name = curFunName;
                isFirstBlock = false;
            }
            else
                block.name = label._MakeLabel();
            
            trans[lastEnter] = {block.name, blocks.size()};
            blocks.push_back(block);
            lastEnter = enter;
            block.quaters.clear();
        }
        
        // 开始建立块与块的转移关系
        int blockIndex = 0;

         //遍历每一个块
        for(auto& block : blocks)
        {
            auto lastQuater = block.quaters.rbegin();

            //跳转语句
            if(lastQuater->_opt[0] == 'j')
            {
                if(lastQuater->_opt == "j")
                {
                    block.next1 = trans[stoi(lastQuater->_rd)].second;
                    block.next2 = -1;
                }
                else
                {
                    //条件跳转语句，两个出口
                    block.next1 = blockIndex + 1;
                    block.next2 = trans[stoi(lastQuater->_rd)].second;
                    block.next2 = block.next1 == block.next2? -1 : block.next2;
                }
                lastQuater->_rd = trans[stoi(lastQuater->_rd)].first;
            }
            else if(lastQuater->_opt == "return"){
                //函数调用或结束
                block.next1 = block.next2 = -1;
            }
            else
            {
                //其他语句（顺序执行）
                block.next1 = blockIndex + 1;
                block.next2 = -1;
            }

            blockIndex++;
        }

        funcBlocks[curFunName] = blocks;
    }

    /*打印函数块*/
    outputFuncBlock("FuncBaseBlock.txt");
}


void ObjectCodeGen::handleBlock()
{
    //1、初始化待用/活跃信息表，将该块后面出现的变量的活跃信息置为true
    //从当前块向后面的逻辑块找
    for(auto iter = funcBlocks.begin(); iter != funcBlocks.end(); iter ++)
    {
        vector<Block> blocks = iter->second;
        for(auto& block : blocks)
        {
            set<string> activeVar;
            for(auto& quater : block.quaters)
            {
                //所有出现过的变量都置为非待用，是否活跃需要根据后面的基本块是否出现在左边决定

                if(isVar(quater._rs)){
                    flag.clear();
                    bool active = getActive(block, quater._rs, blocks);
                    activeInfos[block.name + quater._rs].push_back(ActiveInfo{ -1, active });
                }
                    
                if(isVar(quater._rt)){
                    flag.clear();
                    bool active = getActive(block, quater._rt, blocks);
                    activeInfos[block.name + quater._rt].push_back(ActiveInfo{ -1, active });
                }
                    
                if(isVar(quater._rd))
                {
                    flag.clear();
                    //键值为{函数名 + 变量名}，按理说这里还要加上变量的层级做区分，这里简单处理
                    bool active = getActive(block, quater._rd, blocks);
                    activeInfos[block.name + quater._rd].push_back(ActiveInfo{ -1, active});

                    //这里保存基本块出口后仍然活跃的变量信息
                    if(active)
                    {
                        activeVar.insert(quater._rd);
                    }
                                           
                }
            }
            //这里保存基本块出口后仍然活跃的变量信息
            outStillActive[iter->first].push_back(activeVar);
            
        }
    }

    
    
    /*
        2、从基本块出口到基本块入口由后向前依次处理各个四元式。对每一个四元式(op，B，C，A)，依次执行下面的步骤：
         1)   把符号表中变量A的待用信息和活跃信息附加到四元式i上；
         2)   把符号表中A的待用信息和活跃信息分别置为“非待用”和“非活跃”；
         3)   把符号表中变量B和C的待用信息和活跃信息附加到四元式i上；
         4)   把符号表中B和C的待用信息均置为i，活跃信息均置为“活跃”。
    */
   for(auto iter = funcBlocks.begin(); iter != funcBlocks.end(); iter ++)
   {
        vector<Block> blocks = iter->second;
        vector<BlockWithInfo> blockWithInfos;
        for(auto& block : blocks)
        {
            BlockWithInfo blockWithInfo(block);
            for(int i = block.quaters.size() - 1; i >= 0; i--)
            {
                _Quaternary quater = block.quaters[i];
                QuaternaryWithInfo quaterWithInfo(quater);

                //函数调用四元式不处理
                if(quater._opt == "call"){
                    blockWithInfo.quatersWithInfo[i] = quaterWithInfo;
                    continue;
                }

                if(isVar(quater._rd)){
                    quaterWithInfo.des = activeInfos[block.name + quater._rd].back();
                    activeInfos[block.name + quater._rd].push_back(ActiveInfo{ -1, false });
                }
                
                if(isVar(quater._rs)){
                    quaterWithInfo.lop = activeInfos[block.name + quater._rs].back();
                    activeInfos[block.name + quater._rs].push_back(ActiveInfo{ i, true });
                }

                if(isVar(quater._rt)){
                    quaterWithInfo.rop = activeInfos[block.name + quater._rt].back();
                    activeInfos[block.name + quater._rt].push_back(ActiveInfo{ i, true });
                }
                
                blockWithInfo.quatersWithInfo[i] = quaterWithInfo;
            }
            blockWithInfos.push_back(blockWithInfo);          
        }
        funcBlocksWithInfo[iter->first] = blockWithInfos;
   }

    outputFuncBlockWithInfo("FuncBlockWithActiveInfo.txt");
}
void ObjectCodeGen::outputObjectCode(string fileName)
{
    fstream out(fileName, ios::out);
    for(auto code : objectCodes){
        out << code << endl;
    }
    out.close();
}
void ObjectCodeGen::gen()
{
    objectCodes.push_back("lui $sp,0x1001");
    objectCodes.push_back("j main");
    
    for(auto iter = funcBlocksWithInfo.begin(); iter != funcBlocksWithInfo.end(); iter ++)
    {
        offsets.clear();
        curFuncName = iter->first;
        vector<BlockWithInfo>& blocks = iter->second;
        
        for(auto biter = blocks.begin(); biter != blocks.end(); biter ++){
            curIBlock = biter;

            getInActiveVar(biter - blocks.begin(), blocks);
            for(auto citer = inActive.begin(); citer!= inActive.end(); citer++)
                AVALUE[*citer].insert(*citer);
            
            genForFuncBlock(curIBlock - blocks.begin());


            AVALUE.clear();
            RVALUE.clear();
            
        }

        
            
    }
    objectCodes.push_back("end:");

    outputObjectCode("ObjectCode.asm");
    cout << "Compile successfully! Please open the output file to check the result! " << endl; 
}
void ObjectCodeGen::getInActiveVar(int funcBlockIndx, vector<BlockWithInfo> blocks)
{
    for(int i = 0; i < blocks.size(); i ++)
    {
        if(blocks[i].next1 == funcBlockIndx || blocks[i].next2 == funcBlockIndx)
            for(auto iter = outStillActive[curFuncName][i].begin(); iter != outStillActive[curFuncName][i].end(); iter ++)
                inActive.insert(*iter);
    }
}
void ObjectCodeGen::genForFuncBlock(int curBlockIdx)
{
    //初始化空闲寄存器
    freeRegs.clear();
    for(int i = 0; i <= 7; i++)
        freeRegs.push_back("$s" + to_string(i));

    objectCodes.push_back(curIBlock->name + ":");

    //如果是函数的第一个基本块
    if(curBlockIdx == 0){
        //不是主函数，需要先把返回地址压栈
        if(curFuncName != "main")
            objectCodes.push_back("sw $ra 4($sp)");

        top = 8;   
    }

    //遍历基本块的每一条语句
    int arg_num = 0, par_num = 0;
    list<pair<string,bool>> par_list;  //{实参名，是否活跃}
    for(auto citer = curIBlock->quatersWithInfo.begin(); citer != curIBlock->quatersWithInfo.end(); citer ++)
    {
        if(citer + 1 == curIBlock->quatersWithInfo.end())
        {
            if(isControl(citer->q._opt))
            {
                //如果最后一条是控制语句
                if(!outStillActive[curFuncName][curBlockIdx].empty())
                    storeActiveVar(outStillActive[curFuncName][curBlockIdx]);
                genForQuaternary(citer, arg_num, par_num, par_list);
            }
            else
            {
                genForQuaternary(citer, arg_num, par_num, par_list);
                if(!outStillActive[curFuncName][curBlockIdx].empty())
                    storeActiveVar(outStillActive[curFuncName][curBlockIdx]);
            }
        }
        else
            genForQuaternary(citer, arg_num, par_num, par_list);
    }
    
    
}
void ObjectCodeGen::genForQuaternary(vector<QuaternaryWithInfo>::iterator iter,int& arg_num, int& par_num, list<pair<string,bool>>& par_list)
{
    curQuater = iter;
    string op =  curQuater->q._opt;
    string rs =  curQuater->q._rs;
    string rt =  curQuater->q._rt;
    string rd = curQuater->q._rd;

    if(op != "j" && op != "call")
    {
        if(isVar(rs) && AVALUE[rs].empty())
        {
            _errorLog("[error] The var " + rs + " should be initialised be used!");
            cout << curQuater->q << endl;
            exit(-1);
        }
        if(isVar(rt) && AVALUE[rt].empty())
        {
            _errorLog("[error] The var " + rt + " should be initialised be used!");
            cout << curQuater->q << endl;
            exit(-1);
        }
    }


    if(op == "j")
        objectCodes.push_back(op + " " + curQuater->q._rd);
    else if(op[0] == 'j')
    {
        //条件跳转
        if(op == "j>=")
            op = "bge";
        else if(op == "j<=")
            op = "ble";
        else if(op == "j>")
            op = "bgt";
        else if(op == "j<")
            op = "blt";
        else if(op == "j==")
            op = "beq";
        else if(op == "j!=")
            op = "bne";
        string R1 = getReg(rs);
        string R2 = getReg(rt);
        objectCodes.push_back(op + " " + R1 + " " + R2 + " " + rd);
    }
    else if(op == "para")
    {
        //传递参数
        par_list.push_back({curQuater->q._rs, curQuater->lop.active});
    }
    else if(op == "call")
    {
        //转调函数
        for(auto iter = par_list.begin(); iter != par_list.end(); iter++)
        {
            //分配寄存器，将该参数压栈
            string reg = getReg(iter->first);

            offsets[iter->first] = (top + 4 * (++arg_num + 1));
            objectCodes.push_back("sw " + reg + " " + to_string(offsets[iter->first]) + "($sp)");
            if(!iter->second)
                releaseReg(iter->first);
        }

        arg_num = par_num = 0;
        par_list.clear();

        //更新sp
        objectCodes.push_back("sw $sp " + to_string(top) + "($sp)");
        objectCodes.push_back("addi $sp $sp " + to_string(top));

        //跳转到对应函数
        objectCodes.push_back("jal " + rs);

        //返回现场
        objectCodes.push_back("lw $sp 0($sp)");
    }
    else if(op == "return")
    {
        //函数返回
        if(isdigit(rs[0])){
            objectCodes.push_back("addi $v0 $zero " + rs);
        }
        else if(isVar(rs)){
            auto aiter = AVALUE[rs].begin();
            if((*aiter)[0] == '$'){
                //如果是寄存器
                objectCodes.push_back("add $v0 $zero " + (*aiter));
            }
            else{
                //如果是在内存中
                objectCodes.push_back("lw $v0 " + to_string(offsets[*aiter]) + "($sp)");
            }
        }
        if(curFuncName == "main"){
            //直接结束
            objectCodes.push_back("j end");
        }
        else{
            //返回调用处
            objectCodes.push_back("lw $ra 4($sp)");
            objectCodes.push_back("jr $ra");
        }
    }
    else if(op == "get"){
        //函数传参（子函数接收）
        offsets[rd] = top;
        top += 4;
        AVALUE[rd].insert(rd); 
    }
    else if(op == "="){
        //赋值运算 A = B (=, @RETURN, _, _) (=, a, _, T1);
        string pos;
        if(rs == "@RETURN"){
            pos = "$v0";
        }
        else{
            pos = getReg(rs);
        }
        AVALUE[rd].erase(rd);           //赋值过，就擦掉，如果是活跃变量，就要生成存数指令写入内存
        AVALUE[rd].insert(pos);
        RVALUE[pos].insert(rd);
        if(!curQuater->lop.active)
            releaseReg(rs);
        if(!curQuater->rop.active)
            releaseReg(rt);     
    }
    else{
        // + - * /
        string R1 = getReg(rs);
        string R2 = getReg(rt);
        string Rd = getRegForRd();
        if(op == "+")
            objectCodes.push_back("add " + Rd + " " + R1 + " " + R2);
        else if(op == "-")
            objectCodes.push_back("sub " + Rd + " " + R1 + " " + R2);
        else if(op == "*")
            objectCodes.push_back("mul " + Rd + " " + R1 + " " + R2);
        else if(op == "/"){
            objectCodes.push_back("div " + Rd + " " + R1 + " " + R2);
            objectCodes.push_back("mflo " + Rd);
        }

        AVALUE[rd].insert(rd);
        if(!curQuater->lop.active)
            releaseReg(rs);
        if(!curQuater->rop.active)
            releaseReg(rt);
        
    }
}
void ObjectCodeGen::releaseReg(string var)
{
    for(auto iter = AVALUE[var].begin(); iter != AVALUE[var].end(); iter ++)
    {
        if((*iter)[0] == '$')
        {
            //是寄存器就将其从该寄存器取出，是$si寄存器并且该寄存器取出该变量后为空，将其回收
            RVALUE[*iter].erase(var);
            if(RVALUE[*iter].size() == 0 && (*iter)[1] == 's')
            {
                freeRegs.push_back(*iter);
            }
        }
    }
    AVALUE[var].clear();
}

string ObjectCodeGen::getRegForRd()
{
    /*
        针对 A := B op C
        1 如果B的现行值在某个寄存器Ri中，RVALUE[Ri]中只包含B，
        2 或者B与A是同一个标识符
        3 或者B的现行值在执行四元式A:=B op C之后不会再引用，
        则选取Ri为所需要的寄存器R
    */
    for(auto iter = AVALUE[curQuater->q._rs].begin(); iter != AVALUE[curQuater->q._rs].end(); iter ++)
    {
        if((*iter)[0] == '$' && RVALUE[*iter].size() == 1)
        {
            if(curQuater->q._rs == curQuater->q._rd || !curQuater->lop.active){
                AVALUE[curQuater->q._rd].insert(*iter);
                RVALUE[*iter].insert(curQuater->q._rd);
                return *iter;
            }
        } 
    }

    
    string ret = getReg();
    AVALUE[curQuater->q._rd].insert(ret);
    RVALUE[ret].insert(curQuater->q._rd);
    return ret;
}

string ObjectCodeGen::getReg(string var)
{
    string ret;
    //是常数
    if(isdigit(var[0]))
    {      
        ret = getReg();
        objectCodes.push_back("addi " + ret + " $zero " + var);
        return ret;
    }

    //不是常数，但是已经在某个寄存器中，直接返回该寄存器
    for(auto iter = AVALUE[var].begin(); iter != AVALUE[var].end(); iter ++)
    {
        if((*iter)[0] == '$')
            return *iter;
    }

    //不是常数，也不在某个寄存器中，需要从内存中去取
    ret = getReg();
    objectCodes.push_back("lw " + ret + " " + to_string(offsets[var]) + "($sp)");

    AVALUE[var].insert(ret);
    RVALUE[ret].insert(var);
    return ret;
}
string ObjectCodeGen::getReg()
{
    string ret;

    //2 尽可能用空闲寄存器
    if(freeRegs.size())
    {
        ret = freeRegs.back();
        freeRegs.pop_back();
        return ret;
    }

    //3 抢占用非空闲寄存器
    /*
        从已分配的寄存器中选取一个Ri为所需要的寄存器R。最好使得Ri满足以下条件：
        占用Ri的变量的值也同时存放在该变量的贮存单元中（这样要的时候可以再取）
        在基本块中要在最远的将来才会引用到或不会引用到。(避免马上又要取出来放到寄存中，反复抢占寄存器)
    */


    const int INF = 2e9;
    int maxNextPos = 0;
    for(auto iter = RVALUE.begin(); iter != RVALUE.end(); iter ++)
    {
        //iter : {寄存器名，寄存器存储的变量集合}
        int nextPos = INF;
        for(auto viter = iter->second.begin(); viter != iter->second.end(); viter ++)
        {
            //viter : {变量名}
            int inFlag = false;
            for(auto aiter = AVALUE[*viter].begin(); aiter != AVALUE[*viter].end(); aiter ++)
            {
                //aiter : {viter这个变量存储的位置}
                if((*aiter) != iter->first){
                    //变量在其他地方存储
                    inFlag = true;
                    break;
                }
            }
            if(!inFlag)
            {
                for(auto citer = curQuater; citer != curIBlock->quatersWithInfo.end(); citer ++)
                {
                    //citer : {基本块中出现在curQuater后面的四元式}
                    if(*viter == citer->q._rs || *viter == citer->q._rt)
                        nextPos = citer - curQuater;
                    else if(*viter == citer->q._rd) //在后面viter会被重新定值
                        break;
                }
            }
        }

        //如果后面不会用到
        if(nextPos == INF){
            ret = iter->first;
            break;
        }
        else if(nextPos > maxNextPos){ //或者选一个最远用会用到的
            ret = iter->first;
            maxNextPos = nextPos;
        }

    }

    //清理ret 寄存器
    for(auto iter = RVALUE[ret].begin(); iter != RVALUE[ret].end(); iter ++)
    {
        AVALUE[*iter].erase(ret);
        if(AVALUE[*iter].size() == 0)
        {
            //需要生成存数指令
            storeVar(ret, *iter);
        }
    }

    RVALUE[ret].clear();
    return ret;

}

void ObjectCodeGen::storeVar(string reg, string var)
{
    if(offsets.find(var) == offsets.end())
    {
        offsets[var] = top;
        top += 4;
    }
    objectCodes.push_back("sw " + reg + " " + to_string(offsets[var]) + "($sp)");
    AVALUE[var].erase(reg);
    RVALUE[reg].erase(var);

    AVALUE[var].insert(var);
}
bool ObjectCodeGen::isControl(string op)
{
    return (op[0] == 'j' || op == "call" || op == "return" || op == "get");
}
void ObjectCodeGen::storeActiveVar(set<string> outActiveVars)
{
    for(auto iter = outActiveVars.begin(); iter != outActiveVars.end(); iter ++)
    {
        string curVar = *iter;
        // for(auto aiter = AVALUE[curVar].begin(); aiter != AVALUE[curVar].end(); aiter ++)
        // {
        //     string pos = *aiter;
        //     if(pos[0] == '$')
        //     {
        //         storeVar(pos, curVar);
        //         return;
        //     }
        // }
        string reg;
        bool isIn = false;
        for(auto aiter = AVALUE[curVar].begin(); aiter != AVALUE[curVar].end(); aiter ++)
        {
            string pos = *aiter;
            if(pos[0] != '$'){
                isIn = true;
                break;
            }
            else
                reg = pos;
        }
        if(!isIn)
        {
            //不在内存中，需要存储指令
            storeVar(reg, curVar);
        }
        
    }
}
bool ObjectCodeGen::isVar(string symbol)
{
    return isalpha(symbol[0]);
}
bool ObjectCodeGen::getActive(Block startBlock, string var, vector<Block> blocks)
{
    int next1 = startBlock.next1, next2 = startBlock.next2;
    if(next1 != -1 && next2 != -1) 
        return getActiveInfo(blocks[next1], var, blocks) || getActiveInfo(blocks[next2], var, blocks);
    if(next1 != -1) return getActiveInfo(blocks[next1], var, blocks);
    if(next2 != -1) return getActiveInfo(blocks[next2], var, blocks);

    //是最后一个块
    return false;
}
bool ObjectCodeGen::getActiveInfo(Block block, string var, vector<Block> blocks)
{
   

    //如果在block 后面的块中出现在右边，说明是活跃的，否则是非活跃的
    for(auto& q : block.quaters)
        if(q._rs == var || q._rt == var) 
        {
            return true;
        }

    //否则判断这一个块之后var有没有活跃过，应该是一个递归的过程，直到最后一个基本块
    //避免循环判断同一块
    if(flag[block.name]){
        return false;
    }
    flag[block.name] = true;
    return getActive(block, var, blocks);
}
void ObjectCodeGen::outputFuncBlock(const string fileName)
{
    ofstream out(fileName, ios::out);
    for (map<string, vector<Block> >::iterator iter = funcBlocks.begin(); iter != funcBlocks.end(); iter++) {
		out << "[" << iter->first << "]" << endl;
		for (vector<Block>::iterator bIter = iter->second.begin(); bIter != iter->second.end(); bIter++) {
			out << bIter->name << ":" << endl;
			for (vector<_Quaternary>::iterator cIter = bIter->quaters.begin(); cIter != bIter->quaters.end(); cIter++) {
				out <<"    "<< "(" << cIter->_opt << "," << cIter->_rs << "," << cIter->_rt << "," << cIter->_rd << ")" << endl;
			}
			out << "    " << "next1 = " << bIter->next1 << endl;
			out << "    " << "next2 = " << bIter->next2 << endl;
		}
		out << endl;
	}
    out.close();
}

void ObjectCodeGen::outputOutActiveVar(string fileName)
{
    ofstream out(fileName, ios::out);
    for(auto iter = outStillActive.begin(); iter != outStillActive.end(); iter ++)
    {
        out << iter->first << ":" << endl;
        for(auto biter = iter->second.begin(); biter != iter->second.end(); biter ++)
        {
            out << biter - iter->second.begin() << ":" << endl;
            for(auto citer = (*biter).begin(); citer != (*biter).end(); citer ++)
            {
                out << *citer << " ";
            }
            out << endl;
        }
    }
    out.close();
}
void ObjectCodeGen::outputFuncBlockWithInfo(const string fileName)
{
    ofstream out(fileName, ios::out);
    for (auto iter = funcBlocksWithInfo.begin(); iter != funcBlocksWithInfo.end(); iter++) {
		out << "[" << iter->first << "]" << endl;
		for (auto bIter = iter->second.begin(); bIter != iter->second.end(); bIter++) {
			out << bIter->name << ":" << endl;
			for (auto cIter = bIter->quatersWithInfo.begin(); cIter != bIter->quatersWithInfo.end(); cIter++) {
				out << *cIter << endl;
			}
			out << "    " << "next1 = " << bIter->next1 << endl;
			out << "    " << "next2 = " << bIter->next2 << endl;
		}
		out << endl;
	}
    out.close();
}