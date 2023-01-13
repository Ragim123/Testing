#include <iostream>
#include <fstream>
#include <corona.h>
#include "texture.h"
#include <string.h>
#include <time.h>

using namespace std;

int main()
{
	int size = 512;
	double time;
	int window_size[3] = {5, 7, 9};
	char* files[7] = {"green_pattern", "grid", "orange_flowers", "peppers", "purple_flowers", "stones", "water"};
	std::ofstream out("times_exhaustive.txt");
	char input[40];
	char output[40];
	texture* t;
		
	for(int j=0; j<1; j++)
	{
		strcpy(input, files[j]);
		strcat(input, ".jpg");
		
		for(int i=0; i<1; i++)
		{
			time = clock();
			t = new texture(size, size);
			if(t->getSampleImage(input) != 0)
				return 1;
			t->generateTexture(window_size[i]);
			
			strcpy(output, files[j]);
			strcat(output, "_textured_");
			if(i==0)
				strcat(output, "5.png");
			else if(i==1)
				strcat(output, "7.png");
			else
				strcat(output, "9.png");
			
			t->writeTexture(output);
			cout<<files[j]<<" using window "<<window_size[i]<<"x"<<window_size[i]<<"\n";
			cout<<"Time elapsed: "<<(clock()-time)/CLK_TCK<<" seconds\n";
		}
	cout<<"\a";
	}
	return 0;
}