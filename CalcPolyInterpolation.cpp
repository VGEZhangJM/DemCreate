  #include "matlib.h"
  #pragma hdrstop
  
  #include "CalcPolyInterpolation.h"
  
  
  Mm CalcPolyInterpolation(Mm X, Mm Y, Mm Z, Mm dx, Mm dy, Mm iModeType) {
    begin_scope
    X.setname("X"); Y.setname("Y"); Z.setname("Z"); dx.setname("dx"); dy.setname("dy"); iModeType.setname("iModeType") \
      ; 
    dMm(value); dMm(a); 
    
    #line 1 "d:/matcom45/zjm/calcpolyinterpolation.m"
    call_stack_begin;
    #line 1 "d:/matcom45/zjm/calcpolyinterpolation.m"
    // nargin, nargout entry code
    double old_nargin=nargin_val; if (!nargin_set) nargin_val=6.0;
    nargin_set=0;
    double old_nargout=nargout_val; if (!nargout_set) nargout_val=1.0;
    nargout_set=0;
    
    // translated code
    
    #line 2 "d:/matcom45/zjm/calcpolyinterpolation.m"
_   a = iModeType;
    
    #line 4 "d:/matcom45/zjm/calcpolyinterpolation.m"
_   mswitch (a) {
      beginmswitch {
        #line 5 "d:/matcom45/zjm/calcpolyinterpolation.m"
_       break;
      }
      mcase (0.0) {
        #line 6 "d:/matcom45/zjm/calcpolyinterpolation.m"
_       value = griddata(X,Y,Z,dx,dy,TM("linear"));
        #line 7 "d:/matcom45/zjm/calcpolyinterpolation.m"
_       break;
      }
      mcase (1.0) {
        #line 8 "d:/matcom45/zjm/calcpolyinterpolation.m"
_       value = griddata(X,Y,Z,dx,dy,TM("nearest"));
        #line 9 "d:/matcom45/zjm/calcpolyinterpolation.m"
_       break;
      }
      mcase (2.0) {
        #line 10 "d:/matcom45/zjm/calcpolyinterpolation.m"
_       value = griddata(X,Y,Z,dx,dy,TM("cubic"));
        #line 11 "d:/matcom45/zjm/calcpolyinterpolation.m"
_       break;
      }
      otherwise {
        #line 12 "d:/matcom45/zjm/calcpolyinterpolation.m"
_       value = griddata(X,Y,Z,dx,dy,TM("v4"));
        #line 13 "d:/matcom45/zjm/calcpolyinterpolation.m"
      }
    } endmswitch;
    call_stack_end;
    
    // nargin, nargout exit code
    nargin_val=old_nargin; nargout_val=old_nargout;
    
    // function exit code
    X.setname(NULL); Y.setname(NULL); Z.setname(NULL); dx.setname(NULL); dy.setname(NULL); iModeType.setname(NULL) \
      ; 
    return value;
    end_scope
  }
  
  
