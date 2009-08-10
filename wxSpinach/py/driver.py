
import spinsys

#iden=spinsys.Matrix3(1,0,0,0,1,0,0,0,1)
#print iden.getEigenValX()
#print iden.getEigenvectorX()

diag=spinsys.Matrix3(1,0,0,
                     0,2,0,
                     0,0,3)
v=diag.getEigenvectorX()
print v
