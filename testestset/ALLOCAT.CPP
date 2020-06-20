#include "def.h"

int X_reserve= -1, Y_reserve= -1, reserve_flag= 1;
// -------------------------------------------------------------------
void
error(char* Str)
{
    printf("\n    %s \n", Str );
    exit(1);

}
// --------------------------------------------------------------------
int
dist_calc(int f_x, int f_y, int s_x, int s_y)
{
      int h;
      h = abs(f_x - s_x) + abs(f_y - s_y);
      return h;
}

// --------------------------------------------------------------------

void
Load_Table(char* Filename)
{
    FILE* dfl;
    int Index, i = 0;

    if ((dfl = fopen(Filename, "r")) == NULL) {
        /*error("Unable to open data file ");*/
        FileSize = 0;
        return;
    }
    
    fscanf(dfl, " %d", &FileSize);
    if ( FileSize != 0 )
	{
          if ( FileSize >= MaxSize )
	      {
              if (Vec && strlen(Vec->name) > 0)
		        delete Vec;
		  MaxSize = FileSize + 1;
                  shape* NewVec = new shape[ MaxSize ];
		  Vec = NewVec;
	      }
	  for ( i=0; i <= FileSize; i++ ) {
               fscanf(dfl, "%d %5s %d %d", &Index, Vec[i].name ,
		 &(Vec[i].left), &(Vec[i].right));
	   }
	  FileSize++;
      }
    /*else 
	{
	    fscanf(dfl, " %5s %d %d",Vec[0].name,
		      &(Vec[0].left), &(Vec[0].right));
	    i++;
	    while ( !feof(dfl) )	   
	       {
		   if ( i >= MaxSize ) 
		     {
                         shape* NewVec = new shape[ MaxSize*2 ];
			 for ( int j=0; j < MaxSize; j++)  
			     NewVec[j] = Vec[j];
			 MaxSize *= 2;
			 delete Vec;
			 Vec = NewVec;
		     }	 
       
	           fscanf(dfl, " %d %5s %d %d", &Index, Vec[i].name,
			 &(Vec[i].left), &(Vec[i].right));
		   i++;
	       }     
	    FileSize = i;
	}*/
 } // end Load_Table

// -------------------------------------------------------------------------
void
calculate()
{
    double Sq_cell;
    int numb_elem;
    Sq_cell = pow( sqrt( double ( (230400 * 3.0 / 5.0) / double(FileSize)) ), 2.0);
    numb_elem = 480.0 / sqrt(Sq_cell) + (5.0 / 3.0) * FileSize - 1;
    Res_Sq_cell = pow (sqrt( double(230400 / numb_elem) ), 2.0);
    if ( Res_Sq_cell < MINIMAL_SIZE )
	  error ( " Usage : Unable to allocate chart-flow of this size ");
    column = 480.0 / sqrt ( double(Res_Sq_cell)) + 1;
    row = 480.0 / sqrt ( double(Res_Sq_cell)) + 1;
    numb_elem = column*row;
}  // end of calculate

// ----------------------------------------------------------------

void
Fill_screen(int VecIndex, int ln, int col)
{
  Screen[ln][col].Full = 1;
  if( (Vec[VecIndex].name[0] == 'x') || (Vec[VecIndex].name[0] == 'X'))
      Screen[ln][col].Type = diamond;
    else
	Screen[ln][col].Type = rectangle_type;

  // fill central point of the full element in two directions
  Screen[ln][col].TurnList[cnt].fill(0);
  Screen[ln][col].TurnList[cnt].fill(1);

  if( Vec[VecIndex].left )
     Screen[ln][col].Vec_left_son = Vec[VecIndex].left;
  if( Vec[VecIndex].right )
     Screen[ln][col].Vec_right_son = Vec[VecIndex].right;
  strcpy(Screen[ln][col].Name, Vec[VecIndex].name);
  Screen[ln][col].Vec_node = VecIndex;
  Vec[VecIndex].X = ln;
  Vec[VecIndex].Y = col;

}  // end of Fill_screen

//----------------------------------------------------------------------

