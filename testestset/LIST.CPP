#include "def.h"

// Constructor
screen_elem::screen_elem()
{
}

void ancest_dist::Copy_Lst(List_Intersect_Elem& Lst)
{
   int Dist;
   intersect_elem* cur = Lst.List_pntr();
   while ( cur ) {
     Dist = dist_calc(cur->Get_X_coord(), cur->Get_Y_coord(),
		      Vec[Get_ancest()].X, Vec[Get_ancest()].Y);
     Possible_Intersect_Lst.Order_Insert(cur->Get_X_coord(),
		     cur->Get_Y_coord(), cur->Get_point(), Dist,
		     cur->Get_dir());
     cur = cur->Get_next();
   }

   cur = Possible_Intersect_Lst.List_pntr();
   if ( cur )
     while ( cur->Get_next() )
       cur = cur->Get_next();

   Possible_Intersect_Lst.Set_Last_ptr(cur);

}

//---------------------------------------------------------------------

void
List::Set_last_conn(ancest_dist *prev, ancest_dist *curr)
{
   if ( !prev ) {
     if ( !curr )
       last_connected->Set_next(first);
     else
       last_connected = first;
   }
   else
     if ( prev == last_connected )
       last_connected = curr;
     else  {
       prev->Set_next(curr->Get_next());
       curr->Set_next(last_connected->Get_next());
       last_connected->Set_next(curr);

       // set first ancestor
       if ( last_connected->Get_ancest() == -1 )
         first = curr;
       last_connected = curr;
     }
}

//---------------------------------------------------------------------

void List::Insert_First(ancest_dist* new_first, ancest_dist* next_ptr)
{
   next_ptr->Set_next(new_first->Get_next());
   new_first->Set_next(first);
   first = new_first;
}

//---------------------------------------------------------------------

void List::Insert(int dist, int father)
 {
    ancest_dist* temp;
    ancest_dist* prev_ptr = first;
    ancest_dist* cur_ptr = first;
    if (first) {
        while (cur_ptr)
	    if (cur_ptr->Get_dist() >= dist)
	       if ( cur_ptr->Get_next() ) {
		 prev_ptr = cur_ptr;
		 cur_ptr = cur_ptr->Get_next();
	       }
	       else {
		     temp = new ancest_dist(dist, father, NULL);
                     cur_ptr->Set_next(temp);
		     break;
		 }

	    else  {
		   temp = new ancest_dist(dist, father, cur_ptr);
		   if ( first->Get_dist() >= dist)
		     prev_ptr->Set_next(temp);
		   else first = temp;
                   break;
		}
   }
     else
         first = new ancest_dist(dist, father, NULL);
}
//------------------------------------------------------------------------

void List::Delete_Elem(int NumVecAncest)
{
    ancest_dist* prev = first;
    ancest_dist* curr = first;
    while ( curr ) 
	if ( curr->Get_ancest() == NumVecAncest ) {
	    if ( curr == first )
		first = NULL;
	    else
		prev->Set_next(curr->Get_next());
	    delete curr;
	    break;
	}
        else {
	    prev = curr;
	    curr = curr->Get_next();
	}
} 
	
//------------------------------------------------------------------------

int List::Get()
{
   ancest_dist* temp = first;
   int num_in_vec = first->Get_ancest();
   first = temp->Get_next();
   delete temp;
   return num_in_vec;
}

//-----------------------------------------------------------------------

List::~List()
{
  /* ancest_dist* tmp; 
   while ( first ) {
      tmp = first;
      first = first->Get_next();
      delete tmp;
   }
   first = cur_pntr = NULL; */
}

//-----------------------------------------------------------------------


void Queue_list::Append( int elem )
{
   if ( last )
       {
         last->next = new Queue( elem , 0 );
         last = last->next;
       }
       else
        {
           last = new Queue( elem , 0 );
           first = last;
        }
}
int Queue_list::Get()
{
    if ( first )
     {
        Queue* Res = first;
	int elem1 = first->Index_Vec;
	if (first == last)
	  last = first->next;
	first = first->next;
	delete Res;
        return elem1;
     }
}

