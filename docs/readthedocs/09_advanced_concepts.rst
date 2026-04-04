Advanced Concepts
=================

In this section, we'll explore some advanced features of Grafyte that allow you to create more visually complex and dynamic games, including textures, tinting, and precise time-based movement.

Textures and Tinting
--------------------

While solid colors are great for prototyping, most games use textures (images) for their entities. Grafyte makes it easy to load and apply textures to your objects.

To use a texture, you must set the ``has_texture`` parameter to ``True`` when spawning the object. Then, you can use the ``use_texture(path, slot)`` method.

.. code-block:: python

   # Spawn an object that supports textures
   player = scene.spawn_object((0, 0), (64, 64), has_texture=True)
   
   # Load and use a texture file
   player.use_texture("res/textures/player.png", 0)

You can also apply a **tint** to a textured object. This multiplies the texture's colors by the tint color, allowing you to create effects like damage flashes, status effects, or simple color variations of the same sprite.

The ``set_tint(color, strength)`` method takes an RGB tuple and a strength value from 0.0 to 1.0.

.. code-block:: python

   # Apply a red tint (e.g., when taking damage)
   # (255, 0, 0) is RED, strength 0.5 makes it half-red
   player.set_tint((255, 0, 0), 0.5)
   
   # Clear the tint
   player.set_tint((0, 0, 0), 0.0)

Delta Time and Smooth Movement
------------------------------

In previous examples, we moved objects by a fixed number of units each frame. However, frame rates can vary between different computers. To ensure your game runs at the same speed regardless of the FPS (frames per second), we use **Delta Time** (``dt``).

Delta Time is the amount of time that passed since the last frame. By multiplying your speeds by ``dt``, you convert your movement from "units per frame" to "units per second".

In Grafyte, you get the delta time using ``app.get_delta_time()``.

.. code-block:: python

   speed = 25 # Units per second
   
   while not app.should_close():
       dt = app.get_delta_time()
       
       # Move the player 25 units per second to the right
       player.move((speed * dt, 0))
       
       app.render()

Using ``dt`` is essential for consistent gameplay across all hardware.
