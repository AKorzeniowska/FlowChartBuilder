#include "LogicSegment.h"
#include "DEF.H"
#include "ALL_CONN.h"


void fileWriter(List_Intersect_Elem Turns_Lst, FILE* stream) {
	intersect_elem* curr;

	curr = Turns_Lst.List_pntr();

	while (curr) {
		printf("%d, %d \n", curr->Get_X_coord(), curr->Get_Y_coord());
		fprintf(stream, " %d,%d ", curr->Get_X_coord(), curr->Get_Y_coord());
		curr = curr->Get_next();
	}

}

//LogicSegment::LogicSegment(void)
//{
//	return;
//}

void /*LogicSegment::*/createGraphData(char* arg)
{
	//arg = "and.gsa";
	char Name[256];
	Vec = new shape[MaxSize];
	int filler = 0;
	for (int j = 0; j < 256; j++) {
		if (arg[j] == '.') {
			filler = 1;
			Name[j] = '\0';
			continue;
		}
		if (filler == 0)
			Name[j] = arg[j];
		else if (filler == 1)
			Name[j] = '\0';
	}

	FILE* logger;
	char loggerName[] = "logger.txt";
	logger = fopen(loggerName, "w");

	Load_Table(arg);
	printf("\n BDD has %d elements.\n\n", FileSize);

	if (FileSize == 0) {
		fprintf(logger, "invalid file data - number of nodes was 0 for %s of length %d", arg, strlen(arg));
		fclose(logger);
		return;
	}

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
	fclose(logger);

	// release first row
	for (int k = 0; k < column; k++) 
		if (Screen[0][k].Vec_node == -1)
			Screen[0][k].Full = 0;

	printf("\nPlease wait, tracing in process...\n");
	Connect_All();


	Name[strlen(Name) - 3] = 0;  // Prepare PostScript File Name
	strcat(Name, "ps");
	ancest_dist* pointer;

	stream3 = fopen(filename2, "w");
	shape vec;

	for (int i = 0; i < FileSize; i++) {
		vec = Vec[i];
		fprintf(stream3, "%d %5s ||", i, vec.name);
		if (vec.direct_ancest)
			fileWriter(vec.direct_ancest->Turn_List, stream3);



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
				fileWriter(anc_ptr->my_path.Turn_List, stream3);
				fprintf(stream3, "|");
			}
			anc_ptr = anc_ptr->Get_next();
		}
		fprintf(stream3, "\n");
	}
	fclose(stream3);
	fclose(logger);
}

int testDLL(int a)
{
	return a;
}

void
main(int argc, char* argv[])
{
	char arg[] = "ands.gsa";
	/*LogicSegment logic;
	logic.*/createGraphData(arg);
	int x = testDLL(5);
	printf("%d", x);
	//if ( argc < 2 )
	//error(" Usage : name_of_program   data_file");

} // end of 'Main' function