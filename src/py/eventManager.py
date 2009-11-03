


EVENT_CHANGE=0;
EVENT_SPIN=1;
EVENT_INTERACTION=2;
EVENT_FRAME=3;

class EventManager():
    def __init__(self):
        self.changeCallbacks=[]
        self.spinCallbacks=[]
        self.interactionCallbacks=[]
        self.frameCallbacks=[]
        
        self.callbacks={EVENT_CHANGE:self.changeCallbacks,
                        EVENT_SPIN:self.spinCallbacks,
                        EVENT_INTERACTION:self.interactionCallbacks,
                        EVENT_FRAME:self.frameCallbacks}

    def RegisterHandler(self,callback,type=EVENT_CHANGE):
        self.callbacks[type].append(callback);


    def Trigger(self,type=EVENT_CHANGE):
        for callback in self.callbacks[type]:
            callback()
