function DrawSemiVariogram(x,y)
hold on
grid on
plot(x,y,'o-');
//axis([0,30000,0,40000]);
xlabel('延迟距离');
ylabel('变异值');
title('地形变异函数图');