int
immed_density(int x, int y)
{
  int sum = 0, border = 0;

  if ( !Screen[x+1][y](x+1,y) ) {
    if (Screen[x+1][y].Full) sum++;
    else { X_reserve = x+1; Y_reserve = y; }
  }
  else border++;

  if ( !Screen[x][y+1](x,y+1) ) {
    if (Screen[x][y+1].Full) sum++;
    else { X_reserve = x; Y_reserve = y+1; }
  }
  else border++;

  if ( !Screen[x][y-1](x,y-1) )  {
    if (Screen[x][y-1].Full) sum++;
    else { X_reserve = x; Y_reserve = y-1; }
  }
  else border++;

  if ( !Screen[x-1][y](x-1,y) ) {
    if (Screen[x-1][y].Full) sum++;
    else { X_reserve = x-1; Y_reserve = y; }
  }
  else border++;

  if (sum + border > DENSITY) return 0;
  return 1;

} // end of procedure immed_density
//----------------------------------------------------------------------
int
check_density(int x, int y, int dens = DENSITY)
{
  int sum, border, q1, q2, q3, q4;

  sum = border = q1 = q2 = q3 = q4 = 0;

  if (reserve_flag) {
    X_reserve = x;
    Y_reserve = y;
    reserve_flag = 0;
  }

  if ( Screen[x+1][y](x+1,y) ) border++;
  else if ( Screen[x+1][y].Full ) { sum++; q1++; q4++;}
  if ( Screen[x+1][y+1](x+1,y+1) ) border++;
  else if ( Screen[x+1][y+1].Full ) { sum++; q1++; }
  if ( Screen[x][y+1](x,y+1) ) border++;
  else if ( Screen[x][y+1].Full ) { sum++; q1++; q2++; }
  if ( q1 == 3 ) sum++;

  if ( Screen[x-1][y+1](x-1,y+1) ) border++;
  else if ( (x-1) && Screen[x-1][y+1].Full ) { sum++; q2++; }
  if ( Screen[x-1][y](x-1,y) ) border++;
  else if ( (x-1) && Screen[x-1][y].Full ) { sum++; q2++; q3++; }

  if ( q2 == 3 ) sum++;

  if ( Screen[x-1][y-1](x-1,y-1) ) border++;
  else if ( (x-1) && Screen[x-1][y-1].Full ) { sum++; q3++; }
  if ( Screen[x][y-1](x,y-1) ) border++;
  else if ( Screen[x][y-1].Full ) { sum++; q3++; q4++; }

  if ( q3 == 3 ) sum++;

  if ( Screen[x+1][y-1](x+1,y-1) ) border++;
  else if ( Screen[x+1][y-1].Full ) { sum++; q4++; }

  if ( q4 == 3 ) sum++;

  if (sum + border - 1 > dens) return 0;
  return 1;

 } // end of procedure check_density


//-----------------------------------------------------------------

int
horizontal(int Index_elem, int y1, int y2, int x)
{
  for (int i= y1; i<=y2; i++)
     if ( !Screen[x][i](x,i) && !Screen[x][i].Full && check_density(x, i))
       {
	 Fill_screen(Index_elem, x, i);
	 return 1;
       }
  return 0;
}

//------------------------------------------------------------------

int
vertical(int Index_elem, int y, int x1, int x2)
{
   for (int i= x1; i>=x2; i--)
     if ( !Screen[i][y](i,y) && !Screen[i][y].Full && check_density( i, y) )
       {
	 Fill_screen(Index_elem, i, y);
	 return 1;
       }
   return 0;
}

//------------------------------------------------------------------

int
skip(int Index_elem, int yl, int yr, int x1, int x2)
{
  for (int i= x1; i>=x2; i--)  {
     if (!Screen[i][yl](i,yl) && !Screen[i][yl].Full && check_density(i, yl))
       {
	 Fill_screen(Index_elem, i, yl);
	 return 1;
       }
     if (!Screen[i][yr](i,yr) && !Screen[i][yr].Full && check_density(i, yr))
       {
	 Fill_screen(Index_elem, i, yr);
	 return 1;
       }
  } // end of for(;;)
  return 0;
}

//------------------------------------------------------------------
int
number_of_ancestors(int Index_son)
{
  int Anc_num = 0;

  for (register int i=0; i<FileSize; i++)
    if ( Vec[i].left == Index_son || Vec[i].right == Index_son )
      Anc_num++;

  return Anc_num;
}



