
#include <otkpp/stopcrit/CompoundStoppingCriterion.h>
#include <otkpp/stopcrit/FDistToMinTest.h>
#include <otkpp/stopcrit/GradNormTest.h>
#include <otkpp/stopcrit/MaxNumIterTest.h>
#include <otkpp/stopcrit/StoppingCriterion.h>
#include <otkpp/stopcrit/XDistToMinTest.h>

#include <boost/python.hpp>

void init_stop_crit()
{
  class_< StoppingCriterion, boost::noncopyable >("StoppingCriterion", no_init)
    .def(self + self);
  class_< CompoundStoppingCriterion, bases< StoppingCriterion > >("CompoundStoppingCriterion")
    .def(self + self);
  class_< FDistToMinTest, bases< StoppingCriterion > >("FDistToMinTest", 
    init< double, double, bool >((arg("f_min"), arg("eps"), arg("relative") = false)));
  class_< GradNormTest, bases< StoppingCriterion > >("GradNormTest", init< double >(arg("eps")));
  class_< MaxNumIterTest, bases< StoppingCriterion > >("MaxNumIterTest", init< int >(arg("n")));
  class_< XDistToMinTest, bases< StoppingCriterion > >("XDistToMinTest", 
    init< const vector< double > &, double, bool >((arg("x_min"), arg("eps"), arg("relative") = false)));
}
