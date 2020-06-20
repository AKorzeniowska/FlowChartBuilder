//#include "margr.h"
//
//int Palette[] = { BLACK, WHITE, GREEN, RED, YELLOW, BLUE };
//// ---------------------------------------------------------------------
////GraphicWin::GraphicWin( int x, int y, int z, int txtColor)
////			:graphics(Palette[0],Palette[1] )
////{
////	int i;
////	Factor = 1; // init  level  of zoom;
////	Origin_x = x;
////	Origin_y = y;
////	MaxWinSize = z;
////	textColor = txtColor;
////
////	delta = delta_inner = delta_outer = 0;
////	arrow_shift = 0;
////	center_x = center_y = 0;
////	for( i = 0; i < numIntersectPoints; i++){
////	  screen_points[i].x = 0;   // in every cell of screen we have
////	  screen_points[i].y = 0;//numIntersectPoints points of intersect.
////	}
////	// clears  arrow array
////	int j;
////	for(i = 0; i<size_First_cell_arrow_array; i++)
////	  for( j = rEastAr; j <= dEastAr; j++)
////	    FirstCellArrows[i].oneArrow[j] = 0;
////
////	// rectangle initialisation
////
////	set_p(rect_LU_corner, 0, 0);
////	set_p(rect_RD_corner, 0, 0);
////
////	//diamond   initialisation
////
////	set_p( Ldiamond, 0, 0);
////	set_p( Rdiamond, 0, 0);
////	set_p( Udiamond, 0, 0);
////	set_p( Ddiamond, 0, 0);
////}
//// ---------------------------------------------------------------------
//void
//GraphicWin::change_origins( int x, int y)
//{
//  	Origin_x = Origin_x + x;
//	Origin_y = Origin_y + y;
//}
//
////---------------------------------------------------------------
//// calculates two tails points of arrow
//// accordently to the direction.
//// Peek of the arrow were set in the 2 and 3 array elements
//void calc_arrow_points(int oneArrow[], direction Direct)
//{
//      switch  (Direct) {
//      case EW:
//		  oneArrow[0] = oneArrow[2];// +arrow_shift;
//		  oneArrow[1] = oneArrow[3];// -arrow_shift;
//      	  oneArrow[4] = oneArrow[2];// + arrow_shift ;
//	  oneArrow[5] = oneArrow[3];// + arrow_shift ;
//	  break;
//
//      case WE:
//	  oneArrow[0] = oneArrow[2];// - arrow_shift ;
//	  oneArrow[1] = oneArrow[3];// - arrow_shift ;
//	  oneArrow[4] = oneArrow[2];// - arrow_shift ;
//	  oneArrow[5] = oneArrow[3];// + arrow_shift ;
//	  break;
//
//      case NS:
//	  oneArrow[0] = oneArrow[2];// - arrow_shift ;
//	  oneArrow[1] = oneArrow[3];// - arrow_shift ;
//	  oneArrow[4] = oneArrow[2];// + arrow_shift ;
//	  oneArrow[5] = oneArrow[3];// - arrow_shift ;
//	  break;
//
//      case SN:
//	  oneArrow[0] = oneArrow[2];// - arrow_shift ;
//	  oneArrow[1] = oneArrow[3];// + arrow_shift ;
//	  oneArrow[4] = oneArrow[2];// + arrow_shift ;
//	  oneArrow[5] = oneArrow[3];// + arrow_shift ;
//	  break;
//      }
//}        // end of  calc_arrow_points
//
//// ---------------------------------------------------------------------
//void
//GraphicWin::delta_calc(int num)// calculation of delta according
//			      // to num_of_elements in given flow_chart
//{         // factor used for zooming purposes
//	  delta = floor( floor( MaxWinSize/num) * Factor) ;
//	  if ( delta < 16 )
//	      delta = 16; // minimal visible size of screen cell
//
//	  delta_outer = floor( delta/8) ;// 6/8 is size of outer square
//	  delta_inner = delta_outer <<1; // 4/8 is size of inner square
//	  arrow_shift = delta >> 4;  // arrow_size
//	  if( !arrow_shift ) arrow_shift = 6;
//
//} // end of delta_calc
//// --------------------------------------------------------------------
//void
//GraphicWin::Arrow_calc( arrow_direct k, Point_coord dirEast,
//		        Point_coord dirWest,
//			Point_coord dirNorth, Point_coord dirSouth)
//{
//    // k is arrow index   , if 0 <= k < 4 - it is case of rectangle
//    // if 4 <= k < 8 is case of diamond
//   // init central arrow point for dirEast
//   FirstCellArrows[k].oneArrow[2] = dirEast.x ;
//   FirstCellArrows[k].oneArrow[3] = dirEast.y ;
//
//   // calculate two rest points
//   //calc_arrow_points(FirstCellArrows[k].oneArrow, EW);
//   int value = k;
//   value = value++;
//   // init central arrow point for dirWest
//   FirstCellArrows[value].oneArrow[2] = dirWest.x ;
//   FirstCellArrows[k].oneArrow[3]   = dirWest.y ;
//
//   // calculate two rest points
//   //calc_arrow_points(FirstCellArrows[k].oneArrow, WE);
//
//   // init central arrow point for  dirNorth
//   FirstCellArrows[value].oneArrow[2] = dirNorth.x ;
//   FirstCellArrows[k].oneArrow[3]   = dirNorth.y ;
//
//   // calculate two rest points
//   //calc_arrow_points(FirstCellArrows[k].oneArrow, NS);
//
//   // init central arrow point for  dirSouth
//   FirstCellArrows[value].oneArrow[2] = dirSouth.x ;
//   FirstCellArrows[k].oneArrow[3]   = dirSouth.y ;
//
//   // calculate two rest points
//   //calc_arrow_points(FirstCellArrows[k].oneArrow, SN);
//
//}
//// --------------------------------------------------------------
//void
//GraphicWin::set_first_cell_points()
//{
//   center_x = center_y = floor( delta/2 );
//
////setting first rectangle coordinates:
//// rectangle coord   Left up corner coord x = center_x - delta/8
////                                        y = center_y - delta/16
//
//    set_p(rect_LU_corner, center_x - delta_outer + Origin_x,
//		 center_y - ( delta_outer >> 1 ) + Origin_y );
//
//// rectangle coord   Right down corner coord x = center_x + delta/8
////                                        y = center_y + delta/16
//
//    set_p(rect_RD_corner , center_x +  delta_outer + Origin_x ,
//		 center_y + ( delta_outer >> 1 ) + Origin_y);
//    set_p(W_R,  center_x - delta_outer , center_y  );
//    set_p(E_R,  center_x + delta_outer, center_y  );
//
//    set_p(N_R, center_x , center_y - (delta_outer >> 1) );
//    set_p(S_R, center_x, center_y + (delta_outer >> 1) );
//
////setting first diamands coordinates:
//
//   set_p( Ldiamond , center_x - delta_outer , center_y);
//   set_p( Rdiamond , center_x + delta_outer, center_y);
//   set_p( Udiamond , center_x , center_y - delta_outer );
//   set_p( Ddiamond , center_x , center_y + delta_outer );
//
//// setting of intersection points
//
//	  set_p( L1_U1, delta_inner, delta_inner);
//
//	  set_p( L1_U2,  delta_inner ,  delta_outer);
//
//	  set_p( L1_D1, delta_inner, delta - delta_inner);
//
//	  set_p( L1_D2, delta_inner,  delta - delta_outer);
//
//	  set_p( L1_BD,  delta_inner, delta);
//
//	  set_p( L2_U1, delta_outer, delta_inner);
//
//	  set_p( L2_U2, delta_outer, delta_outer);
//
//	  set_p( L2_D1,  delta_outer, delta - delta_inner);
//
//	  set_p( L2_D2,  delta_outer, delta - delta_outer);
//
//	  set_p( L2_BD, delta_outer, delta);
//
//	  set_p( R2_U1, delta - delta_outer, delta_inner);
//
//	  set_p( R2_U2, delta - delta_outer, delta_outer);
//						    
//	  set_p( R2_D1, delta - delta_outer, delta - delta_inner);
//
//	  set_p( R2_D2, delta - delta_outer, delta - delta_outer);
//
//	  set_p( R2_BD, delta - delta_outer, delta );
//
//	  set_p( R1_U1, delta - delta_inner, delta_inner);
//
//	  set_p( R1_U2, delta - delta_inner, delta_outer );
//
//	  set_p( R1_D1, delta - delta_inner, delta - delta_inner);
//
//	  set_p( R1_D2, delta - delta_inner, delta - delta_outer);
//
//	  set_p( R1_BD,  delta - delta_inner, delta);
//
//	  set_p( BR_U1, delta, delta_inner);
//
//	  set_p( BR_U2, delta, delta_outer);
//
//	  set_p( BR_D1, delta , delta - delta_inner);
//
//	  set_p( BR_D2, delta , delta - delta_outer );
//
//	  set_p( BR_BD, delta, delta);
//
//	  // setting w, e, n, e, points
//	  // following needs check
//
//	  set_p( W_L2,  delta_outer, center_y);
//	  set_p( W_L1,  delta_inner, center_y);
//	  set_p( E_R1,  delta - delta_inner, center_y);
//	  set_p( E_R2,  delta - delta_outer, center_y);
//	  set_p( E_BR,  delta, center_y );
//
//	  set_p( N_U1, center_x, delta_inner);
//	  set_p( N_U2, center_x, delta_outer);
//	  set_p( S_D1, center_x, delta - delta_inner);
//	  set_p( S_D2, center_x, delta - delta_outer);
//	  set_p( S_BD, center_x, delta);
//	  set_p(  CNT, center_x, center_y);
//
//
//	  set_p(screen_points[l1_u1], L1_U1.x,L1_U1.y);
//	  set_p(screen_points[l1_u2], L1_U2.x,L1_U2.y);
//	  set_p(screen_points[l1_d1], L1_D1.x,L1_D1.y);
//	  set_p(screen_points[l1_d2], L1_D2.x,L1_D2.y);
//	  set_p(screen_points[l1_bd], L1_BD.x,L1_BD.y);
//
//	  set_p(screen_points[l2_u1], L2_U1.x,L2_U1.y);
//	  set_p(screen_points[l2_u2], L2_U2.x,L2_U2.y);
//	  set_p(screen_points[l2_d1], L2_D1.x,L2_D1.y);
//	  set_p(screen_points[l2_d2], L2_D2.x,L2_D2.y);
//	  set_p(screen_points[l2_bd], L2_BD.x,L2_BD.y);
//
//          set_p(screen_points[r2_u1], R2_U1.x,R2_U1.y);
//	  set_p(screen_points[r2_u2], R2_U2.x,R2_U2.y);
//	  set_p(screen_points[r2_d1], R2_D1.x,R2_D1.y);
//	  set_p(screen_points[r2_d2], R2_D2.x,R2_D2.y);
//	  set_p(screen_points[r2_bd], R2_BD.x,R2_BD.y);
//
//	  set_p(screen_points[r1_u1], R1_U1.x,R1_U1.y);
//	  set_p(screen_points[r1_u2], R1_U2.x,R1_U2.y);
//	  set_p(screen_points[r1_d1], R1_D1.x,R1_D1.y);
//	  set_p(screen_points[r1_d2], R1_D2.x,R1_D2.y);
//	  set_p(screen_points[r1_bd], R1_BD.x,R1_BD.y);
//
//	  set_p(screen_points[br_u1], BR_U1.x,BR_U1.y);
//	  set_p(screen_points[br_u2], BR_U2.x,BR_U2.y);
//	  set_p(screen_points[br_d1], BR_D1.x,BR_D1.y);
//	  set_p(screen_points[br_d2], BR_D2.x,BR_D2.y);
//	  set_p(screen_points[br_bd], BR_BD.x,BR_BD.y);
//
//	  set_p(screen_points[w_l2],W_L2.x, W_L2.y);
//	  set_p(screen_points[w_l1],W_L1.x, W_L1.y);
//	  set_p(screen_points[e_r1],E_R1.x, E_R1.y);
//	  set_p(screen_points[e_r2],E_R2.x, E_R2.y);
//	  set_p(screen_points[e_br],E_BR.x, E_BR.y);
//
//	  set_p(screen_points[n_u1],N_U1.x, N_U1.y);
//	  set_p(screen_points[n_u2],N_U2.x, N_U2.y);
//	  set_p(screen_points[s_d1],S_D1.x, S_D1.y);
//	  set_p(screen_points[s_d2],S_D2.x, S_D2.y);
//	  set_p(screen_points[s_bd],S_BD.x, S_BD.y);
//
//	  set_p(screen_points[cnt], CNT.x, CNT.y);
//
//	  set_p(screen_points[e],  E_R.x, E_R.y);
//	  set_p(screen_points[e+1],W_R.x, W_R.y);
//	  set_p(screen_points[e+2],N_R.x, N_R.y);
//	  set_p(screen_points[e+3],S_R.x, S_R.y);
//
//	  set_p(screen_points[e+4],Rdiamond.x, Rdiamond.y);
//	  set_p(screen_points[e+5],Ldiamond.x, Ldiamond.y);
//	  set_p(screen_points[e+6],Udiamond.x, Udiamond.y);
//	  set_p(screen_points[e+7],Ddiamond.x, Ddiamond.y);
//
//	  // correction of origin coordinates
//	  for (int k = 0;  k < numIntersectPoints; k++){
//	      screen_points[k].x += Origin_x;
//	      screen_points[k].y += Origin_y;
//	    }
//	    center_x += Origin_x;
//	    center_y += Origin_y;
//
//	    Arrow_calc( rEastAr, screen_points[e],
//	         screen_points[e+1], screen_points[e+2], screen_points[e+3]);
//	    Arrow_calc( dEastAr,screen_points[e+4] ,
//		 screen_points[e+5], screen_points[e+6], screen_points[e+7]);
//
//	    Ldiamond.x += Origin_x;
//	    Ldiamond.y += Origin_y;
//
//	    Rdiamond.x += Origin_x;
//	    Rdiamond.y += Origin_y;
//
//	    Udiamond.x += Origin_x;
//	    Udiamond.y += Origin_y;
//
//	    Ddiamond.x += Origin_x;
//	    Ddiamond.y += Origin_y;
//
//
//} // end of set_first_cell_points()
//
//// --------------------------------------------------------------
//void
//GraphicWin::set_X_Y_cell(Cell_arr& cl, int y, int x )
//{
//	set_p( cl.center,  center_x + x * delta , center_y + y * delta);
//
//	for (int k = 0; k < numIntersectPoints; k++) {
//	     cl.arr[k].x =  screen_points[k].x + x * delta;
//	     cl.arr[k].y =  screen_points[k].y + y * delta;
//	}
//        int t ;
//	if( Screen[y][x].Type == rectangle_type )
//	    for(int k = 0; k<4; k++) 
//	      for( t = 0; t < 6; t += 2) {
//	       Screen[y][x].CellArrows[k].oneArrow[t]  =
//		     FirstCellArrows[k].oneArrow[t] + x * delta;
//
//	       Screen[y][x].CellArrows[k].oneArrow[t + 1]  =
//		     FirstCellArrows[k].oneArrow[t + 1] + y * delta;
//              }
//
//	else // case of diamond
//	    for(int k = 0; k<4; k++)
//	      for( t = 0; t < 6; t += 2)  {
//	       Screen[y][x].CellArrows[k].oneArrow[t]  =
//		 FirstCellArrows[ k + 4 ].oneArrow[t] + x * delta;
//
//	       Screen[y][x].CellArrows[k].oneArrow[t + 1]  =
//		 FirstCellArrows[ k + 4 ].oneArrow[t + 1] + y * delta;
//	      }
//}    // end of  set_X_Y_cell( );
//// --------------------------------------------------------------
//void
//GraphicWin::display_cell_points(Cell_arr cell)
//{
//	//draw_point( cell.center.x , cell.center.y );
//	//for (int t = 0; t < numIntersectPoints; t++)
//	//     draw_point(cell.arr[t].x, cell.arr[t].y);
//
//
//}
//// ---------------------------------------------------------------------
//// if size of cell > 32 pixel, draw the text into rectangle or diamond
//// else  text not drawed
//int
//GraphicWin::check_font_size()
//{
//   return ( delta >= 32 )?1:0;
//}
//// ---------------------------------------------------------------------
//void
//GraphicWin::draw_txt(int cx,
//		    int cy,
//		    char* name = "noname",
//		    int Color= -1)
//{ int x_coord, y_coord;
//   if ( check_font_size() ){
//      x_coord = center_x + delta * cy ;// rounded coordinates
//      y_coord = center_y + delta * cx; // x go to y ; y go to x
//      /*if (ps_flag!=0)
//       draw_str( name, x_coord, y_coord+delta/30,
//	       (Color == -1) ? textColor : Color, (delta >= 64));
//      else
//       draw_str( name, x_coord, y_coord,
//	       (Color == -1) ? textColor : Color, (delta >= 64));*/
//
//      // here 64  is middle size the side  of screen cell
//   }
//}
//// ---------------------------------------------------------
////void
////GraphicWin::draw_rectangle(int cntr_x,
////			   int cntr_y,
////			   int Color = -1,
////			   int FillStyle = 0 )
////{   int left , top , right , bottom ;
////
////    left = rect_LU_corner.x + cntr_y * delta;
////    top = rect_LU_corner.y + cntr_x * delta;
////    right = rect_RD_corner.x + cntr_y * delta;
////    bottom = rect_RD_corner.y + cntr_x * delta;
////
////    if ( Color < 0 ) Color= ForGrCol;
////    setcolor(Color);
////    if ( FillStyle ) 
////	setfillstyle(FillStyle,Color);
////    if (ps_flag==0)
////     rectangle(left, top, right, bottom);
////    else
////    {
////     fprintf(out," %d xx %d %d yy sub moveto\n",10+left,flip,top);
////     fprintf(out," %d xx %d %d yy sub lineto\n",10+right,flip,top);
////     fprintf(out," %d xx %d %d yy sub lineto\n",10+right,flip,bottom);
////     fprintf(out," %d xx %d %d yy sub lineto\n",10+left,flip,bottom);
////     fprintf(out," %d xx %d %d yy sub lineto\n",10+left,flip,top);
////    }
////}     // end of draw_rectangle proc
////// --------------------------------------------------------------
////void
////GraphicWin::draw_diamond(int c_x,
////			 int c_y,
////			 int Color = -1
////			 )
////{
////	int xOffset = delta * c_y;
////	int yOffset = delta * c_x;
////int Diamond_corners[] = { (Ldiamond.x + xOffset) ,(Ldiamond.y + yOffset) ,
////			  (Udiamond.x + xOffset) ,(Udiamond.y + yOffset) ,
////                          (Rdiamond.x + xOffset) ,(Rdiamond.y + yOffset) ,
////      			  (Ddiamond.x + xOffset) ,(Ddiamond.y + yOffset) ,
////			  (Ldiamond.x + xOffset) , (Ldiamond.y + yOffset) };
////// Here 4 is number of diamond rectangles   = 1 end point
////  if ( Color < 0 ) Color= ForGrCol;
////  draw_poly( 5, Diamond_corners, Color);
////}
////
////// --------------------------------------------------------------
////void
////GraphicWin::draw_circle(int c_x, int c_y, int Color = -1)
////{
////  if ( Color < 0 ) Color= ForGrCol;
////  setcolor(Color);
////
////  circle(Screen[c_x][c_y].crossPoint.center.x,
////	 Screen[c_x][c_y].crossPoint.center.y, delta_outer);
////}
//
////------------------------------------------------------------------------
//void translate_to_pixel(intersect_elem* turn_ptr,
//			       int& x_coord, int& y_coord)
//{
//	int i = 0;
//
//	int t_x = turn_ptr->Get_X_coord();
//	int t_y = turn_ptr->Get_Y_coord();
//	points p = turn_ptr->Get_point();
//
//	i = int(p);
//
//	if( e <= p && p<= s)
//	  if (Screen[t_x][t_y].Type != rectangle_type) {
//              x_coord =  Screen[t_x][t_y].crossPoint.arr[i + 4].x;
//	      y_coord =  Screen[t_x][t_y].crossPoint.arr[i + 4].y;
//	      return;
//	  }
//	x_coord =  Screen[t_x][t_y].crossPoint.arr[i].x;
//	y_coord =  Screen[t_x][t_y].crossPoint.arr[i].y;
//
//}
////------------------------------------------------------------------------
//void draw_arrow(int x_graph, int y_graph, int x, int y,
//                        direction direct, points p)
//{
//
//      if ( e <= p && p <= s ) {
//	//draw_poly(3, Screen[x][y].CellArrows[p - e].oneArrow, WHITE);
//	return;
//      }
//      // case of non - element connection
//      int Temp_arr[6];
//
//      // set Peek of the arrow
//
//      Temp_arr[2] = x_graph;
//      Temp_arr[3] = y_graph;
//
//      // calculate two rest arrow points
//      //calc_arrow_points(Temp_arr, direct);
//	  for (int i = 0; i < 6; i++) {
//		  printf("%d", Temp_arr[i]);
//	  }
//      //draw_poly(3, Temp_arr, WHITE);
//}
//
////------------------------------------------------------------------------
//
//void call_draw_arrow(intersect_elem* curr, intersect_elem* prev)
//{
//   int x_prev, y_prev, x_curr, y_curr;
//
//   int tmp_x = prev->Get_X_coord();
//   int tmp_y = prev->Get_Y_coord();
//
//   x_prev = Screen[tmp_x][tmp_y].crossPoint.arr[int(prev->Get_point())].x;
//   y_prev = Screen[tmp_x][tmp_y].crossPoint.arr[int(prev->Get_point())].y;
//
//   tmp_x = curr->Get_X_coord();
//   tmp_y = curr->Get_Y_coord();
//
//   x_curr = Screen[tmp_x][tmp_y].crossPoint.arr[int(curr->Get_point())].x;
//   y_curr = Screen[tmp_x][tmp_y].crossPoint.arr[int(curr->Get_point())].y;
//
//   if ( x_prev < x_curr )
//     draw_arrow(x_curr, y_curr, tmp_x, tmp_y, WE, curr->Get_point());
//   else
//     if ( x_prev > x_curr )
//	draw_arrow(x_curr, y_curr, tmp_x, tmp_y, EW, curr->Get_point());
//     else
//	 if ( y_prev < y_curr )
//	   draw_arrow(x_curr, y_curr, tmp_x, tmp_y, NS, curr->Get_point());
//         else
//	   draw_arrow(x_curr, y_curr, tmp_x, tmp_y, SN, curr->Get_point());
//
//}
//
////------------------------------------------------------------------------
//void draw_path_pair(List_Intersect_Elem Turns_Lst)
//{
//    int count = 0, i = 0;
//    int* polynom;
//    intersect_elem *first_point, *prev_point,
//		   *curr;
//
//    curr = Turns_Lst.List_pntr();
//    while ( curr ) {
//       count++;
//       curr = curr->Get_next();
//    }
//
//    polynom = new int[ count * 2 ];
//
//    curr = Turns_Lst.List_pntr();
//    first_point = Turns_Lst.List_pntr();
//    prev_point  = first_point->Get_next();
//
//    while ( curr )  {
//	translate_to_pixel(curr, polynom[i], polynom[i+1]);
//	i += 2;
//        curr = curr->Get_next();
//    }
//    //draw_poly(count, polynom, WHITE);
//
//    call_draw_arrow(first_point, prev_point);
//
//    if( polynom )
//	delete polynom ;
//
//}
////------------------------------------------------------------------------
//
//void
//choice_point(points anc_P, int anc_X, int anc_Y, int& str_X, int& str_Y)
//{
//
//  switch (anc_P) {
//  case s:
//	  str_X = Screen[anc_X][anc_Y].crossPoint.arr[s_d1].x+4;
//	  str_Y = Screen[anc_X][anc_Y].crossPoint.arr[s_d1].y;
//	  break;
//  case e:
//	  str_X = Screen[anc_X][anc_Y].crossPoint.arr[e_r1].x;
//	  str_Y = Screen[anc_X][anc_Y].crossPoint.arr[e_r1].y-4;
//	  break;
//  case n:
//	  str_X = Screen[anc_X][anc_Y].crossPoint.arr[n_u1].x+4;
//	  str_Y = Screen[anc_X][anc_Y].crossPoint.arr[n_u1].y;
//	  break;
//
//  case w:
//	  str_X = Screen[anc_X][anc_Y].crossPoint.arr[w_l1].x;
//	  str_Y = Screen[anc_X][anc_Y].crossPoint.arr[w_l1].y-4;
//	  break;
//  }
//}
////------------------------------------------------------------------------
//void
//GraphicWin::draw_port_str(ancest_dist* anc_ptr, int son,
//			  int ind_anc = -1, points P = s)
//{
//   int Color, anc_X, anc_Y, X, Y,
//       str_X, str_Y;
//   char* Str;
//   points anc_P;
//
//   if ( anc_ptr != NULL ) {
//     anc_X = Vec[anc_ptr->Get_ancest()].X,
//     anc_Y = Vec[anc_ptr->Get_ancest()].Y;
//   }
//   else {
//     anc_X = Vec[ind_anc].X;
//     anc_Y = Vec[ind_anc].Y;
//   }
//
//   if ( anc_ptr == NULL || anc_ptr->cir_X == -1 ) {
//     if ( Screen[anc_X][anc_Y].Vec_right_son == son )
//       return;
//     if ( anc_ptr != NULL )
//       anc_P = anc_ptr->my_path.Turn_List.Get_Last()->Get_point();
//     else
//       anc_P = P;
//
//     Str = new char[2];
//     sprintf(Str, "1");
//     Color = LIGHTGREEN;
//   }
//   else  { // non conventional case
//     if ( !Screen[anc_X][anc_Y].TurnList[w].is_full() )
//	    anc_P = w;
//     else if ( !Screen[anc_X][anc_Y].TurnList[e].is_full() )
//	    anc_P = e;
//     else if ( !Screen[anc_X][anc_Y].TurnList[n].is_full() )
//	    anc_P = n;
//     else if ( !Screen[anc_X][anc_Y].TurnList[s].is_full() )
//	    anc_P = s;
//
//     Screen[anc_X][anc_Y].TurnList[anc_P].fill();
//
//     X = anc_ptr->cir_X;
//     Y = anc_ptr->cir_Y;
//
//     Str = Screen[X][Y].Name;
//     Color = RED;
//   }
//
//   choice_point(anc_P, anc_X, anc_Y, str_X, str_Y);
//   //if ( check_font_size() )
//   //  draw_str(Str, str_X, str_Y, Color);
//
//   if ( Color == RED && Screen[anc_X][anc_Y].Vec_left_son == son ) {
//     anc_P = anc_ptr->my_path.Turn_List.Get_Last()->Get_point();
//     choice_point(anc_P, X, Y, str_X, str_Y);
//     Str = new char[2];
//     sprintf(Str, "1");
//     Color = LIGHTRED;
//     //if ( check_font_size() )
//     //  draw_str(Str, str_X, str_Y, Color);
//   }
//}
//
////------------------------------------------------------------------------
//void
//GraphicWin::draw_pathes()
//{
//   ancest_dist *anc_ptr = NULL;
//   int ind_anc, anc_X, anc_Y;
//   points anc_P;
//
//   for(int i=1; i<FileSize; i++) {
//
//     //if ( Vec[i].direct_ancest )   
//     //  draw_path_pair(Vec[i].direct_ancest->Turn_List);
//       
//     anc_ptr = Vec[i].ancest_list.List_pntr();
//     while ( anc_ptr ) {
//       /*if (anc_ptr && anc_ptr->my_path.Turn_List.List_pntr())
//	 draw_path_pair(anc_ptr->my_path.Turn_List);*/
//
//       anc_ptr = anc_ptr->Get_next();
//     }
//   }
//
//   for(int i=1; i<FileSize; i++) {
//
//     if ( Vec[i].direct_ancest ) {
//       anc_X = Vec[i].direct_ancest->Turn_List.Get_Last()->Get_X_coord();
//       anc_Y = Vec[i].direct_ancest->Turn_List.Get_Last()->Get_Y_coord();
//       anc_P = Vec[i].direct_ancest->Turn_List.Get_Last()->Get_point();
//       ind_anc = Screen[anc_X][anc_Y].Vec_node;
//       if ( Screen[anc_X][anc_Y].Vec_right_son != -1 )
//	 draw_port_str(NULL, i, ind_anc, anc_P);
//     }
//
//     anc_ptr = Vec[i].ancest_list.List_pntr();
//     int anc_ind;
//     while ( anc_ptr ) {
//       anc_ind = anc_ptr->Get_ancest();
//       if (anc_ptr && anc_ptr->my_path.Turn_List.List_pntr() &&
//	   Screen[Vec[anc_ind].X][Vec[anc_ind].Y].Vec_right_son != -1)
//	 draw_port_str(anc_ptr, i);
//
//       anc_ptr = anc_ptr->Get_next();
//     }
//   }
//
//   for (int i=0; i<FileSize; i++) {
//     if ( Screen[Vec[i].X][Vec[i].Y].TurnList[s].is_full() == THREE )
//       Screen[Vec[i].X][Vec[i].Y].TurnList[s].release();
//     if ( Screen[Vec[i].X][Vec[i].Y].TurnList[n].is_full() == THREE )
//       Screen[Vec[i].X][Vec[i].Y].TurnList[n].release();
//     if ( Screen[Vec[i].X][Vec[i].Y].TurnList[w].is_full() == THREE )
//       Screen[Vec[i].X][Vec[i].Y].TurnList[w].release();
//     if ( Screen[Vec[i].X][Vec[i].Y].TurnList[e].is_full() == THREE )
//       Screen[Vec[i].X][Vec[i].Y].TurnList[e].release();
//   }
//}  // end of member function 'draw_pathes'
//// --------------------------------------------------------------
////void
////GraphicWin::redraw_graph(GraphicWin* gptr, int xShift, int  yShift)
////{
////   int j, txtColor;
////		 gptr->change_origins( xShift, yShift );
////		 gptr->delta_calc( row );
////		 gptr->set_first_cell_points();
////		 // expected ,what graphic mode was setted
////
////		 gptr->set_txt_just();
////		 if (ps_flag==0)
////		  gptr->clear_draw_area();
////		 for (int i = 0; i < row ; i++)
////		  for (j = 0; j < column ; j++)
////		     gptr->set_X_Y_cell( Screen[i][j].crossPoint, i, j );
////		  //  gptr->display_cell_points( Screen[i][j].crossPoint );
////
////		 setlinestyle(DASHED_LINE, 1, 2);
////		 setcolor(GREEN);
////		 rectangle(Screen[0][0].crossPoint.arr[l2_u2].x-2,
////			   Screen[0][0].crossPoint.arr[l2_u2].y-2,
////			   Screen[row-1][column-1].crossPoint.arr[br_bd].x,
////			   Screen[row-1][column-1].crossPoint.arr[br_bd].y);
////		 setlinestyle(SOLID_LINE, 1, 1);
////
////		 for(int i = 0; i < row ; i++)
////		  for (j = 0; j < column ; j++) {
////		     txtColor = YELLOW;
////
////		     if (Screen[i][j].Vec_node >= 0) {
////		       if (Screen[i][j].Type == rectangle_type)
////			 gptr->draw_rectangle(i, j, LIGHTGREEN);
////		       else
////			 if (Screen[i][j].Type == diamond)
////			   gptr->draw_diamond(i, j, LIGHTGREEN);
////			 else {
////			   gptr->draw_circle(i, j, LIGHTRED);
////			   txtColor = Palette[3];
////			 }
////		       gptr->draw_txt(i, j, Screen[i][j].Name, txtColor);
////		     }
////		  }
////		  draw_pathes();
////} // end of redraw_graph()
