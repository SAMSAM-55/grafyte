Representing Entities
=====================

Grafyte organizes gameplay entities around scenes and objects.

Scenes
------

Create a scene with ``app.make_new_scene()``:

.. code-block:: python

   import grafyte

   app = grafyte.Application("Entities Example", (800, 600))
   scene = app.make_new_scene()

Objects
-------

Spawn a rectangle with ``scene.spawn_object(position, size)``:

.. code-block:: python

   player = scene.spawn_object((0, 0), (20, 20))
   player.color = (0, 255, 0)

Objects expose a small set of mutable properties:

- ``pos`` for position
- ``scale`` for size scaling
- ``rot`` for rotation
- ``color`` for non-textured objects
- ``tint`` for textured objects

Example movement:

.. code-block:: python

   while not app.should_close():
       player.pos += (25 * app.dt, 0)
       app.render()

Coordinate System
-----------------

The point ``(0, 0)`` is at the center of the world view.

- X increases to the right
- Y increases upward

Grafyte keeps the visible world height fixed at 200 units. The visible width changes with the window aspect ratio, which lets the window resize without changing the apparent scale of the world.

Layers
------

Use the ``layer`` argument to control draw order:

.. code-block:: python

   background = scene.spawn_object((0, 0), (200, 200), layer=0)
   player = scene.spawn_object((0, 0), (20, 20), layer=1)

Higher layers render in front of lower ones.

Text Objects
------------

Use ``spawn_text_object`` for text placed inside the world:

.. code-block:: python

   title = scene.spawn_text_object((0, 60), "Grafyte", scale=18)
   title.color = (255, 255, 255)
