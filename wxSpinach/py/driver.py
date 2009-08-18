
import spinsys


ss=spinsys.SpinSystem();
ss.LoadFromG03File('data/tyrosine.log')

#iden=spinsys.Matrix3(1,0,0,0,1,0,0,0,1)
#print iden.getEigenValX()
#print iden.getEigenvectorX()
 
#diag=spinsys.Matrix3(1,1,1,
#                     1,2,1,
#                     1,1,3)
#v=diag.getEigenvectorX()
#print v
 
#print """Expected Eigenvalues 1,2,3"""
#print
#print
 
#diag=spinsys.Matrix3(1,2,3,
#                     4,5,6,
#                     7,8,9)
#v=diag.getEigenvectorX()
#print v
#print """Expected Eigenvalues 16.1169,-1.1168,0.000"""

#print
#print
#
# diag=spinsys.Matrix3(1,0,1,
#                     2,2,3,
#                     5,7,2)
#v=diag.getEigenvectorX()
#print v
#print """Expected Eigenvalues 7.2386,0.6268,-2.8654"""
# print
#print
#
#
# diag=spinsys.Matrix3(1,0,-6,
#                     0,1,0,
#                     6,0,2)
#v=diag.getEigenvectorX()
#print v
#print """Expected Eigenvalues 1.5 + 5.9791j,1.5-5.9791j,1.0000"""
#print
#print
