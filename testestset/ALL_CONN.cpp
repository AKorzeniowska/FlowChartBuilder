//#include <conio.h>
#include "def.h"
#include "all_conn.h"
#include <stdio.h>
#include <iostream>

int     FileSize;
int     MaxSize = 10, use_Reserve = 0, non_Convent = 0;
double  Res_Sq_cell;
int     column, row, * arr_port;
char    ps_flag;
char 	Name[256];

screen_elem** Screen;
shape* Vec;
points Line_to_Turn[lnum][lnum] =
{ l2_u2, l2_u1, w_l2,  l2_d1, l2_d2, l2_bd,
  l1_u2, l1_u1, w_l1,  l1_d1, l1_d2, l1_bd,
  n_u2,  n_u1,  cnt,   s_d1,  s_d2,  s_bd,
  r1_u2, r1_u1, e_r1,  r1_d1, r1_d2, r1_bd,
  r2_u2, r2_u1, e_r2,  r2_d1, r2_d2, r2_bd,
  br_u2, br_u1, e_br,  br_d1, br_d2, br_bd };

lines Turn_to_Line[e][2] = { {up2,       left2},
				{up1,       left2},
				{west_east, left2},
				{down1,     left2},
				{down2,     left2},
				{board_down,left2},
				{up2,       left1},
				{up1,       left1},
				{west_east, left1},
				{down1,     left1},
				{down2,     left1},
				{board_down,left1},
				{up2,       north_south},
				{up1,       north_south},
				{west_east, north_south},
				{down1,     north_south},
				{down2,     north_south},
				{board_down,north_south},
				{up2,       right1},
				{up1,       right1},
				{west_east, right1},
				{down1,     right1},
				{down2,     right1},
				{board_down,right1},
				{up2,       right2},
				{up1,       right2},
				{west_east, right2},
				{down1,     right2},
				{down2,     right2},
				{board_down,right2},
				{up2,       board_right},
				{up1,       board_right},
				{west_east, board_right},
				{down1,     board_right},
				{down2,     board_right},
				{board_down,board_right}
};

//--------------------------------------------------------------------------
// Fills Screen matrix (turns)
// Parameters :  Lst    -  list of turns,
//		 Father -  father's index in Vec,
//		 Son    -  son's index in Vec.
void
screen_filling(List_Intersect_Elem& Lst)
{
	intersect_elem* cur = Lst.List_pntr();
	int X, Y, dir;
	points p;

	while (cur) {
		X = cur->Get_X_coord();
		Y = cur->Get_Y_coord();
		p = cur->Get_point();
		dir = cur->Get_dir();

		// fill father and son in turn of givven element
		Screen[X][Y].TurnList[p].fill(dir);

		// get next point
		cur = cur->Get_next();

	} // end 'while' of turns
} // end of function 'screen_filling'


//---------------------------------------------------------------------
// return corner produced by two points
// Parameters: son_P - son point
//             anc_P - ancestor point
//             dir   - direction from the corner to the ancestor
points
get_corner(points son_P, points anc_P, int dir)
{
	if (anc_P == no_point)
		anc_P = cnt;
	if (son_P == no_point)
		son_P = cnt;

	lines son_L = Turn_to_Line[son_P][!dir],
		anc_L = Turn_to_Line[anc_P][dir];

	if (dir)
		return Line_to_Turn[anc_L - left2][son_L - up2];
	else
		return Line_to_Turn[son_L - left2][anc_L - up2];
}

//------------------------------------------------------------------
// checks if points lie on the one line
int
points_on_line(points P1, points P2, int dir = -1)
{
	if (P1 == no_point) P1 = cnt;
	if (P2 == no_point) P2 = cnt;

	if (dir == -1)
		return (Turn_to_Line[P1][0] == Turn_to_Line[P2][0] ||
			Turn_to_Line[P1][1] == Turn_to_Line[P2][1]);
	else
		return (Turn_to_Line[P1][dir] == Turn_to_Line[P2][dir]);

}


//------------------------------------------------------------------------
// Connect two points of the element
// Parameters : X       - element x coord,
//              Y       - element Y coord,
//              St_p    - start point ( of St_ind ),
//              Fin_p   - final point ( of Fin_ind ),
//              dir     - loop direction,
//              P_list  - produces point list.
//
// Returns 0 if all points on the line are empty and 1 otherwise

int
line_loop(int X, int Y, points St_p, points Fin_p,
	direction dir, List_Intersect_Elem& P_list)
{
	int is_Vert = (dir == SN || dir == NS); // horisontal or vertical direction
	int is_Incr = (dir == WE || dir == NS); // loop direction incr. or decr.
	lines Line = Turn_to_Line[St_p][is_Vert];
	register int i, Start = 0;
	points ti = no_point;

	if (is_Incr)

		// incremental loop
		for (i = 0; i < lnum, ti != Fin_p; i++) {

			if (is_Vert) ti = Line_to_Turn[Line - left2][i];
			else           ti = Line_to_Turn[i][Line - up2];

			if (ti == St_p) // turn is >=  St_p
				Start = 1;

			if (Start)
				// check filling of the point
				if (Screen[X][Y].TurnList[ti].is_full(is_Vert)) return 1;
				else P_list.Insert(X, Y, ti, is_Vert);

		} // end increment
	else
		// decremental loop
		for (i = lnum; i, ti != Fin_p; i--) {

			if (is_Vert) ti = Line_to_Turn[Line - left2][i - 1];
			else           ti = Line_to_Turn[i - 1][Line - up2];

			if (ti == St_p) // turn is <= St_p
				Start = 1;

			if (Start)
				// check filling of the point
				if (Screen[X][Y].TurnList[ti].is_full(is_Vert)) return 1;
				else P_list.Insert(X, Y, ti, is_Vert);

		}  // end decrement

	return 0;
}

//------------------------------------------------------------------------
// Connect two direct line elements and set list of turns
// Parameters : Ret_List - building list of the points,
//              st_X     - start element X coord,
//              st_Y     - start element Y coord,
//              fin_X    - final element X coord,
//              fin_Y    - final element Y coord,
//              St_p     - start point,
//              Fin_p    - final point.
//
// Returns 0 if all points on the path are empty and 1 otherwise
//
int
connect_elem_points(List_Intersect_Elem& Ret_List,
	int st_X, int st_Y, int fin_X, int fin_Y,
	points Start_p, points Final_p)
{
	points St_p = Start_p, Fin_p = Final_p, point;
	int P_reset = 0, // 1 if point was reset
		ret_val = 0,
		dir;

	if (Start_p == Final_p) { // one point
		Ret_List.Insert(st_X, st_Y, Start_p, 0);
		return ret_val;
	}

	// different points

	if (St_p == no_point) {
		St_p = cnt;
		P_reset = 1;
	}
	else
		if (Fin_p == no_point) {
			Fin_p = cnt;
			P_reset = 2;
		}

	// temporary release cnt if it is needed
	if (P_reset && Screen[st_X][st_Y].Full)
		Screen[st_X][st_Y].TurnList[cnt].release();

	// horizontal line
	if (Turn_to_Line[St_p][0] == Turn_to_Line[Fin_p][0]) {
		dir = 0;
		if (St_p >= Fin_p) { // EW
			if (Start_p == no_point)
				point = w;
			else if (Final_p == no_point)
				point = e;
			ret_val = line_loop(st_X, st_Y, St_p, Fin_p, EW, Ret_List);
		}
		else { // WE
			if (Start_p == no_point)
				point = e;
			else if (Final_p == no_point)
				point = w;
			ret_val = line_loop(st_X, st_Y, St_p, Fin_p, WE, Ret_List);
		}
	}
	else  // vertical line
		if (Turn_to_Line[St_p][1] == Turn_to_Line[Fin_p][1]) {
			dir = 1;
			if (St_p >= Fin_p) { // SN
				if (Start_p == no_point)
					point = n;
				else if (Final_p == no_point)
					point = s;
				ret_val = line_loop(st_X, st_Y, St_p, Fin_p, SN, Ret_List);
			}
			else {  //NS
				if (Start_p == no_point)
					point = s;
				else if (Final_p == no_point)
					point = n;
				ret_val = line_loop(st_X, st_Y, St_p, Fin_p, NS, Ret_List);
			}
		}
		else
			error("direct_connect: points of different lines were choosed!\n");

	// fill temporary released cnt
	if (P_reset && Screen[st_X][st_Y].Full) {
		Screen[st_X][st_Y].TurnList[cnt].fill();
		if (P_reset == 1) {
			Ret_List.Remove_Elem(Ret_List.List_pntr());
			if (!Screen[st_X][st_Y].TurnList[point].is_full())
				Ret_List.Insert_First(st_X, st_Y, point, dir);
			else return 1;
		}
		else {
			Ret_List.Remove_Elem(Ret_List.Get_Last());
			if (!Screen[fin_X][fin_Y].TurnList[point].is_full())
				Ret_List.Insert(fin_X, fin_Y, point, dir);
			else return 1;
		}
	}

	return ret_val;
}

