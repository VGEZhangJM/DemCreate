function [ value ] = libLeastSquare( A,Z )
%UNTITLED3 Summary of this function goes here
%   Detailed explanation goes here

AT=A';

value = inv(AT*A)*AT*Z;

end

