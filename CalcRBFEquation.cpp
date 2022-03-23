  #include "matlib.h"
  #pragma hdrstop
  
  #include "CalcRBFEquation.h"
  
  
  Mm CalcRBFEquation(Mm A, Mm b, Mm L) {
    begin_scope
    A.setname("A"); b.setname("b"); L.setname("L"); 
    dMm(value); dMm(e); 
    
    #line 1 "d:/matcom45/zjm/calcrbfequation.m"
    call_stack_begin;
    #line 1 "d:/matcom45/zjm/calcrbfequation.m"
    // nargin, nargout entry code
    double old_nargin=nargin_val; if (!nargin_set) nargin_val=3.0;
    nargin_set=0;
    double old_nargout=nargout_val; if (!nargout_set) nargout_val=1.0;
    nargout_set=0;
    
    // translated code
    
    #line 2 "d:/matcom45/zjm/calcrbfequation.m"
_   e = inv(A)*b;
    #line 3 "d:/matcom45/zjm/calcrbfequation.m"
_   value = L*e;
    
    call_stack_end;
    
    // nargin, nargout exit code
    nargin_val=old_nargin; nargout_val=old_nargout;
    
    // function exit code
    A.setname(NULL); b.setname(NULL); L.setname(NULL); 
    return value;
    end_scope
  }
  
  
