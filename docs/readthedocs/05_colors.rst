Colors
=============

Colors in Grafyte are defined as tuples of the base colors red, green and blue (RGB). Each base color is represented as a number between 0 and 255.

Here are some examples of colors and their RGB codes (red, green and blue values).

.. code-block:: python

   BLACK = (0, 0, 0)
   GRAY = (127, 127, 127)
   WHITE = (255, 255, 255)
   RED = (255, 0, 0)
   GREEN = (0, 255, 0)
   BLUE = (0, 0, 255)
   YELLOW = (255, 255, 0)

To change the background color of the window, use ``app.set_background_color(color)``:

.. code-block:: python

   app.set_background_color(YELLOW) # or (255, 255, 0)
