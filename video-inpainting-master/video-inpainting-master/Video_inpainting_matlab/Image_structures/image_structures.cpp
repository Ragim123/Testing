//this functions holds the definitions for the image_structures used in the spatio-temporal patchMatch

#include "image_structures.h"

float min_float(float a, float b)
{
    if (a<b)
        return a;
    else
        return b;
}

float max_float(float a, float b)
{
    if (a>b)
        return a;
    else
        return b;
}

int min_int(int a, int b)
{
    if (a<b)
        return a;
    else
        return b;
}

int max_int(int a, int b)
{
    if (a>b)
        return a;
    else
        return b;
}

float rand_float_range(float a, float b)
{
	if (a == b)
		return a;
	else
		return ((b-a)*((float)rand()/RAND_MAX))+a;
}

int rand_int_range(int a, int b)
{
	if (a == b)
		return a;
	else
		return ( rand()%(b-a+1) + a);
}

float round_float(float a)
{
	float aFloor,aCeil;
	aFloor = (float)floor((float)a);
	aCeil = (float)ceil((float)a);
	if (a<0)	//less than 0
	{
		if ( (a-aCeil) < -0.5)
			return(aFloor);	//round up
		else
			return(aCeil);
	}
	else	//greater or equal to 0
	{
		if ( (a-aFloor) < 0.5)	//round down
			return(aFloor);
		else
			return(aCeil);
	}
}

template <class T>
int sub_to_ind(nTupleVolume<T>* imgVol, int x, int y, int t)
{
    return( t*(imgVol->xSize)*(imgVol->ySize) + x*(imgVol->ySize) + y);
}

template <class T>
void ind_to_sub(nTupleVolume<T>* imgVol, int linearIndex, int *x, int *y, int *t)
{
    *t = (int)floor((float) linearIndex/((imgVol->xSize)*(imgVol->ySize)));
    
    *x = (int) floor((float) (linearIndex - (*t)*(imgVol->xSize)*(imgVol->ySize)) /
            ( imgVol->ySize ) );
    *y = (int) (linearIndex - (*t)*(imgVol->xSize)*(imgVol->ySize) - (*x)*(imgVol->ySize));
}

int sub_to_ind(int xSize, int ySize, int tSize, int x, int y, int t)
{
    return( t*(xSize)*(ySize) + x*(ySize) + y);
}

void ind_to_sub(int xSize, int ySize, int tSize, int linearIndex, int *x, int *y, int *t)
{
    *t = (int)floor((float) linearIndex/((xSize)*(ySize)));
    
    *x = (int) floor((float) (linearIndex - (*t)*(xSize)*(ySize)) /
            ( ySize ) );
    *y = (int) (linearIndex - (*t)*(xSize)*(ySize) - (*x)*(ySize));
}

int sub_to_ind(int nTupleSize, int xSize, int ySize, int tSize, int x, int y, int t, int nTuple)
{
    return( t*(xSize)*(ySize)*(nTupleSize) + x*(ySize*nTupleSize) + y*nTupleSize + nTuple);
}

void ind_to_sub(int nTupleSize, int xSize, int ySize, int tSize, int linearIndex, int *x, int *y, int *t, int *nTuple)
{
    *t = (int)floor((float) linearIndex/(xSize*ySize*nTupleSize));
    
    *x = (int) floor((float) (linearIndex - (*t)*(xSize)*(ySize)*(nTupleSize)) /
            ( ySize*nTupleSize ) );
    *y = (int) floor((float) (linearIndex - (*t)*(xSize*ySize*nTupleSize) - (*x)*(ySize*nTupleSize)) /
            ( nTupleSize ) );
    *nTuple = (int) (linearIndex - (*t)*(xSize*ySize*nTupleSize) - (*x)*(ySize*nTupleSize) - (*y)*nTupleSize);   
}

