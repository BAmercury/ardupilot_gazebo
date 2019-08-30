m = 50;
k = 1.0;
A = [0 1; 0 -k/m];
B = [0; 1/m]
C = [1 0]
D = 0
sys = ss(A, B, C, D);

% Find gains to place poles
K = place(A, B, [-9 -5])
% Calculate Feedforward
s = size(A, 1);
Z = [zeros([1,s]) 1];
N = inv([A B; C D])*Z'
Nx = N(1:s)
Nu = N(1+s)
Nbar = Nu + K*Nx


