#include <iostream>
#include <iomanip>
#include "interactive_controller.h"
#include "clPrinter.h"

using namespace std;
using namespace cl;

InteractiveController::InteractiveController(){}
  //:m_currentStage(OperationControllerStage::WAITING){}

InteractiveController::~InteractiveController(){}

clbool InteractiveController::RequestAnalyze(){
  Text("Going to analysing config file items, do you agree? (y/n)",ConsoleForeground::WHITE,false);
  clchar input;
  do{
    input=cin.get();
  } while(input!='y'&&input!='Y'&&input!='n'&&input!='N');
  if(input=='y'||input=='Y'){
    //m_currentStage=OperationControllerStage::ANALYSING;
    return true;
  } else return false;
}
