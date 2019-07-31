#include "FuzzyControl.h"
#include "Arduino.h"

Var::Var(int numSet){
    this->numSet = numSet;
    //Mf *mf[numSet];
}
void Var::rangeSet(int rangeMin,int rangeMax) {
    this->rangeMin = rangeMin;
    this->rangeMax = rangeMax ;
  }
  void Var::membershipFunction(int mfIndex,double trimf_a,double trimf_b,double trimf_c) {
    mf[mfIndex-1] =new Mf(trimf_a,trimf_b,trimf_c);
  }
  void Var::membershipFunction(int mfIndex,double trapmf_a,double trapmf_b,double trapmf_c,double trapmf_d) {
    mf[mfIndex-1] =new  Mf(trapmf_a,trapmf_b,trapmf_c,trapmf_d);
  }

Mf::Mf(double trimf_a,double trimf_b,double trimf_c){
  this->trimf_a=trimf_a;
  this->trimf_b=trimf_b;
  this->trimf_c=trimf_c;
  this->mfFlag ="trimf";
}
Mf::Mf(double trapmf_a,double trapmf_b,double trapmf_c,double trapmf_d){
  this->trapmf_a=trapmf_a;
  this->trapmf_b=trapmf_b;
  this->trapmf_c=trapmf_c;
  this->trapmf_d=trapmf_d;
  this->mfFlag = "trapmf";
}
double Mf::mfvalue(double input) {
   double res;
     if(mfFlag=="trimf"){
      if (input<=trimf_a) {
        return 0;
      }
      else if(trimf_a<input&&input<=trimf_b)
      { 
        res = (1/(trimf_b-trimf_a))*input-trimf_a/(trimf_b-trimf_a);
        return res;
      }
      else if(trimf_b<input&&input<trimf_c) {
        res = -(1/(trimf_c-trimf_b))*input+trimf_c/(trimf_c-trimf_b);
        return res;
      }
      else {
        return 0;
      }
     }
      else if(mfFlag=="trapmf"){
        if (input<trapmf_a) {
          return 0;
        }
        else if(trapmf_a!=trapmf_b&&trapmf_a<input&&input<=trapmf_b)
        { 
          res = (1/(trapmf_b-trapmf_a))*input-trapmf_a/(trapmf_b-trapmf_a);
          return res;
        }
        else if(trapmf_b<=input&&input<=trapmf_c) {
          return 1;
        }
        else if(trapmf_c==trapmf_d&&trapmf_c<input) {
          return 0;
        }
        else if(trapmf_c!=trapmf_d&&trapmf_c<input&&input<trapmf_d) {
          res = -(1/(trapmf_d-trapmf_c))*input+trapmf_d/(trapmf_d-trapmf_c);
          return res;
        }else
          return 0 ;
      }  
   else{
      return 0;
    
    }
  }
  //返回重心模糊集的重心
  //对于对称的三角隶属函数来说，就是trimf_b
  double Mf::getcenter() {
    return trimf_b;
  }

Rule::Rule(int var1Index,int var2Index,int var3Index){
    this->input1Index = var1Index-1;
    this->input2Index = var2Index-1;
    this->outputIndex = var3Index-1;
}
FuzzyControl::FuzzyControl(int numSet1,int numSet2,int numSet3) {
    this->numSet1=numSet1;
    this->numSet2=numSet2;
    this->numSet3=numSet3;
    input1 = new Var(this->numSet1);
    input2 = new Var(this->numSet2);
    output = new Var(this->numSet3);    
  }
 void FuzzyControl::setRange(int input1rangeMin,int input1rangeMax,int input2rangeMin,int input2rangeMax,int outputrangeMin,int outputrangeMax) {
    this->input1->rangeSet(input1rangeMin,input1rangeMax);
    this->input2->rangeSet(input2rangeMin,input2rangeMax);
    this->output->rangeSet(outputrangeMin,outputrangeMax);
  }
void FuzzyControl::addmf(int  varType,int mfIndex,double trimf_a,double trimf_b,double trimf_c) {
    if(varType==1) {
      input1->membershipFunction(mfIndex,trimf_a,trimf_b,trimf_c);
    }
    if(varType==2) {
      input2->membershipFunction(mfIndex,trimf_a,trimf_b,trimf_c);
    }
    if(varType==3) {
      output->membershipFunction(mfIndex,trimf_a,trimf_b,trimf_c);
    }
  }
 void FuzzyControl::addmf(int  varType,int mfIndex,double trapmf_a,double trapmf_b,double trapmf_c,double trapmf_d) {
    if(varType==1) {
      input1->membershipFunction(mfIndex,trapmf_a,trapmf_b,trapmf_c,trapmf_d);
    }
    if(varType==2) {
      input2->membershipFunction(mfIndex,trapmf_a,trapmf_b,trapmf_c,trapmf_d);
    }
    if(varType==3) {
      output->membershipFunction(mfIndex,trapmf_a,trapmf_b,trapmf_c,trapmf_d);
    }
  }
void FuzzyControl::setrulenum(int num) {
    this->rulenum = num;
    //this->rule = new Rule[rulenum]; 
  }
  //计算输出数值
  //传入数值：input1和input2
 double FuzzyControl::caculate(double value1,double value2) {
    this->degreeFiringRule(value1,value2);
    this->returnCenter();
    this->getTrapezoid();
    return this->defuzzification();
    
  }

  void FuzzyControl::addrule(int ruleIndex,int var1Index,int var2Index,int var3Index) {
    rule[ruleIndex-1] = new Rule(var1Index,var2Index,var3Index);
  }
  //计算每条规则的激活度
  void FuzzyControl::degreeFiringRule(double x1,double x2){
    for(int i = 0;i<rulenum;i++) {
      double tempinput1 = input1->mf[rule[i]->input1Index]->mfvalue(x1);
      double tempinput2 = input2->mf[rule[i]->input2Index]->mfvalue(x2);
      if(tempinput1<tempinput2) {
        rule[i]->degree = tempinput1;
      }
      else {
        rule[i]->degree = tempinput2;
      }
      if(rule[i]->degree!=0) {
        rule[i]->flag=true;
      }
      else {
        rule[i]->flag = false;
      }
    }
  }
  //计算每条规则对应的模糊输出集的重心
  void FuzzyControl::returnCenter() {
    for(int i = 0;i<rulenum;i++) {
    rule[i]->center = output->mf[rule[i]->outputIndex]->getcenter();
    }
  }
  //计算梯形面积
  void FuzzyControl::getTrapezoid() {
    for(int i = 0;i<rulenum;i++) {
      if(rule[i]->flag==true) {
    rule[i]->trapezoid =  (output->mf[rule[i]->outputIndex]->trimf_c-output->mf[rule[i]->outputIndex]->trimf_a)*(rule[i]->degree-(rule[i]->degree*rule[i]->degree)/2);
    }
      else {
        rule[i]->trapezoid = 0;
      }
    }
  }
  double FuzzyControl::defuzzification() {
    double sum1=0;  //分子
    double sum2=0;  //分母
    for(int i = 0;i<rulenum;i++) {
      if(rule[i]->flag==true) {
        sum1 =sum1+ (rule[i]->center)*(rule[i]->trapezoid);
        sum2 = sum2 + rule[i]->trapezoid;
      }
    }
    return (sum1/sum2);
  }
  
