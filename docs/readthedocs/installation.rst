Installation
============

Prerequisites
-------------

- Python 3.10+
- A C++ compiler (MSVC, GCC, or Clang)
- CMake
- OpenGL drivers

.. note::
   When applicable please make sure to test your code on both windows and linux as well as on Python 3.10 **to** 3.14 if possible.

Building from Source
--------------------

To set up the development environment and build Grafyte:

Windows
^^^^^^^

.. code-block:: powershell

   scripts\setup.bat
   .\.venv\Scripts\activate
   pip install .

Linux
^^^^^

.. code-block:: bash

   chmod +x scripts/setup.sh
   ./scripts/setup.sh
   source .venv/bin/activate
   pip install .

Using CMake
-----------

Alternatively, you can build the project using CMake for development:

.. code-block:: bash

   mkdir build
   cd build
   cmake ..
   cmake --build .
