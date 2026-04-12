Handling Input
==============

Grafyte exposes input through ``app.input``.

Raw Key States
--------------

The input manager provides three accessors:

- ``app.input.key[Key.X]`` for keys currently held down
- ``app.input.key_pressed[Key.X]`` for the frame a key was pressed
- ``app.input.key_released[Key.X]`` for the frame a key was released

Example:

.. code-block:: python

   import grafyte
   from grafyte import Key

   app = grafyte.Application("Input Example", (640, 360))

   RED = (255, 0, 0)
   GREEN = (0, 255, 0)
   GRAY = (127, 127, 127)

   app.background_color = GRAY

   while not app.should_close():
       if app.input.key_pressed[Key.R]:
           app.background_color = RED
       elif app.input.key_pressed[Key.G]:
           app.background_color = GREEN

       app.render()

   app.quit()

Use ``key_pressed`` for one-frame actions such as toggles, and ``key`` for continuous movement.
