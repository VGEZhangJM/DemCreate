  #include "matlib.h"
  #pragma hdrstop
  
  #include "DrawSemiVariogram.h"
  
  
  Mm DrawSemiVariogram(Mm x, Mm y) {
    begin_scope
    x.setname("x"); y.setname("y"); 
    #line 1 "f:/01-软件研制/02-mapviewer(10.0版)(vge)/402-vr_demcreator(m)/m/drawsemivariogram.m"
    call_stack_begin;
    #line 1 "f:/01-软件研制/02-mapviewer(10.0版)(vge)/402-vr_demcreator(m)/m/drawsemivariogram.m"
    // nargin, nargout entry code
    double old_nargin=nargin_val; if (!nargin_set) nargin_val=2.0;
    nargin_set=0;
    double old_nargout=nargout_val; if (!nargout_set) nargout_val=0.0;
    nargout_set=0;
    
    // translated code
    
    #line 2 "f:/01-软件研制/02-mapviewer(10.0版)(vge)/402-vr_demcreator(m)/m/drawsemivariogram.m"
_   hold(TM("on"));
    #line 3 "f:/01-软件研制/02-mapviewer(10.0版)(vge)/402-vr_demcreator(m)/m/drawsemivariogram.m"
_   grid(TM("on"));
    #line 4 "f:/01-软件研制/02-mapviewer(10.0版)(vge)/402-vr_demcreator(m)/m/drawsemivariogram.m"
_   plot((CL(x),y,TM("o-")));
    #line 5 "f:/01-软件研制/02-mapviewer(10.0版)(vge)/402-vr_demcreator(m)/m/drawsemivariogram.m"
_   //axis((BR(0.0),30000.0,0.0,40000.0));
    #line 6 "f:/01-软件研制/02-mapviewer(10.0版)(vge)/402-vr_demcreator(m)/m/drawsemivariogram.m"
_   xlabel((CL(TM("延迟距离"))));
    #line 7 "f:/01-软件研制/02-mapviewer(10.0版)(vge)/402-vr_demcreator(m)/m/drawsemivariogram.m"
_   ylabel((CL(TM("变异值"))));
    #line 8 "f:/01-软件研制/02-mapviewer(10.0版)(vge)/402-vr_demcreator(m)/m/drawsemivariogram.m"
_   title((CL(TM("地形变异函数图"))));
    
    call_stack_end;
    
    // nargin, nargout exit code
    nargin_val=old_nargin; nargout_val=old_nargout;
    
    // function exit code
    x.setname(NULL); y.setname(NULL); 
    
    return x_M;
    end_scope
  }
  
  
