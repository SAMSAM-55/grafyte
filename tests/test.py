import grafyte

app = grafyte.Application("Grafyte test")
app.init(540, 380)

renderer = grafyte.Renderer()

obj = grafyte.Object((10, 20), (-50, 0), "res/shaders/Basic.shader")
obj.set_color(0, 0, 255)

bg = grafyte.Object((200, 200), (0, 0), "res/shaders/Basic.shader")
bg.set_color(0, 255, 0)

test_overflow = grafyte.Object((300, 200), (0, 0), "res/shaders/Basic.shader")
test_overflow.set_color(0, 0, 0)

renderer.add_object(test_overflow)
renderer.add_object(bg)
renderer.add_object(obj)
app.use_renderer(renderer)

r = 0
increment = 25

while not app.should_close():
    dt = app.get_delta_time()

    if app.get_current_input() != -1:
        print(chr(app.get_current_input()))

    if r >= 255:
       increment *= -1

    if r <= 0:
        increment *= -1

    r += increment * dt
    app.set_background_color(r, 0, 0)

    app.render()

app.quit()