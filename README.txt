Justin Weigle
CG Project 5
Wankel Rotary Engine

MIPMAPPING has been applied using SOIL's automatic mipmapping at texture load
time.
CUBE MAP applied to barrel.
Sphere environment map applied to overhead lights for chrome look.
6 total textures used.

CAMERA CONTROLS have been implemented using the keyboard rather than the menu.
Camera controls are as follows:
    A/a -> Rotate camera left / clockwise around the Y axis
    D/d -> Rotate camera right / counter clockwise around the Y axis
    W/w -> Rotate camera clockwise around X axis
    S/s -> Rotate camera counter clockwise around X axis

    E/e/mouse wheel up   -> Zoom in
    Q/q/mouse wheel down -> Zoom out

SPOTLIGHT CONTROLS have also been implemented using the keyboard.
Spotlight controls are as follows:
    UP_ARROW    -> Move spotlight up
    DOWN_ARROW  -> Move spotlight down
    LEFT_ARROW  -> Move spotlight left
    RIGHT_ARROW -> Move spotlight right

MAKEFILE FUNCTIONALITY
    make            -compiles (outputs executable named "rotary")
    make run        -compiles and runs
    make clean      -deletes executable
