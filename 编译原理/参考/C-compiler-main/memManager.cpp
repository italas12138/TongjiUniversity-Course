#include "memManager.h"
MemManager::MemManager()
{
    R_Value.clear();
    A_Value.clear();
    var_mem_pos.clear();
    top_ptr = 0; // 0 合理吗
    free_regs.clear();
}
string MemManager::get_free_reg(string &now_fun, map<string, vector<set<string>>> &fun_outls, map<string, vector<InfoBlock>> &info_fun_blocks, vector<InfoQuaternary>::iterator &cur_quaternary, vector<InfoBlock>::iterator &cur_info_block, vector<string> &object_codes)
{
    // 首先寻找
    string free_reg; // 空闲寄存器
    if (free_regs.size())
    {
        free_reg = free_regs.back();
        free_regs.pop_back();
        return free_reg;
    }
    // 如果不存在空闲寄存器，那么我们就需要找到下个最远引用变量所在的寄存器
    int max_next_pos = 0;
    for (auto &reg : R_Value)
    {
        // 遍历所有寄存器
        int cur_next_pos = INF;
        auto &vars = reg.second;
        auto &cur_reg = reg.first;
        for (auto &var : vars)
        {
            bool has_another_pos = false;
            for (auto &pos : A_Value[var])
            {
                if (pos != cur_reg)
                {
                    has_another_pos = true;
                    break;
                }
            }
            if (!has_another_pos)
            {
                //  如果说没有第二个位置就需要找一下这个变量的下一次引用位置...
                for (vector<InfoQuaternary>::iterator code_iter = cur_quaternary; code_iter != cur_info_block->codes.end(); code_iter++)
                {
                    auto &rs = code_iter->base_q._rs;
                    auto &rt = code_iter->base_q._rt;
                    auto &rd = code_iter->base_q._rd;
                    if (rs == var || rt == var)
                    {
                        cur_next_pos = code_iter - cur_quaternary;
                        break;
                    }
                    else if (rd == var)
                    {
                        break;
                    }
                }
            }
        }
        if (cur_next_pos == INF)
        {
            // 说明当前寄存器可用
            free_reg = cur_reg;
            break;
        }
        else if (cur_next_pos > max_next_pos)
        {
            max_next_pos = cur_next_pos;
            free_reg = cur_reg;
        }
    }

    // 由于上面使用了一个保存了变量的寄存器，所以说这里需要把其中的数据保存
    for (auto &var : R_Value[free_reg])
    {
        A_Value[var].erase(free_reg);
        if (A_Value[var].size() == 0)
        {
            // 没有在其他地方存放
            bool need_store = false;
            auto code_iter = cur_quaternary;
            for (; code_iter != cur_info_block->codes.end(); code_iter++)
            {
                if (code_iter->base_q._rs == var || code_iter->base_q._rt == var)
                {
                    // 如果被引用
                    need_store = true;
                    break;
                }
                if (code_iter->base_q._rd == var)
                {
                    need_store = false;
                    break;
                }
            }
            if (code_iter == cur_info_block->codes.end())
            {
                // 没有被引用并且没有被定义
                // 那么需要看一下是不是出口活跃变量
                auto block_idx = cur_info_block - info_fun_blocks[now_fun].begin();
                if (fun_outls[now_fun][block_idx].count(var) == 1)
                {
                    need_store = true;
                }
            }
            if (need_store)
            {
                store_var(object_codes, free_reg, var); // 保存这个变量
            }
        }
    }

    // 清空这个寄存器
    R_Value[free_reg].clear();

    return free_reg;
}
string MemManager::get_reg_for_src(string &now_fun, map<string, vector<set<string>>> &fun_outls, map<string, vector<InfoBlock>> &info_fun_blocks, vector<InfoQuaternary>::iterator &cur_quaternary, vector<InfoBlock>::iterator &cur_info_block, vector<string> &object_codes, string src)
{
    string reg; // 返还的reg名称
    // 先查看一下src是否已经存在于寄存器内
    for (auto &pos : A_Value[src])
    {
        if (pos[0] == '$')
        {
            return pos; // 在寄存器内找到了
        }
    }
    // 否则需要分配一个寄存器
    reg = get_free_reg(now_fun, fun_outls, info_fun_blocks, cur_quaternary, cur_info_block, object_codes);
    // 根据是立即数还是变量使用不同的指令
    if (if_valid_num(src))
    {
        object_codes.push_back(string("addi ") + reg + " $zero " + src);
        return reg;
    }
    else
    {
        object_codes.push_back(string("lw ") + reg + " " + to_string(var_mem_pos[src]) + "($sp)");
        A_Value[src].insert(reg);
        R_Value[reg].insert(src);
    }
    return reg;

} // 这里需要传入变量是因为里面会有加载src到寄存器内的指令
string MemManager::get_reg_for_des(string &now_fun, map<string, vector<set<string>>> &fun_outls, map<string, vector<InfoBlock>> &info_fun_blocks, vector<InfoQuaternary>::iterator &cur_quaternary, vector<InfoBlock>::iterator &cur_info_block, vector<string> &object_codes)
{
    // 这里做一点小优化 考虑是否可以把rs寄存器利用起来
    auto &rs = cur_quaternary->base_q._rs;
    auto &rd = cur_quaternary->base_q._rd;
    if (!if_valid_num(rs))
    {
        // 检查rs存储的位置
        for (auto &pos : A_Value[rs])
        {
            if (pos[0] == '$')
            {
                if (R_Value[pos].size() == 1)
                {
                    if (rs == rd || !cur_quaternary->info[0].active)
                    {
                        // 选择使用这个寄存器
                        R_Value[pos].insert(rd);
                        A_Value[rd].insert(pos);
                        return pos;
                    }
                }
            }
        }
    }
    string reg = get_free_reg(now_fun, fun_outls, info_fun_blocks, cur_quaternary, cur_info_block, object_codes);
    R_Value[reg].insert(rd);
    A_Value[rd].insert(reg);
    return reg;
}
void MemManager::store_var(vector<string> &object_codes, string reg, string var)
{
    // 首先考虑该变量是否已经存储在内存中
    string code = "";
    code += "sw ";
    code += reg;
    code += " ";
    if (var_mem_pos.count(var) == 0)
    {
        // 在内存中没有已经分配的位置
        var_mem_pos[var] = top_ptr;
        top_ptr += 4; // 移动栈顶
    }
    code += to_string(var_mem_pos[var]);
    code += "($sp)";
    object_codes.push_back(code);
    A_Value[var].insert(var); //

} // 把寄存器内的数据存入到内存中