//--------------------------------------------------------------------
// Connect two direct line elements and set list of turns
// Parameters : Ret_List - building list of the points,
//              st_X     - start element X coord,
//              st_Y     - start element Y coord,
//              fin_X    - final element X coord,
//              fin_Y    - final element Y coord,
//              St_p     - start point,
//              Fin_p    - final point.
//
// Returns 0 if all points on the path are empty and 1 otherwise
//
int
direct_connect(List_Intersect_Elem& Ret_List,
	int st_X, int st_Y, int fin_X, int fin_Y,
	points Start_p = no_point, points Final_p = no_point)
{
	int i;
	points Fin_p = Final_p, St_p = Start_p;
	lines line;

	// if board point (st. or fin.) is eq. no_point,
	// set it next after corresponding elem. exit

	if (fin_X == st_X) {  // one row

		if (fin_Y == st_Y) {  // one element
			if (points_on_line(Start_p, Final_p))
				return (connect_elem_points(Ret_List, st_X, st_Y,
					fin_X, fin_Y, Start_p, Final_p));
			else
				return 1;
		}
		else
			if (fin_Y > st_Y) { // W to E
				if (!points_on_line(Start_p, Final_p, 0))
					return 1;
				if (Start_p == no_point) {
					Ret_List.Insert(st_X, st_Y, e, 0);
					St_p = e_r1;
				}

				// set final point as boarder
				line = Turn_to_Line[St_p][0];
				Fin_p = Line_to_Turn[lnum - 1][line - up2];   //e_br;

				// loop to the first board ( same elem. )
				if (line_loop(st_X, st_Y, St_p, Fin_p, WE, Ret_List))
					return 1;

				// set start as boarder
				St_p = Line_to_Turn[0][line - up2];  //w_l2;

				for (i = st_Y + 1; i < fin_Y; i++)
					if (line_loop(st_X, i, St_p, Fin_p, WE, Ret_List))
						return 1;

				// set real final point if need
				if (Final_p == no_point) Fin_p = w_l1;
				else Fin_p = Final_p;

				// loop to the first board ( same elem. )
				if (line_loop(fin_X, fin_Y, St_p, Fin_p, WE, Ret_List))
					return 1;

				if (Final_p == no_point) Ret_List.Insert(fin_X, fin_Y, w, 0);
			}
			else {    // E to W

				if (!points_on_line(Start_p, Final_p, 0))
					return 1;
				if (Start_p == no_point) {
					Ret_List.Insert(st_X, st_Y, w, 0);
					St_p = w_l1;
				}

				// set final point as boarder
				line = Turn_to_Line[St_p][0];
				Fin_p = Line_to_Turn[0][line - up2];   //w_l2;

				// loop to the first board ( same elem. )
				if (line_loop(st_X, st_Y, St_p, Fin_p, EW, Ret_List))
					return 1;

				// set start as boarder
				St_p = Line_to_Turn[lnum - 1][line - up2]; // e_br;

				for (i = st_Y - 1; i > fin_Y; i--)
					if (line_loop(st_X, i, St_p, Fin_p, EW, Ret_List))
						return 1;

				// set real final point if need
				if (Final_p == no_point) Fin_p = e_r1;
				else Fin_p = Final_p;

				// loop to the first board ( same elem. )
				if (line_loop(fin_X, fin_Y, St_p, Fin_p, EW, Ret_List))
					return 1;

				if (Final_p == no_point) Ret_List.Insert(fin_X, fin_Y, e, 0);

			}  // end E to W
	}   // end one row
	else  if (fin_Y == st_Y) {    // one column

		if (fin_X > st_X) { // N to S

			if (!points_on_line(Start_p, Final_p, 1))
				return 1;
			if (Start_p == no_point) {
				Ret_List.Insert(st_X, st_Y, s, 1);
				St_p = s_d1;
			}

			// set final point as boarder
			line = Turn_to_Line[St_p][1];
			Fin_p = Line_to_Turn[line - left2][lnum - 1];   //s_bd;

			// loop to the first board ( same elem. )
			if (line_loop(st_X, st_Y, St_p, Fin_p, NS, Ret_List))
				return 1;

			// set start as boarder
			St_p = Line_to_Turn[line - left2][0]; // n_u2;

			for (i = st_X + 1; i < fin_X; i++)
				if (line_loop(i, st_Y, St_p, Fin_p, NS, Ret_List))
					return 1;

			// set real final point if need
			if (Final_p == no_point) Fin_p = n_u1;
			else Fin_p = Final_p;

			// loop to the first board ( same elem. )
			if (line_loop(fin_X, fin_Y, St_p, Fin_p, NS, Ret_List))
				return 1;

			if (Final_p == no_point) Ret_List.Insert(fin_X, fin_Y, n, 1);
		}   // end N to S
		else {   // S to N

			if (!points_on_line(Start_p, Final_p, 1))
				return 1;
			if (Start_p == no_point) {
				Ret_List.Insert(st_X, st_Y, n, 1);
				St_p = n_u1;
			}

			// set final point as boarder
			line = Turn_to_Line[St_p][1];
			Fin_p = Line_to_Turn[line - left2][0];   //n_u2;

			// loop to the first board ( same elem. )
			if (line_loop(st_X, st_Y, St_p, Fin_p, SN, Ret_List))
				return 1;

			// set start as boarder
			St_p = Line_to_Turn[line - left2][lnum - 1]; // s_bd;

			for (i = st_X - 1; i > fin_X; i--)
				if (line_loop(i, st_Y, St_p, Fin_p, SN, Ret_List))
					return 1;

			// set real final point if need
			if (Final_p == no_point) Fin_p = s_d1;
			else Fin_p = Final_p;

			// loop to the first board ( same elem. )
			if (line_loop(fin_X, fin_Y, St_p, Fin_p, SN, Ret_List))
				return 1;

			if (Final_p == no_point) Ret_List.Insert(fin_X, fin_Y, s, 1);
		} // end S to N
	}  // end one column
	else
		return 1;

	return 0;
}

// ------------------------------------------------------------------------
// Connects direct elements and creates Path
// Parameters : Father - index in Vec
//              Son    - index in Vec
void
connect_direct_elem(int Father, int Son)
{

	Vec[Son].direct_ancest = new(Path);
	direct_connect(Vec[Son].direct_ancest->Turn_List,
		Vec[Son].X, Vec[Son].Y, Vec[Father].X, Vec[Father].Y);

	Vec[Son].direct_ancest->Source_N = Father;
	Vec[Son].direct_ancest->Target_N = Son;

} // end of function 'connect_direct_elem'

// -------------------------------------------------------
// Checks if two elements of Vec are allocated on one row
// or column.
// Parameters :  Father - index in Vec,
//               Son    - index in Vec.
int
is_direct(int Father, int Son)
{
	int St, Fin;

	if (Vec[Son].direct_ancest)  // element has direct ancestor
		return 0;

	if (Vec[Father].X == Vec[Son].X) {  // one row

		if (Vec[Father].Y == Vec[Son].Y) // one element
			return 0;

		St = Vec[Father].Y;
		Fin = Vec[Son].Y;

		set_minmax(St, Fin);
		for (register int i = St + 1; i < Fin; i++)
			// checks empty elements between FatherEl and SonEl
			if (Screen[Vec[Son].X][i].Full)
				return 0;  // there isn't direct connection

		return 1;  // we have direct connection
	}

	if (Vec[Father].Y == Vec[Son].Y) {  // one column

		St = Vec[Father].X;
		Fin = Vec[Son].X;

		set_minmax(St, Fin);
		for (register int i = St + 1; i < Fin; i++)
			// checks empty elements between FatherEl and SonEl
			if (Screen[i][Vec[Son].Y].Full)
				return 0;  // there isn't direct connection

		return 1;  // we have direct connection 
	}

	return 0;
}  // end of function 'is_direct'



