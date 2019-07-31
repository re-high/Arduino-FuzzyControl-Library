# Arduino-FuzzyControl-Library
Using this library, you can easily construct a fuzzy controller with Arduino
##### Arduino的模糊控制库FuzzyControl

###### FuzzyControl库简介
FuzzyControl是我为了在Arduino中更方便地实现模糊控制而开发的Arduino库。不再需要用户自己编写模糊化、模糊推理、解模糊的过程，你只要添加变量的隶属度函数和控制规则就能直接计算出输出量。当你想实现电机转速控制、双轮自平衡车的角度控制时，都可以用FuzzyControl库轻松方便地构建一个双输入单输出模糊控制器。
下面，我将首先介绍FuzzyControl库函数，然后以Arduino Uno配合带编码器的直流电机，演示如何使用FuzzyControl库构造一个模糊控制器，实现**电机的位置追踪控制**。


###### FuzzyControl库函数
 **setRange**
 功能：设置输入输出变量的论域
 格式：setRange(input1rangeMin, input1rangeMax,input2rangeMin, input2rangeMax, outputrangeMin, outputrangeMax)
 说明：setRange函数有6个输入变量：
 * input1rangeMin：输入量1的论域最小值
 * input1rangeMax：输入量1的论域最大值
 * input2rangeMin：输入量2的论域最小值
 * input2rangeMax：输入量2的论域最大值
 * outputrangeMin：输出量的论域最小值
 * outputrangeMax：输出量的论域最大值

**addmf**
 功能：添加隶属度函数
 格式：addmf( varType, mfIndex, trimf_a, trimf_b, trimf_c);
         addmf( varType, mfIndex, trapmf_a, trapmf_b, trapmf_c, trapmf_d);
 说明：addmf函数有5或6个输入变量（根据隶属度函数是三角型还是梯形决定）：
 * varType：要添加隶属度函数的变量类型（1表示input1，2表示input2，3表示input3）
 * mfIndex：模糊子集索引（模糊子集名称）
 * trimf_a、trimf_b、trimf_c是三角型隶属度函数的参数
 * trapmf_a、trapmf_b、trapmf_c、trapmf_d是梯型隶属度函数的参数
 
 **setrulenum**
 功能：输入控制规则总数目
 格式：setrulenum(ruleNum);
 说明：setrulenum函数有1个输入变量：
 * ruleNum：控制规则总数目

 **addrule**
 功能：增加控制规则
 格式：addrule(ruleIndex, var1Index, var2Index, var3Index)
 说明：addrule函数有4个输入变量：
 * ruleIndex：要添加的控制规则是第几条规则
 * var1Index：这条控制规则中的input1所属的模糊子集索引
 * var2Index：这条控制规则中的input2所属的模糊子集索引
 * var3Index：这条控制规则中的output所属的模糊子集索引
 
 **caculate**
 功能：完成模糊推理计算
 格式：caculate( value1, value2);
 说明：caculate函数有2个输入变量：
 * value1：输入量1的数值
 * value2：输入量2的数值
