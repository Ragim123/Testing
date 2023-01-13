#ifndef TEXTURE_H
#define TEXTURE_H

#include <corona.h>

typedef unsigned char byte;

class texture
{
public:
	//constructor
	texture(int w, int h);

	//user functions
	int getSampleImage(char* filename);
	void generateTexture(int size);
	void writeTexture(char* filename);

private:
	//private functions
	void convertSample();
	void convertTexture();
	void initializeTexture();
	void findBestMatch(int i, int j, int size);

	//data
	int sample_w, sample_h;
	corona::Image* sample_image;
	void* sample_data;
	byte** sample_red;
	byte** sample_green;
	byte** sample_blue;
	
	int texture_w, texture_h;
	corona::Image* texture_image;
	void * texture_data;
	byte** texture_red;
	byte** texture_green;
	byte** texture_blue;

	byte** red;
	byte** green;
	byte** blue;
};

#endif