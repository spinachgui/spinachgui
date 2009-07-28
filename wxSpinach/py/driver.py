
import spinsys

ss=spinsys.Spinsys()
ss.loadFromFile("data/spinsys.xml")
ss.saveToFile("data/spinsys_saved.xml")

for i in range(ss.getSpinCount()):
    spin=ss.getSpin(i)
    print "spin "+str(i)+" coords=" + str(spin.getCoords())

for i in range(ss.getInteractionCount()):
    inter=ss.getInteraction(i)
    form=inter.getFormAsString()
    print "The form of the interaction "+str(i)+" is "+form
    if form != "scalar" and form != "matrix":
        orient=inter.getOrientation()
        print orient

