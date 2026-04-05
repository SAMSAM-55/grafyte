Representing Entities
=====================

In Grafyte, the main way to handle things in the game world is using the ``Scene`` and ``Object`` system.

The Scene
---------

A ``Scene`` represents a single level or a distinct part of your game (like a menu, or a specific level). You create a scene using ``app.make_new_scene()``.

The Object
----------

An ``Object`` is anything that exists in the scene. This could be a player, an enemy, a bullet, or a background image.

To create an object, you use the ``spawn_object`` method on a scene:

.. code-block:: python

   import grafyte

   app = grafyte.Application("Entities Example", (800, 600))
   scene = app.make_new_scene()

   # Spawn an object at position (0, 0) with size (50, 50)
   player = scene.spawn_object((0, 0), (50, 50))

   # Set its color (RGB) -> green
   player.set_color((0, 255, 0))

   while not app.should_close():
       # Move the player object
       player.move((0.1, 0))
       
       app.render()

   app.quit()

Coordinate System
-----------------

In Grafyte, the coordinate ``(0, 0)`` is the **center of the screen**.

- **X** increases towards the right.
- **Y** increases towards the top.

Grafyte also has a custom coordinate system unit. In Grafyte - as the window is resizable -, the units used to define objects' size and position
is completely unrelated to pixels. The way Grafyte handles coordinate is that the part of the scene shown on the window will measure exactly 200 units
in height, then the width is recalculated when the window dimensions change to maintain the fixed height. As a result, your users will be able
to resize your application without the objects looking smaller on bigger screens.

Layering with Z-Index
---------------------

When multiple objects overlap, you can control which one appears on top using the ``layer`` parameter of ``spawn_object``.

Objects with a **higher** z-index are drawn **in front of** objects with a lower one. The z-index is an integer and can be negative.

.. code-block:: python

   # Background sits behind everything else
   background = scene.spawn_object((0, 0), (200, 200), layer=0)

   # Player renders on top of the background
   player = scene.spawn_object((0, 0), (20, 20), 1) # layer is the third parameter of the method so its name can be omitted

.. note::

   Objects sharing the same z-index have no guaranteed draw order relative to each other.
   If two objects must reliably overlap, always assign them distinct z-index values.