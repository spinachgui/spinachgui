
import spinsys

ss=spinsys.Spinsys()
ss.loadFromFile("data/spinsys.xml")
ss.saveToFile("data/spinsys_saved.xml")
ss.addSpin()
print ss.getSpins()

s1=ss.getSpin(0)
s2=ss.getSpin(0)
print s1
print s2
s1.dump()
s2.dump()

