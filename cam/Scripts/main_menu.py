from component import *
from controller import *
from physics import *
from ui import *
from entity import *

def set_position(marker, item):
    marker.position = item.position + Vec2(-0.1, 0.05)

def start_game(self):
    e = self.entity()
    while e.get_parent():
        e = e.get_parent()

    level = Entity.create(e).lock()
    mesh = Mesh(ModelShader(), "map_mesh.fbx", "", Vec3(0.2, 0.4, 0.2), Vec3(1.0, 1.0, 1.0), 4) # TODO hacky hacky hardcoded opengl constant, 4 is GL_TRIANGLES
    body = RigidBody(self.physics(), "map_mesh.fbx", 1.0, False)
    level.add_component(mesh)
    level.add_component(body)

    ai = Entity.create(e).lock()
    mesh = Mesh(ModelShader(), "runner_mesh.fbx", "", Vec3(1.0, 0.84, 0.0), Vec3(4.427, 2.426, 12.935), 4)
    vehicle = ScriptComponent("chaser_ai", self.physics())
    chaser = ScriptComponent("chaser", self.physics())
    ai.add_component(mesh)
    ai.add_component(vehicle)
    ai.add_component(chaser)

    player = Entity.create(e).lock()
    mesh = Mesh(ModelShader(), "runner_mesh.fbx", "", Vec3(0.1, 0.1, 0.6), Vec3(4.427, 2.426, 12.935), 4)
    vehicle = ScriptComponent("vehicle", self.physics())
    # runner = ScriptComponent("runner", self.physics())

    player.add_component(mesh)
    player.add_component(vehicle)
    # player.add_component(runner)

    e.add_component(ScriptComponent("start_game", self.physics()))

    self.entity().destroy()

def init(self):
    global control
    global marker
    global background
    global startgame
    global quitgame
    global selected

    control = Controller(0)
    background = Image("background.png", Vec2(-1.0, -1.0), Vec2(2.0, 2.0), 3)
    startgame = Image("start-game.png", Vec2(-0.3, 0.0), Vec2(0.6, 0.2), 2)
    quitgame = Image("quit-game.png", Vec2(-0.3, -0.2), Vec2(0.6, 0.2), 2)
    marker = Image("menu-marker.png", Vec2(-1.0, -1.0), Vec2(0.2, 0.2), 1)
    selected = startgame

    set_position(marker, startgame)

    self.entity().add_component(background)
    self.entity().add_component(startgame)
    self.entity().add_component(quitgame)
    self.entity().add_component(marker)

def update(self, dt):
    global control
    global marker
    global background
    global startgame
    global quitgame
    global selected

    control.update()
    print "control direction: " + str(control.direction)
    print "contol select: " + str(control.select)
    if control.direction == 7: # down - probably shouldn't hardcode these...
        if selected != quitgame:
            selected = quitgame
            set_position(marker, quitgame)
    elif control.direction == 6:
        if selected != startgame:
            selected = startgame
            set_position(marker, startgame)

    if control.select:
        if selected == startgame:
            start_game(self)
