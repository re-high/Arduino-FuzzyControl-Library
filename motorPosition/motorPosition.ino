/*
  Arduino通过FuzzyControl库构造一个双输入单输出的模糊控制器，控制直流电机追踪到理想的位置（本程序中理想位置设置的为正弦信号）
  by 梁悦（https://re-high.github.io/）
 
  如需获得本示例程序详细电路信息以及如何使用FuzzyControl库的更多知识，请参考我的博客：
   https://re-high.github.io/
 
*/
#include <FuzzyControl.h>
//初始化一个双输入单输出的模糊控制器
//三个输入参数分别是输入量1、输入量2、输出量的模糊子集个数
FuzzyControl fuzzyController(3,3,5);
int fuzhi = 50;              //fuzhi：正弦波的幅值
#define ENCODER_A_PIN 2      //直流电机编码器的A相连接引脚
#define ENCODER_B_PIN 3      //直流电机编码器的B相连接引脚
#define motor1 8             //电机
#define motor2 9             //电机
#define ENA 5                //使能引脚
#define STBY 12
//定义模糊子集名称及其对应的索引
#define N 1
#define Z 2
#define P 3
#define NB 1
#define NS 2
#define ZZ 3
#define PS 4
#define PB 5
long position;               //编码器每获取一个脉冲，position+1
double position_actual;      //当前位置
double error_last = 0;              
long   preTime=0;


    
void setup() {
  // put your setup code here, to run once:
   Serial.begin(2000000);
   //setRange函数设置输入输出论域，输入范围为[-20，20]，输出范围为[-255，255]
   fuzzyController.setRange(-20,20,-20,20,-255,255);   
   /*addmf函数添加隶属度函数
    * 第一个参数表示变量类型（1表示input1，2表示input2，3表示input3）
    * 第二个参数表示模糊子集索引（模糊子集名称）
    * 后面的参数是三角型或梯形隶属度函数的参数    */
   fuzzyController.addmf(1,N,-20,-20,-5,0);
   fuzzyController.addmf(1,Z,-3,0,3);
   fuzzyController.addmf(1,P,0,5,20,20);
   fuzzyController.addmf(2,N,-20,-20,-5,0);
   fuzzyController.addmf(2,Z,-3,0,3);
   fuzzyController.addmf(2,P,0,5,20,20);
   fuzzyController.addmf(3,NB,-255,-180,-100);
   fuzzyController.addmf(3,NS,-200,-110,-20);
   fuzzyController.addmf(3,ZZ,-75,0,75);
   fuzzyController.addmf(3,PS,20,110,200);
   fuzzyController.addmf(3,PB,100,180,255);
   //setrulenum函数设置模糊控制规则总数
   fuzzyController.setrulenum(9); 
   //依次添加控制规则，第一次输入参数表示当前输入的是第几条规则
   //后面三个参数分别为input1、input2、output所属的模糊子集索引（模糊子集名称）
   fuzzyController.addrule(1,N,N,NB);  
   fuzzyController.addrule(2,N,Z,NS); 
   fuzzyController.addrule(3,N,P,ZZ); 
   fuzzyController.addrule(4,Z,N,NS); 
   fuzzyController.addrule(5,Z,Z,ZZ); 
   fuzzyController.addrule(6,Z,P,PS); 
   fuzzyController.addrule(7,P,N,ZZ); 
   fuzzyController.addrule(8,P,Z,PS);
   fuzzyController.addrule(9,P,P,PB); 
  pinMode(motor1, OUTPUT);                          //电机引脚设置
  pinMode(motor2, OUTPUT); 
  pinMode(ENA,OUTPUT);
  pinMode(STBY,OUTPUT);
  digitalWrite(STBY, HIGH);   
  pinMode(ENCODER_A_PIN, INPUT);
  pinMode(ENCODER_B_PIN, INPUT);
  attachInterrupt(0, read_quadrature, CHANGE);           //设置外部中断


     
}

void loop() {
unsigned long now = millis();   //获取当前时间
float position_ideal=fuzhi*sin(0.1*3.14*now/1000);  //产生正弦波
float TimeCh = (now-preTime)/1000.0;  
float SampleTime = 0.05;       //设置采样时间
if (TimeCh>=SampleTime)
{
 position_actual =(double) position/130;    //标准化当前位置，和编码器的线数有关，这里的position_actual单位是圈数
 double error = position_actual - position_ideal;        //计算当前时刻的追踪偏差
 double  DTerm = (error-error_last)/TimeCh;               //计算偏差变化率
 error = constrain(error,-20,20);  //避免输入量1超过设置的论域，对输入量1进行限幅
 DTerm = constrain(DTerm,-20,20);  //避免输入量2超过设置的论域，对输入量2进行限幅
int  u = (int)fuzzyController.caculate(error,DTerm);  // 计算模糊控制器的输出量
u = constrain(u,-255,255);  //对输出量限幅
dianji(u);    //把计算得到的控制量传给电机转动函数
error_last = error;   //把当前控制周期的追踪偏差赋值给error_last，给下个控制周期计算用
preTime = now;        //把当前控制周期的时刻值赋值给preTime，给下个控制周期计算用
serialprint(position_ideal,position_actual);  //串口监控函数
}
}
/**************************************************************************
函数功能：外部中断读取编码器数据
**************************************************************************/
void read_quadrature(){  
  // found a low-to-high on channel A ENA脚下降沿中断触发
  if (digitalRead(ENCODER_A_PIN) == LOW){   
    // check channel B to see which way 查询ENB的电平以确认是顺时针还是逆时针旋转
    if (digitalRead(ENCODER_B_PIN) == LOW)
      position++;
  }
  // found a high-to-low on channel A ENA脚上升沿中断触发
  else{
    // check channel B to see which way 查询ENB的电平以确认是顺时针还是逆时针旋转
    if (digitalRead(ENCODER_B_PIN) == LOW)
      position--;
  }
} 
/**************************************************************************
函数功能：电机转动
入口参数：控制量u
**************************************************************************/
void dianji(int u){
  if(u<0){
    digitalWrite(motor1, HIGH);  //设置电机转动方向
    digitalWrite(motor2, LOW);  
    analogWrite(ENA,abs(u));     //把PWM绝对值传入使能引脚，让电机转动
  }
  else{
     digitalWrite(motor1, LOW);  
     digitalWrite(motor2, HIGH);  
     analogWrite(ENA,abs(u));  
  }
}
/**************************************************************************
函数功能：串口输出当前时刻理想的位置信号和当前位置
**************************************************************************/
void serialprint(float position_ideal,double position_actual){
Serial.print(position_ideal); 
Serial.print(" "); 
Serial.println(position_actual);
}
