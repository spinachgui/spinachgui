
import spinsys

ss=spinsys.Spinsys()
ss.loadFromFile("data/spinsys.xml")
ss.dump()
ss.saveToFile("data/spinsys_saved.xml")
ss.dump()
ss.addSpin()
ss.dump()
print ss.getSpins()

