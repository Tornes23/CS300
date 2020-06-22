/* ---------------------------------------------------------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen 
Institute of Technology is prohibited.
Project:    cs300_nestor.uriarte_2

Author:     Nestor Uriarte    nestor.uriarte        540000817
Creation date: 22/6/2020
----------------------------------------------------------------------------------------------------------*/
• How to run your program: 
   The program can run in release and debug in x64 setting.

• How to use your program:
   
   Keys to move the camera:
   ▪ W: Move up.
   ▪ S: Move down.
   ▪ A: Move left.
   ▪ D: Move right.
   ▪ E: Further from object.
   ▪ Q: Closer to object. 

   Numbers 1 to 5: Change the shape to be rendered:
    ▪ 1: Plane
    ▪ 2: Cube
    ▪ 3: Cone
    ▪ 4: Cylinder
    ▪ 5: Sphere

   Render modifications:
    ▪ P: Toggle to pause/start the light animation.
    ▪ N: Toggle normal rendering
    ▪ F: Toggle face/averaged normal
    ▪ M: Toggle wireframe mode on/off
    ▪ T: Change between rendering modes:
        ▪ Normal Mapping: Scene with lighting and normal mapping with texture
        ▪ Normals: Geometry normal in camera space as color
        ▪ Tangent: Geometry tangent in camera space as color
        ▪ Bitangent: Geometry bitangent in camera space as color
        
    ▪ Arrows Up/Down: Rotate the shape along X-axis
    ▪ Arrows Right/Left: Rotate the shape along Y-axis
    ▪ +/-: Increase/reduce number of slices (Minimum of 3 maximum of 500)
    
   Lights:
    ▪ L: Remove light (maximum of 8 lights)
    ▪ R: Add a lights (minimum of 1)

• Important parts of the code: 

• Known issues and problems: 
The averaged Tangents on the cube give weird color output, don't know why