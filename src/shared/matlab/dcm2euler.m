
% Converts DCM into Euler angles (Varshalovich B convention).
%
% I can't believe I'm writing this, but this does appear (after very considerable
% literature work and effort) to be the most reliable way of clawing Euler angles
% back from the DCM. Please avoid converting into Euler angles or using them as
% intermediate parameters at all costs (read Siminovitch on why). This function
% is only to be used in desperate cases, such as external software being unable
% to accept anything else as orientation parameters.
%
% ilya.kuprov@oerc.ox.ac.uk

function [alpha,beta,gamma]=dcm2euler(dcm)

%% Make it dead certain that the DCM is valid

% Make sure the DCM is unitary
if norm(dcm'*dcm-eye(3))>1e-6
    error('dcm2euler: the DCM is not orthogonal.');
end

% Make sure the DCM has a unit determinant
if abs(det(dcm)-1)>1e-6
    error('dcm2euler: the DCM is not a proper rotation.');
end

% Warn the user if we end up close to the singularity
if (1-abs(dcm(3,3)))<1e-3
    disp('dcm2euler: WARNING - small beta angle, alpha and gamma are degenerate');
end

%% Get the Euler angles out 

% Pull the beta angle out and project it into [0,pi]
beta=acos(dcm(3,3));
if beta>pi
    beta=beta-pi;
elseif beta<0
    beta=beta+pi;
end

% Do a stupid surface scan with respect to alpha and gamma
alphas=pi*linspace(-0.95,0.95,20);
gammas=pi*linspace(-0.95,0.95,20);
n_min=1; k_min=1; err_min=1;
for n=1:20
    for k=1:20
        err_current=norm(euler2dcm([alphas(n),beta,gammas(k)])-dcm);
        if err_current<err_min;
            n_min=n; k_min=k; err_min=err_current;
        end
    end
end
alpha=alphas(n_min); gamma=gammas(k_min);

% Run the optimization on alpha and gamma
options=optimset('Display','iter','LargeScale','off','TolX',1e-12,'TolFun',1e-12);
answer=fminunc(@(angles)norm(euler2dcm(angles(1),beta,angles(2))-dcm),[alpha gamma],options);
alpha=answer(1); gamma=answer(2);

% Project all angles into their proper ranges
if alpha>pi
    alpha=alpha-2*pi;
elseif alpha<-pi
    alpha=alpha+2*pi;
end
if gamma>pi
    gamma=gamma-2*pi;
elseif gamma<-pi
    gamma=gamma+2*pi;
end

%% Make sure the answer is valid 

% Make sure the result is good enough and bomb out if it's not
if norm(dcm-euler2dcm(alpha,beta,gamma))>1e-3
    disp(dcm); disp(euler2dcm(alpha,beta,gamma));
    error('dcm2euler: DCM to Euler conversion failed.');
end

end