//------------------------------------------------------------------
// The procedure allocates elements and fills matrix Screen
int
Allocation( int Index_father, int Index_son )
{
  int I, J, coef = 2;
  int E = -1, W = -1, S = -1, N = -1;
  int YL,YR,XU,XD, fl_immed = 0, Anc_num;

  I = Vec[Index_father].X;
  J = Vec[Index_father].Y;
  if(( Vec[Index_son].X != -1 ) && ( Vec[Index_son].Y != -1 ))
     return 1;

  Anc_num = number_of_ancestors(Index_son);
//-------------- case of rectangle  and diamond ----------------------
  if ( Anc_num > 1 )
    fl_immed = immed_density(I, J);

  if ( fl_immed || Anc_num == 1 ) {

    if (!(Screen[I+1][J](I+1,J)) && !Screen[I+1][J].Full  &&
	check_density(I+1,J, DENSITY-1)) {
      Fill_screen(Index_son, I+1, J);
      return 0;
    }

   if (!(Screen[I][J+1](I,J+1)) && !Screen[I][J+1].Full &&
       check_density(I,J+1, DENSITY-1)) {
     Fill_screen(Index_son, I, J+1);
     return 0;
   }

   if (!(Screen[I][J-1](I,J-1)) && !Screen[I][J-1].Full &&
       check_density(I,J-1, DENSITY-1)) {
     Fill_screen(Index_son, I, J-1);
     return 0;
   }

   if (!(Screen[I+1][J+1](I+1,J+1)) && !Screen[I+1][J+1].Full &&
       check_density(I,J+1, DENSITY-2)) {
     Fill_screen(Index_son, I+1, J+1);
     return 0;
   }

   if (!(Screen[I+1][J-1](I+1,J-1)) && !Screen[I+1][J-1].Full &&
       check_density(I+1,J-1, DENSITY-2)) {
     Fill_screen(Index_son, I+1, J-1);
     return 0;
   }

   if (!(Screen[I-1][J+1](I-1,J+1)) && !Screen[I-1][J+1].Full &&
       check_density(I-1,J+1, DENSITY-2)) {
     Fill_screen(Index_son, I-1, J+1);
     return 0;
   }

   if (!(Screen[I-1][J-1](I-1,J-1)) && !Screen[I-1][J-1].Full &&
       check_density(I-1,J-1, DENSITY-2)) {
     Fill_screen(Index_son, I-1, J-1);
     return 0;
   }

   if (!(Screen[I-1][J](I-1,J)) && !Screen[I-1][J].Full &&
       check_density(I+1,J, DENSITY-1)) {
     Fill_screen(Index_son, I-1, J);
     return 0;
   }

  }

  while (1) {
     if ( E == -1 )
       if ( (J + coef) >= column )
	  E = column - 1;
       else  YR = J + coef;
     if ( W == -1 )
       if ( (J - coef) < 0 )
	  W = 0;
       else  YL = J - coef;
     if ( N == -1 )
       if ( (I - coef) < 0 )
	  N = 0;
       else  XU = I - coef;
     if ( S == -1 )
       if ( (I + coef) >= row )
	  S = row - 1;
       else  XD = I + coef;

     if ((S == -1) && (N == -1) && (E == -1) && (W == -1)) {
       if (horizontal(Index_son, YL, YR, XD)) return 0;
       if (skip(Index_son, YL, YR, XD-1, XU+1)) return 0;
       if (horizontal(Index_son, YL, YR, XU)) return 0;
     }
     else         
     if ((S == -1) && (N == -1) && (E == -1) && (W != -1)) {
       if (horizontal(Index_son, W, YR, XD)) return 0;
       if (vertical(Index_son, YR, XD-1, XU+1)) return 0;
       if (horizontal(Index_son, W, YR, XU)) return 0;
     }
     else
     if ((S == -1) && (N == -1) && (E != -1) && (W == -1)) {
       if (horizontal(Index_son, YL, E, XD)) return 0;
       if (vertical(Index_son, YL, XD-1, XU+1)) return 0;
       if (horizontal(Index_son, YL, E, XU)) return 0;
     }
     else
     if ((S == -1) && (N == -1) && (E != -1) && (W != -1)) {
       if (horizontal(Index_son, W, E, XD)) return 0;
       if (horizontal(Index_son, W, E, XU)) return 0;
     }
     else
     if ((S == -1) && (N != -1) && (E == -1) && (W == -1)) {
       if (horizontal(Index_son, YL, YR, XD)) return 0;
       if (skip(Index_son, YL, YR, XD-1, N)) return 0;
     }
     else
     if ((S == -1) && (N != -1) && (E == -1) && (W != -1)) {
       if (horizontal(Index_son, W, YR, XD)) return 0;
       if (vertical(Index_son, YR, XD-1, N)) return 0;
     }
     else
     if ((S == -1) && (N != -1) && (E != -1) && (W == -1)) {
       if (horizontal(Index_son, YL, E, XD)) return 0;
       if (vertical(Index_son, YL, XD-1, N)) return 0;
     }
     else
     if ((S == -1) && (N != -1) && (E != -1) && (W != -1)) {
       if (horizontal(Index_son, W, E, XD)) return 0;
     }
     else
     if ((S != -1) && (N == -1) && (E == -1) && (W == -1)) {
       if (skip(Index_son, YL, YR, S, XU+1)) return 0;
       if (horizontal(Index_son, YL, YR, XU)) return 0;
     }
     else
     if ((S != -1) && (N == -1) && (E == -1) && (W != -1)) {
       if (vertical(Index_son, YR, S, XU+1)) return 0;
       if (horizontal(Index_son, W, YR, XU)) return 0;
     }
     else
     if ((S != -1) && (N == -1) && (E != -1) && (W == -1)) {
       if (vertical(Index_son, YL, S, XU+1)) return 0;
       if (horizontal(Index_son, YL, E, XU)) return 0;
     }
     else
     if ((S != -1) && (N == -1) && (E != -1) && (W != -1)) {
       if (horizontal(Index_son, W, E, XU)) return 0;
     }
     else
     if ((S != -1) && (N != -1) && (E == -1) && (W == -1)) {
       if (skip(Index_son, YL, YR, S, N)) return 0;
     }
     else
     if ((S != -1) && (N != -1) && (E == -1) && (W != -1)) {
       if (vertical(Index_son, YR, S, N)) return 0;
     }
     else
     if ((S != -1) && (N != -1) && (E != -1) && (W == -1)) {
       if (vertical(Index_son,YL, S, N)) return 0;
     }
     else
     if ((S != -1) && (N != -1) && (E != -1) && (W != -1)) {
       Fill_screen(Index_son, X_reserve, Y_reserve);
       return 0;
     }
     coef++;
  } // end of while, that finds empty cell
} // end of procedure Allocation

