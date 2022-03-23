  #include "matlib.h"
  #pragma hdrstop
  
  #include "CalcKRGEquation.h"
  
  
  Mm CalcKRGEquation(Mm A, Mm b) {
    begin_scope
    A.setname("A"); b.setname("b"); 
    dMm(value); 
    
    #line 1 "d:/matcom45/zjm/calckrgequation.m"
    call_stack_begin;
    #line 1 "d:/matcom45/zjm/calckrgequation.m"
    // nargin, nargout entry code
    double old_nargin=nargin_val; if (!nargin_set) nargin_val=2.0;
    nargin_set=0;
    double old_nargout=nargout_val; if (!nargout_set) nargout_val=1.0;
    nargout_set=0;
    
    // translated code
    
    #line 2 "d:/matcom45/zjm/calckrgequation.m"
_   value = inv(A)*b;
    
    call_stack_end;
    
    // nargin, nargout exit code
    nargin_val=old_nargin; nargout_val=old_nargout;
    
    // function exit code
    A.setname(NULL); b.setname(NULL); 
    return value;
    end_scope
  }
  
  
