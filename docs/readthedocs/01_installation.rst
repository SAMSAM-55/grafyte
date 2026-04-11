Installation
============

Grafyte targets Python 3.10 and newer and exposes a Python API backed by a native rendering engine.

Requirements
------------

- Python 3.10+
- A machine with working OpenGL drivers
- Windows 10/11 or a modern Linux distribution

Install From PyPI
-----------------

The simplest installation path is ``pip``:

.. code-block:: bash

   python -m pip install grafyte

Quick Verification
------------------

Run the following snippet to confirm that the package imports correctly:

.. code-block:: python

   import grafyte

   app = grafyte.Application("Grafyte Check", (640, 360))
   app.background_color = (30, 30, 30)

   while not app.should_close():
       app.render()

   app.quit()

If a window opens, the installation is working and you can continue with the tutorial.
