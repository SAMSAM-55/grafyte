from grafyte.__converters import Vec2d, Color

class Application:
    """
    The main application class that manages the window, rendering context, and input.
    """
    def __init__(self, name: str, window_dimensions: Vec2d):
        """
        Initializes the application window and rendering context.

        :param name: The title of the window.
        :param window_dimensions: A Vec2d containing the width and height of the window.
        """
        ...
    def should_close(self) -> bool:
        """
        Checks if the application window should close (e.g., if the close button was clicked).
        Also updates internal timing for delta time calculations.

        :return: True if the window should close, False otherwise.
        """
        ...
    def quit(self) -> None:
        """
        Closes the application window and cleans up resources.
        """
        ...
    def get_now(self) -> float:
        """
        Returns the current time in seconds since the application started.

        :return: Current time as a float.
        """
        ...
    def get_delta_time(self) -> float:
        """
        Returns the time elapsed since the last frame.

        :return: Delta time in seconds.
        """
        ...
    def render(self) -> None:
        """
        Renders all objects and text in the current frame.
        """
        ...
    def add_text(self, text: str, scale: float, pos: Vec2d) -> int:
        """
        Adds text to be rendered in the application window.

        :param text: The string of text to display.
        :param scale: The scale/size of the text.
        :param pos: A Vec2d representing the (x, y) position of the text.
        :return: An integer ID for the added text, which can be used to remove it later.
        """
        ...
    def remove_text(self, id: int):
        """
        Removes previously added text from the application.

        :param id: The ID of the text to remove (returned by add_text).
        """
        ...
    def set_background_color(self, color: Color):
        """
        Sets the background (clear) color of the application window.

        :param color: A Color object (RGB) representing the new background color.
        """
        ...
    def is_key_down(self, key: str) -> bool:
        """
        Checks if a specific key is currently being held down.

        :param key: The character representation of the key (e.g., 'W', 'A', 'S', 'D').
        :return: True if the key is down, False otherwise.
        """
        ...
    def was_key_pressed(self, key: str) -> bool:
        """
        Checks if a specific key was pressed in the current frame.

        :param key: The character representation of the key.
        :return: True if the key was just pressed, False otherwise.
        """
        ...
    def was_key_released(self, key: str) -> bool:
        """
        Checks if a specific key was released in the current frame.

        :param key: The character representation of the key.
        :return: True if the key was just released, False otherwise.
        """
        ...
    def use_renderer(self, renderer: Renderer):
        """
        Sets the renderer to be used by the application.

        :param renderer: The Renderer object to use.
        """
        ...

class Renderer:
    """
    Manages a collection of objects to be rendered.
    """
    def __init__(self):
        """
        Initializes a new Renderer.
        """
        ...
    def add_object(self, obj: Object) -> None:
        """
        Adds an object to the renderer's list of objects to be drawn.

        :param obj: The Object to add.
        """
        ...

class Object:
    """
    Represents a renderable 2D object in the engine.
    """
    __has_texture: bool

    def __init__(self,
                 pos: Vec2d,
                 size: Vec2d,
                 layer: int = 0,
                 has_texture: bool = False,
                 shader_source_path: str = "", ):
        """
        Initializes a new renderable object.

        :param pos: A Vec2d representing the initial position of the object.
        :param size: A Vec2d representing the dimensions (width, height) of the object.
        :param layer: The rendering layer (higher numbers are rendered on top). (default = 0)
        :param has_texture: Boolean indicating if this object will use a texture. (default = False)
        :param shader_source_path: Optional path to a custom shader. If empty, a default shader is used.
        """
        ...
    def set_color(self, color: Color, a: float = 1) -> None:
        """
        Sets the uniform color of the object. Only applicable if the object doesn't have a texture.

        :param color: A Color object (RGB) for the object.
        :param a: The alpha chanel for the color. (default = 1)
        """
        ...
    def move(self, offset: Vec2d) -> None:
        """
        Moves the object by a given offset from its current position.

        :param offset: A Vec2d representing the (dx, dy) to move the object.
        """
        ...
    def move_to(self, pos: Vec2d) -> None:
        """
        Moves the object to a specific absolute position.

        :param pos: A Vec2d representing the new (x, y) position.
        """
        ...
    def use_texture(self, path: str) -> None:
        """
        Loads and applies a texture to the object.

        :param path: The file path to the texture.
        """
        ...
    def set_tint(self, tint: Color, strength: float = 1):
        """
        Sets a color tint to be applied over the object's texture.

        :param tint: A Color object (RGB) to use as the tint.
        :param strength: The strength of the tint effect. (default = 1)
        """
        ...