Explore a simple ball game
==========================

To show what Grafyte can do, here is a simple program which demonstrates a bouncing square.

Bouncing Square Example
-----------------------

.. code-block:: python

    import grafyte

    window_size = (640, 320)
    size = 20

    app = grafyte.Application("Bouncing Square", window_size)
    scene = app.make_new_scene()

    # Spawn a square at (0, 0) with size (20, 20)
    # Grafyte coordinates: (0, 0) is the center of the screen
    square = scene.spawn_object((0, 0), (size, size))
    square.set_color((255, 0, 0)) # red

    speed = [65, 50] # Units per second with diagonal movement

    HALF_H = 100  # screen is always 200 units tall
    aspect = window_size[0] / window_size[1]
    HALF_W = HALF_H * aspect  # 200.0 for a 640x320 window

    app.set_background_color((0, 255, 0)) # green background

    while not app.should_close():
        dt = app.get_delta_time()

        # Move the square
        square.move((speed[0] * dt, speed[1] * dt))

        # Check for grafyte space boundaries this only works fine as long as the window preserves its original aspect-ratio
        if not (-HALF_W + size / 2) <= square.pos.x <= (HALF_W - size / 2):
            speed[0] = -speed[0]
        if not (-HALF_H + size / 2) <= square.pos.y <= (HALF_H - size / 2):
            speed[1] = -speed[1]

        app.render()

    app.quit()

Try to understand what the program does. Then try to modify its parameters.
