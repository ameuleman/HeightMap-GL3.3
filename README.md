# HeightMap

## Description
The program load a black and white image and perform edge detection thanks to Canny algorithm. Then it convert the original image, the processed one and the intermediate steps as height maps to display them using OpenGL. 

It is possible to activate a plan that enables to highlight edges over a threshold. 

Shadows, diffuse and specular lightings are simulated for a better rendering.

It is also possible to save the displayed image.


## Instructions
The project requires ***C++11***, ***OpenGL 3.3***, ***Qt 5.6*** and ***QtCreator 4*** or later.

To launch it, open ***src/heightMapQt.pro*** with QtCreator.

To control the display, use **ZQSD** to rotate the model, **arrows** to rotate the light source, **space bar** to make the plan appear or disappear, **RF** to raise or lower it and **W** to save the current rendering as an image.
