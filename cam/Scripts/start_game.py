from events import *

class GameStarted:
    pass

class StartGame:
    def start(self):
        self.time = 0.0
        self.fired = False

    def update(self, dt):
        self.time += dt
        if self.time > 3.0 and not self.fired:
            self.entity.broadcast_event(GameStarted())
            print "starting game..."
            self.fired = True
