function [ output_args ] = libDrawHistOfResidual( x )
%UNTITLED2 Summary of this function goes here
%   Detailed explanation goes here
hold on;

grid on;

hist(x);

xlabel('残差');

ylabel('个数');

title('残差直方图');

end

