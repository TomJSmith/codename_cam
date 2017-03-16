import events

e = None

def infected(event):
    global e

    print "infected yo"
    e.destroy()
    e = None
    # self.entity().destroy()

def destroyed(event):
    print "destroyed"

def init(self):
    global e

    e = self.entity()
    e.register_infected_handler(infected)
    e.register_destroyed_handler(destroyed)