template <class T>
void patch_index_to_sub(nTupleVolume<T> *imgVol, int patchIndex, int *colourInd,int *xInd, int *yInd, int *tInd)
{
    ASSERT(patchIndex>=0 && (patchIndex<((imgVol->patchSizeT)*(imgVol->patchSizeX)*(imgVol->patchSizeY)*(imgVol->nTupleSize)) ));
        *tInd = (int)floor((float) patchIndex/((imgVol->patchSizeX)*(imgVol->patchSizeY)*(imgVol->nTupleSize)));
	    
		*xInd = (int) floor((float) (patchIndex - (*tInd)*(imgVol->patchSizeX)*(imgVol->patchSizeY)*(imgVol->nTupleSize)) /
				(( imgVol->patchSizeY )*(imgVol->nTupleSize)) );
		*yInd = (int) floor((float)(patchIndex - (*tInd)*(imgVol->patchSizeX)*(imgVol->patchSizeY)*(imgVol->nTupleSize)
			- (*xInd)*(imgVol->patchSizeY)*(imgVol->nTupleSize))/
                (imgVol->nTupleSize));
        *colourInd = (int) (patchIndex - (*tInd)*(imgVol->patchSizeX)*(imgVol->patchSizeY)*(imgVol->nTupleSize) 
                            - (*xInd)*(imgVol->patchSizeY)*(imgVol->nTupleSize) 
							- (*yInd)*(imgVol->nTupleSize) );
}

void parse_patch_match_parameters(const mxArray* prhs,parameterStruct* patchMatchParams)
{
    int i;
    
    //set default parameters
    patchMatchParams->patchSizeX = 5;
    patchMatchParams->patchSizeY = 5;
    patchMatchParams->patchSizeT = 5;
    patchMatchParams->nIters = 10;
    patchMatchParams->w = 100;
    patchMatchParams->alpha = 0.5;
    patchMatchParams->partialComparison = 0;
    patchMatchParams->fullSearch = 0;
    patchMatchParams->patchIndexing = 0;
    patchMatchParams->gradX = NULL;
    patchMatchParams->gradY = NULL;
    patchMatchParams->normGradX = NULL;
    patchMatchParams->normGradY = NULL;
    
    for (i = 0; i< mxGetNumberOfFields(prhs); i++)
    {
            const char* strTemp = mxGetFieldNameByNumber(prhs, i);
        
            if (strcmp("patchSizeX",strTemp) == 0)
                patchMatchParams->patchSizeX = (int)(*mxGetPr(mxGetField(prhs, (mwIndex)0,strTemp)));
            else if(strcmp("patchSizeY",strTemp) == 0)
                patchMatchParams->patchSizeY = (int)(*mxGetPr(mxGetField(prhs, (mwIndex)0,strTemp)));
            else if(strcmp("patchSizeT",strTemp) == 0)
                patchMatchParams->patchSizeT = (int)(*mxGetPr(mxGetField(prhs, (mwIndex)0,strTemp)));
            else if(strcmp("nbItersPatchMatch",strTemp) == 0)
                patchMatchParams->nIters = (int)(*mxGetPr(mxGetField(prhs, (mwIndex)0,strTemp)));
            else if(strcmp("w",strTemp) == 0)
                patchMatchParams->w = (int)(*mxGetPr(mxGetField(prhs, (mwIndex)0,strTemp)));
            else if(strcmp("alpha",strTemp) == 0)
                patchMatchParams->alpha = (float)(*mxGetPr(mxGetField(prhs, (mwIndex)0,strTemp)));
            else if(strcmp("partialComparison",strTemp) == 0)
                patchMatchParams->partialComparison = (int)(*mxGetPr(mxGetField(prhs, (mwIndex)0,strTemp)));
            else if(strcmp("fullSearch",strTemp) == 0)
                patchMatchParams->fullSearch = (int)(*mxGetPr(mxGetField(prhs, (mwIndex)0,strTemp)));
            else if(strcmp("patchIndexing",strTemp) == 0)
                patchMatchParams->patchIndexing = (int)(*mxGetPr(mxGetField(prhs, (mwIndex)0,strTemp)));
            
            else if(strcmp("gradX",strTemp) == 0)
                patchMatchParams->gradX = (float*)(mxGetPr(mxGetField(prhs, (mwIndex)0,strTemp)));
            else if(strcmp("gradY",strTemp) == 0)
                patchMatchParams->gradY = (float*)(mxGetPr(mxGetField(prhs, (mwIndex)0,strTemp)));
            else if(strcmp("normGradX",strTemp) == 0)
                patchMatchParams->normGradX = (float*)(mxGetPr(mxGetField(prhs, (mwIndex)0,strTemp)));
            else if(strcmp("normGradY",strTemp) == 0)
                patchMatchParams->normGradY = (float*)(mxGetPr(mxGetField(prhs, (mwIndex)0,strTemp)));
//            else
//                MY_PRINTF("Error, uknown parameter : %s.\n",strTemp);
    }   
}


