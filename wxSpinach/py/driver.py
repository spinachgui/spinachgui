
import spinsys

ss=spinsys.Spinsys()
ss.loadFromFile("data/spinsys.xml")
ss.loadFromG03File("data/tryosine.log")
ss.saveToFile("data/spinsys_saved.xml")

for i in range(ss.getSpinCount()):
    spin=ss.getSpin(i)
    print "spin "+str(i)+" coords=" + str(spin.getCoords())

for i in range(ss.getInteractionCount()):
    inter=ss.getInteraction(i)
    form=inter.getFormAsString()
    print "The form of the interaction "+str(i)+" is "+form
    if form == "scalar" or form == "matrix":
        print "The interaction matrix is:"
        m=inter.getAsMatrix();
        m.dump()
    if form != "scalar" and form != "matrix":
        orient=inter.getSpinachOrientation()
        print orient

