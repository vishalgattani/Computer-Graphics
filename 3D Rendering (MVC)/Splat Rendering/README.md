#  3D Rendering Using MVC Architecture

Render a 3D scene using more than two objects in the scene. The objects will be rendered using their surface
meshes given in .ply file format.

Implement a splat for each triangle in the mesh, where a splat is a filled circle contained in the
triangle, placed at the incircle of the triangle in the mesh. The circle must be coloured using the
normal coordinates, (R,G,B) corresponding to the (x-,y-,z-) coordinates of the normal vector. [There
can be different display options for the mesh – wireframe , filled triangles, just vertices without the
edges, and as splats.]

Usually splats are implemented on vertices and not surface elements. Implemented the same for a vertex in this code.

![Splat_pointsize1(1)](https://user-images.githubusercontent.com/24211929/54107096-30bda580-43fe-11e9-8c28-a2104475c135.png)
![Splat_pointsize1(2)](https://user-images.githubusercontent.com/24211929/54107098-30bda580-43fe-11e9-8577-9fe07767bec9.png)
![Splat_pointsize3(1)](https://user-images.githubusercontent.com/24211929/54107099-30bda580-43fe-11e9-8c6b-90e5ef877cc4.png)
![Splat_pointsize3(2)](https://user-images.githubusercontent.com/24211929/54107102-31563c00-43fe-11e9-940b-026f6a1fe9f7.png)
