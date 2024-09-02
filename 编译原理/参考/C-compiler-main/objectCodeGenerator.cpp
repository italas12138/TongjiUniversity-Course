#include "objectCodeGenerator.h"
ObjectCodeGenerator::ObjectCodeGenerator(vector<_Quaternary> &inter_codes, vector<pair<int, string>> &fun_enters) : block_generator(inter_codes, fun_enters)
{
    block_generator.get_info_fun_blocks();
}
void ObjectCodeGenerator::generate_code()
{
    // 1层循环 为整个中间代码生成目标代码
    object_codes.push_back("lui $sp,0x1001"); // 设置栈顶
    object_codes.push_back("j main");         // 跳转到main
    for (auto fun_iter = block_generator.info_fun_blocks.begin(); fun_iter != block_generator.info_fun_blocks.end(); fun_iter++)
    {
        generate_code_for_fun(fun_iter);
    }
    object_codes.push_back("end:");

    // 打印结果
    cout << "目标代码生成完毕！"
         << "\n";
    ofstream fout;
    fout.open("code.asm");
    for (auto &e : object_codes)
        fout << e << "\n";
    fout.close();
}
void ObjectCodeGenerator::generate_code_for_fun(map<string, vector<InfoBlock>>::iterator fun_iter)
{
    // 2层循环 为每个函数块生成目标代码

    now_fun = fun_iter->first; // 获取当前函数名称
    auto &cur_blocks = fun_iter->second;
    int block_idx = 0;
    mem_manager.var_mem_pos.clear(); // 对于每个函数块要清空内存情况
    for (auto block_iter = cur_blocks.begin(); block_iter != cur_blocks.end(); block_iter++, block_idx++)
    {
        cur_info_block = block_iter;
        generate_code_for_block(block_idx);
    }
}
void ObjectCodeGenerator::generate_code_for_block(int block_idx)
{
    // 3层循环 为每个基本块生成目标代码

    // 初始化mem_manager
    auto &cur_inls = block_generator.fun_inls[now_fun][block_idx];
    mem_manager.reset_mem(cur_inls);
    object_codes.push_back(cur_info_block->block_name + ":");
    if (block_idx == 0)
    {
        if (now_fun != "main")
        {
            object_codes.push_back("sw $ra 4($sp)"); // 返回值压栈
        }
        mem_manager.top_ptr = 8;
    }
    // 遍历调用四元式生成目标代码
    int var_nums = 0;
    int paras_nums = 0;
    list<pair<string, bool>> paras_list;
    auto &cur_codes = cur_info_block->codes;
    for (auto code_iter = cur_codes.begin(); code_iter != cur_codes.end(); code_iter++)
    {
        cur_quaternary = code_iter;
        // 这里需要考虑一下是否为最后一条语句
        auto cur_op = code_iter->base_q._opt;
        if (code_iter + 1 == cur_codes.end() && need_store_first(cur_op))
        {
            mem_manager.store_outl_var(object_codes, block_generator.fun_outls[now_fun][block_idx]);
        }
        generate_code_for_quaternary(block_idx, var_nums, paras_nums, paras_list);
        if (code_iter + 1 == cur_codes.end() && !need_store_first(cur_op))
        {
            mem_manager.store_outl_var(object_codes, block_generator.fun_outls[now_fun][block_idx]);
        }
    }
}
void ObjectCodeGenerator::generate_code_for_quaternary(int block_idx, int &var_nums, int &paras_nums, list<pair<string, bool>> &paras_list)
{
    // 4层循环 为每个四元式生成目标代码

    // 整体逻辑就是根据当前op来判断
    auto &op = cur_quaternary->base_q._opt;
    auto &rs = cur_quaternary->base_q._rs;
    auto &rt = cur_quaternary->base_q._rt;
    auto &rd = cur_quaternary->base_q._rd;
    auto &rs_info = cur_quaternary->info[0];
    auto &rt_info = cur_quaternary->info[1];
    auto &rd_info = cur_quaternary->info[2];
    auto &fun_outls = block_generator.fun_outls;
    auto &info_fun_blocks = block_generator.info_fun_blocks;
    // 检查变量引用之前是否没有赋值
    check_valid(op, rs, rt);
    if (op == "=")
    {
        string rs_pos;
        if (rs == "RETURN")
            rs_pos = "$v0";
        else
            rs_pos = mem_manager.get_reg_for_src(now_fun, fun_outls, info_fun_blocks, cur_quaternary, cur_info_block, object_codes, rs);

        // rd也放到这个寄存器
        mem_manager.R_Value[rs_pos].insert(rd);
        mem_manager.A_Value[rd].insert(rs_pos);
    }
    else if (op == "get")
    {
        mem_manager.var_mem_pos[rd] = mem_manager.top_ptr;
        mem_manager.top_ptr += 4;
        mem_manager.A_Value[rd].insert(rd); // 把rd存入到栈内
    }
    else if (op == "return")
    {
        if (if_valid_num(rs))
        {
            // 返回值为数字
            object_codes.push_back("addi $v0 $zero " + rs);
        }
        else if (if_valid_var(rs))
        {
            // 返回值为变量
            auto pos = mem_manager.A_Value[rs].begin();
            if ((*pos)[0] == '$')
            {
                // 寄存器内
                object_codes.push_back(string("add $v0 $zero ") + *pos);
            }
            else
            {
                // 内存中
                object_codes.push_back(string("lw $v0 ") + to_string(mem_manager.var_mem_pos[*pos]) + "($sp)");
            }
        }
        if (now_fun == "main")
        {
            object_codes.push_back("j end");
        }
        else
        {
            object_codes.push_back("lw $ra 4($sp)"); // 返回地址在$ra
            object_codes.push_back("jr $ra");        // 跳转 $ra
        }
    }
    else if (op == "para")
    {
        // 这有点神奇
        paras_list.push_back(pair<string, bool>(rs, rs_info.active));
    }
    else if (op == "call")
    {
        // 参数压栈
        for (auto para_iter = paras_list.begin(); para_iter != paras_list.end(); para_iter++)
        {
            string pos = mem_manager.get_reg_for_src(now_fun, fun_outls, info_fun_blocks, cur_quaternary, cur_info_block, object_codes, para_iter->first);
            // 获得一个寄存器来存放参数
            object_codes.push_back(string("sw ") + pos + " " + to_string(mem_manager.top_ptr + 4 * (++var_nums + 1)) + "($sp)");
            if (!para_iter->second)
            {
                // 非活跃释放
                mem_manager.clear_var_reg(para_iter->first);
            }
        }
        // 更新$sp
        object_codes.push_back(string("sw $sp ") + to_string(mem_manager.top_ptr) + "($sp)");
        object_codes.push_back(string("addi $sp $sp ") + to_string(mem_manager.top_ptr));

        // 跳转
        object_codes.push_back(string("jal ") + rs);

        // 恢复现场
        object_codes.push_back(string("lw $sp 0($sp)"));
    }
    else if (op == "j")
    {
        object_codes.push_back(op + " " + rd);
    }
    else if (op[0] == 'j')
    {
        string oper;
        if (op == "j>=")
        {
            oper = "bge";
        }
        else if (op == "j>")
        {
            oper = "bgt";
        }
        else if (op == "j==")
        {
            oper = "beq";
        }
        else if (op == "j!=")
        {
            oper = "bne";
        }
        else if (op == "j<")
        {
            oper = "blt";
        }
        else if (op == "j<=")
        {
            oper = "ble";
        }
        else
        {
            cout << "Invalid j: " << op << "\n";
            exit(-1);
        }
        string reg_rs = mem_manager.get_reg_for_src(now_fun, fun_outls, info_fun_blocks, cur_quaternary, cur_info_block, object_codes, rs);
        string reg_rt = mem_manager.get_reg_for_src(now_fun, fun_outls, info_fun_blocks, cur_quaternary, cur_info_block, object_codes, rt);
        object_codes.push_back(oper + " " + reg_rs + " " + reg_rt + " " + rd);
        if (!rs_info.active)
            mem_manager.clear_var_reg(rs);
        if (!rt_info.active)
            mem_manager.clear_var_reg(rt);
    }
    else
    {
        string reg_rs = mem_manager.get_reg_for_src(now_fun, fun_outls, info_fun_blocks, cur_quaternary, cur_info_block, object_codes, rs);
        string reg_rt = mem_manager.get_reg_for_src(now_fun, fun_outls, info_fun_blocks, cur_quaternary, cur_info_block, object_codes, rt);
        string reg_rd = mem_manager.get_reg_for_des(now_fun, fun_outls, info_fun_blocks, cur_quaternary, cur_info_block, object_codes);
        if (op == "+")
        {
            object_codes.push_back(string("add ") + reg_rd + " " + reg_rs + " " + reg_rt);
        }
        else if (op == "-")
        {
            object_codes.push_back(string("sub ") + reg_rd + " " + reg_rs + " " + reg_rt);
        }
        else if (op == "*")
        {
            object_codes.push_back(string("mul ") + reg_rd + " " + reg_rs + " " + reg_rt);
        }
        else if (op == "/")
        {
            object_codes.push_back(string("div ") + reg_rs + " " + reg_rt);
            object_codes.push_back(string("mflo ") + reg_rd);
        }
        else
        {
            cout << "Invaild operation: " << op << "\n";
            exit(-1);
        }
        if (!rs_info.active)
            mem_manager.clear_var_reg(rs);
        if (!rt_info.active)
            mem_manager.clear_var_reg(rt);
    }
}
void ObjectCodeGenerator::check_valid(string &op, string &rs, string &rt)
{
    if (op[0] != 'j' && op != "call")
    {
        if (if_valid_var(rs) && mem_manager.A_Value[rs].empty())
        {
            cerr << "Var: " << rs << " 未初始化的变量"
                 << "\n";
            exit(-1);
        }
        if (if_valid_var(rt) && mem_manager.A_Value[rt].empty())
        {
            cerr << "Var: " << rs << " 未初始化的变量"
                 << "\n";
            exit(-1);
        }
    }
}