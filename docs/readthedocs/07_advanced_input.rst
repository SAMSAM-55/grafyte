Advanced Input (Actions)
========================

Action-based input lets you bind one or more keys to a named gameplay action.

Creating Actions
----------------

Use ``app.input.create_action(name, trigger, *keys)``:

.. code-block:: python

   import grafyte
   from grafyte import InputTrigger, Key

   app = grafyte.Application("Actions Example", (640, 360))

   app.input.create_action("move_up", InputTrigger.Hold, Key.W, Key.Up)
   app.input.create_action("move_down", InputTrigger.Hold, Key.S, Key.Down)
   app.input.create_action("reset", InputTrigger.Press, Key.R)

Checking Actions
----------------

Use ``app.input["action_name"]`` to test whether the action is active:

.. code-block:: python

   player_y = 0.0
   speed = 40.0

   while not app.should_close():
       if app.input["move_up"]:
           player_y += speed * app.dt
       if app.input["move_down"]:
           player_y -= speed * app.dt
       if app.input["reset"]:
           player_y = 0.0

       app.render()

   app.quit()

Available Triggers
------------------

- ``InputTrigger.Press``: active for one frame when the key is pressed
- ``InputTrigger.Hold``: active while the key remains down
- ``InputTrigger.Release``: active for one frame when the key is released

This pattern keeps gameplay code independent from the exact key layout.
