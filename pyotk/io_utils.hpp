
#ifndef IO_UTILS_HPP

#include <otkpp/localsolvers/native/NativeSolver.h>

#include <boost/numeric/ublas/vector.hpp>
#include <iomanip>
#include <iostream>
#include <vector>

using namespace boost::numeric::ublas;

std::ostream &operator<<(std::ostream &os, const vector< double > &v)
{
  std::cout<<"(";
  for(int i = 0; i < v.size() - 1; i++)
    std::cout<<v[i]<<",";
  std::cout<<v[v.size() - 1];
  std::cout<<")";
  
  return os;
}

void printResultsTableHeader(const NativeSolver &s, const Function &f)
{
  int i;
  std::list< std::string > varNames;
  
  std::cout<<std::setw(6)<<std::left<<"k";
  
  f.getVariableNames(varNames);
  std::list< std::string >::iterator vn;
  vn = varNames.begin();
  for(i = 0; i < f.getN(); i++)
  {
    std::cout<<std::setw(20)<<std::left<<*vn;
    vn++;
  }
  
  std::cout<<"f(";
  vn = varNames.begin();
  for(i = 0; i < f.getN() - 1; i++)
  {
    std::cout<<*vn<<",";
    vn++;
  }
  std::cout<<*vn;
  std::cout<<")"<<std::endl;
}

void printResultsTableRow(int k, const NativeSolver &s, const Function &f)
{
  std::cout<<std::setw(6)<<std::left<<k;
  for(int i = 0; i < f.getN(); i++)
    std::cout<<std::setw(20)<<std::left<<s.getX()(i);
  
  std::cout<<std::setw(8)<<std::left<<s.getFVal()<<std::endl;
}

void printSummary(int k, const NativeSolver &s, const Function &f)
{
  std::cout<<std::endl;
  std::cout<<std::left<<std::setw(35)<<"Minimizer:"<<s.getX()<<std::endl;
  std::cout<<std::setw(35)<<"Minimum value:"<<s.getFVal()<<std::endl;
  if(s.usesGradient())
    std::cout<<std::setw(35)<<"Minimum gradient:"<<s.getGradient()<<std::endl;
  std::cout<<std::setw(35)<<"iterations:"<<k<<std::endl;
  std::cout<<std::setw(35)<<"function evaluations:"<<f.getFuncEvalCounter()<<std::endl;
  if(s.usesGradient())
    std::cout<<std::setw(35)<<"gradient evaluations:"<<f.getGradEvalCounter()<<
      " ("<<f.getN() * f.getGradEvalCounter()<<")"<<std::endl;
  /*if(s.getType() == GSLMinimizerWrapper::FD2F)
    std::cout<<std::setw(35)<<"Hessian evaluations:"<<f.getHessEvalCounter()<<
      " ("<<f.n() * (f.n() + 1) / 2.0 * f.getHessEvalCounter()<<")"<<std::endl;*/
  std::cout<<std::endl;
}

#define IO_UTILS_HPP

#endif
