Explore a Simple Bouncing Square
================================

This example combines application setup, object spawning, colors, and delta-time movement.

.. code-block:: python

   import grafyte

   window_size = (640, 320)
   size = 20

   app = grafyte.Application("Bouncing Square", window_size)
   scene = app.make_new_scene()
   app.background_color = (30, 120, 30)

   square = scene.spawn_object((0, 0), (size, size))
   square.color = (255, 0, 0)

   velocity = [65.0, 50.0]

   half_h = 100.0
   aspect = window_size[0] / window_size[1]
   half_w = half_h * aspect

   while not app.should_close():
       square.pos += (velocity[0] * app.dt, velocity[1] * app.dt)

       if not (-half_w + size / 2) <= square.pos.x <= (half_w - size / 2):
           velocity[0] = -velocity[0]
       if not (-half_h + size / 2) <= square.pos.y <= (half_h - size / 2):
           velocity[1] = -velocity[1]

       app.render()

   app.quit()

What this example shows:

- ``Application`` owns the window and timing.
- ``Scene`` owns the square object.
- ``square.pos += (...)`` updates the object each frame.
- ``app.dt`` keeps movement frame-rate independent.
