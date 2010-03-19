% Converts Euler angles to DCM, Varshalovich B convention.
%
% ilya.kuprov@oerc.ox.ac.uk

function R=euler2dcm(angles)

alpha=angles(1); beta=angles(2); gamma=angles(3);

R_alpha=[cos(alpha) -sin(alpha) 0;
         sin(alpha)  cos(alpha) 0;
         0           0          1];

R_beta= [1           0          0        ;
         0           cos(beta) -sin(beta);
         0           sin(beta)  cos(beta)];
     
R_gamma=[cos(gamma) -sin(gamma) 0;
         sin(gamma)  cos(gamma) 0;
         0           0          1];
     
R=R_alpha*R_beta*R_gamma;

end
