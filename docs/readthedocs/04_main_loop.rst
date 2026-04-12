The Main Loop
=============

Grafyte applications run inside a frame loop:

.. code-block:: python

   import grafyte

   app = grafyte.Application("My Game", (640, 360))
   scene = app.make_new_scene()

   while not app.should_close():
       app.render()

   app.quit()

``app.should_close()`` returns ``True`` when the user requests to close the window. ``app.render()`` processes window events and draws the current frame, so it must be called every iteration.

Frame Timing
------------

Grafyte exposes frame timing as properties:

- ``app.dt``: elapsed time since the previous frame, in seconds
- ``app.now``: current engine time, in seconds

Example:

.. code-block:: python

   while not app.should_close():
       elapsed = app.now
       delta = app.dt
       app.render()

Shutdown
--------

Call ``app.quit()`` once the loop ends so native resources are released cleanly.