template <class T>
int check_in_boundaries( nTupleVolume<T> *imgVol, int x, int y, int t, const parameterStruct *params)
{
    if ( (x>=0) && (y>=0) && (t>=0) &&
    (x < ( (imgVol->xSize) )) && (y < ( (imgVol->ySize) )) && (t < ( (imgVol->tSize) )))
    {
            return 1;
    }
    else
        return 0;
}

//check if the pixel is occluded
template <class T>
int check_in_inner_boundaries( nTupleVolume<T> *imgVol, int x, int y, int t, const parameterStruct *params)
{
	int hPatchSizeX,hPatchSizeY,hPatchSizeT;
    int patchSizeX,patchSizeY,patchSizeT;

	hPatchSizeX = imgVol->hPatchSizeX;
	hPatchSizeY = imgVol->hPatchSizeY;
	hPatchSizeT = imgVol->hPatchSizeT;

    patchSizeX = imgVol->patchSizeX;
	patchSizeY = imgVol->patchSizeY;
	patchSizeT = imgVol->patchSizeT;
    
    if (params->patchIndexing == 0) //centre patch indexing
    {
        if ( (x>=hPatchSizeX) && (y>=hPatchSizeY) && (t>=hPatchSizeT) &&
		(x < ( (imgVol->xSize)-hPatchSizeX )) && (y < ( (imgVol->ySize)-hPatchSizeY )) && (t < ( (imgVol->tSize)-hPatchSizeT )))
        {
            return 1;
        }
        else
            return 0;
    }
    else if (params->patchIndexing == 1) //top left before patch indexing
    {
        if ( (x>=patchSizeX) && (y>=patchSizeY) && (t>=hPatchSizeT) &&
            (x < ( (imgVol->xSize)-patchSizeX-1)) && (y < ( (imgVol->ySize)-patchSizeY-1)) && (t < ( (imgVol->tSize)-hPatchSizeT)))
        {
            return 1;
        }
        else
            return 0;
    }
}

template <class T>
nTupleVolume<T>::nTupleVolume()    //create empty volume
{
    xSize = 0;
    ySize = 0;
    tSize = 0;
	nDims = 4;
    patchSizeX = 0;
	patchSizeY = 0;
	patchSizeT = 0;
	hPatchSizeX = 0;
	hPatchSizeY = 0;
	hPatchSizeT = 0;
    nT = 0;
    nY = 0;
    nX = 0;
	nElsTotal = 0;
	values = NULL;
    indexing = -1;
    destroyValues = 0;
}

template <class T>
nTupleVolume<T>::nTupleVolume(nTupleVolume *imgVolIn)
{
    //copy information
	nTupleSize = imgVolIn->nTupleSize;

	xSize = imgVolIn->xSize;
	ySize = imgVolIn->ySize;
	tSize = imgVolIn->tSize;
	patchSizeX = imgVolIn->patchSizeX;
	patchSizeY = imgVolIn->patchSizeY;
	patchSizeT = imgVolIn->patchSizeT;
	hPatchSizeX = imgVolIn->hPatchSizeX;
	hPatchSizeY = imgVolIn->hPatchSizeY;
	hPatchSizeT = imgVolIn->hPatchSizeT;

    indexing = imgVolIn->indexing;
    nT = imgVolIn->nT;
    nY = imgVolIn->nY;
    nX = imgVolIn->nX;

	nElsTotal = imgVolIn->nElsTotal;
	//copy the image info
	values = (float*)malloc( (size_t) nElsTotal*nTupleSize*sizeof(float));
    memcpy(values,imgVolIn->get_value_ptr(0, 0, 0, 0),nElsTotal*nTupleSize*sizeof(float));

    destroyValues = 1;
}

