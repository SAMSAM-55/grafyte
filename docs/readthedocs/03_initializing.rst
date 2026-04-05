Initializing the Application
============================

The primary entry point for any Grafyte application is the ``Application`` class. It sets the window size and title:

.. code-block:: python

   import grafyte
   app = grafyte.Application("My First Game", (640, 240))

.. note:: The window size of your application is not fixed. Users can always resize it and Grafyte will handle the maths to preserve the look of your game.

The variable ``app`` represents the application and the window we see. It will be one of the most used variables.

You can now run this program and test it. At this moment it does very little. It opens a window and closes it immediately.
