The Main Loop
=============

The most essential part of any interactive application is the main loop.

In Grafyte, we use ``app.should_close()`` to check if the user wants to quit:

.. code-block:: python

   import grafyte

   app = grafyte.Application("My Game", (640, 240))

   while not app.should_close():
       app.render()

   app.quit()

The ``app.render()`` function is responsible for drawing everything and handling system events. Without it, the window will not respond.

Quit the application properly
-----------------------------

In Grafyte, the ``Application`` class handles the window close button automatically. When you click the close button, ``app.should_close()`` returns ``True``, and the loop ends. We then call ``app.quit()`` to shut down correctly.
