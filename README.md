# HeightMap

## Description
The program loads a black and white image and perform edge detection thanks to Canny algorithm. Then it converts the original image, the processed one and the intermediate steps as height maps to display them using OpenGL. 

It is possible to activate a plan that enables to highlight edges over a threshold. 

Shadows, diffuse and specular lightings are simulated for a better rendering.

It is also possible to save the displayed image.


## Instructions
The project requires a ***C++11*** capable compiler, ***OpenGL 3.3***, ***Qt 5.6*** and ***QtCreator 4*** or later.

To launch it, open ***src/heightMapQt.pro*** with QtCreator.

## Results
![raw](/results/city_raw.png)
*Height map corresponding to the original image*

![Canny](/results/city_canny.png)
*Height map corresponding to the Canny image with a plan to hightlight edges*
