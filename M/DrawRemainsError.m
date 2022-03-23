function DrawRemainsError(x)
hold on
grid on
hist(x);
xlabel('残差');
ylabel('个数');
title('残差直方图');