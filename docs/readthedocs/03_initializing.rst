Initializing the Application
============================

The ``Application`` class is the entry point for every Grafyte program.

.. code-block:: python

   import grafyte

   app = grafyte.Application("My First Game", (640, 360))

The first argument is the window title. The second is the initial window size as ``(width, height)``.

Optional Font
-------------

``Application`` also accepts an optional ``font_path`` argument:

.. code-block:: python

   app = grafyte.Application(
       "My First Game",
       (640, 360),
       font_path="@embed/Fonts/Base",
   )

If you omit it, Grafyte uses the embedded default font.

Creating Your First Scene
-------------------------

Most applications create a scene immediately after creating the window:

.. code-block:: python

   scene = app.make_new_scene()

At this point the program opens a window, but it still needs a main loop to stay alive and render frames.
