
//this function defines the patch match measure with which we compare patches

#include "patch_match_measure.h"

#define LAMDA_MEASURE 0

template <class T>
float ssd_patch_measure(nTupleVolume<T> *imgVolA, nTupleVolume<T> *imgVolB, nTupleVolume<T> *dispField, nTupleVolume<T> *occVol, int xA, int yA, int tA,
						int xB, int yB, int tB, float minVal, const parameterStruct *params)
{
	//declarations
	int i,j,k, p,xAtemp, yAtemp, tAtemp, xBtemp, yBtemp, tBtemp;
	int xMinA,yMinA,tMinA;
	int xMinB,yMinB,tMinB;
    int sumOcc, occA;
	float tempFloat;
    float beta = 50.0;
	float *ptrA, *ptrB;

	float ssd = 0;

	if ( ((imgVolA->patchSizeX) != (imgVolB->patchSizeX)) || ((imgVolA->patchSizeY) != (imgVolB->patchSizeY)) ||
		((imgVolA->patchSizeT) != (imgVolB->patchSizeT)) )
	{
		MY_PRINTF("Error in ssd_minimum_value, the patch sizes are not equal.\n");
		return -1;
	}

    if (params->patchIndexing == 0)
    {
        xMinA = xA-imgVolA->hPatchSizeX;
        yMinA = yA-imgVolA->hPatchSizeY;
        tMinA = tA-imgVolA->hPatchSizeT;
        xMinB = xB-imgVolB->hPatchSizeX;
        yMinB = yB-imgVolB->hPatchSizeY;
        tMinB = tB-imgVolB->hPatchSizeT;
    }
    else if(params->patchIndexing == 1)
    {
        xMinA = xA;
        yMinA = yA;
        tMinA = tA;
        xMinB = xB;
        yMinB = yB;
        tMinB = tB;
    }
    
    sumOcc = 0;
    
    if (params->partialComparison)
    {
        for (k=0; k<imgVolA->patchSizeT; k++)
            for (j=0; j<imgVolA->patchSizeY; j++)
                for (i=0; i<imgVolA->patchSizeX; i++)
                {
                    xAtemp = xMinA + i;
                    yAtemp = yMinA + j;
                    tAtemp = tMinA + k;
                    xBtemp = xMinB + i;
                    yBtemp = yMinB + j;
                    tBtemp = tMinB + k;
                    //do not compare the edges in any case
                    if ((!check_in_boundaries( imgVolA, xAtemp, yAtemp, tAtemp,params)))
                        continue;   //if we are not in the inner boundaries, do not compare

                    sumOcc = sumOcc + (int)(!(*(occVol->get_value_ptr(xAtemp,yAtemp,tAtemp,0))) == 1);

                }
    }
    else    //calculate patch size
    {
        int patchSizeXtemp = min_int(xA + imgVolA->hPatchSizeX,imgVolA->xSize-1) - max_int(xA - imgVolA->hPatchSizeX,0) + 1;
        int patchSizeYtemp = min_int(yA + imgVolA->hPatchSizeY,imgVolA->ySize-1) - max_int(yA - imgVolA->hPatchSizeY,0) + 1;
        int patchSizeTtemp = min_int(tA + imgVolA->hPatchSizeT,imgVolA->tSize-1) - max_int(tA - imgVolA->hPatchSizeT,0) + 1;
        
        sumOcc = patchSizeXtemp * patchSizeYtemp * patchSizeTtemp;
    }
	sumOcc = max_int(sumOcc,1);
    
	for (k=0; k<imgVolA->patchSizeT; k++)
		for (j=0; j<imgVolA->patchSizeY; j++)
			for (i=0; i<imgVolA->patchSizeX; i++)
			{
				xAtemp = xMinA + i;
				yAtemp = yMinA + j;
				tAtemp = tMinA + k;
                
				xBtemp = xMinB + i;
				yBtemp = yMinB + j;
				tBtemp = tMinB + k;
                

                //do not compare if we are not in the boundaries
                if ((!check_in_boundaries( imgVolA, xAtemp, yAtemp, tAtemp,params)))
                    occA = 1;
                else
                    occA = 0;
                if (occA == 1)
                    continue;   //we do not wish to compare this pixel
                /*if we want partial patch comparison*/
                if (params->partialComparison && occVol->xSize >0)
                    occA = (int)(*(occVol->get_value_ptr(xAtemp, yAtemp, tAtemp,0)) == 1);
                if (occA == 1)
                    continue;   //we do not wish to compare this pixel
                
				ptrA = imgVolA->get_begin_ptr(xAtemp, yAtemp, tAtemp);
				ptrB = imgVolB->get_begin_ptr(xBtemp, yBtemp, tBtemp);
                
                /* similarity */
				for (p=0; p<imgVolA->nTupleSize; p++)
				{
					tempFloat = (*(ptrA+p)) - (*(ptrB+p));
					ssd = ssd + (((tempFloat)*(tempFloat))/sumOcc);
                    //ssd = ssd + (abs(tempFloat))/sumOcc;
				}
                
                if( params->gradX != NULL)
                {
// // //                     float gradXtemp = *((params->gradX) + (tAtemp)*(imgVolA->xSize)*(imgVolA->ySize)
// // //                              + (xAtemp)*(imgVolA->ySize) + yAtemp) - 
// // //                             *((params->gradX) + (tBtemp)*(imgVolB->xSize)*(imgVolB->ySize)
// // //                              + (xBtemp)*(imgVolB->ySize) + yBtemp);
// // //                     
// // //                     float gradYtemp = *((params->gradY) + (tAtemp)*(imgVolA->xSize)*(imgVolA->ySize)
// // //                              + (xAtemp)*(imgVolA->ySize) + yAtemp) - 
// // //                             *((params->gradY) + (tBtemp)*(imgVolB->xSize)*(imgVolB->ySize)
// // //                              + (xBtemp)*(imgVolB->ySize) + yBtemp);
//                                                             
                    float normGradXtemp = *((params->normGradX) + (tAtemp)*(imgVolA->xSize)*(imgVolA->ySize)
                             + (xAtemp)*(imgVolA->ySize) + yAtemp) - 
                            *((params->normGradX) + (tBtemp)*(imgVolB->xSize)*(imgVolB->ySize)
                             + (xBtemp)*(imgVolB->ySize) + yBtemp);
                                        
                    float normGradYtemp = *((params->normGradY) + (tAtemp)*(imgVolA->xSize)*(imgVolA->ySize)
                             + (xAtemp)*(imgVolA->ySize) + yAtemp) - 
                            *((params->normGradY) + (tBtemp)*(imgVolB->xSize)*(imgVolB->ySize)
                             + (xBtemp)*(imgVolB->ySize) + yBtemp);

// //                     ssd = ssd + beta*gradXtemp*gradXtemp/sumOcc;
// //                     ssd = ssd + beta*gradYtemp*gradYtemp/sumOcc;
                    ssd = ssd + beta*normGradXtemp*normGradXtemp/sumOcc;
                    ssd = ssd + beta*normGradYtemp*normGradYtemp/sumOcc;
                }
                
		if ((minVal != -1) && (ssd > minVal))
                {
			return(-1);
                }
			}
    
	return(ssd);
}
