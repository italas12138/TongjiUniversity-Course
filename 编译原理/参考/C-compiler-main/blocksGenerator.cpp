#include "blocksGenerator.h"
//=================================Info类===================================//
Info::Info(int nxt, bool active)
{
    this->nxt = nxt;
    this->active = active;
}
Info::Info(const Info &i)
{
    this->nxt = i.nxt;
    this->active = i.active;
}
Info::Info()
{
    // defalut: nxt = -1 active = false
    this->nxt = -1;
    this->active = false;
}
//=============================InfoQuaternary类=============================//
InfoQuaternary::InfoQuaternary(_Quaternary base_q, Info i1, Info i2, Info i3)
{
    this->base_q = base_q;
    this->info[0] = i1;
    this->info[1] = i2;
    this->info[2] = i3;
}
InfoQuaternary::InfoQuaternary(_Quaternary base_q, Info i[])
{
    this->base_q = base_q;
    this->info[0] = i[0];
    this->info[1] = i[1];
    this->info[2] = i[2];
}

InfoQuaternary::InfoQuaternary(_Quaternary base_q)
{
    this->base_q = base_q;
}
//=============================InfoBlock类=============================//
InfoBlock::InfoBlock()
{
    // pass
}
InfoBlock::InfoBlock(struct BaseBlock &base_block)
{
    this->block_name = base_block.block_name;
    this->nxtb[0] = base_block.nxtb[0];
    this->nxtb[1] = base_block.nxtb[1];
    for (auto &e : base_block.codes)
    {
        this->codes.push_back(e); // 这里调用了默认构造函数
    }
}

