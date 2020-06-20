#include "margr.h"

FILE* out;
int xx = 1;
int yy = 1;

//graphics::graphics(int Bgr, int Fgr) : Gdriver(DETECT),
//BackGrCol(Bgr), ForGrCol(Fgr)
//{
//	// initialize graphics and local variables
//	initgraph(&Gdriver, &Gmode, "");
//
//	// read result of initialization
//	int errorcode = graphresult();
//
//	if (errorcode != grOk) {  // an error occurred
//		printf("Graphics error: %s\n", grapherrormsg(errorcode));
//		printf("Press any key to halt:");
//		getch();
//		exit(1); // terminate with an error code
//	}
//	CLIP_ON = 1;
//	// turns on clip mode ( for setviewport )
//	Width = getmaxx();
//	Hight = getmaxy();
//
//	// set background
//	setfillstyle(1, BackGrCol);
//	setcolor(ForGrCol);
//	bar3d(0, 0, Width, Hight, 10, 2);
//	setlinestyle(SOLID_LINE, 1, 1);
//}
//
//Gwindow::Gwindow(graphics* Graph_tmp)
//{
//	Factor = 4;
//	MaxX = getmaxx();
//	MaxY = getmaxy();
//	// draw of draw_area , which has square form
//
//	int Draw_area_corners[] =
//	{ 0,0, 0,MaxY, MaxY,MaxY, MaxY,0, 0,0 };
//	Graph_tmp->draw_poly(5, Draw_area_corners
//		, Palette[4], 0);
//
//	// draw the menu
//
//	int X_Menu = MaxY + 2;
//	int Draw_menu_corners[] = { X_Menu,0, MaxX,0,  MaxX,MaxY, X_Menu,MaxY, X_Menu,0 };
//	Graph_tmp->draw_poly(5, Draw_menu_corners, BLUE, 1);
//	Graph_tmp->draw_poly(5, Draw_menu_corners, Palette[4], 0);
//
//	int Ytext = 20;
//	int Xtext = MaxY + 5;
//
//	settextjustify(LEFT_TEXT, CENTER_TEXT);
//	settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
//	Graph_tmp->draw_str(" MOVING", Xtext, 8 * Ytext, -1, 1);
//	Graph_tmp->draw_str(" DATA", Xtext, 2 * Ytext, -1, 1);
//	Graph_tmp->draw_str(" Window Size", Xtext, 3 * Ytext, -1, 1);
//	Graph_tmp->draw_str(" X Left", Xtext, 5 * Ytext, -1, 1);
//	Graph_tmp->draw_str(" Y Top", Xtext, 6 * Ytext, -1, 1);
//	Graph_tmp->draw_str(" Use the Arrows", Xtext, 9 * Ytext, -1, 1);
//	Graph_tmp->draw_str("   Right", Xtext, 10 * Ytext, -1, 1);
//	Graph_tmp->draw_str("   Left", Xtext, 11 * Ytext, -1, 1);
//	Graph_tmp->draw_str("   Up", Xtext, 12 * Ytext, -1, 1);
//	Graph_tmp->draw_str("   Down", Xtext, 13 * Ytext, -1, 1);
//	Graph_tmp->draw_str(" ZOOM", Xtext, 15 * Ytext, -1, 1);
//	Graph_tmp->draw_str("   End  Zoom In", Xtext, 16 * Ytext, -1, 1);
//	Graph_tmp->draw_str("   home Zoom Out", Xtext, 17 * Ytext, -1, 1);
//	Graph_tmp->draw_str(" F1  for PS File", Xtext, 18 * Ytext, -1, 1);
//	Graph_tmp->draw_str(" ESC to QUIT", Xtext, 19 * Ytext, -1, 1);
//	setviewport(1, 1, 478, 478, 1);
//}
//
////---------------------------------------------------------------
//void
//GraphicWin::set_factor(float fact)
//{
//	int elem_size = Factor * MaxWinSize / (row * 64);
//	Factor = fact;
//	settextjustify(CENTER_TEXT, CENTER_TEXT);
//	settextstyle(DEFAULT_FONT, HORIZ_DIR, elem_size);
//	//setusercharsize(1, 3, 1, 1);
//}
//
//// --------------------------------------------------------------
//// Draw string
//void
//graphics::draw_str(char* name, int X, int Y, int Color, int Factor)
//{
//	if (Factor) settextstyle(DEFAULT_FONT, HORIZ_DIR, Factor);
//
//	if (Color >= 0) setcolor(Color);
//
//	if (ps_flag == 0)
//		outtextxy(X, Y, name);
//	else
//	{
//		fprintf(out, "%3d xx (%s) stringwidth pop 2 div sub %3d %3d yy sub moveto\n"
//			, X + 10, name, flip, Y);
//		fprintf(out, "(%s) show\n", name);
//	}
//}
//
//// --------------------------------------------------------------
//void
//graphics::draw_poly(int PointNum,
//	int* Points,
//	int Color,
//	int FillStyle)
//{
//	if (Color < 0) Color = WHITE;
//	setcolor(Color);
//
//	if (ps_flag == 0)
//		if (FillStyle) {
//			setfillstyle(FillStyle, Color);
//			fillpoly(PointNum, Points);
//		}
//		else drawpoly(PointNum, Points);
//	else            // Write To PostScript File
//	{
//		fprintf(out, "%3d xx %3d %3d yy sub moveto\n", 10 + Points[0], flip, Points[1]);
//		int i;
//		for (i = 1; i < PointNum; i++)
//			fprintf(out, "%3d xx %3d %3d yy sub lineto\n", 10 + Points[i * 2], flip, Points[i * 2 + 1]);
//	}
//}
//// --------------------------------------------------------------
//void
//graphics::draw_point(int x, int y, int Color)
//{
//	if (Color < 0) Color = Palette[1];
//	if (ps_flag == 0)
//		putpixel(x, y, Color);
//	else
//	{
//		fprintf(out, "%3d xx %3d %3d yy sub moveto\n", 10 + x, flip, y);
//		fprintf(out, "%3d xx %3d %3d yy sub lineto\n", 10 + x, flip, y);
//	}
//}
// ---------------------------------------------------------------------
int
disturb_border(int dir = -1)
{
	switch (dir) {
	case SN:  if (!(Screen[0][0].crossPoint.arr[cnt].y >= 480))
		return 0;
		break;
	case NS:  if (!(Screen[row - 1][0].crossPoint.arr[cnt].y <= 0))
		return 0;
		break;
	case EW:  if (!(Screen[0][0].crossPoint.arr[cnt].x >= 480))
		return 0;
		break;
	case WE:  if (!(Screen[0][column - 1].crossPoint.arr[cnt].x <= 0))
		return 0;
		break;
	default:  if (!(Screen[0][column - 1].crossPoint.arr[cnt].x <= 0 ||
		Screen[0][0].crossPoint.arr[cnt].x >= 480 ||
		Screen[row - 1][0].crossPoint.arr[cnt].y <= 0 ||
		Screen[0][0].crossPoint.arr[cnt].y >= 480))
		return 0;
	}

	printf("\007");
	return 1;
} // end of function 'disturb_border'

