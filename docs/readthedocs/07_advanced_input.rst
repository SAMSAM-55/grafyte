Advanced Input (Actions)
========================

Instead of checking for individual key presses, Grafyte's input system allows you to define "Actions". This makes it easy to handle input triggers like "Press" or "Hold" in a clean, high-level way.

Defining Actions
----------------

To create an action, use ``app.input.create_action(name, key, trigger)``.

- **name**: A string identifying the action.
- **key**: The ``Key`` you want to map to this action.
- **trigger**: An ``InputTrigger`` that defines when the action is active.

There are two main ``InputTrigger`` types:

- ``InputTrigger.Press``: Active only for the single frame when the key is first pressed.
- ``InputTrigger.Release``: Active only for the single frame when the key is released.
- ``InputTrigger.Hold``: Active as long as the key is held down.

Using Actions in the Game Loop
------------------------------

Once an action is defined, you can check its status using ``app.input.is_action_active(name)``.

.. code-block:: python

   import grafyte
   from grafyte import Key, InputTrigger

   app = grafyte.Application("Actions Example", (640, 360))

   # Define actions
   app.input.create_action("move_up", Key.Z, InputTrigger.Hold)
   app.input.create_action("move_up_alt", Key.Up, InputTrigger.Hold)
   app.input.create_action("reset", Key.R, InputTrigger.Press)

   while not app.should_close():
       # Handle actions
       if app.input.is_action_active("move_up") or app.input.is_action_active("move_up_alt"):
           # Logic for moving up
           pass
       
       if app.input.is_action_active("reset"):
           # Logic for resetting
           pass

       app.render()

   app.quit()

Benefits of Action-Based Input
------------------------------

1. **Abstraction**: Your gameplay logic doesn't need to know which specific key was pressed, only that the "move_up" action was triggered.
2. **Rebinding**: You can easily change the key mapping for an action without modifying your core game loop.
3. **Trigger Control**: No need to manually track "was just pressed" vs "is currently held" for every key.