// ----------------------------------------------------------------

void
Clean_Screen()
{
  
  register int i,j,a;
  for (i=0; i<row; i++)
   for (j=0; j<column; j++){

     Screen[i][j].Full = 0;
     Screen[i][j].Vec_left_son = -1;
     Screen[i][j].Vec_right_son = -1;
     Screen[i][j].Vec_node = -1;
     strcpy(Screen[i][j].Name,"");


     Screen[i][j].crossPoint.center.x = 0;
     Screen[i][j].crossPoint.center.y = 0;
     for ( a = 0; a <numIntersectPoints; a++){
	   Screen[i][j].crossPoint.arr[a].x = 0;
	   Screen[i][j].crossPoint.arr[a].y = 0;
     }
   }
} // end of Clean_Screen (initialization of Screen)

// ----------------------------------------------------------------

void
chart_flow_alloc()
{
 int Cur_index;
 Queue_list Q;
 int i, j;

   calculate();

   Screen = new screen_elem*[row];
   for (i = 0; i < row; i++)
     Screen[i] = new screen_elem[column];

   // initialization in  Vec  Screen_coordinates

   for( i = 0; i < FileSize; i++ )
   {
      Vec[i].X = -1;
      Vec[i].Y = -1;
   }
   Clean_Screen();
   int place = column / 2;
   Fill_screen( 0 , 0, place );

   for ( i = 0 ; i < column; i++)
	Screen[0][i].Full = 1;
    Fill_screen( Vec[0].left, 1, place );
    Q.Append( Vec[0].left );

 // ----------- Bread First Search algorithm --------------------

 while( Q.LookFirst() )
 {
   Cur_index = Q.Get();
   if( Vec[Cur_index].right )
     {
       // allocation of right son
       if (!Allocation(Cur_index, Vec[Cur_index].right))
          Q.Append( Vec[Cur_index].right );
       reserve_flag = 1;
     }
    if( Vec[Cur_index].left )
     {
       // allocation of left son
       if (!Allocation(Cur_index, Vec[Cur_index].left))
	  Q.Append( Vec[Cur_index].left );
       reserve_flag = 1;
     }

 } // end of while
} // end of function 'chart_flow_alloc'


