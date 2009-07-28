
import spinsys

ss=spinsys.Spinsys()
ss.loadFromFile("data/spinsys.xml")
ss.saveToFile("data/spinsys_saved.xml")
ss.addSpin()

s1=ss.getSpin(0)
s2=ss.getSpin(1)
print s1
print s2
s1.dump()
s2.dump()

for i in range(ss.getInteractionCount()):
    inter=ss.getInteraction(i)
    form=inter.getFormAsString()
    print "The form of the interaction "+str(i)+" is "+form
    if form != "scalar" and form != "matrix":
        orient=inter.getOrientation()
        print orient

