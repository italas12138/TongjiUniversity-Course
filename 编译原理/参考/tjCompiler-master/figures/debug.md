![wrongFuncBlock](D:\totalStudy\sixth\Compiler\yuzhuoCompiler\figures\wrongFuncBlock.png)

问题：函数只有一个基本块的情况没有考虑，

![image-20230516133717988](C:\Users\youmeng\AppData\Roaming\Typora\typora-user-images\image-20230516133717988.png)

![image-20230516094724573](C:\Users\youmeng\AppData\Roaming\Typora\typora-user-images\image-20230516094724573.png)

![image-20230516094606597](C:\Users\youmeng\AppData\Roaming\Typora\typora-user-images\image-20230516094606597.png)

---

活跃信息获取出错：

![image-20230516164600833](C:\Users\youmeng\AppData\Roaming\Typora\typora-user-images\image-20230516164600833.png)



函数名也加在里面了：

![image-20230516164855118](C:\Users\youmeng\AppData\Roaming\Typora\typora-user-images\image-20230516164855118.png)

---

return 了还活跃？

![image-20230516171429631](C:\Users\youmeng\AppData\Roaming\Typora\typora-user-images\image-20230516171429631.png)

![image-20230516190040385](C:\Users\youmeng\AppData\Roaming\Typora\typora-user-images\image-20230516190040385.png)

map值未初始化！！

---

诡异字符：

![image-20230517204015489](C:\Users\youmeng\AppData\Roaming\Typora\typora-user-images\image-20230517204015489.png)

![image-20230517205923822](C:\Users\youmeng\AppData\Roaming\Typora\typora-user-images\image-20230517205923822.png)

string + int 没有对int进行强转

![image-20230517211202552](C:\Users\youmeng\AppData\Roaming\Typora\typora-user-images\image-20230517211202552.png)

a拿的还是原来上一块分配的a

![image-20230517211655575](C:\Users\youmeng\AppData\Roaming\Typora\typora-user-images\image-20230517211655575.png)

a未初始化



lw、store出了问题

![image-20230518080351085](C:\Users\youmeng\AppData\Roaming\Typora\typora-user-images\image-20230518080351085.png)



---

![image-20230518082844548](C:\Users\youmeng\AppData\Roaming\Typora\typora-user-images\image-20230518082844548.png)

![image-20230518084208068](C:\Users\youmeng\AppData\Roaming\Typora\typora-user-images\image-20230518084208068.png)

寄存器分配不恰当





![image-20230518090357606](C:\Users\youmeng\AppData\Roaming\Typora\typora-user-images\image-20230518090357606.png)

待用信息出错

---

![image-20230518135949493](C:\Users\youmeng\AppData\Roaming\Typora\typora-user-images\image-20230518135949493.png)

![image-20230518140039166](C:\Users\youmeng\AppData\Roaming\Typora\typora-user-images\image-20230518140039166.png)

只存了一个j，i没有存，下次循环马上又要取