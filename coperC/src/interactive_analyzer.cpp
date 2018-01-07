#include "interactive_controller.h"
#include <iostream>
#include <iomanip>

using namespace std;
using namespace cl;

InteractiveController::InteractiveController(){}
  //:m_currentStage(OperationControllerStage::WAITING){}

InteractiveController::~InteractiveController(){}

clbool InteractiveController::RequestAnalyzing(){
  T("Going to analysing config file items, do you agree? (y/n)",0);
  clbool r;
  clchar input;
  do{
    input=cin.get();
  } while(input!='y'&&input!='Y'&&input!='n'&&input!='N');
  if(input=='y'||input=='Y'){
    //m_currentStage=OperationControllerStage::ANALYSING;
    r= true;
  } else r= false;
  rewind(stdin);
  return r;
}

clbool InteractiveController::RequestValidation(){
  T("Going to load items, and validate folder and|or file existance, do you agree? (y/n)",false);
  clbool r;
  clchar input;
  do{
    input=cin.get();
  } while(input!='y'&&input!='Y'&&input!='n'&&input!='N');
  if(input=='y'||input=='Y'){
    //m_currentStage=OperationControllerStage::ANALYSING;
    r= true;
  } else r= false;
  rewind(stdin);
  return r;
}
