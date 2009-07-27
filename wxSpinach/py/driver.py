
import spinsys

ss=spinsys.Spinsys()
ss.loadFromFile("data/spinsys.xml")
ss.saveToFile("data/spinsys_saved.xml")
ss.addSpin()
print ss.getSpins()

s=spinsys.SpinachSpin()
print "printing s"
print s
print "printing hello"
print s.sayHello()
print "printing coords"
print s.getCoords()

