a = dlmread("output");
hold on
#plot(a(:,1), 'g-')
#plot(a(:,2), 'y-')
#plot(a(:,3), 'r-')
#plot(a(:,4), 'b-')
figure(1)
plot(a(:,5), 'r-')
figure(2)
plot(cumsum(a(:,5)), 'k-')