// ------------------------------------------------------- 
void
creation_ancest_list()
{
	int Dist;

	for (register int i = 0; i < FileSize; i++) {
		if (Vec[i].left) {

			Dist = dist_calc(Vec[i].X, Vec[i].Y,
				Vec[Vec[i].left].X, Vec[Vec[i].left].Y);
			if (is_direct(i, Vec[i].left))
				connect_direct_elem(i, Vec[i].left);
			else
				Vec[Vec[i].left].ancest_list.Insert(Dist, i);
		}

		if (Vec[i].right) {

			Dist = dist_calc(Vec[i].X, Vec[i].Y,
				Vec[Vec[i].right].X, Vec[Vec[i].right].Y);
			if (is_direct(i, Vec[i].right))
				connect_direct_elem(i, Vec[i].right);
			else
				Vec[Vec[i].right].ancest_list.Insert(Dist, i);
		}
	}
}
// ---------------------------------------------------------------
// Produces intersect points to the next ancestor.
// Parameters -
void
distribute_points(Path* Source_Path,
	ancest_dist* Next_Anc,
	List_Intersect_Elem* Turn_List_ptr = nullptr)
{
	intersect_elem* curr_ptr;
	List_Intersect_Elem* Turn_List = &Source_Path->Turn_List;

	// filling of Screen matrix
	screen_filling(Source_Path->Turn_List);

	// save first and last point of path
	intersect_elem first_elem =
		Turn_List->Get_Turn(Turn_List->List_pntr());
	intersect_elem last_elem =
		Turn_List->Get_Turn(Turn_List->Get_Last());

	// removing first and last point from path
	Turn_List->Remove_Elem(Turn_List->List_pntr());
	Turn_List->Remove_Elem(Turn_List->Get_Last());

	// copy Turn_List to Possible_Intersect_Lst of the next ancestor 
	if (Next_Anc)
		Next_Anc->Copy_Lst(*Turn_List);

	Turn_List->Delete_List();

	// for drawing polygon in Graphics
	Turn_List->Insert(first_elem.Get_X_coord(),
		first_elem.Get_Y_coord(),
		first_elem.Get_point());

	// append list of turns
	if (Turn_List_ptr)
		if (Turn_List_ptr->List_pntr()) {
			curr_ptr = Turn_List_ptr->List_pntr();

			while (curr_ptr) {
				Turn_List->Insert(curr_ptr->Get_X_coord(),
					curr_ptr->Get_Y_coord(),
					curr_ptr->Get_point());
				curr_ptr = curr_ptr->Get_next();
			}
			Turn_List_ptr->Delete_List();
		}

	Turn_List->Insert(last_elem.Get_X_coord(),
		last_elem.Get_Y_coord(),
		last_elem.Get_point());

} // end of procedure 'distribute_points'

//---------------------------------------------------------------------------
// returns 0 if line connected to elements over only empty elements
int
line_is_full(int curr_X, int curr_Y, int corner_X, int corner_Y)
{
	register int i;

	if (curr_X == corner_X) { // horizontal line

		set_minmax(curr_Y, corner_Y);
		for (i = curr_Y; i <= corner_Y; i++)
			if (Screen[curr_X][i].Full) return 1;
	}
	else if (curr_Y == corner_Y) { // vertical case
		set_minmax(curr_X, corner_X);
		for (i = curr_X; i <= corner_X; i++)
			if (Screen[i][curr_Y].Full) return 1;
	}
	else error("line_is_full: Diagonal line was choosen !");

	return 0;
}

// --------------------------------------------------------------- 
// produce intersect points and fills Possible_Intersect_Lst
// of first ancestor in List of undirect ancestors
void
distribute_direct_points()
{
	ancest_dist* first_anc_ptr;
	points point;
	int x_coord, y_coord;

	for (register int i = 1; i < FileSize; i++)
		if (Vec[i].direct_ancest) {

			// copy Turn_List to Possible_Intersect_Lst of first ancestor in List
			// of non-direct ancestors
			first_anc_ptr = Vec[i].ancest_list.List_pntr();

			if (first_anc_ptr) {
				// temporary allocate first of direct path
				List_Intersect_Elem* Turns = &(Vec[i].direct_ancest->Turn_List);

				point = Turns->List_pntr()->Get_next()->Get_point();
				x_coord = Turns->List_pntr()->Get_next()->Get_X_coord();
				y_coord = Turns->List_pntr()->Get_next()->Get_Y_coord();
				Screen[x_coord][y_coord].TurnList[point].fill();
				arr_port[i] = 1;
			}

			distribute_points(Vec[i].direct_ancest, first_anc_ptr);
		}  // end 'if' (case of direct ancestor)
} // end of function 'distribute_direct_points'

//-----------------------------------------------------------------
// checks if line for point is lie on the port of some full element
int
is_elem_port_line(int son_X, int son_Y,
	int anc_X, int anc_Y, points anc_P)
{
	int is_Vert = (son_Y == anc_Y);
	points Pnt = get_corner(cnt, anc_P, is_Vert);

	if (line_is_full(son_X, son_Y, anc_X, anc_Y))
		if (is_Vert)
			return (Pnt == w_l1) || (Pnt == e_r1);
		else
			return (Pnt == n_u1) || (Pnt == s_d1);

	return 0;
}


//------------------------------------------------------------
// connects point of the elements on the diagonal
int
connect_diag_elem(ancest_dist* anc_ptr, int son_X, int son_Y,
	int anc_X, int anc_Y, List_Intersect_Elem& Turn,
	points son_P, points anc_P)
{
	if (son_P == no_point && anc_P == no_point &&
		(son_X == anc_X || son_Y == anc_Y)) return 1;

	points corner_P;

	corner_P = get_corner(son_P, anc_P, 1);

	if (!(Screen[son_X][anc_Y].TurnList[corner_P].is_full() ||
		!use_Reserve &&
		(is_elem_port_line(son_X, son_Y, son_X, anc_Y, corner_P) ||
			is_elem_port_line(son_X, anc_Y, anc_X, anc_Y, corner_P)))) {
		// try to connect it to the son element

		if (!(direct_connect(anc_ptr->my_path.Turn_List, son_X, son_Y,
			son_X, anc_Y, son_P, corner_P) ||
			direct_connect(anc_ptr->my_path.Turn_List, son_X, anc_Y,
				anc_X, anc_Y, corner_P, anc_P))) {

			Turn.Insert(son_X, anc_Y, corner_P);
			return 0;
		}
	}
	else {
		corner_P = get_corner(son_P, anc_P, 0);

		if (!(Screen[anc_X][son_Y].TurnList[corner_P].is_full() ||
			!use_Reserve &&
			(is_elem_port_line(son_X, son_Y, anc_X, son_Y, corner_P) ||
				is_elem_port_line(anc_X, son_Y, anc_X, anc_Y, corner_P)))) {
			// try to connect it to the another cnt. element

			if (!(direct_connect(anc_ptr->my_path.Turn_List, son_X, son_Y,
				anc_X, son_Y, son_P, corner_P) ||
				direct_connect(anc_ptr->my_path.Turn_List, anc_X, son_Y,
					anc_X, anc_Y, corner_P, anc_P))) {

				Turn.Insert(anc_X, son_Y, corner_P);
				return 0;
			}
		}
	}

	anc_ptr->my_path.Turn_List.Delete_List();

	return 1;
} // end of 'connect_diag_elem'

