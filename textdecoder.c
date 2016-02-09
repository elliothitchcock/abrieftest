#include <stdio.h>
#include <stdlib.h>

float fltopen(char chunk[], int);

main()
{
	int i, pos, dflow;
	float fflow1, fflow2;
	char filename[60];
	
	pos = 0;
	FILE *fr;								/*Sets file to read as pointer fr*/
	FILE *fp;								/*Sets file to print to as pointer fp*/
		
	fp = fopen ("data.txt", "w"); 										/*Opens write to clear*/
	
	if (fp != 0){
		fprintf(fp, "Date\t  Time\t\tFlow(l/m)\t+Total (l)\t-Total(l)\n"); 	/*Sets up headings*/
		fclose(fp);															/*Closes file*/	
	}
	else{
		printf("Error in opening write file\n");
		exit(1);
	}
	
	printf("\nEnter filename of capture: ");
	
	scanf("%59s", filename);
		
	fr = fopen (("%59s", filename), "rb");		/*Opens file as read binary*/
	
	if (fr != 0)
		NULL;
	else{
		printf("error in reading file\n");
		exit(1);
	}

	long len = filelength(fileno(fr)); 		/*Finds length of file*/
	char data_chunk[16];					/*Initialises smaller buffer for data storage*/
	char buffer[len];						/*Sets an array for data bytes*/
	
	fread (buffer, sizeof(buffer), 1, fr); 	/*Copies binary to buffer*/
	fclose(fr);								/*Closes file*/
	
	i = 2;									/*Starts at 2 due to garbage data*/
	
	fp = fopen ("data.txt", "a");			/*Open file to be appended*/
	
	while (i < sizeof(buffer)){ 			/*Loops until end of file*/
		
		pos = ((i - 2) % 16);				/*Assigns value 0-15 each loop*/
		data_chunk[pos] = buffer[i];		/*Corresponding values to each array*/
		
		if (pos == 15 && data_chunk[0] != -1){ 			/*At end of data set*/
														/*Ignores error data (negative data)*/
			dflow = data_chunk[7] | data_chunk[6] << 8; /*flow int made decimal*/

			fflow1 = fltopen(data_chunk, 8);			/*Calls function to change 4 bytes to single float*/
			fflow2 = fltopen(data_chunk, 12);			/*Two floats, so two function calls*/
						
			fprintf(fp, "%d/%d/%-3d %d:%d:%-3d \t%-5d\t\t%-4.2f\t\t%-2.2f \n",
			data_chunk[0], data_chunk[1], data_chunk[2],/*Typesetting for printing to filelength*/
			data_chunk[5], data_chunk[4], data_chunk[3],/*First 6 are decimals, rest have to be 'translated'*/
			dflow, fflow1, fflow2);						/*Includes spacing for neatness in output filelength*/
														/*Floats kept to 2dp, otherwise too long*/
			}
		i++;											/*Increments after each loop, including after potential*/
		}												/*data print to file*/
	fclose(fp);
	printf("Output data can be found in 'data.txt'\n");
}

float fltopen(char chunk[], int start)					/*Function outputs a float from 4 bytes*/
{														/*Takes an input list and an index to start from*/
		union {
			char chars1[4];								/*For the most part, I don't know what union does*/
			float f;									/*But it does take 4 input bytes*/
		} u;											/*Taken from chunk[] value, and outputs as a float*/
														
		int k;
		float final;
		for (k = 0; k < 4; k++)
		  u.chars1[3-k] = chunk[k+start];
		final = u.f;									/*Unneat, but didn't compile otherwise*/
		return final;									/*u.f couldn't be returned by itself*/
}
