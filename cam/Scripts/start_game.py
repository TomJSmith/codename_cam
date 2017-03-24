from events import *

time = 0.0
fired = False

class StartGame:
    pass

def update(self, dt):
    global time
    global fired

    time += dt
    if time > 3.0 and not fired:
        self.entity().broadcast_event(StartGame())
        print "starting game..."
        fired = True