//---------------------------------------------------------------
int
zip_line(ancest_dist* anc_ptr, int son_X, int son_Y, int anc_X, int anc_Y,
	direction dir, List_Intersect_Elem& Turn,
	points son_P = no_point, points anc_P = no_point)
{
	List_Intersect_Elem Turn_List_S,
		Turn_List_A,
		Turn_List_B,
		Turn_List_D;
	intersect_elem* curr;
	int curr_X, curr_Y, corner_X, corner_Y;
	int is_Vert = (dir == SN || dir == NS),
		// store distance between last trying to connect
		last_dist = column + row;
	points curr_P, corner_P, nport2;
	lines line;

	switch (dir) {

	case SN:

		if (son_P == no_point)
			nport2 = n_u2;

		else { // not element zip
			line = Turn_to_Line[son_P][1];
			nport2 = Line_to_Turn[line - left2][0]; // n_u2
		}

		direct_connect(Turn_List_S, son_X, son_Y, 0, son_Y,
			son_P, nport2);
		nport2 = n_u1;
		break;

	case NS:
		if (son_P == no_point)
			nport2 = s_bd;

		else { // not element zip
			line = Turn_to_Line[son_P][1];
			nport2 = Line_to_Turn[line - left2][lnum - 1]; // s_bd
		}

		direct_connect(Turn_List_S, son_X, son_Y, row - 1, son_Y,
			son_P, nport2);
		nport2 = n_u1;
		break;

	case EW:

		if (son_P == no_point)
			nport2 = w_l2;

		else { // not element zip
			line = Turn_to_Line[son_P][0];
			nport2 = Line_to_Turn[0][line - up2]; // w_l2
		}

		direct_connect(Turn_List_S, son_X, son_Y, son_X, 0,
			son_P, nport2);
		nport2 = w_l1;
		break;

	case WE:
		if (son_P == no_point)
			nport2 = e_br;

		else { // not element zip
			line = Turn_to_Line[son_P][0];
			nport2 = Line_to_Turn[lnum - 1][line - up2]; // e_br
		}

		direct_connect(Turn_List_S, son_X, son_Y, son_X, column - 1,
			son_P, nport2);
		nport2 = w_l1;
	}  // end of cases

	curr = Turn_List_S.List_pntr();

	// Insert first point and move to the next one
	if (curr) {
		Turn_List_B.Insert(curr->Get_X_coord(), curr->Get_Y_coord(),
			curr->Get_point(), curr->Get_dir());

		curr = curr->Get_next();
	}

	while (curr) {
		// loop for all points except anc. elem
		curr_X = curr->Get_X_coord();
		curr_Y = curr->Get_Y_coord();
		curr_P = curr->Get_point();

		// Changes
		Turn_List_B.Insert(curr_X, curr_Y, curr_P, curr->Get_dir());

		if (is_Vert) {
			corner_X = curr_X;
			corner_Y = anc_Y;
		}
		else {
			corner_X = anc_X;
			corner_Y = curr_Y;
		}

		corner_P = get_corner(curr_P, anc_P, is_Vert);

		if (use_Reserve || !(is_elem_port_line(curr_X, curr_Y,
			corner_X, corner_Y, corner_P) ||
			is_elem_port_line(son_X, son_Y,
				curr_X, curr_Y, curr_P))) {

			if (direct_connect(Turn_List_A, corner_X, corner_Y,
				anc_X, anc_Y, corner_P, anc_P)) {

				// if last distance > current it's means that our direction
				// is opposite to the father element
				if (dist_calc(corner_X, corner_Y, anc_X, anc_Y) > last_dist)
					break;
				else
					last_dist = dist_calc(corner_X, corner_Y, anc_X, anc_Y);
			}
			else if (!direct_connect(Turn_List_D, curr_X, curr_Y,
				corner_X, corner_Y, curr_P, corner_P)) {

				Turn_List_D &= Turn_List_A;
				Turn_List_B &= Turn_List_D;

				// insert two turns for the graphics
				Turn.Insert(curr_X, curr_Y, curr_P);
				Turn.Insert(corner_X, corner_Y, corner_P);

				anc_ptr->my_path.Turn_List &= Turn_List_B;

				Turn_List_S.Delete_List(); // Changes
				return 0;
			}
		}  // ignore port points

		// continue loop
		Turn_List_A.Delete_List(); //  Changes !!!
		Turn_List_D.Delete_List();
		curr = curr->Get_next();

	} // end of loop

	Turn_List_S.Delete_List();
	Turn_List_A.Delete_List();
	Turn_List_B.Delete_List();
	Turn_List_D.Delete_List();
	anc_ptr->my_path.Turn_List.Delete_List();

	return 1;

} // end of 'zip_line'

//------------------------------------------------------------
// return 1 if second coordinate or point is righter or downer
int
is_right_down(int Coor1, int Coor2, points P1, points P2, int dir)
{
	if (Coor1 > Coor2) return 1;
	if (Coor1 < Coor2) return 0;

	if (P1 == no_point) P1 = cnt;
	if (P2 == no_point) P2 = cnt;

	lines Line1 = Turn_to_Line[P1][!dir];
	lines Line2 = Turn_to_Line[P2][!dir];

	return (Line1 > Line2);
}


//---------------------------------------------------------------
// tries to create zip to the one of list points 
int
zip_to_points(List_Intersect_Elem& Lst, ancest_dist* anc_ptr,
	int son_X, int son_Y, int anc_X, int anc_Y,
	direction dir1, direction dir2, List_Intersect_Elem& Turn,
	points son_P, points anc_P)
{
	intersect_elem* curr;
	int curr_X, curr_Y;
	points curr_P;

	curr = Lst.List_pntr();

	while (curr) {

		curr_X = curr->Get_X_coord();
		curr_Y = curr->Get_Y_coord();
		curr_P = curr->Get_point();

		// tries to create zip in two directions
		if (Screen[curr_X][curr_Y].TurnList[curr_P].is_full() ||
			(zip_line(anc_ptr, son_X, son_Y,
				curr_X, curr_Y, dir1, Turn, son_P, curr_P) &&
				zip_line(anc_ptr, son_X, son_Y,
					curr_X, curr_Y, dir2, Turn, son_P, curr_P)))

			curr = curr->Get_next();

		else {
			direct_connect(anc_ptr->my_path.Turn_List, curr_X, curr_Y,
				anc_X, anc_Y, curr_P, anc_P);

			Turn.Insert(curr_X, curr_Y, curr_P);
			return 0;
		}
	} // loop on all list points

	return 1;
}

//---------------------------------------------------------------
// tries to create zip and direct line combination
int
zip_and_direct_line(ancest_dist* anc_ptr, int son_X, int son_Y,
	int anc_X, int anc_Y, List_Intersect_Elem& Turn,
	points son_P, points anc_P)
{
	List_Intersect_Elem DirectLst;
	direction dir1, dir2;
	points nport1, nport2;
	lines line;

	// horizontal case 
	line = Turn_to_Line[(anc_P == no_point) ? cnt : anc_P][1];

	if (!is_right_down(son_Y, anc_Y, son_P, anc_P, 0)) {
		dir1 = WE;
		dir2 = EW;
	}
	else {
		dir1 = EW;
		dir2 = WE;
	}
	nport1 = anc_P;

	// North
	nport2 = Line_to_Turn[line - left2][0]; // n_u2

	direct_connect(DirectLst, anc_X, anc_Y, 0, anc_Y, nport1, nport2);
	DirectLst.Remove_Elem(DirectLst.List_pntr());

	if (!zip_to_points(DirectLst, anc_ptr, son_X, son_Y,
		anc_X, anc_Y, dir1, dir2, Turn, son_P, anc_P)) {

		DirectLst.Delete_List();
		return 0;
	}

	// South
	DirectLst.Delete_List();

	nport2 = Line_to_Turn[line - left2][lnum - 1]; // s_bd

	direct_connect(DirectLst, anc_X, anc_Y, row - 1, anc_Y, nport1, nport2);
	DirectLst.Remove_Elem(DirectLst.List_pntr());

	if (!zip_to_points(DirectLst, anc_ptr, son_X, son_Y,
		anc_X, anc_Y, dir1, dir2, Turn, son_P, anc_P)) {

		DirectLst.Delete_List();
		return 0;
	}

	// vertical case
	line = Turn_to_Line[(anc_P == no_point) ? cnt : anc_P][0];

	if (!is_right_down(son_X, anc_X, son_P, anc_P, 1)) {
		dir1 = NS;
		dir2 = SN;
	}
	else {
		dir1 = SN;
		dir2 = NS;
	}

	// West
	DirectLst.Delete_List();

	nport2 = Line_to_Turn[0][line - up2]; // w_l2

	direct_connect(DirectLst, anc_X, anc_Y, anc_X, 0, nport1, nport2);
	DirectLst.Remove_Elem(DirectLst.List_pntr());

	if (!zip_to_points(DirectLst, anc_ptr, son_X, son_Y,
		anc_X, anc_Y, dir1, dir2, Turn, son_P, anc_P)) {

		DirectLst.Delete_List();
		return 0;
	}

	// East
	DirectLst.Delete_List();

	nport2 = Line_to_Turn[lnum - 1][line - up2]; // e_br

	direct_connect(DirectLst, anc_X, anc_Y, anc_X, column - 1, nport1, nport2);
	DirectLst.Remove_Elem(DirectLst.List_pntr());

	if (!zip_to_points(DirectLst, anc_ptr, son_X, son_Y,
		anc_X, anc_Y, dir1, dir2, Turn, son_P, anc_P)) {

		DirectLst.Delete_List();
		return 0;
	}

	DirectLst.Delete_List();
	return 1;
}