void MemManager::store_outl_var(vector<string> &object_codes, set<string> &outls)
{
    for (auto &outl : outls)
    {
        bool has_stored_in_mem = false;
        string reg;
        // 首先值得注意的是这里不会存在outl没有被存储的可能！
        for (auto &pos : A_Value[outl])
        {
            if (pos[0] != '$')
            {
                has_stored_in_mem = true;
                break;
            }
            else
            {
                reg = pos;
            }
        }
        if (!has_stored_in_mem)
        {
            store_var(object_codes, reg, outl);
        }
    }
}
void MemManager::clear_var_reg(string var)
{
    for (auto &pos : A_Value[var])
    {
        if (pos[0] == '$')
        {
            // 说明是寄存器
            R_Value[pos].erase(var);
            if (R_Value[pos].size() == 0)
            {
                // 寄存器空了
                free_regs.push_back(pos);
            }
        }
    }
    A_Value[var].clear(); // 该变量已经不存储在任何位置
}

void MemManager::reset_mem(set<string> vars)
{
    A_Value.clear();
    R_Value.clear();
    for (auto &var : vars)
    {
        A_Value[var].insert(var);
    }
    free_regs.clear();
    for (int i = 0; i <= 7; i++)
    {
        free_regs.push_back(string("$s") + to_string(i));
    }
    // top_ptr = 8;
}