template <class T>
nTupleVolume<T>::nTupleVolume(int nTupleSizeIn, int xSizeIn, int ySizeIn, int tSizeIn, int indexingIn)
{
    //declarations
	int i;
	nTupleSize = nTupleSizeIn;

	xSize = xSizeIn;
	ySize = ySizeIn;
	tSize = tSizeIn;
	nDims = 4;
	patchSizeX = 0;
	patchSizeY = 0;
	patchSizeT = 0;
	hPatchSizeX = 0;
	hPatchSizeY = 0;
	hPatchSizeT = 0;

    if (indexingIn == 0)    //row first
    {
        nT = (xSize)*(ySize)*(nTupleSize);
        nY = (xSize)*(nTupleSize);
        nX = (nTupleSize);
    }
    else if (indexingIn == 1)   //column first
    {
        nT = (xSize)*(ySize)*(nTupleSize);
        nX = (ySize)*(nTupleSize);
        nY = (nTupleSize);
    }
    else
    {
        MY_PRINTF("Unknown indexing : %d\n", indexingIn);
    }

	nElsTotal = (xSize)*(ySize)*(tSize);
	//get dimensions and total number of elements
	values = (T*)malloc( (size_t) nElsTotal*nTupleSize*sizeof(T));

	for (i=0; i<(nElsTotal*nTupleSize); i++)
		values[i] = 0;
    
    indexing = indexingIn;  //row first
    destroyValues = 1;
}

template <class T>
nTupleVolume<T>::nTupleVolume(int nTupleSizeIn, int xSizeIn, int ySizeIn, int tSizeIn, 
            int patchSizeXIn, int patchSizeYIn, int patchSizeTIn, int indexingIn)
{
	//declarations
	int i;
	nTupleSize = nTupleSizeIn;

	xSize = xSizeIn;
	ySize = ySizeIn;
	tSize = tSizeIn;
	nDims = 4;
	patchSizeX = patchSizeXIn;
	patchSizeY = patchSizeYIn;
	patchSizeT = patchSizeTIn;
	hPatchSizeX = (int)floor((float)patchSizeX/2);
	hPatchSizeY = (int)floor((float)patchSizeY/2);
	hPatchSizeT = (int)floor((float)patchSizeT/2);


    if (indexingIn == 0)    //row first
    {
        nT = (xSize)*(ySize)*(nTupleSize);
        nY = (xSize)*(nTupleSize);
        nX = (nTupleSize);
    }
    else if (indexingIn == 1)   //column first
    {
        nT = (xSize)*(ySize)*(nTupleSize);
        nX = (ySize)*(nTupleSize);
        nY = (nTupleSize);
    }
    else
    {
        MY_PRINTF("Unknown indexing : %d\n", indexingIn);
    }

	nElsTotal = (xSize)*(ySize)*(tSize);
	//get dimensions and total number of elements
	values = (T*)malloc( (size_t) nElsTotal*nTupleSize*sizeof(T));

	for (i=0; i<(nElsTotal*nTupleSize); i++)
		values[i] = 0;
    
    indexing = indexingIn;  //row first
    destroyValues = 1;
}

//create image volume with an already existing array for the values
template <class T>
nTupleVolume<T>::nTupleVolume(int nTupleSizeIn, int xSizeIn, int ySizeIn, int tSizeIn, 
            int patchSizeXIn, int patchSizeYIn, int patchSizeTIn, int indexingIn, T* valuesIn)
{
	//declarations
	nTupleSize = nTupleSizeIn;

	xSize = xSizeIn;
	ySize = ySizeIn;
	tSize = tSizeIn;
	nDims = 4;
	patchSizeX = patchSizeXIn;
	patchSizeY = patchSizeYIn;
	patchSizeT = patchSizeTIn;
	hPatchSizeX = (int)floor((float)patchSizeX/2);
	hPatchSizeY = (int)floor((float)patchSizeY/2);
	hPatchSizeT = (int)floor((float)patchSizeT/2);


    if (indexingIn == 0)    //row first
    {
        nT = (xSize)*(ySize)*(nTupleSize);
        nY = (xSize)*(nTupleSize);
        nX = (nTupleSize);
    }
    else if (indexingIn == 1)   //column first
    {
        nT = (xSize)*(ySize)*(nTupleSize);
        nX = (ySize)*(nTupleSize);
        nY = (nTupleSize);
    }
    else
    {
        MY_PRINTF("Unknown indexing : %d\n", indexingIn);
    }

	nElsTotal = (xSize)*(ySize)*(tSize);
	//get dimensions and total number of elements
	values = valuesIn;
    
    indexing = indexingIn;
    destroyValues = 0;
}

