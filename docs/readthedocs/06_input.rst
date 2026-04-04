Handling Input
==============

Grafyte provides an ``InputManager`` accessed via ``app.input``. We can check if a key was pressed using ``app.input.was_key_pressed(Key)``.
Below is an example program that allows you to switch the color of the background based on keyboard inputs.

Switch the background color
---------------------------

The following example switches the background color when the 'R' or 'G' keys are pressed.

.. code-block:: python

   import grafyte
   from grafyte import Key

   app = grafyte.Application("My Game", (640, 240))

   GRAY = (127, 127, 127)
   RED = (255, 0, 0)
   GREEN = (0, 255, 0)

   background = GRAY

   while not app.should_close():
       if app.input.was_key_pressed(Key.R):
           background = RED
       elif app.input.was_key_pressed(Key.G):
           background = GREEN
           
       app.set_background_color(background)
       app.render()

   app.quit()

Test the program. Pressing the R and G keys allows you to switch the background color.
