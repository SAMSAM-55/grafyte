import grafyte

app = grafyte.Application("Grafyte test", (540, 360))
renderer = grafyte.Renderer()

obj = grafyte.Object((50, 0), (10, 20), 20)
obj.set_color((0, 0, 255))

bg = grafyte.Object((0, 0), (100, 100), -1)
bg.set_color((0, 255, 0))

test_texture = grafyte.Object((-75, 75), (25, 25), 0, True)
test_texture.use_texture("res/textures/ChernoLogo.png")

renderer.add_object(obj)
renderer.add_object(test_texture)
renderer.add_object(bg)
app.use_renderer(renderer)

r = 0
increment = 25
speed = 100
is_frozen = False
frozen_time = 0
frozen_duration = 5

while not app.should_close():
    dt = app.get_delta_time()

    if app.is_key_down("A"):
        obj.move((-speed*dt, 0))
    if app.is_key_down("D"):
        obj.move((speed*dt, 0))
    if app.is_key_down("w"):
        obj.move((0, speed*dt))
    if app.is_key_down("S"):
        obj.move((0, -speed*dt))

    if app.is_key_down("R"):
        obj.move_to((0, 0))

    if app.is_key_down("F"):
        is_frozen = True
        frozen_time = app.get_now()

    if is_frozen and app.get_now() - frozen_time > 0:
        t = app.get_now() - frozen_time
        freeze_strength = max(0.0, 1 - t / frozen_duration)
        test_texture.set_tint((100, 214, 255), freeze_strength)

        if t >= frozen_duration:
            is_frozen = False
            test_texture.set_tint((0, 0, 0), 0)

    if r >= 255:
       increment *= -1
    if r <= 0:
        increment *= -1

    r += increment * dt
    app.set_background_color(r, 0, 0)

    app.render()

app.quit()