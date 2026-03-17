Quick Start
===========

Here's a minimal example to get you started with Grafyte:

.. code-block:: python

   import grafyte
   from grafyte import Key, InputTrigger

   # Initialize application
   app = grafyte.Application("My Game", (800, 600))
   scene = app.make_new_scene()

   # Create a player object
   player = scene.spawn_object((0, 0), (50, 50))
   player.set_color((0, 255, 0), 1.0) # Green color

   # Register an action
   app.create_input_action("move_right", Key.D, InputTrigger.Hold)

   while not app.should_close():
       dt = app.get_delta_time()
       
       # Handle input
       if app.is_action_active("move_right"):
           player.move((100 * dt, 0))
       
       # Render scene
       app.render()

   app.quit()
