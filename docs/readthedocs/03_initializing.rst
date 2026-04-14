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

Most applications create a scene immediately after creating the window with the line below:

.. code-block:: python

   scene = app.make_new_scene()

Here is a complete example of how to initialize an application and create a simple scene for it.

For example:

.. code-block:: python

   import grafyte

   app = grafyte.Application("My First Game", (640, 360))

   # Create a new scene
   menu_scene = app.make_new_scene()

   # Add a text for visual feedback
   menu_title = menu_scene.spawn_text_object((20, 20), "Main Menu", 24)

At this point the program opens a window, but it still needs a main loop to stay alive and render frames.
