  #include "matlib.h"
  #pragma hdrstop
  
  #include "CalcOLSquation.h"
  
  
  Mm CalcOLSquation(Mm A, Mm Z) {
    begin_scope
    A.setname("A"); Z.setname("Z"); 
    dMm(value); 
    
    #line 1 "c:/matcom45/calcolsquation.m"
    call_stack_begin;
    #line 1 "c:/matcom45/calcolsquation.m"
    // nargin, nargout entry code
    double old_nargin=nargin_val; if (!nargin_set) nargin_val=2.0;
    nargin_set=0;
    double old_nargout=nargout_val; if (!nargout_set) nargout_val=1.0;
    nargout_set=0;
    
    // translated code
    
    #line 2 "c:/matcom45/calcolsquation.m"
_   value = inv(ctranspose(A)*A)*ctranspose(A)*Z;
    
    call_stack_end;
    
    // nargin, nargout exit code
    nargin_val=old_nargin; nargout_val=old_nargout;
    
    // function exit code
    A.setname(NULL); Z.setname(NULL); 
    return value;
    end_scope
  }
  
  