//=============================BlockGenerator类=============================//
BlocksGenerator::BlocksGenerator(vector<_Quaternary> &inter_codes, vector<pair<int, string>> &fun_enters)
{
    // 这里只需要读入中间代码和函数入口即可
    this->inter_codes = inter_codes;
    this->fun_enters = fun_enters;
}
void BlocksGenerator::get_base_fun_blocks()
{
    // 这里我先打印一下中间代码看是否正确

    // cout << "进入到基本块生成！"  << "\n";
    /*
for (auto &_e : inter_codes)
{
   cout << "( " << _e._opt << " , " << _e._rs << " , " << _e._rt << " , " << _e._rd << " )\n";
}*/

    // 打印一下函数入口
    /*
    for (auto &e : fun_enter)
    {
        cout << e.first << " " << e.second << "\n";
    }
    */
    // 下面开始基本块的划分，并且我要把内容存储到base_fun_blocks中

    /*
    关于基本块划分的思路：
    总体分为三步：
    一、找到每一块的入口
    二、每一块内的内容聚合
    三、建立块之间的执行联系
    */

    // 遍历函数块
    for (auto fun = fun_enters.begin(); fun != fun_enters.end(); fun++)
    {
        // 先把每个Block的起点找到

        /*
        应当是针对每个函数块进行遍历寻找符合课本中描述的三类：
        1. 函数块的第一句
        2. 控制语句 j< j> 等后面的部分 + 跳转到的部分
        3. 控制语句j 跳转到的部分
        */
        auto &fun_name = fun->second;  // 当前函数名称
        vector<BaseBlock> fun_blocks;  // 当前函数的所有块，这里我们需要一个个去添加
        BaseBlock base_block;          // 这里是单个的base_block
        auto cur_sta_idx = fun->first; // 当前函数块起点
        auto nxt_fun = fun + 1;
        set<int> block_enters;        // 块的入口
        map<int, string> block_names; // 对应位置的block的名称 key: 入口点 value: block 名称 用于修改跳转指令中去向的子模块名称
        map<int, int> block_idxs;     // 对应位置的block的下标 key: 入口点 value: block 编号 用于修改block的next
        auto cur_end_idx = nxt_fun == fun_enters.end() ? inter_codes.size() : (nxt_fun->first);
        bool first_flag = true; // 第一块用函数名

        // 第一步： 找到所有块的入口
        block_enters.insert(cur_sta_idx); // 函数块起始
        for (int idx = cur_sta_idx; idx != cur_end_idx; idx++)
        {
            // 这里主要就是判断是否为条件语句 或者无条件语句
            if (inter_codes[idx]._opt[0] == 'j')
            {
                if (inter_codes[idx]._opt == "j")
                {
                    // 无条件语句
                    block_enters.insert(atoi(inter_codes[idx]._rd.c_str())); // 将转移到的位置加入
                }
                else
                {
                    // 有条件语句
                    block_enters.insert(atoi(inter_codes[idx]._rd.c_str())); // 将转移到的位置加入
                    if (idx + 1 < cur_end_idx)
                    {
                        block_enters.insert(idx + 1); // 下一句
                    }
                }
            }
            else if (if_control(inter_codes[idx]._opt))
            {
                // 如果是retun 或者 call
                if (idx + 1 < cur_end_idx)
                {
                    block_enters.insert(idx + 1); // 下一句
                }
            }
        }
        // 第二步： 聚合每一块

        /*
            主体思路为取得两个块直接的内容，写入一个块内，并且完成block_names, block_idxs的映射关系
            当然这里需要注意如果只有一个块的情况，并且还要注意最后一个块的特殊处理
        */

        auto pre_block = block_enters.begin();
        auto cur_block = block_enters.begin();
        cur_block++;
        for (; cur_block != block_enters.end(); cur_block++)
        {
            // 完成上一块的处理
            // pre_block -> cur_block - 1
            if (first_flag)
            {
                base_block.block_name = fun_name;
                first_flag = false;
            }
            else
                base_block.block_name = get_block_name();        // 获取一个名称
            base_block.codes.clear();                            // 上一个的清空，下面开始填充自己的
            for (int idx = *pre_block; idx != *cur_block; idx++) // 压入代码
                base_block.codes.push_back(inter_codes[idx]);
            block_names[*pre_block] = base_block.block_name; // 当前块名称
            block_idxs[*pre_block] = fun_blocks.size();      // 当前块下标
            fun_blocks.push_back(base_block);                // 压入当前函数的blocks中
            pre_block++;
        }
        // 这里需要完成最后一块的处理
        // pre_block -> cur_end_idx - 1
        if (first_flag)
        {
            base_block.block_name = fun_name;
            first_flag = false;
        }
        else
            base_block.block_name = get_block_name();         // 获取一个名称
        base_block.codes.clear();                             // 上一个的清空，下面开始填充自己的
        for (int idx = *pre_block; idx != cur_end_idx; idx++) // 压入代码
            base_block.codes.push_back(inter_codes[idx]);
        block_names[*pre_block] = base_block.block_name; // 当前块名称
        block_idxs[*pre_block] = fun_blocks.size();      // 当前块下标
        fun_blocks.push_back(base_block);                // 压入当前函数的blocks中

        // 第三步： 建立块与块之间联系
        // 也就是针对每个块的最后一句代码 找到他的出口

        auto block_idx = 0; // 这里还要使用下标的原因在于 使用最后一句代码时无法根据他的入口找到块号
        for (auto block_iter = fun_blocks.begin(); block_iter != fun_blocks.end(); block_iter++, block_idx++)
        {
            auto last_code_in_block = block_iter->codes.rbegin();
            if (last_code_in_block->_opt[0] == 'j')
            {
                // 无条件指令：
                if (last_code_in_block->_opt == "j")
                {
                    block_iter->nxtb[0] = block_idxs[atoi(last_code_in_block->_rd.c_str())];
                    block_iter->nxtb[1] = -1;
                }
                // 条件指令
                else
                {
                    block_iter->nxtb[0] = block_idx + 1;
                    block_iter->nxtb[1] = block_idxs[atoi(last_code_in_block->_rd.c_str())];
                    if (block_iter->nxtb[0] == block_iter->nxtb[1])
                        block_iter->nxtb[1] = -1;
                }
                last_code_in_block->_rd = block_names[atoi(last_code_in_block->_rd.c_str())]; // 改动这里终点的名称
            }
            else if ((last_code_in_block->_opt) == "return")
            {
                // return call
                block_iter->nxtb[0] = -1;
                block_iter->nxtb[1] = -1;
            }
            else
            {
                // normal
                block_iter->nxtb[0] = block_idx + 1;
                block_iter->nxtb[1] = -1;
            }
        }

        // 该块完成，记录下来
        base_fun_blocks[fun_name] = fun_blocks; //
    }
}
void BlocksGenerator::get_info_fun_blocks()
{
    // 首先获得base_fun_blocs;
    get_base_fun_blocks();

    // 打印一下base_fun_blocks 的信息
    /*
    for (auto &e : base_fun_blocks)
    {
        // 打印函数名称
        cout << e.first << ":"
             << "\n";
        // 下面打印每个块
        for (auto &b : e.second)
        {
            // 打印块名称
            cout << b.block_name << ":"
                 << "\n";
            // 打印块中的代码
            for (auto &c : b.codes)
                cout << c._opt << " " << c._rs << " " << c._rt << " " << c._rd << "\n";
            // 打印 next信息
            cout << "next1 : " << b.nxtb[0] << "\n";
            cout << "next2 : " << b.nxtb[1] << "\n";
        }
    }*/
    // 检验发现基本块生成成功！
    // 下面开始分析和生成带信息的blocks
    // 外层循环，对于每个函数块而言

    for (auto fun_iter = base_fun_blocks.begin(); fun_iter != base_fun_blocks.end(); fun_iter++)
    {
        // 这里主要的工作为获得 fun_inl, fun_outl, info_fun_blocks
        // 在获取之后要记得存入
        auto &cur_fun_name = fun_iter->first;        // 获取当前函数名称，这是各变量存放的下标
        auto &cur_base_block = fun_iter->second;     // 当前函数块的基本块
        vector<InfoBlock> cur_info_blocks;           // 当前函数块的信息基本块
        vector<map<string, Info>> cur_symbol_tables; // 当前函数块的变量表们
        vector<set<string>> cur_inls;                // 当前函数块的入口活跃集
        vector<set<string>> cur_outls;               // 当前函数块的出口活跃集
        vector<set<string>> cur_defs;                // 当前函数块的定义变量集
        vector<set<string>> cur_uses;                // 当前函数块使用过的变量集(整个集合和上面集合是互斥的)

        // 一： 遍历每个基本块，找到其内的信息集合，并且要完成info_blocks/symbol_tables的初始化工作
        for (auto block_iter = cur_base_block.begin(); block_iter != cur_base_block.end(); block_iter++)
        {
            // 粒度为基本块，所以这里需要定义上述需要的变量
            map<string, Info> symbol_table;    // 这里默认都是-1,false, 后续通过出口活跃变量更改
            InfoBlock info_block(*block_iter); // 直接赋值即可
            set<string> inl, outl, def, use;
            // 遍历查找遍历
            auto &cur_codes = block_iter->codes;
            for (auto code_iter = cur_codes.begin(); code_iter != cur_codes.end(); code_iter++)
            {
                auto &cur_op = code_iter->_opt;
                auto &cur_rs = code_iter->_rs;
                auto &cur_rt = code_iter->_rt;
                auto &cur_rd = code_iter->_rd;
                if (if_no_var(cur_op))
                    continue; // 这里不存在变量
                if (if_valid_var(cur_rs) && def.count(cur_rs) == 0)
                    use.insert(cur_rs);
                if (if_valid_var(cur_rt) && def.count(cur_rt) == 0)
                    use.insert(cur_rt);
                if (cur_op[0] != 'j')
                {
                    if (if_valid_var(cur_rd) && use.count(cur_rd) == 0)
                        def.insert(cur_rd);
                }
            }
            cur_symbol_tables.push_back(symbol_table);
            cur_info_blocks.push_back(info_block);
            cur_inls.push_back(use);
            cur_outls.push_back(outl);
            cur_uses.push_back(use);
            cur_defs.push_back(def);
        }

        // 二： 根据初步找到的inl 和 outl信息 获得真正的inl集和 outl集
        /*
        - 出口活跃集：下一个集合内的所有入口变量
        - 入口活跃集：出口活跃集中有而自身定义变量集中不存在的所有变量
        - 注意：这里由于是动态变化的过程，所以需要迭代更新，直到没有变化结束，有点类似于求first集的过程
        */

        bool end_flag = 0;
        while (!end_flag)
        {
            end_flag = 1;
            // 遍历所有block
            auto block_idx = 0;
            for (auto block_iter = cur_base_block.begin(); block_iter != cur_base_block.end(); block_iter++, block_idx++)
            {
                auto nxtb_1 = block_iter->nxtb[0];
                auto nxtb_2 = block_iter->nxtb[1];
                if (nxtb_1 != -1)
                {
                    // 找到后续block的所有入口活跃集， 插入到自己的出口活跃集中，并且如果自己的def集中不存在，那么还要插入到自己的入口活跃集
                    for (auto post_outl_iter = cur_inls[nxtb_1].begin(); post_outl_iter != cur_inls[nxtb_1].end(); post_outl_iter++)
                    {
                        if (cur_outls[block_idx].insert(*post_outl_iter).second == true)
                        {
                            // 插入成功
                            end_flag = 0;
                            // 判断自己def中是否有
                            if (cur_defs[block_idx].count(*post_outl_iter) == 0)
                                cur_inls[block_idx].insert(*post_outl_iter);
                        }
                    }
                }
                if (nxtb_2 != -1)
                {
                    for (auto post_outl_iter = cur_inls[nxtb_2].begin(); post_outl_iter != cur_inls[nxtb_2].end(); post_outl_iter++)
                    {
                        if (cur_outls[block_idx].insert(*post_outl_iter).second == true)
                        {
                            // 插入成功
                            end_flag = 0;
                            // 判断自己def中是否有
                            if (cur_defs[block_idx].count(*post_outl_iter) == 0)
                                cur_inls[block_idx].insert(*post_outl_iter);
                        }
                    }
                }
            }
        }

        // 三： 对于每个基本块的出口活跃变量， 置符号表内的信息为 -1, true
        for (auto block_idx = 0; block_idx < cur_symbol_tables.size(); block_idx++)
        {
            for (auto out_var_iter = cur_outls[block_idx].begin(); out_var_iter != cur_outls[block_idx].end(); out_var_iter++)
            {
                cur_symbol_tables[block_idx][*out_var_iter] = Info(-1, true);
            }
        }

        // 四: 对于每个基本块，根据符号表的内容，倒推每个四元式的info
        auto block_idx = 0;
        for (auto info_block_iter = cur_info_blocks.begin(); info_block_iter != cur_info_blocks.end(); info_block_iter++, block_idx++)
        {
            auto &cur_codes = info_block_iter->codes;
            auto code_idx = cur_codes.size() - 1;
            // 这里要根据算法倒序推导
            for (auto code_iter = cur_codes.rbegin(); code_iter != cur_codes.rend(); code_iter++, code_idx--)
            {
                // 这里主要还是对三个变量进行判断，参考前面求infl,outl的逻辑
                auto &cur_op = (code_iter->base_q)._opt;
                auto &cur_rs = (code_iter->base_q)._rs;
                auto &cur_rt = (code_iter->base_q)._rt;
                auto &cur_rd = (code_iter->base_q)._rd;
                auto &cur_info_0 = code_iter->info[0];
                auto &cur_info_1 = code_iter->info[1];
                auto &cur_info_2 = code_iter->info[2];
                if (if_no_var(cur_op))
                    continue; // 这里不存在变量
                if (if_valid_var(cur_rs))
                {
                    // 取符号表内rs信息为当前info0
                    cur_info_0 = cur_symbol_tables[block_idx][cur_rs];
                    cur_symbol_tables[block_idx][cur_rs] = Info(code_idx, true); // 更新符号表
                }
                if (if_valid_var(cur_rt))
                {
                    // 取符号表内rt信息为当前info1
                    cur_info_1 = cur_symbol_tables[block_idx][cur_rt];
                    cur_symbol_tables[block_idx][cur_rt] = Info(code_idx, true); // 更新符号表
                }
                if (cur_op[0] != 'j')
                {
                    if (if_valid_var(cur_rd))
                    {
                        // 取符号表内rd信息为当前info2
                        cur_info_2 = cur_symbol_tables[block_idx][cur_rd];
                        cur_symbol_tables[block_idx][cur_rd] = Info(-1, false); // 更新符号表
                    }
                }
            }
        }
        // 记录下来要获得的信息
        info_fun_blocks[cur_fun_name] = cur_info_blocks;
        fun_inls[cur_fun_name] = cur_inls;
        fun_outls[cur_fun_name] = cur_outls;
    }

    // 这里完成之后，我们可以同上述一样 打印一下infoblock的内容
    /*
    for (auto &e : info_fun_blocks)
    {
        // 打印函数名称
        cout << e.first << ":"
             << "\n";
        // 下面打印每个块
        for (auto &b : e.second)
        {
            // 打印块名称
            cout << b.block_name << ":"
                 << "\n";
            // 打印块中的代码
            for (auto &c : b.codes)
            {
                cout << c.base_q._opt << " " << c.base_q._rs << " " << c.base_q._rt << " " << c.base_q._rd << "   ";
                for (int i = 0; i < 3; i++)
                {
                    if (c.info[i].nxt != -1)
                        cout << "(" << c.info[i].nxt << ",";
                    else
                        cout << "("
                             << "^,";
                    if (c.info[i].active == 1)
                        cout << "true)";
                    else
                        cout << "^)";
                }
                cout << '\n';
            }

            // 打印 next信息
            cout << "next1 : " << b.nxtb[0] << "\n";
            cout << "next2 : " << b.nxtb[1] << "\n";
        }
    }*/

    // 下面可以再检查一下inls 和outls的内容
    /*
    for (auto &e : fun_inls)
    {
        cout << "fun:" << e.first << "\n";
        for (auto &b : e.second)
        {
            cout << "======="
                 << "\n";
            for (auto &c : b)
                cout << c << " ";
            cout << "\n";
        }
    }*/
    /*
    for (auto &e : fun_outls)
    {
        cout << "fun:" << e.first << "\n";
        for (auto &b : e.second)
        {
            cout << "======="
                 << "\n";
            for (auto &c : b)
                cout << c << " ";
            cout << "\n";
        }
    }*/
    // 经过检查可以发现得到的结果是正确的， 那么到目前位置，我们就得到了带有出口、入口活跃信息的基本块集，接下来可以开始生成目标代码了！
}

string BlocksGenerator::get_block_name()
{
    return string("SubBlock") + to_string(block_cnt++);
}
