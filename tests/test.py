import grafyte

app = grafyte.Application("Grafyte test")
app.init(2560, 1440)

renderer = grafyte.Renderer()

obj = grafyte.Object((10, 20), (50, 0), "res/shaders/Basic.shader", 20)
obj.set_color(0, 0, 255)

bg = grafyte.Object((100, 100), (0, 0), "res/shaders/Basic.shader", -1)
bg.set_color(0, 255, 0)

test_texture = grafyte.Object((25, 25), (-75, 75), "res/shaders/Texture.shader", 0, True)
test_texture.set_color(255, 255, 255)
test_texture.use_texture("res/textures/ChernoLogo.png")

renderer.add_object(obj)
renderer.add_object(test_texture)
renderer.add_object(bg)
app.use_renderer(renderer)

r = 0
increment = 25
speed = 100

while not app.should_close():
    dt = app.get_delta_time()
    current_key = app.get_current_input()

    if current_key != -1:

        if chr(current_key) == "D":
            obj.move((speed*dt, 0))

        if chr(current_key) == "A":
            obj.move((-speed*dt, 0))

        if chr(current_key) == "W":
            obj.move((0, speed*dt))

        if chr(current_key) == "S":
            obj.move((0, -speed*dt))

        if chr(current_key) == "R":
            obj.move_to((0, 0))

    if r >= 255:
       increment *= -1

    if r <= 0:
        increment *= -1

    r += increment * dt
    app.set_background_color(r, 0, 0)

    app.render()

app.quit()