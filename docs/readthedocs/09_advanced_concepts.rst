Advanced Concepts
=================

This chapter covers textured objects, camera control, collision helpers, and UI text.

Textured Objects and Tint
-------------------------

To use a texture, create the object with ``has_texture=True`` and then load the texture:

.. code-block:: python

   player = scene.spawn_object((0, 0), (32, 32), has_texture=True)
   player.use_texture("res/textures/player.png", 0)

Textured objects use ``tint`` instead of ``color``:

.. code-block:: python

   player.tint = (255, 0, 0, 0.5)

The fourth component is the tint strength from ``0.0`` to ``1.0``.

Camera
------

Each scene exposes a camera object:

.. code-block:: python

   camera = scene.camera
   camera.follow(player)
   camera.follow_offset = (0, 15)
   camera.zoom = 1.1

You can also place the camera manually with ``camera.pos = (x, y)``.

Collisions
----------

Attach collision boxes to objects and query the results:

.. code-block:: python

   wall = scene.spawn_object((40, 0), (20, 60))
   wall.color = (180, 180, 180)
   wall.add_collision_box((0, 0), (20, 60))

   player.add_collision_box((0, 0), (20, 20))

   hit = player.collides_with(wall)
   if hit:
       print(hit.direction)

For automatic collision resolution order, set ``player.auto_collides`` to an integer priority.

For exemple, if you want the player to be blocked automatically by many walls:

.. code-block:: python

   wall = scene.spawn_object((40, 0), (20, 60))
   wall.color = (180, 180, 180)
   wall.add_collision_box((0, 0), (20, 60))

   wall2 = scene.spawn_object((0, -10), (40, 10))
   wall2.color = (180, 180, 180)
   wall2.add_collision_box((0, 0), (40, 10))

   # wall3 etc.

   player.add_collision_box((0, 0), (20, 20))
   player.auto_collides = 1 # This will handle collision automatically

Alternatively, if you want multiple objects to collide in a predictable way, you must use higher orders.
In grafyte, a higher order object will push the objects with a lower resolution order:

.. code-block:: python

   player.auto_collides = 1
   # ...
   ball.auto_collides = 2

In the above exemple, the ball will always push back the player when the two objects are colliding.
Please note that if two objects have the same order, their collisions can be unpredictable when coding (but
deterministic at runtime)

UI Text
-------

For screen-space overlays, create a UI manager:

.. code-block:: python

   from grafyte import Anchor

   ui = app.make_new_ui()
   score = ui.add_text((10, 10), "Score: 0", scale=18, anchor=Anchor.TopLeft)
   score.color = (255, 255, 255)

UI text stays attached to the screen instead of moving with the world camera.
