//this class is created at runtime (not saved) from map data, it allows us to save
//pointers and things for quick access.  Basically another array of tiles for direct
//access.

class CTileExtra
{
public:
   int i_x, i_y; //our place in the map
   int i_wall_tex[5];
   int i_wall; //don't need to save this, built when walls are built
   int i_roof_index;
 
   
   void set_defaults()
   {
	 //set defaults
	   i_wall_tex[0] = 1;
	   i_wall_tex[1] = 1;
	   i_wall_tex[2] = 1;
	   i_wall_tex[3] = 1;
	   i_wall_tex[4] = 2;
	   i_wall = -1;
	   i_roof_index = -1;
   }

};