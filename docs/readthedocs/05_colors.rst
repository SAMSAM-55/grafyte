Colors
======

Grafyte colors use integer RGB values from ``0`` to ``255``. Some APIs also accept an alpha or tint-strength value as a fourth component.

.. code-block:: python

   BLACK = (0, 0, 0)
   GRAY = (127, 127, 127)
   WHITE = (255, 255, 255)
   RED = (255, 0, 0)
   GREEN = (0, 255, 0)
   BLUE = (0, 0, 255)
   YELLOW = (255, 255, 0)

Background Color
----------------

Set the window clear color through the ``background_color`` property:

.. code-block:: python

   app.background_color = YELLOW

Object Color
------------

Non-textured objects use the ``color`` property:

.. code-block:: python

   square = scene.spawn_object((0, 0), (20, 20))
   square.color = GREEN
   square.color = (255, 255, 255, 0.5)

The 4-value form sets RGB plus alpha.

Text Color
----------

Scene text and UI text expose the same property:

.. code-block:: python

   label = scene.spawn_text_object((0, 30), "Hello")
   label.color = RED
