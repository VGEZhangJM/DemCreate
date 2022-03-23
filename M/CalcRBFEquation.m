function value=CalcRBFEquation(A,b,L)
e = inv(A)*b;
value = L*e;