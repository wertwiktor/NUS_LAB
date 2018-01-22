d=load('/project/robot_state.txt');

n = size(d,1);
t = d(:,1)-d(1,1);
x = d(:,2);
y = d(:,3);
th = d(:,4);
wR = d(:,5);
wL = d(:,6);
uR = d(:,7);
uL = d(:,8);
