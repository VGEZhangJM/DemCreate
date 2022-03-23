  #include "matlib.h"
  #pragma hdrstop
  
  #include "DrawHistogram.h"
  
  
  Mm DrawHistogram(Mm x) {
    begin_scope
    x.setname("x"); 
    #line 1 "d:/matcom45/drawhistogram.m"
    call_stack_begin;
    #line 1 "d:/matcom45/drawhistogram.m"
    // nargin, nargout entry code
    double old_nargin=nargin_val; if (!nargin_set) nargin_val=1.0;
    nargin_set=0;
    double old_nargout=nargout_val; if (!nargout_set) nargout_val=0.0;
    nargout_set=0;
    
    // translated code
    
    #line 2 "d:/matcom45/drawhistogram.m"
_   hold(TM("on"));
    #line 3 "d:/matcom45/drawhistogram.m"
_   grid(TM("on"));
    #line 4 "d:/matcom45/drawhistogram.m"
_   hist(x);
    #line 5 "d:/matcom45/drawhistogram.m"
_   xlabel((CL(TM("坡度\\曲率\\..."))));
    #line 6 "d:/matcom45/drawhistogram.m"
_   ylabel((CL(TM("个数"))));
    #line 7 "d:/matcom45/drawhistogram.m"
_   title((CL(TM("直方图"))));
    
    call_stack_end;
    
    // nargin, nargout exit code
    nargin_val=old_nargin; nargout_val=old_nargout;
    
    // function exit code
    x.setname(NULL); 
    
    return x_M;
    end_scope
  }
  
  