//-----------------------------------------------------------
int
connect_by_zip(ancest_dist* anc_ptr, int son_X,
	int son_Y, int anc_X, int anc_Y,
	List_Intersect_Elem& Turn,
	points son_P, points anc_P)
{
	// if it is connection to the same element
	if (son_P == anc_P && anc_X == son_X && anc_Y == son_Y)
		return 1;

	//if (son_X != anc_X) // horizontal case

	if (!(Screen[anc_X][anc_Y].TurnList[w].is_full() &&
		Screen[anc_X][anc_Y].TurnList[e].is_full()))

		// first move to the second element direction
		if (!zip_line(anc_ptr, son_X, son_Y, anc_X, anc_Y,
			(is_right_down(son_Y, anc_Y, son_P, anc_P, 0)) ? EW : WE,
			Turn, son_P, anc_P))
			return 0;

	//if (son_Y != anc_Y)  // vertical case

	if (!(Screen[anc_X][anc_Y].TurnList[s].is_full() &&
		Screen[anc_X][anc_Y].TurnList[n].is_full()))

		// first move to the second element direction
		if (!zip_line(anc_ptr, son_X, son_Y, anc_X, anc_Y,
			(is_right_down(son_X, anc_X, son_P, anc_P, 1)) ? SN : NS,
			Turn, son_P, anc_P))
			return 0;

	//if (son_X != anc_X) // horizontal case

	if (!(Screen[anc_X][anc_Y].TurnList[w].is_full() &&
		Screen[anc_X][anc_Y].TurnList[e].is_full()))

		// move to the opposite direction of the second element
		if (!zip_line(anc_ptr, son_X, son_Y, anc_X, anc_Y,
			(is_right_down(son_Y, anc_Y, son_P, anc_P, 0)) ? WE : EW,
			Turn, son_P, anc_P))
			return 0;

	//if (son_Y != anc_Y)  // vertical case

	if (!(Screen[anc_X][anc_Y].TurnList[s].is_full() &&
		Screen[anc_X][anc_Y].TurnList[n].is_full()))

		// move to the opposite direction of the second element
		if (!zip_line(anc_ptr, son_X, son_Y, anc_X, anc_Y,
			(is_right_down(son_X, anc_X, son_P, anc_P, 0)) ? NS : SN,
			Turn, son_P, anc_P))
			return 0;

	//   if (! zip_and_direct_line(anc_ptr, son_X, son_Y, anc_X, anc_Y,
	//			     Turn, son_P, anc_P))
	//     return 0;

	return 1;
}


//------------------------------------------------------------
// connects all first ancestors which was not direct
void
connect_first_ancestor(int (*connection)(ancest_dist*, int, int, int, int,
	List_Intersect_Elem&,
	points, points))
{
	ancest_dist* curr_anc, * last_ptr;
	int son_X, son_Y, anc_X, anc_Y, ind;

	for (register int i = 1; i < FileSize; i++)
		if (!(Vec[i].direct_ancest ||
			Vec[i].ancest_list.List_pntr()->my_path.Turn_List.List_pntr())) {

			son_X = Vec[i].X;
			son_Y = Vec[i].Y;

			// get first ancestor pointer
			curr_anc = Vec[i].ancest_list.List_pntr();;
			last_ptr = nullptr;

			while (curr_anc) {
				ind = curr_anc->Get_ancest();
				anc_X = Vec[ind].X;
				anc_Y = Vec[ind].Y;

				List_Intersect_Elem Turn;  // turns for graphical path

				if (connection(curr_anc, son_X, son_Y,
					anc_X, anc_Y, Turn, no_point, no_point)) {

					last_ptr = curr_anc;  // save previous ancestor
					curr_anc = curr_anc->Get_next();  //get next ancestor

				}
				else {
					if (last_ptr)
						Vec[i].ancest_list.Insert_First(curr_anc, last_ptr);

					ancest_dist* conn_anc = Vec[i].ancest_list.List_pntr();
					conn_anc->my_path.Source_N = ind;
					conn_anc->my_path.Target_N = i;

					distribute_points(&(conn_anc->my_path),
						conn_anc->Get_next(), &Turn);
					break;
				}
			}  // end of 'while' for all ancestors of givven son
		}
} // end of the procedure 'connect_first_ancestor'


//------------------------------------------------------------
int
connect_by_direct_line(ancest_dist* anc_ptr,
	int son_X, int son_Y, int anc_X, int anc_Y,
	List_Intersect_Elem& Turn,
	points son_P, points anc_P)
{
	List_Intersect_Elem Turn_List;

	if (((son_X != anc_X || son_Y != anc_Y) &&
		(son_P == n_u1 || son_P == s_d1 || son_P == e_r1 || son_P == w_l1)) ||
		direct_connect(Turn_List, son_X, son_Y, anc_X, anc_Y, son_P, anc_P)) {
		Turn_List.Delete_List();
		return 1;
	}

	anc_ptr->my_path.Turn_List &= Turn_List;
	return 0;
}

//------------------------------------------------------------
int
zip_and_line_to_point(List_Intersect_Elem& Lst, ancest_dist* anc_ptr,
	int son_X, int son_Y, int anc_X, int anc_Y,
	List_Intersect_Elem& Turn, points son_P, points anc_P,
	int is_Vert)
{
	intersect_elem* curr;
	List_Intersect_Elem NewLst;
	int curr_X, curr_Y;
	points curr_P;
	direction dir1, dir2;

	if (is_Vert) {
		dir1 = NS;
		dir2 = SN;
	}
	else {
		dir1 = WE;
		dir2 = EW;
	}

	curr = Lst.List_pntr();

	while (curr) {

		curr_X = curr->Get_X_coord();
		curr_Y = curr->Get_Y_coord();
		curr_P = curr->Get_point();

		// tries to create zip and line in two directions
		if (!Screen[curr_X][curr_Y].TurnList[curr_P].is_full()) {

			if (is_Vert) {
				direct_connect(NewLst, curr_X, curr_Y,
					curr_X, 0, curr_P, get_corner(curr_P, w_l2, 1));
				direct_connect(NewLst, curr_X, curr_Y,
					curr_X, column - 1, curr_P, get_corner(curr_P, e_br, 1));
			}
			else {
				direct_connect(NewLst, curr_X, curr_Y,
					0, curr_Y, curr_P, get_corner(curr_P, n_u2, 0));
				direct_connect(NewLst, curr_X, curr_Y,
					row - 1, curr_Y, curr_P, get_corner(curr_P, s_bd, 0));
			}

			if (zip_to_points(NewLst, anc_ptr, son_X, son_Y,
				curr_X, curr_Y, dir1, dir2, Turn, son_P, curr_P)) {

				curr = curr->Get_next();
			}
			else {
				direct_connect(anc_ptr->my_path.Turn_List, curr_X, curr_Y,
					anc_X, anc_Y, curr_P, anc_P);

				NewLst.Delete_List();
				Turn.Insert(curr_X, curr_Y, curr_P);
				return 0;
			}
		} // end of if
		else
			curr = curr->Get_next();

		NewLst.Delete_List();

	} // loop on all list points

	return 1;
}

//------------------------------------------------------------
// connects givven ancestor
int
connect_ancestor(int (*connection)(ancest_dist*,
	int, int, int, int,
	List_Intersect_Elem&,
	points, points),
	int son,
	ancest_dist* curr_anc, int anc_X, int anc_Y,
	List_Intersect_Elem& Turn)
{
	intersect_elem* curr_ptr;  // pointer to the current turn
	points curr_P;
	int curr_X, curr_Y, fl_port;

	fl_port = 0;

	curr_ptr = curr_anc->Possible_Intersect_Lst.List_pntr();

	while (curr_ptr) { // loop on all possible points

		curr_X = curr_ptr->Get_X_coord();
		curr_Y = curr_ptr->Get_Y_coord();
		curr_P = curr_ptr->Get_point();

		if (arr_port[son] && Vec[son].X == curr_X && Vec[son].Y == curr_Y &&
			Vec[son].direct_ancest) {

			switch (Vec[son].direct_ancest->Turn_List.List_pntr()->Get_point()) {
			case n: if (curr_P == n_u1)
				fl_port = 1;
				break;
			case s: if (curr_P == s_d1)
				fl_port = 1;
				break;
			case e: if (curr_P == e_r1)
				fl_port = 1;
				break;
			case w: if (curr_P == w_l1)
				fl_port = 1;
			}
		}

		if (fl_port)
			Screen[curr_X][curr_Y].TurnList[curr_P].release();

		if (!connection(curr_anc, curr_X, curr_Y, anc_X, anc_Y,
			Turn, curr_P, no_point)) {

			if (fl_port) {
				Screen[curr_X][curr_Y].TurnList[curr_P].fill();
				arr_port[son] = 0;
			}

			return 0; // connection is created
		}
		else   // get next point
			curr_ptr = curr_ptr->Get_next();

		if (fl_port) {
			Screen[curr_X][curr_Y].TurnList[curr_P].fill();
			fl_port = 0;
		}

	} // end of loop on all points

	return 1;
} // end of function 'connect_ancestor'

