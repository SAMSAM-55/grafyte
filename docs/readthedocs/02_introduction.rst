Introduction to Grafyte
=======================

Grafyte is a 2D engine for Python. The public API is small: you create an ``Application``, build one or more ``Scene`` instances, spawn ``Object`` or ``TextObject`` entities, and drive them from the main loop.

Basic Imports
-------------

Every program starts with the package import:

.. code-block:: python

   import grafyte

It is also common to import the enums you use for input and collision logic:

.. code-block:: python

   import grafyte
   from grafyte import Direction, InputTrigger, Key

Core Concepts
-------------

- ``Application`` owns the window, timing, rendering, and input state.
- ``Scene`` stores world objects and exposes a ``camera``.
- ``Object`` represents a renderable rectangle in the scene.
- ``TextObject`` renders text inside a scene.
- ``UIManager`` renders screen-space text overlays.
- ``InputManager`` provides raw key states and action-based input.

Most of the tutorial examples follow the same structure:

1. Create an ``Application``.
2. Create a ``Scene``.
3. Spawn objects.
4. Update them inside ``while not app.should_close():``.
5. Call ``app.render()`` every frame.
