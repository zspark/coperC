#pragma once
#include "coper.h"

/*
namespace OperationControllerStage{
enum stage{
  WAITING=0,
  PRE_PARSING_CONFIG_FILE,
  ANALYSING,
  FILE_LOADING,
  HANDLING,
};
}
*/

class InteractiveController final{
public:
  InteractiveController();
  ~InteractiveController();

  clbool RequestAnalyzing();
  clbool RequestValidation();

private:
  //OperationControllerStage::stage m_currentStage;

};