//---------------------------------------------------------------------
// extremal connection between imagine element and givven point
int
connect_elem_to_points(ancest_dist* anc_ptr,
	int son_X, int son_Y, int anc_X, int anc_Y,
	List_Intersect_Elem& Turn)
{
	int son = Screen[son_X][son_Y].Vec_node;
	static char Cir_Name = 'a';

	// fill temporary
	Screen[anc_X][anc_Y].Full = 1;
	Screen[anc_X][anc_Y].TurnList[cnt].fill();

	if (connect_ancestor(connect_by_direct_line, son,
		anc_ptr, anc_X, anc_Y, Turn) &&
		connect_ancestor(connect_diag_elem, son,
			anc_ptr, anc_X, anc_Y, Turn) &&
		connect_ancestor(connect_by_zip, son,
			anc_ptr, anc_X, anc_Y, Turn)) {

		// release temporary filled element and its center
		Screen[anc_X][anc_Y].Full = 0;
		Screen[anc_X][anc_Y].TurnList[cnt].release();
		return 1;
	}

	Screen[anc_X][anc_Y].Type = circle_type;
	Screen[anc_X][anc_Y].Vec_node = anc_ptr->Get_ancest();
	sprintf(Screen[anc_X][anc_Y].Name, "%c", Cir_Name++);
	anc_ptr->cir_X = anc_X;
	anc_ptr->cir_Y = anc_Y;

	return 0;
}
//------------------------------------------------------------------------
// this function looks for empty element for purpose of
// non_conventional connect
int
find_empty_element(int& curr_X, int& curr_Y, int is_Vert, int coeff)
{
	if (is_Vert) {
		if (!(Screen[curr_X][curr_Y - coeff](curr_X, curr_Y - coeff) ||
			Screen[curr_X][curr_Y - coeff].Full ||
			Screen[curr_X][curr_Y - coeff].TurnList[cnt].is_full())) {
			curr_Y -= coeff;
			return 0;
		}

		if (!(Screen[curr_X][curr_Y + coeff](curr_X, curr_Y + coeff) ||
			Screen[curr_X][curr_Y + coeff].Full ||
			Screen[curr_X][curr_Y + coeff].TurnList[cnt].is_full())) {
			curr_Y += coeff;
			return 0;
		}
	}
	else {
		if (!(Screen[curr_X - coeff][curr_Y](curr_X - coeff, curr_Y) ||
			Screen[curr_X - coeff][curr_Y].Full ||
			Screen[curr_X - coeff][curr_Y].TurnList[cnt].is_full())) {
			curr_X -= coeff;
			return 0;
		}
		if (!(Screen[curr_X + coeff][curr_Y](curr_X + coeff, curr_Y) ||
			Screen[curr_X + coeff][curr_Y].Full ||
			Screen[curr_X + coeff][curr_Y].TurnList[cnt].is_full())) {
			curr_X += coeff;
			return 0;
		}
	}

	return 1;

} // end of function 'find_empty_element'

//------------------------------------------------------------------------
// tries to creat non_conventional connect between elements
int
non_convent_connect(ancest_dist* anc_ptr, int son_X, int son_Y,
	List_Intersect_Elem& Turn)
{
	intersect_elem* curr;
	int curr_X, curr_Y,
		is_Vert, coeff;

	for (coeff = 0; coeff < row; coeff++) {
		curr = anc_ptr->Possible_Intersect_Lst.List_pntr();

		while (curr) {  // loop on all intersection points

			curr_X = curr->Get_X_coord();
			curr_Y = curr->Get_Y_coord();
			is_Vert = curr->Get_dir();

			if (!(find_empty_element(curr_X, curr_Y, is_Vert, coeff) ||
				connect_elem_to_points(anc_ptr, son_X, son_Y,
					curr_X, curr_Y, Turn)))
				return 0;

			curr = curr->Get_next();
		} // end of while
	}  // end of loop on all possible coeff.

	return 1;
} // end of function 'non_convent_connect'

//-------------------------------------------------------------------------
int
complex_connect(ancest_dist* anc_ptr,
	int son_X, int son_Y, int anc_X, int anc_Y,
	List_Intersect_Elem& Turn,
	points son_P, points anc_P)
{
	List_Intersect_Elem DirectLst;

	if (!zip_and_direct_line(anc_ptr, son_X, son_Y, anc_X, anc_Y,
		Turn, son_P, anc_P))
		return 0;

	// North
	direct_connect(DirectLst, anc_X, anc_Y, 0, anc_Y, n_u1, n_u2);

	if (!zip_and_line_to_point(DirectLst, anc_ptr, son_X, son_Y, anc_X, anc_Y,
		Turn, son_P, anc_P, 1)) {

		DirectLst.Delete_List();
		return 0;
	}

	// South
	direct_connect(DirectLst, anc_X, anc_Y, row - 1, anc_Y, s_d1, s_bd);

	if (!zip_and_line_to_point(DirectLst, anc_ptr, son_X, son_Y, anc_X, anc_Y,
		Turn, son_P, anc_P, 1)) {

		DirectLst.Delete_List();
		return 0;
	}

	// West
	direct_connect(DirectLst, anc_X, anc_Y, anc_X, 0, w_l1, w_l2);

	if (!zip_and_line_to_point(DirectLst, anc_ptr, son_X, son_Y, anc_X, anc_Y,
		Turn, son_P, anc_P, 0)) {

		DirectLst.Delete_List();
		return 0;
	}

	// East
	direct_connect(DirectLst, anc_X, anc_Y, anc_X, column - 1, e_r1, e_br);

	if (!zip_and_line_to_point(DirectLst, anc_ptr, son_X, son_Y, anc_X, anc_Y,
		Turn, son_P, anc_P, 0)) {

		DirectLst.Delete_List();
		return 0;
	}

	DirectLst.Delete_List();

	return 1;
}  // end of 'complex_connect'

// ------------------------------------------------------------------------
int
extremal_connect(ancest_dist* anc_ptr,
	int son_X, int son_Y, int anc_X, int anc_Y,
	List_Intersect_Elem& Turn,
	points son_P, points anc_P)
{
	if (!non_convent_connect(anc_ptr, son_X, son_Y, Turn))
		return 0;
	return 1;
}

//------------------------------------------------------------------------
// connects all ancestors which was has first one
void
connect_all_ancestors()
{
	ancest_dist* prev_anc, * curr_anc, * last_conn;
	int anc_X, anc_Y, ind;
	int non_Connected;

	for (register int i = 1; i < FileSize; i++)
		if (Vec[i].direct_ancest ||
			Vec[i].ancest_list.List_pntr()->my_path.Turn_List.List_pntr()) {

			//get first non_connected ancestor
			prev_anc = Vec[i].ancest_list.Get_last_conn();
			curr_anc = prev_anc->Get_next();

			if (!curr_anc) continue;  // all ancestors are connected

			while (curr_anc) { // loop all nonconnected ancestors
				List_Intersect_Elem Turn;
				ind = curr_anc->Get_ancest();
				anc_X = Vec[ind].X;            // ancestor's coordinates
				anc_Y = Vec[ind].Y;

				if (non_Convent)
					non_Connected = connect_ancestor(extremal_connect, i,
						curr_anc, anc_X, anc_Y, Turn);
				else
					non_Connected = (connect_ancestor(connect_by_direct_line, i,
						curr_anc, anc_X, anc_Y, Turn) &&
						connect_ancestor(connect_diag_elem, i,
							curr_anc, anc_X, anc_Y, Turn) &&
						connect_ancestor(connect_by_zip, i,
							curr_anc, anc_X, anc_Y, Turn) &&
						(!use_Reserve ||
							connect_ancestor(complex_connect, i,
								curr_anc, anc_X, anc_Y, Turn)));


				if (non_Connected) {

					if (curr_anc->Get_next()) {
						curr_anc->Get_next()->Copy_Lst(
							curr_anc->Possible_Intersect_Lst
						);
						curr_anc->Possible_Intersect_Lst.Delete_List();
					}
					else {
						last_conn = Vec[i].ancest_list.Get_last_conn();
						if (last_conn->Get_next())
							if (last_conn->Get_next() != curr_anc) {
								last_conn->Get_next()->Copy_Lst(
									curr_anc->Possible_Intersect_Lst
								);
								curr_anc->Possible_Intersect_Lst.Delete_List();
							}
					}
				}
				else { // connection is created
					curr_anc->my_path.Source_N = ind;   // fill path's coordinates
					curr_anc->my_path.Target_N = i;

					if (curr_anc->Get_next())
						curr_anc->Get_next()->Copy_Lst(curr_anc->Possible_Intersect_Lst);

					// delete possible list from curr_anc 
					curr_anc->Possible_Intersect_Lst.Delete_List();

					Vec[i].ancest_list.Set_last_conn(prev_anc, curr_anc);

					distribute_points(&(curr_anc->my_path),
						curr_anc->Get_next(), &Turn);

				} // end of case - connection is created

				prev_anc = curr_anc;
				curr_anc = curr_anc->Get_next();  //get next ancestor

			}  // end of 'while' for all ancestors of givven son

		}  // if we have first ancestor

} // end of the procedure 'connect_all_ancestors'

//-----------------------------------------------------------------------
void
first_connection()
{
	connect_first_ancestor(connect_diag_elem);
	connect_first_ancestor(connect_by_zip);
	connect_first_ancestor(zip_and_direct_line);
	use_Reserve = 1;
	connect_first_ancestor(connect_diag_elem);
	connect_first_ancestor(connect_by_zip);
	connect_first_ancestor(zip_and_direct_line);
	use_Reserve = 0;
}

