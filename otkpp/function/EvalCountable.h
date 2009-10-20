
#ifndef EVALCOUNTABLE_H

class EvalCountable
{
 public:
  EvalCountable();
  
  void disableEvalCounting();
  void enableEvalCounting();
  int getEvalCounter() const;
  void resetEvalCounter();
 protected:
  mutable unsigned int evalCounter_;
  bool evalCounting_;
};

#define EVALCOUNTABLE_H

#endif
