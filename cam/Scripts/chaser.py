from events import *
from component import *
from physics import *

class Chaser:
    def __init__(self, manager):
        self.manager = manager

    def collided(self, event):
        thisEvent = Infected()
        thisEvent.other = self.entity
        event.getother().fire_event(thisEvent)
        #print "chaser collided with " + str(event.getother().id)

    def start(self):
        self.currentNodeXZ = self.manager.astar.findCurrentNode(
            (self.entity.transform().global_position().x, self.entity.transform().global_position().z))
        self.manager.chaserXZ.append(self.currentNodeXZ)
        self.manager.map[self.currentNodeXZ].weight += 50
        self.manager.chaserPos.append((self.entity.transform().global_position().x, self.entity.transform().global_position().z))
        self.manager.chaser_e.append(self.entity)
        self.entity.add_component(Mesh(ModelShader("chaser_texture.jpg"), "chaser_mesh.fbx", Vec3(1.0, 0.84, 0.0), Vec3(1, 1, 1), 4))
        self.entity.register_collided_handler(self.collided)
