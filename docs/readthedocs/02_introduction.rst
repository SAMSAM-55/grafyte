Introduction to Grafyte
=======================

Grafyte is a high-performance 2D multimedia library for Python for making games and interactive applications.

It is built on a powerful C++ engine using modern OpenGL. In this section we introduce the basics of Grafyte functions and classes.

Import the module
-----------------

Once, you have successfully installed Grafyte, you must import the library into your script before using it:

.. code-block:: python

   import grafyte

The ``import grafyte`` statement is always placed at the beginning of the program. It imports the Grafyte classes, methods and attributes into the current name space. Now these new methods can be called via ``grafyte.Method()``.

For further uses, it is also recommended to explicitly import some of Grafyte's module to simplify later methods and classes usage. Here are the complete recommended import statements:

.. code-block:: python
    import grafyte
    from grafyte import Key, InputTrigger, Direction