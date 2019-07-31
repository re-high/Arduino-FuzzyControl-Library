#include "WString.h"

class Mf{
private:
     
  public:
       double trimf_a;
       double trimf_b;
       double trimf_c;
       double trapmf_a;
       double trapmf_b;
       double trapmf_c;
       double trapmf_d;
       String  mfFlag;
       Mf(double trimf_a,double trimf_b,double trimf_c);
       Mf(double trapmf_a,double trapmf_b,double trapmf_c,double trapmf_d);
       double mfvalue(double input);
       double getcenter();
};
class Var{
  private:
    int numSet;
    int rangeMin;
    int rangeMax;
   
   
    
  public:
   Mf *mf[20];
   Var(int numSet);
   void rangeSet(int rangeMin,int rangeMax);
   void membershipFunction(int mfIndex,double trimf_a,double trimf_b,double trimf_c);
   void membershipFunction(int mfIndex,double trapmf_a,double trapmf_b,double trapmf_c,double trapmf_d);
};

class Rule {
  private:
 
  
  public:
  bool flag = false;
  double degree;   //激活的隶属度（min法）
  double center;  //规则对应的输出模糊集的重心
  double trapezoid; //对应的模糊子集的梯形面积
  int input1Index;
  int input2Index;
  int outputIndex;
  Rule(int var1Index,int var2Index,int var3Index);
  };

class FuzzyControl {
private:
  int numSet1;  //输入1的子集个数
  int numSet2;
  int numSet3;  //输出的子集个数
  Var *input1 ;
  Var *input2 ;
  Var *output ;
  Rule *rule[100];
  
  public:
  int rulenum;
  FuzzyControl(int numSet1,int numSet2,int numSet3);
  void setRange(int input1rangeMin,int input1rangeMax,int input2rangeMin,int input2rangeMax,int outputrangeMin,int outputrangeMax);
  void addmf(int  varType,int mfIndex,double trimf_a,double trimf_b,double trimf_c);
  void addmf(int  varType,int mfIndex,double trapmf_a,double trapmf_b,double trapmf_c,double trapmf_d);
  void setrulenum(int num);
  double caculate(double value1,double value2);
  void addrule(int ruleIndex,int var1Index,int var2Index,int var3Index); 
  void degreeFiringRule(double x1,double x2);
  void returnCenter();
  void getTrapezoid();
  double defuzzification();
};



