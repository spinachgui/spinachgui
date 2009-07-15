
import spinsys

ss=spinsys.Spinsys()
ss.loadFromFile("data/spinsys.xml")
ss.dump()
ss.saveToFile("data/spinsys_saved.xml")
ss.createNew()
ss.dump()
ss.addSpin()
ss.dump()