// ---------------------------------------------------------------------
//void
//graphic_interface()
//{
//	int   Xstart = 0; // picture beginning
//	int   Ystart = 0;
//
//	graphics* Graph = new graphics(Palette[0], Palette[4]);
//
//	GraphicWin* PaintWin = new GraphicWin(Xstart, Ystart, 480, Palette[1]);
	//Gwindow* Main_window = new Gwindow(Graph);
//	PaintWin->redraw_graph(PaintWin, 0, 0);
//	//      printf("%d",PaintWin->delta_());
//
//	int Key;
//	//  while ( ( Key = bioskey(0)) != kESC )
//	//  {
//	//     switch(Key)
//	//     {
//	   //case kUP:
//	   //	   if ( !disturb_border(SN) )
//	   //	     PaintWin->redraw_graph( PaintWin , 0, 10);
//	   //	   break;
//	   //case kDOWN:
//	   //	   if ( !disturb_border(NS) )
//	   //	     PaintWin->redraw_graph( PaintWin , 0, -10);
//	   //	   break;
//	   //case kRIGHT:
//	   //	   if ( !disturb_border(WE) )
//	   //	     PaintWin->redraw_graph( PaintWin , -10, 0);
//	   //	   break;
//	   //case kLEFT:
//	   //	   if ( !disturb_border(EW) )
//	   //	     PaintWin->redraw_graph( PaintWin , 10, 0);
//	   //	   break;
//
//	   //case kEND:
//	   //	   if ( !disturb_border() ) {
//	   //	      PaintWin->set_factor( PaintWin->get_factor() - 0.1);
//	   //	      PaintWin->redraw_graph( PaintWin , 0, 0);
//	   //	   }
//	   //	   break;
//
//	   //case kHOME:
//	   //	   if ( !disturb_border() ) {
//	   //	     PaintWin->set_factor( PaintWin->get_factor() + 0.1);
//	   //	     PaintWin->redraw_graph( PaintWin , 0, 0);
//	   //	   }
//	   //	   break;
//	   //case kF1:
//	   //	 ps_flag=1;
//	   //	 if ((out = fopen(Name, "wt")) == NULL)
//	   //	 {
//	   //	  sound(200),delay(1000),nosound();
//	   //	 }
//	   //	 else {
//	   //	  Name[strlen(Name)-3]=0;  //Removing Extention
//	   //	  fprintf(out,"%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%");
//	   //	  fprintf(out,"%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n");
//	   //	  fprintf(out,"%%%% PostScript Coded GSA File : '%s.gsa'\n",Name);
//	   //	  fprintf(out,"%%%% PostScript ADD-ON , Was Added By Freddy Bembarone\n");
//	   //	  fprintf(out,"%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%");
//	   //	  fprintf(out,"%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n");
//	   //	  fprintf(out,"\n%%%% You May Change The Scaling Paramters Below\n");
//	   //	  fprintf(out,"/xx {  %3d mul } def\n",xx);
//	   //	  fprintf(out,"/yy {  %3d mul } def\n\n",yy);
//	   //	  fprintf(out,"%%%% Please Do'nt Touch Any Thing Below This Line\n");
//	   //	  fprintf(out,"  0 setlinewidth    % thin line\n");
//	   //	  fprintf(out,"  /Times findfont\n");
//	   //	  fprintf(out,"  1 xx 1 yy min %3d mul scalefont setfont\n",(int) (PaintWin->delta_()/12));
//
//	   //	  PaintWin->redraw_graph( PaintWin , 0, 0);
//	   //	  fprintf(out,"stroke\n");
//	   //	  fprintf(out,"showpage\n");
//	   //	  fclose(out);
//	   //	  sound(200),delay(200),sound(400),delay(200),
//	   //	  sound(600),delay(200),nosound();
//	   //	 }
//	   //	 ps_flag=0;
//	   //	 break;
//
//	   // default : sound(200),delay(20),nosound();
//	//     }
//	//  }  // end of while
//
//	cleardevice();
//	closegraph();
//	return;
//}
