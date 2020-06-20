#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "path.h"

#define MINIMAL_SIZE   4
#define Y_SCREEN     480
#define X_SCREEN     480
#define SCREEN_SIZE  250000
#define DENSITY      4
#define THREE        3

typedef struct Points_coord{
	int x;
	int y;
} Point_coord;

typedef struct Cell_points{
	Point_coord center;
	Point_coord arr[ numIntersectPoints ];
} Cell_arr;

// this is  structure which needed for graphics purposes
typedef
struct { int oneArrow[6]; } ElemArrow; // one arrow

inline void set_minmax(int& i, int& j)
{
   if ( i > j ) {
     int tmp = i;
     i = j;
     j = tmp;
   }
}


// from module allocat.cpp
extern int  reserve_flag;
extern void error(char* Str);
extern int  dist_calc(int f_x, int f_y, int s_x, int s_y);
extern void Load_Table(char* Filename);
extern void chart_flow_alloc();

// from module all_conn.cpp
extern int row, column;
extern int FileSize;
extern int MaxSize;
extern double Res_Sq_cell;

// from module graph.cpp
extern void graphic_interface();

//------------------------------------------------------------------------

class ancest_dist {
        int h;
        int N_ancest;
	ancest_dist* next;

public:
	Path my_path;
	int cir_X, cir_Y; // coord of nonconvensional connection

	//list of possible intersect points
	List_Intersect_Elem Possible_Intersect_Lst;
	 
	ancest_dist(): h(-1), N_ancest(-1), next(NULL),
	               cir_X(-1), cir_Y(-1) {}
	ancest_dist(int dist, int N_fath , ancest_dist* pntr):
		       h(dist), N_ancest(N_fath), next(pntr),
	               cir_X(-1), cir_Y(-1) {}

	int Get_dist() { return h; }
	int Get_ancest() { return N_ancest; }
	ancest_dist* Get_next() { return next; }
	void Set_dist(int dist) { h = dist; }
	void Set_ancest(int ancest) { N_ancest = ancest; }
	void Set_next(ancest_dist* pntr) { next = pntr; }
	void Copy_Lst(List_Intersect_Elem& Lst); // copy intersects to
	                                        // Possible_Intersect_Lst
	
};


//------------------------------------------------------------------------


class List {
     ancest_dist* first;
     ancest_dist* last_connected;

     public:
     List() { first = NULL;
	      last_connected = new ancest_dist(-1, -1, NULL); }
     List(int dist, int N_ancest)
      {
	first = new ancest_dist(dist, N_ancest, NULL);
	last_connected = new ancest_dist(-1, -1, first);
      }
     int LookFirst() { if (first) return 1; return 0;}

     void Set_List_pntr(ancest_dist* pntr) { first = pntr; }
     void Set_last_conn(ancest_dist *prev = NULL, ancest_dist *curr = NULL);

     ancest_dist* List_pntr() { return first; }
     ancest_dist* Get_last_conn() { return last_connected; }

     void Insert_First(ancest_dist* new_first, ancest_dist* new_ptr);
     void Insert(int dist, int father);
     
     void Delete_Elem(int NumVecAncest); // use in procedure 'Connection'
     
     int Get();
     ~List();
};

//------------------------------------------------------------------------


typedef struct figure_shape {
    char  name[6];
    int   left;
    int   right;
    Path* direct_ancest;
    List ancest_list;
    int X;
    int Y;
    figure_shape() : left(-1), right(-1),
		     direct_ancest(NULL) {
      name[0] = '\0';
    }
} shape;



enum shape_type { rectangle_type, diamond, circle_type };

enum direction { SN, WE, NS, EW };

// class of the turns
class turn {
	int Full; // 0 is means empty, 1 - horizontal line used
		  // 2 - vertical line used, 3 - both.
public:
	// constructors
	turn() : Full(0) {};

	turn(int fill) : Full(fill) {};

	// checks if turn is full in the corresponding direction
	int is_full(int dir) { return (Full & (1<<dir)); }

	// return full
	int is_full() { return Full; }

	// release point
	void release() { Full = 0; }

	// set the corresponding direction filling,
	// if turn is empty set also father and son
	void fill(int dir = -1) {
	    if (dir != -1) Full |= (1<<dir);
	    else Full = 3;  // by default fill two directions
	}
};

struct screen_elem {
	int Full;
	shape_type Type;
	int Vec_left_son;
	int Vec_right_son;
	char  Name[6];
	int Vec_node;
	Cell_arr crossPoint; // cross points
	ElemArrow CellArrows[4];  // arrows  is end of pathes
	turn TurnList[pnum];

	// constructor : init Turn_to_Line
	screen_elem();
	int operator()(int i, int j) {
		 if (i<0 || i>=row || j<0 || j>=column) return 1;
		 return 0;
	}
};

class Queue {
	friend class Queue_list;
	Queue *next;
	int Index_Vec;
public:
	Queue( int q_elem , Queue* pntr )
	     {
		Index_Vec = q_elem;
		next = pntr;
	     }
  };

class Queue_list{

      Queue* last;
      Queue* first;

public:
      void Append(int elem);
      int Get();
      int LookFirst();
      Queue_list() { last = first = NULL;}
      Queue_list( int s_elem )
        {
	 first = new Queue(s_elem , NULL);
	 last = first;
	}
};

extern screen_elem **Screen;
extern shape *Vec;
extern points Line_to_Turn[lnum][lnum];
extern lines  Turn_to_Line[e][2];

