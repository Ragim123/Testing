#include "texture.h"
#include <iostream>
#include <time.h>

using namespace std;

texture::texture(int w, int h)
// initialize the texture to size w x h
{
	texture_w = w;
	texture_h = h;
	texture_red = new byte* [texture_w];
	texture_green = new byte* [texture_w];
	texture_blue = new byte* [texture_w];
	for(int i=0; i<texture_w; i++)
	{
		texture_red[i] = new byte[texture_h];
		texture_green[i] = new byte[texture_h];
		texture_blue[i] = new byte[texture_h];
	}
}

int texture::getSampleImage(char* filename)
// read in the sample from file
{
	sample_image = corona::OpenImage(filename, corona::PF_R8G8B8);
	if (!sample_image)
	{
		std::cout<<"Error: image couldn't be loaded.\n";
		return 1;
	}
	sample_w = sample_image->getWidth();
	sample_h = sample_image->getHeight();
	sample_data = sample_image->getPixels();
	convertSample();

	return 0;
}

void texture::generateTexture(int size)
// generate the texture from the sample using a search window of size x size
{
	int i, j, a=0;

	cout<<"Initializing texture...";
	initializeTexture();
	cout<<"done\n";

	cout<<"Performing exhaustive search...\n";

	red = new byte*[size];
	green = new byte*[size];
	blue = new byte*[size];
	for(int x=0; x<size; x++)
	{
		red[x] = new byte[size/2+1];
		blue[x] = new byte[size/2+1];
		green[x] = new byte[size/2+1];
	}
	
	for(j=0; j<texture_h; j++)
	{
		if(j>=double(a*texture_h)/100)
		{
			cout<<a<<"% done\n";
			a+=10;
		}
		for(i=0; i<texture_w; i++)
		{
			findBestMatch(i, j, size);
		}
	}
	cout<<"100% done\n";
	cout<<"Texture generation complete\n\n";

	return;
}

void texture::writeTexture(char* filename)
// write the texture from the stream of bytes to a file
{
	convertTexture();
	texture_image = corona::CreateImage(texture_w, texture_h, corona::PF_R8G8B8, texture_data);
	corona::SaveImage(filename, corona::FF_PNG, texture_image);
	return;
}

void texture::convertSample()
// convert the sample from the stream of bytes to 2d array of pixels
{
	int i, j;
	byte* p = (byte*) sample_data;

	sample_red = new byte*[sample_w];
	sample_green = new byte*[sample_w];
	sample_blue = new byte*[sample_w];
	for(i=0; i<sample_w; i++)
	{
		sample_red[i] = new byte[sample_h];
		sample_green[i] = new byte[sample_h];
		sample_blue[i] = new byte[sample_h];
	}

	for (j=0; j<sample_h; j++)
	{
		for(i=0; i<sample_w; i++)
		{
			sample_red[i][j] = *p++;
			sample_green[i][j] = *p++;
			sample_blue[i][j] = *p++;
		}
	}
	return;
}

void texture::convertTexture()
// convert the texture from 2d array of pixels to stream of bytes for output
{	
	int i, j;
	byte* p = new byte[texture_h*texture_w*3];
	byte* t = p;
	for (j=0; j<texture_h; j++)
	{
		for(i=0; i<texture_w; i++)
		{
			*t = texture_red[i][j];
			*t++;
			*t = texture_green[i][j];
			*t++;
			*t = texture_blue[i][j];
			*t++;
		}
	}
	texture_data = p;
	return;
}

void texture::initializeTexture()
// initialize output texture with random pixels from the sample
{
	int i, j;
	int w, h;

	srand(time(NULL));

	for (j=0; j<texture_h; j++)
	{
		for(i=0; i<texture_w; i++)
		{
			w = rand() % sample_w;
			h = rand() % sample_h;
			texture_red[i][j] = sample_red[w][h];
			texture_green[i][j] = sample_green[w][h];
			texture_blue[i][j] = sample_blue[w][h];
		}
	}
	return;
}

void texture::findBestMatch(int i, int j, int size)
// find the best match for the texture image at pixel (i, j) using a window of size x size
{
	int endw = sample_w-size/2;
	int endh = sample_h;
	int startw = size/2;
	int starth = size/2;
	int bestw, besth;
	int bestd = 200000*size*size;
	int tempd;
	int ti = i-size/2;
	int tj = j-size/2;
	int r, g, b;
	
	// make local texture window
	for(int y=0; y<size/2+1; y++, tj++)
	{
		for(int x=0, ti = i-size/2; x<size; x++, ti++)
		{
			// break if center of window reached (end of causal neighborhood)
			if(y==size/2 && x==size/2)
				break;
			// otherwise
			if(ti < 0)
				ti += texture_w;
			else if(ti >= texture_w)
				ti = ti%texture_w;
			if(tj < 0)
				tj += texture_h;
			else if(tj >= texture_h)
				tj = tj%texture_h;
			red[x][y] = texture_red[ti][tj];
			green[x][y] = texture_green[ti][tj];
			blue[x][y] = texture_blue[ti][tj];
		}
	}
	for(int w=startw; w<endw; w++)
	{
		for(int h=starth; h<endh; h++)
		{
			tempd = 0;
			ti = 0;
			tj = 0;
			
			for(int y=h-size/2; y<h+1; y++, tj++)
			{
				for(int x=w-size/2, ti=0; x<w+size/2+1; x++, ti++)
				{
					if(y==h && x==w || tempd > bestd)
					{
						y = h+1;
						break;
					}
					r = int(red[ti][tj]-sample_red[x][y]);
					g = int(green[ti][tj]-sample_green[x][y]);
					b = int(blue[ti][tj]-sample_blue[x][y]);
					tempd += r*r + g*g + b*b;
				}
			}
			if(tempd < bestd)
			{
				bestw = w;
				besth = h;
				bestd = tempd;
			}
		}
	}
	texture_red[i][j] = sample_red[bestw][besth];
	texture_green[i][j] = sample_green[bestw][besth];
	texture_blue[i][j] = sample_blue[bestw][besth];
	return;
}