template <class T>
nTupleVolume<T>::~nTupleVolume()
{
	if((xSize)> 0)
    {
        if (destroyValues  == 1)
            free(values);
    }
}

template <class T>
T nTupleVolume<T>::get_value(int x, int y, int t, int z)
{
	//check parameters
	if( (x<0) || (y<0) || (t<0) || (z<0) || (x>=xSize) || (y>=ySize) || (t>=tSize) || (z>=nTupleSize))
	{
		MY_PRINTF("Error, in get_value_nTuple_volume. At least one of the indices is incorrect.\n");
		MY_PRINTF(" x = %d \n y = %d\n t = %d\n z = %d\n",x,y,t,z);
		return(-1);
	}
    return( values[ (t*(nT)) + (y*(nY)) + (x*(nX)) + z] );
}

template <class T>
T* nTupleVolume<T>::get_value_ptr(int x, int y, int t, int z)
{
	//check parameters
	if( (x<0) || (y<0) || (t<0) || (x>=xSize) || (y>=ySize) || (t>=tSize))
	{
		MY_PRINTF("Error, in get_value_nTuple_volume. At least one of the indices is incorrect.\n");
		MY_PRINTF(" x = %d \n y = %d\n t = %d\n",x,y,t);
		return(NULL);
	}
	return( (values) + (  (t*(nT)) + (y*(nY)) + (x*(nX)) + z ) );
}

template <class T>
T* nTupleVolume<T>::get_begin_ptr(int x, int y, int t)
{
	//check parameters
	if( (x<0) || (y<0) || (t<0) || (x>=xSize) || (y>=ySize) || (t>=tSize))
	{
		MY_PRINTF("Error, in get_value_nTuple_volume. At least one of the indices is incorrect.\n");
		MY_PRINTF(" x = %d \n y = %d\n t = %d\n",x,y,t);
		return(NULL);
	}
	return( (values) + ( t*(nT) + y*(nY) + x*(nX) ) );
}

template <class T>
void nTupleVolume<T>::set_value(int x, int y, int t, int z, T value)
{
	//check parameters
	if( (x<0) || (y<0) || (t<0) || (z<0) || (x>=xSize) || (y>=ySize) || (t>=tSize) || (z>=nTupleSize))
	{
		MY_PRINTF("Error, at least one of the indices is incorrect.\n");
		MY_PRINTF("x = %d \n y = %d\n t = \n z = %d\n",x,y,t,z);
	}
	values[ (t*(nT)) + (y*(nY)) + (x*(nX)) + z] = value;
}

//copy the pixel array from the point in imgVolA(xA,yA,tA) to imgVolB(xB,yB,tB)
template <class T>
void copy_pixel_values_nTuple_volume(nTupleVolume<T>* imgVolA, nTupleVolume<T>* imgVolB, int xA, int yA, int tA, 
									 int xB, int yB, int tB)
{
	int nTupleSize;

	int nTA = imgVolA->nT;
	int nYA = imgVolA->nY;
	int nXA = imgVolA->nX;

	int nTB = imgVolB->nT;
	int nYB = imgVolB->nY;
	int nXB = imgVolB->nX;

	nTupleSize = imgVolA->nTupleSize;
	//copy vector
	memcpy((imgVolB->get_begin_ptr(0,0,0) + xB*nXB+yB*nYB+tB*nTB), (imgVolA->get_begin_ptr(0,0,0) + xA*nXA+yA*nYA+tA*nTA), nTupleSize*sizeof(T));
	//for (i=0; i<nTupleSize; i++)
	//	set_value_nTuple_volume(imgVolB,xB,yB,tB,i,get_value_nTuple_volume(imgVolA,xA,yA,tA,i));
}

template <class T>
void clamp_coordinates(nTupleVolume<T>* imgVolA, int *x, int *y, int *t)
{
    //x coordinates
    *x = max_int(min_int( *x, (imgVolA->xSize)-1),0);
    *y = max_int(min_int( *y, (imgVolA->ySize)-1),0);
    *t = max_int(min_int( *t, (imgVolA->tSize)-1),0);
}
