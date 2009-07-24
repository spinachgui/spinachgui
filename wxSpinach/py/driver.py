
import spinsys

ss=spinsys.Spinsys()
ss.loadFromFile("data/spinsys.xml")
ss.dump()
ss.saveToFile("data/spinsys_saved.xml")
ss.dump()
ss.addSpin()
ss.dump()
print ss.getSpins()

s=spinsys.SpinachSpin()
print s
s=spinsys.SpinachSpin()
print s
print s.sayHello()
print s.dump()