int Queue_list::LookFirst()
{
     if( first )
        return 1;
     return 0;
}

//-----------------------------------------------------------------------

void
List_Intersect_Elem::Insert(int x, int y, points p, int dir)
{
    if (last) {
       intersect_elem* temp = new intersect_elem(x, y, p, NULL, dir);
       last->Set_next(temp);
       last = temp;
     }
    else
       first = last = new intersect_elem(x, y, p, NULL, dir);
}

//-----------------------------------------------------------------------

void
List_Intersect_Elem::Insert_First(int x, int y, points p, int dir)
{
       intersect_elem* new_first;

       if (first)  {
	 new_first = new intersect_elem(x, y, p, first, dir);
	 first = new_first;
       }
       else
	 first = last = new intersect_elem(x, y, p, NULL, dir);

}

//-----------------------------------------------------------------------

//  This function removes element, that pointed by pntr
//  from list
void
List_Intersect_Elem::Remove_Elem(intersect_elem* pntr)
{
    intersect_elem *prev, *curr;
    prev = curr = first;
    while ( curr ) {
       if ( curr == pntr ) {
	  if ( curr == first ) 
	     if ( first == last ) 
	       first = last = NULL;
	     else
	       first = first->Get_next();
	  else { 
	     prev->Set_next(curr->Get_next());
	     last = prev;
	  }

	  delete curr;
	  break;
       }
       prev = curr;
       curr = curr->Get_next();
    }
}      
//----------------------------------------------------------------------------

void
List_Intersect_Elem::Order_Insert(int x, int y, points p, int dist, int dir)
 {
    intersect_elem* temp;
    intersect_elem* prev_ptr = first;
    intersect_elem* cur_ptr = first; 
    if (first) {
        while (cur_ptr)
            if (cur_ptr->Get_dist() < dist)
               if ( cur_ptr->Get_next() ) {
                 prev_ptr = cur_ptr;
                 cur_ptr = cur_ptr->Get_next();
	       }
	       else {
		 temp = new intersect_elem(x, y, p, dist, dir, NULL);
		 cur_ptr->Set_next(temp);
		 break;
		 }

	    else  {
	      temp = new intersect_elem(x, y, p, dist, dir, cur_ptr);
	      if ( first->Get_dist() < dist)
		prev_ptr->Set_next(temp);
	      else first = temp;
              break;
	   }
   }
     else
	 first = new intersect_elem(x, y, p, dist, dir, NULL);
}
//------------------------------------------------------------------------

void
List_Intersect_Elem::Delete_List(intersect_elem* From)
{
   intersect_elem *tmp, *curr = From;

   if ( ! curr ) { // by default deletes all list
      curr = first;
      first = last = NULL;
   }
   else {
     curr = From->Get_next();
     From->Set_next(NULL);
     last = From;
   }

   while ( curr ) {
      tmp = curr;
      curr = curr->Get_next();
      delete tmp;
   }
   
}

//-------------------------------------------------------------------------
// appends list by next to the From element of Lst
void
List_Intersect_Elem::Append(List_Intersect_Elem& Lst, intersect_elem* From)
{
   if ( !From ) {
     if (! first)
       first = Lst.first;
     else
       last->Set_next(Lst.first);
     
     last = Lst.last;
     Lst.first = Lst.last = NULL;
   }
   else {
     last->Set_next(From->Get_next());
     last = Lst.last;
     From->Set_next(NULL);
     Lst.last = From;
   }
}

//-------------------------------------------------------------------
// reverses list
void
List_Intersect_Elem::Reverse()
{
  intersect_elem *curr = first, *tmp_ptr, *tmp_next;

  if ( curr )  {
    tmp_next = curr->Get_next();
    curr->Set_next(NULL);
  }
  else
    return;

  while ( tmp_next ) {
    tmp_ptr = tmp_next->Get_next();
    tmp_next->Set_next(curr);
    curr = tmp_next;
    tmp_next = tmp_ptr;
  }

  last = first;
  first = curr;

}

