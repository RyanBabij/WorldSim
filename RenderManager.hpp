/* #inc

RenderManager.hpp

Container for classes that do rendering. Handles device io and panels.

*/


/* Objects that can render, recieve device IO, etc, inherit this. */
class CanRender
{
   int panelX1, panelY1, panelX2, panelY2; /* Coordinates that the object can render within. */
   bool active; /* If false, the panel won't render or do IO. */
};



class RenderManager
{
   public:
   int panelX1, panelY1, panelX2, panelY2; /* Coordinates for the renderManager to render within. */
   bool active; /* Whether the render manager should do anything. */
   
   //Vector <>
   
   
};


