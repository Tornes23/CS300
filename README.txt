/* ---------------------------------------------------------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen 
Institute of Technology is prohibited.
Project:    cs300_nestor.uriarte_0

Author:     Nestor Uriarte    nestor.uriarte        540000817
Creation date: 27/5/2020
----------------------------------------------------------------------------------------------------------*/
• How to run your program: 
   The program can run in release and debug in x64 setting.

• How to use your program:
   Numbers 1 to 5: Change the shape to be rendered
    ▪ 1: Plane
    ▪ 2: Cube
    ▪ 3: Cone
    ▪ 4: Cylinder
    ▪ 5: Sphere
   N: Toggle normal rendering
   T: Toggle texture-mapping on/off
   F: Toggle face/averaged normal (requires normal rendering active)
   M: Toggle wireframe on/off
   Arrows Up/Down: Rotate the shape along X-axis
   Arrows Right/Left: Rotate the shape along Y-axis
   +/-: Increase/reduce number of slices (Minimum of 3 maximum of 500)

• Important parts of the code: 
   On Model.cpp CreateSphere() a big part of the logic was based on http://www.songho.ca/opengl/gl_sphere.html
   the logic was made by Song Ho Ahn.

• Known issues and problems: 
   The normals of the cone are not correct probably since the scale of the object is not uniform
