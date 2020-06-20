class ancest_dist;

//--------------------------------------------------------------------

enum points { no_point = -1,
	      l2_u2, l2_u1, w_l2,  l2_d1, l2_d2, l2_bd,
	      l1_u2, l1_u1, w_l1,  l1_d1, l1_d2, l1_bd,
	      n_u2,  n_u1,  cnt,   s_d1,  s_d2,  s_bd,
	      r1_u2, r1_u1, e_r1,  r1_d1, r1_d2, r1_bd,
	      r2_u2, r2_u1, e_r2,  r2_d1, r2_d2, r2_bd,
	      br_u2, br_u1, e_br,  br_d1, br_d2, br_bd,
	      e, w, n, s, pnum, numIntersectPoints = pnum + 4};

enum lines   { no_line = -1,
	       up2,   up1,   west_east, down1,  down2,  board_down,
	       left2, left1, north_south, right1, right2, board_right,
	       lnum = left2};

class intersect_elem {
      int Cell_X_coord;
      int Cell_Y_coord;
      points PointName;
      intersect_elem* next;
      int direction;

public:
      int h;

      // constructors
      intersect_elem() {}
      intersect_elem(int X, int Y, points P, intersect_elem* pntr, int Dir)
       { Cell_X_coord = X; Cell_Y_coord = Y;
	 PointName = P,  direction = Dir,  next = pntr; }

      intersect_elem(int X, int Y, points P, int Dist, intersect_elem* Pntr)
       { Cell_X_coord = X; Cell_Y_coord = Y;
	 PointName = P; h = Dist;  next = Pntr; }

      intersect_elem(int X, int Y, points P, int Dist, int Dir,
		     intersect_elem* Pntr)
       { Cell_X_coord = X; Cell_Y_coord = Y;
	 PointName = P; h = Dist;  direction = Dir; next = Pntr; }

      int Get_X_coord() { return Cell_X_coord; }
      int Get_Y_coord() { return Cell_Y_coord; }
      points Get_point() { return PointName; }
      int Get_dist() { return h; }
      int Get_dir() { return direction; }
      void Set_x_coord( int coord_x) { Cell_X_coord = coord_x;}
      void Set_y_coord( int coord_y) { Cell_Y_coord = coord_y;}
      void Set_point(points pt) {  PointName = pt; }
      void Set_next(intersect_elem* pntr) { next = pntr; }
      intersect_elem* Get_next() { return next; }
};


class List_Intersect_Elem {
     intersect_elem* first;
     intersect_elem* last;
public:
     List_Intersect_Elem() { first = last = NULL; }
     List_Intersect_Elem(int X, int Y, points P)
      {
	first = last = new intersect_elem(X, Y , P, NULL, -1);
      }
     int LookFirst() { if (first) return 1; return 0; }
     intersect_elem* List_pntr() { return first; }
     intersect_elem* Get_Last() { return last; }
     intersect_elem  Get_Turn(intersect_elem* pntr) {
       intersect_elem elem(pntr->Get_X_coord(),
			   pntr->Get_Y_coord(),
			   pntr->Get_point(),
			   NULL, pntr->Get_dir());
       return elem;
     }
    // void Reset_Curr() { current = first; }
     void Insert(int X, int Y, points p, int dir = -1);  // as Append
     void Order_Insert(int x, int y, points p, int dist, int dir);
     void Insert_First(int x, int y, points p, int dir = -1);

     // append lists
     List_Intersect_Elem& operator&=(List_Intersect_Elem& Turns) {
       if (last)
	 last->Set_next(Turns.first);
       else
	 first = Turns.first;

       last = Turns.last;
       return *this;
     }

     void Remove_Elem(intersect_elem* pntr);
     // set pointers
     void Set_Last_ptr(intersect_elem* pntr) { last = pntr; }

     // deletes list from pointer
     void Delete_List(intersect_elem* = NULL);

     // appends current list by parameter from pointer
     void Append(List_Intersect_Elem&, intersect_elem* = NULL);

     // reverse of list
     void Reverse();

     // ~List_Intersect_Elem() { Delete_List(); }
};


//--------------------------------------------------------------------------


struct Path {
    int Source_N;                          // Source - ancestor index in Vec
    int Target_N;                          // Target - son index in Vec 
    List_Intersect_Elem Turn_List;         // bend points of line

    // constructors
    Path() : Source_N(-1), Target_N(-1) {}
    Path(int Anc, int Son) : Source_N(Anc), Target_N(Son) {}

    // destrustor
    ~Path() { Delete_Path(); }

    void Delete_Path() {
       Source_N = -1;
       Target_N = -1;
       Turn_List.Delete_List();
    }
    void print_path();    // prints transit elements coord and it's lines
};