//-------------------------------------------------------------------
// prints path between two elements :
//                                    transit elements coordinates
//                                    and its lines.              

void
Path::print_path()
{
  FILE *stream;

/* open a file for update */
	stream = fopen("DUMMY.FIL", "a");

/* write some data to the file */

    fprintf(stream," \n");
    fprintf(stream," Path between Son = %d and Ancestor = %d \n",
	     Target_N, Source_N);
    intersect_elem* curr;
    List_Intersect_Elem tmp_lst;

   fprintf(stream," TURN POINTS \n");
   tmp_lst = Turn_List;
   curr = tmp_lst.List_pntr();
   while ( curr ) {
       fprintf(stream," X = %3d , Y = %3d, turn = ",
		curr->Get_X_coord(),
		curr->Get_Y_coord() );
       switch ( curr->Get_point() )   {
       case l1_u1 :  fprintf(stream,"l1_u1, "); break;
       case l1_u2 :  fprintf(stream,"l1_u2, "); break;
       case l1_d1 :  fprintf(stream,"l1_d1, "); break;
       case l1_d2 :  fprintf(stream,"l1_d2, "); break;
       case l1_bd :  fprintf(stream,"l1_bd, "); break;
       case l2_u1 :  fprintf(stream,"l2_u1, "); break;
       case l2_u2 :  fprintf(stream,"l2_u2, "); break;
       case l2_d1 :  fprintf(stream,"l2_d1, "); break;
       case l2_d2 :  fprintf(stream,"l2_d2, "); break;
       case l2_bd :  fprintf(stream,"l2_bd, "); break;
       case r1_u1 :  fprintf(stream,"r1_u1, "); break;
       case r1_u2 :  fprintf(stream,"r1_u2, "); break;
       case r1_d1 :  fprintf(stream,"r1_d1, "); break;
       case r1_d2 :  fprintf(stream,"r1_d2, "); break;
       case r1_bd :  fprintf(stream,"r1_bd, "); break;
       case r2_u1 :  fprintf(stream,"r2_u1, "); break;
       case r2_u2 :  fprintf(stream,"r2_u2, "); break;
       case r2_d1 :  fprintf(stream,"r2_d1, "); break;
       case r2_d2 :  fprintf(stream,"r2_d2, "); break;
       case r2_bd :  fprintf(stream,"r2_bd, "); break;
       case br_u1 :  fprintf(stream,"br_u1, "); break;
       case br_u2 :  fprintf(stream,"br_u2, "); break;
       case br_d1 :  fprintf(stream,"br_d1, "); break;
       case br_d2 :  fprintf(stream,"br_d2, "); break;
       case br_bd :  fprintf(stream,"br_bd, "); break;
       case n_u1  :  fprintf(stream," n_u1, "); break;
       case n_u2  :  fprintf(stream," n_u2, "); break;
       case s_d1  :  fprintf(stream," s_d1, "); break;
       case s_d2  :  fprintf(stream," s_d2, "); break;
       case s_bd  :  fprintf(stream," s_bd, "); break;
       case w_l1  :  fprintf(stream," w_l1, "); break;
       case w_l2  :  fprintf(stream," w_l2, "); break;
       case e_r1  :  fprintf(stream," e_r1, "); break;
       case e_r2  :  fprintf(stream," e_r2, "); break;
       case e_br  :  fprintf(stream," e_br, "); break;
       case cnt   :  fprintf(stream,"  cnt, "); break;
       case e     :  fprintf(stream,"    e, "); break;
       case w     :  fprintf(stream,"    w, "); break;
       case s     :  fprintf(stream,"    s, "); break;
       case n     :  fprintf(stream,"    n, "); break;
}  // end of switch for different kinds of turn-points
       fprintf(stream," dir = %d \n", curr->Get_dir());
       curr = curr->Get_next();
   } // end of while for points
  // close the file
  fclose(stream);
} // end of print_path       		 