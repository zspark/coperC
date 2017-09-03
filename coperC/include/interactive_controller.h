#pragma once
#include "cllib.h"

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

  clbool RequestAnalyze();
  clbool RequestLoadWithoutErrorItems();

private:
  //OperationControllerStage::stage m_currentStage;

};