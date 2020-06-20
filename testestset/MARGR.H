#include "graphics.h"
//#include <conio.h>
#include <dos.h>
#include <stdio.h>
#include "def.h"

# define  kESC            0x011b
# define  kRIGHT          0x4D00
# define  kLEFT           0x4B00
# define  kUP             0x4800
# define  kDOWN           0x5000
# define  kHOME           0x4700
# define  kEND            0x4F00
# define  kF1		  0x3B00
# define  flip		  790         // The Top Y Cordinate On PS Page


const int size_First_cell_arrow_array = 8;

extern int Palette[] ; // = { BLACK, WHITE, GREEN, RED, YELLOW, BLUE };
extern char 	ps_flag;
extern char 	Name[13];
extern FILE 	*out;
extern int	xx;
extern int 	yy;
// types of possible arrows - needs for arrow indexing
enum arrow_direct { rEastAr, rWestAr , rNorthAr, rSouthAr, 
		    dEastAr, dWestAr , dNorthAr, dSouthAr };

class graphics {
protected:
	int Width;
	int Hight;
	int Gdriver;
	int Gmode;
	int BackGrCol;
	int ForGrCol;
	int CLIP_ON;
public:
	    //  Constructor
	graphics(int,int);
	    //  Destructor
	~graphics() { closegraph(); }
	    // Draw string
	void draw_str(char* name, int x, int y, int Color= -1, int Factor= 0);
	    // Draw polygon
	void draw_poly(int PointNum, int[] , int Color = -1,
			int FillStyle = 0);
	void draw_point( int x, int y, int Color = -1);
        void set_p( Point_coord& point, int x, int y)
	    { point.x = x; point.y = y; }
};
// -----------------------------------------------------------------
class Gwindow {
	int MaxX;
	int MaxY;
	int Factor;
	int Xstart;
        int Ystart;
public:
	// Constructor
	Gwindow(graphics* Graph_tmp);

	// Destructor
	~Gwindow() {
	    cleardevice();
	    closegraph();
	}
      
};
// ---------------------------------------------------

class GraphicWin  : public graphics {

protected:
  int Origin_x; //   |--------->
  int Origin_y; //   |	 | \ delta
  int delta; //      |---| /
	     //	     \/
  int delta_inner;
  int delta_outer;
  int arrow_shift;
  int center_x;
  int center_y;
  int MaxWinSize;
  float Factor;// used as factor in zooming function

  // data for drawing rectangles and diamonds
  Points_coord rect_LU_corner, rect_RD_corner; // rect_coordinates
  Points_coord  Ldiamond, Rdiamond, Udiamond, Ddiamond;

  Point_coord L1_U1, L1_U2, L1_D1, L1_D2, L1_BD,
	      L2_U1, L2_U2, L2_D1, L2_D2, L2_BD,
	      R2_U1, R2_U2, R2_D1, R2_D2, R2_BD,
	      R1_U1, R1_U2, R1_D1, R1_D2, R1_BD,
	      BR_U1, BR_U2, BR_D1, BR_D2, BR_BD,
	      W_L2,  W_L1,  E_R1,  E_R2,  E_BR,
	      N_U1,  N_U2,  S_D1,  S_D2,  S_BD,
	      CNT, E_R, W_R, N_R, S_R;

  Point_coord screen_points[numIntersectPoints];   // intersection points
  ElemArrow FirstCellArrows[size_First_cell_arrow_array];
  int textColor;
  // calculates two tails points of arrow
  // accordently to the direction.
  void calc_arrow_points(int [], direction);

 public: // constructor
      GraphicWin( int x, int y, int z, int txtColor);
      ~GraphicWin(){;}
      void set_factor(float fact);
      float get_factor() { return Factor; }
      void delta_calc(int num);// calculation of delta according
	     // to num_of_elements in given flow_chart
      void set_p( Point_coord& point, int x, int y)
	    { point.x = x; point.y = y; }
      void set_first_cell_points();
      void change_origins( int x, int y);
      void set_X_Y_cell(Cell_arr& cl, int x, int y );
      void display_cell_points(Cell_arr cell);
      int  check_font_size();
      void draw_path_pair(List_Intersect_Elem Turns_Lst);
      void draw_port_str(ancest_dist* anc_ptr, int son,
                         int ind_anc, points P);
      void translate_to_pixel(intersect_elem* turn_ptr,
				int& x_coord, int& y_coord);
      int  delta_() { return delta; }

      void Arrow_calc(arrow_direct k, Point_coord dirEast,
       		      Point_coord dirWest,
			Point_coord dirNorth, Point_coord dirSouth);
      void draw_arrow(int x_graph, int y_graph, int x, int y,
		       direction direct, points p);
      void call_draw_arrow(intersect_elem* curr, intersect_elem* prev);


      void draw_pathes();
      void redraw_graph(GraphicWin* gptr, int xShift, int  yShift);

      void draw_rectangle(int cntr_x,int cntr_y ,
			  int Color, int FillStyle);
      void draw_diamond(int c_x, int c_y, int Color);
      void draw_circle(int c_x, int c_y, int Color);
      void draw_txt(int cx, int cy, char* name, int Color);
      void set_txt_just(){
		// set text justifying
		settextjustify( CENTER_TEXT , CENTER_TEXT  );
	    }
      void set_view_port(int left, int top, int right, int bottom)
	    {   setviewport (left, top, right, bottom, 1);  }
      /*void clear_draw_area()
	   {  clearviewport(); }*/

};