//-----------------------------------------------------------------------
void
Connect_All()
{
	// creates two lists , one for direct connection
	// and other for undirect connection and also calls for
	// function 'connect_direct_elem'
	creation_ancest_list();

	// array indicates if son port is temporary filled
	arr_port = new int[FileSize];
	for (register int i = 0; i < FileSize; i++)
		arr_port[i] = 0;

	// produce intersect points and fills Possible_Intersect_Lst
	// of first ancestor in List of undirect ancestors
	distribute_direct_points();

	// connects all first ancestors which was not direct
	first_connection();

	// set last_connected for all elements
	for (int i = 1; i < FileSize; i++)
		if (Vec[i].direct_ancest)
			Vec[i].ancest_list.Set_last_conn();
		else
			if (Vec[i].ancest_list.List_pntr()->my_path.Turn_List.List_pntr())
				Vec[i].ancest_list.Set_last_conn(NULL, Vec[i].ancest_list.List_pntr());
			else	    // error occures
				printf("first connection isn't created between son %d and anc %d\n",
					i, Vec[i].ancest_list.List_pntr()->Get_ancest());


	// connects all ancestors
	connect_all_ancestors();

	use_Reserve = 1;
	connect_all_ancestors();

	non_Convent = 1;
	connect_all_ancestors();

	// delete Possible_Intersect_Lst before graphical interface
	ancest_dist* curr;
	for (int i = 1; i < FileSize; i++) {
		curr = Vec[i].ancest_list.List_pntr();
		while (curr) {
			if (!(curr->my_path.Turn_List.List_pntr()))
				printf("Connection isn't created! anc = %d , son = %d \n",
					curr->Get_ancest(), i);

			curr->Possible_Intersect_Lst.Delete_List();
			curr = curr->Get_next();
		}
	}
}  // end of function 'Connect_All'

//----------------------------------------------------------------
















//void translate_to_pixel(intersect_elem* turn_ptr,
//	int& x_coord, int& y_coord)
//{
//	int i = 0;
//
//	int t_x = turn_ptr->Get_X_coord();
//	int t_y = turn_ptr->Get_Y_coord();
//	points p = turn_ptr->Get_point();
//
//	i = int(p);
//
//	if (e <= p && p <= s)
//		if (Screen[t_x][t_y].Type != rectangle_type) {
//			x_coord = Screen[t_x][t_y].crossPoint.arr[i + 4].x;
//			y_coord = Screen[t_x][t_y].crossPoint.arr[i + 4].y;
//			return;
//		}
//	x_coord = Screen[t_x][t_y].crossPoint.arr[i].x;
//	y_coord = Screen[t_x][t_y].crossPoint.arr[i].y;
//
//}
//
void calc_arrow_points(int oneArrow[], direction Direct)
{
	switch (Direct) {
	case EW:
		oneArrow[0] = oneArrow[2];// +arrow_shift;
		oneArrow[1] = oneArrow[3];// -arrow_shift;
		oneArrow[4] = oneArrow[2];// + arrow_shift ;
		oneArrow[5] = oneArrow[3];// + arrow_shift ;
		break;

	case WE:
		oneArrow[0] = oneArrow[2];// - arrow_shift ;
		oneArrow[1] = oneArrow[3];// - arrow_shift ;
		oneArrow[4] = oneArrow[2];// - arrow_shift ;
		oneArrow[5] = oneArrow[3];// + arrow_shift ;
		break;

	case NS:
		oneArrow[0] = oneArrow[2];// - arrow_shift ;
		oneArrow[1] = oneArrow[3];// - arrow_shift ;
		oneArrow[4] = oneArrow[2];// + arrow_shift ;
		oneArrow[5] = oneArrow[3];// - arrow_shift ;
		break;

	case SN:
		oneArrow[0] = oneArrow[2];// - arrow_shift ;
		oneArrow[1] = oneArrow[3];// + arrow_shift ;
		oneArrow[4] = oneArrow[2];// + arrow_shift ;
		oneArrow[5] = oneArrow[3];// + arrow_shift ;
		break;
	}
}

//graphics::draw_poly(int PointNum,
//	int* Points,
//	int Color,
//	int FillStyle)

void draw_arrow(int x_graph, int y_graph, int x, int y,
	direction direct, points p)
{

	if (e <= p && p <= s) {
		//draw_poly(3, Screen[x][y].CellArrows[p - e].oneArrow, WHITE);
		printf("ex1: %d \n", Screen[x][y].CellArrows[p - e].oneArrow);
		return;
	}
	// case of non - element connection
	int Temp_arr[6];

	// set Peek of the arrow

	Temp_arr[2] = x_graph;
	Temp_arr[3] = y_graph;

	// calculate two rest arrow points
	calc_arrow_points(Temp_arr, direct);
	for (int i = 0; i < 6; i++) {
		printf("ex2: %d \n", Temp_arr[i]);
	}
	//draw_poly(3, Temp_arr, WHITE);
}



void call_draw_arrow(intersect_elem* curr, intersect_elem* prev)
{
	int x_prev, y_prev, x_curr, y_curr;

	int tmp_x = prev->Get_X_coord();
	int tmp_y = prev->Get_Y_coord();

	x_prev = Screen[tmp_x][tmp_y].crossPoint.arr[int(prev->Get_point())].x;
	y_prev = Screen[tmp_x][tmp_y].crossPoint.arr[int(prev->Get_point())].y;

	tmp_x = curr->Get_X_coord();
	tmp_y = curr->Get_Y_coord();

	x_curr = Screen[tmp_x][tmp_y].crossPoint.arr[int(curr->Get_point())].x;
	y_curr = Screen[tmp_x][tmp_y].crossPoint.arr[int(curr->Get_point())].y;

	if (x_prev < x_curr)
		draw_arrow(x_curr, y_curr, tmp_x, tmp_y, WE, curr->Get_point());
	else
		if (x_prev > x_curr)
			draw_arrow(x_curr, y_curr, tmp_x, tmp_y, EW, curr->Get_point());
		else
			if (y_prev < y_curr)
				draw_arrow(x_curr, y_curr, tmp_x, tmp_y, NS, curr->Get_point());
			else
				draw_arrow(x_curr, y_curr, tmp_x, tmp_y, SN, curr->Get_point());

}

//
//
//void draw_path_pair(List_Intersect_Elem Turns_Lst)
//{
//	int count = 0, i = 0;
//	int* polynom;
//	intersect_elem* first_point, * prev_point,
//		* curr;
//
//	curr = Turns_Lst.List_pntr();
//	while (curr) {
//		count++;
//		curr = curr->Get_next();
//	}
//
//	polynom = new int[count * 2];
//
//	curr = Turns_Lst.List_pntr();
//	first_point = Turns_Lst.List_pntr();
//	prev_point = first_point->Get_next();
//
//	while (curr) {
//		translate_to_pixel(curr, polynom[i], polynom[i + 1]);
//		i += 2;
//		curr = curr->Get_next();
//	}
//	//draw_poly(count, polynom, WHITE);
//
//	call_draw_arrow(first_point, prev_point);
//
//	if (polynom)
//		delete polynom;
//
//}
//
//
////      Ok so could it be useful?
//void draw_pathes()
//{
//	ancest_dist* anc_ptr = NULL;
//	int ind_anc, anc_X, anc_Y;
//	points anc_P;
//
//	for (int i = 1; i < FileSize; i++) {
//
//		if (Vec[i].direct_ancest)
//			draw_path_pair(Vec[i].direct_ancest->Turn_List);
//
//		anc_ptr = Vec[i].ancest_list.List_pntr();
//		while (anc_ptr) {
//			if (anc_ptr && anc_ptr->my_path.Turn_List.List_pntr())
//				draw_path_pair(anc_ptr->my_path.Turn_List);
//
//			anc_ptr = anc_ptr->Get_next();
//		}
//	}
//
//	for (int i = 1; i < FileSize; i++) {
//
//		if (Vec[i].direct_ancest) {
//			anc_X = Vec[i].direct_ancest->Turn_List.Get_Last()->Get_X_coord();
//			anc_Y = Vec[i].direct_ancest->Turn_List.Get_Last()->Get_Y_coord();
//			anc_P = Vec[i].direct_ancest->Turn_List.Get_Last()->Get_point();
//			ind_anc = Screen[anc_X][anc_Y].Vec_node;
//			if (Screen[anc_X][anc_Y].Vec_right_son != -1) {
//				//draw_port_str(NULL, i, ind_anc, anc_P);
//				int x, y;
//				if (anc_ptr != NULL) {
//					x = Vec[anc_ptr->Get_ancest()].X,
//						y = Vec[anc_ptr->Get_ancest()].Y;
//				}
//				else {
//					x = Vec[ind_anc].X;
//					y = Vec[ind_anc].Y;
//				}
//				printf("%d %d \n", x, y);
//			}
//		}
//
//		anc_ptr = Vec[i].ancest_list.List_pntr();
//		int anc_ind;
//		while (anc_ptr) {
//			anc_ind = anc_ptr->Get_ancest();
//			if (anc_ptr && anc_ptr->my_path.Turn_List.List_pntr() &&
//				Screen[Vec[anc_ind].X][Vec[anc_ind].Y].Vec_right_son != -1) {
//				//draw_port_str(anc_ptr, i);
//				int x, y;
//				if (anc_ptr != NULL) {
//					x = Vec[anc_ptr->Get_ancest()].X,
//						y = Vec[anc_ptr->Get_ancest()].Y;
//				}
//				else {
//					x = Vec[ind_anc].X;
//					y = Vec[ind_anc].Y;
//				}
//				printf("%d %d \n", x, y);
//			}
//			anc_ptr = anc_ptr->Get_next();
//		}
//	}
//
//	for (int i = 0; i < FileSize; i++) {
//		if (Screen[Vec[i].X][Vec[i].Y].TurnList[s].is_full() == THREE)
//			Screen[Vec[i].X][Vec[i].Y].TurnList[s].release();
//		if (Screen[Vec[i].X][Vec[i].Y].TurnList[n].is_full() == THREE)
//			Screen[Vec[i].X][Vec[i].Y].TurnList[n].release();
//		if (Screen[Vec[i].X][Vec[i].Y].TurnList[w].is_full() == THREE)
//			Screen[Vec[i].X][Vec[i].Y].TurnList[w].release();
//		if (Screen[Vec[i].X][Vec[i].Y].TurnList[e].is_full() == THREE)
//			Screen[Vec[i].X][Vec[i].Y].TurnList[e].release();
//	}
//}



// end of  calc_arrow_points

//void
//GraphicWin::draw_port_str(ancest_dist* anc_ptr, int son,
//	int ind_anc = -1, points P = s)
//{
//	int Color, anc_X, anc_Y, X, Y,
//		str_X, str_Y;
//	char* Str;
//	points anc_P;
//
//	if (anc_ptr != NULL) {
//		anc_X = Vec[anc_ptr->Get_ancest()].X,
//			anc_Y = Vec[anc_ptr->Get_ancest()].Y;
//	}
//	else {
//		anc_X = Vec[ind_anc].X;
//		anc_Y = Vec[ind_anc].Y;
//	}
//
//	if (anc_ptr == NULL || anc_ptr->cir_X == -1) {
//		if (Screen[anc_X][anc_Y].Vec_right_son == son)
//			return;
//		if (anc_ptr != NULL)
//			anc_P = anc_ptr->my_path.Turn_List.Get_Last()->Get_point();
//		else
//			anc_P = P;
//
//		Str = new char[2];
//		sprintf(Str, "1");
//	}
//	else { // non conventional case
//		if (!Screen[anc_X][anc_Y].TurnList[w].is_full())
//			anc_P = w;
//		else if (!Screen[anc_X][anc_Y].TurnList[e].is_full())
//			anc_P = e;
//		else if (!Screen[anc_X][anc_Y].TurnList[n].is_full())
//			anc_P = n;
//		else if (!Screen[anc_X][anc_Y].TurnList[s].is_full())
//			anc_P = s;
//
//		Screen[anc_X][anc_Y].TurnList[anc_P].fill();
//
//		X = anc_ptr->cir_X;
//		Y = anc_ptr->cir_Y;
//
//		Str = Screen[X][Y].Name;
//	}
//
//	choice_point(anc_P, anc_X, anc_Y, str_X, str_Y);
//	//if ( check_font_size() )
//	//  draw_str(Str, str_X, str_Y, Color);
//
//	if (Color == RED && Screen[anc_X][anc_Y].Vec_left_son == son) {
//		anc_P = anc_ptr->my_path.Turn_List.Get_Last()->Get_point();
//		choice_point(anc_P, X, Y, str_X, str_Y);
//		Str = new char[2];
//		sprintf(Str, "1");
//		//if ( check_font_size() )
//		//  draw_str(Str, str_X, str_Y, Color);
//	}
//}












//void test3(intersect_elem* curr, intersect_elem* prev) {
//	call_draw_arrow(curr, prev);
//}




void test(List_Intersect_Elem Turns_Lst, FILE* stream) {
	intersect_elem* curr;

	curr = Turns_Lst.List_pntr();

	while (curr) {
		printf("%d, %d \n", curr->Get_X_coord(), curr->Get_Y_coord());
		fprintf(stream, " %d,%d ", curr->Get_X_coord(), curr->Get_Y_coord());
		curr = curr->Get_next();
	}

}









void logic(char arg[]) {
	Vec = new shape[MaxSize];
	for (int j = 0; arg[j] != '.' && j < strlen(arg); j++) {
		Name[j] = arg[j];
	}

	Load_Table(arg);
	printf("\n BDD has %d elements.\n\n", FileSize);
	reserve_flag = 1;
	chart_flow_alloc();
	FILE* stream;
	FILE* stream2;
	FILE* stream3;

	char filename[265];
	for (int i = 0; i < 256; i++) {
		filename[i] = Name[i];
	}

	char filename2[265];
	for (int i = 0; i < 256; i++) {
		filename2[i] = Name[i];
	}


	strcat(Name, ".PLC");
	strcat(filename, "-nodes.txt");
	strcat(filename2, "-lines.txt");

	/* open a file for update */
	stream = fopen(Name, "w");
	stream2 = fopen(filename, "w");

	fprintf(stream, "\n           Elements  Allocation    \n\n");

	for (int k = 0; k < row; k++) {
		for (int h = 0; h < column; h++)
			if (Screen[k][h].Vec_node == -1) {
				fprintf(stream, "   -");
				//	 printf("   -");
			}
			else {
				fprintf(stream, " %3d", Screen[k][h].Vec_node);
				fprintf(stream2, "%d %5s %d %d \n", Screen[k][h].Vec_node, Vec[Screen[k][h].Vec_node].name, k, h);
				//	 printf(" %3d", Screen[k][h].Vec_node);
			}
		fprintf(stream, "\n");
		//    printf("\n");
	}
	fclose(stream2);
	fclose(stream);

	// release first row
	for (int k = 0; k < column; k++)
		if (Screen[0][k].Vec_node == -1)
			Screen[0][k].Full = 0;

	printf("\nPlease wait, tracing in process...\n");
	Connect_All();

	/*    printf("\nWould you like to look at the picture (y/n) ? - ");
		char ch = getchar();

		if ( ch == 'y' || ch == 'Y') */

	ps_flag = 0;               // No ps request
	Name[strlen(Name) - 3] = 0;  // Prepare PostScript File Name
	strcat(Name, "ps");
	//graphic_interface();

	ancest_dist* pointer;

	printf("\nEnd of the program.\n\n");
	//for (int i = 0; i < FileSize; i++) {
	//	if (Vec[i].direct_ancest != NULL) {
	//		printf("%5s %d %d \n", Vec[i].name, Vec[i].direct_ancest->Turn_List.Get_Last()->Get_X_coord(), Vec[i].direct_ancest->Turn_List.Get_Last()->Get_Y_coord());
	//	}
	//	else
	//		printf("oopsie, nie ma dla %5s \n", Vec[i].name);
	//}

	stream3 = fopen(filename2, "w");
	shape vec;

	for (int i = 0; i < FileSize; i++) {
		vec = Vec[i];
		fprintf(stream3, "%d %5s ||", i, vec.name);
		if (vec.direct_ancest)
			test(vec.direct_ancest->Turn_List, stream3);


		//ancest_dist* path = vec.ancest_list.List_pntr();
		//while (path) {
		//	if (path && path->my_path.Turn_List.List_pntr()) {
		//		printf("\n");
		//		test(path->my_path.Turn_List);
		//	}

		//	path = path->Get_next();
		//}

		fprintf(stream3, "||");
		int anc_X, anc_Y, ind_anc;
		points anc_P;
		ancest_dist* anc_ptr;

		anc_ptr = vec.ancest_list.List_pntr();
		int anc_ind;
		while (anc_ptr) {
			anc_ind = anc_ptr->Get_ancest();
			if (anc_ptr && anc_ptr->my_path.Turn_List.List_pntr()) {
				printf("\n");
				test(anc_ptr->my_path.Turn_List, stream3);
				fprintf(stream3, "|");
			}
			anc_ptr = anc_ptr->Get_next();
		}
		fprintf(stream3, "\n");
	}
}



//void
//main(int argc, char* argv[])
//{
//	char arg[] = "and.gsa";
//	logic(arg);
//	//if ( argc < 2 )
//	//error(" Usage : name_of_program   data_file");
//	
//} // end of 'Main' function




