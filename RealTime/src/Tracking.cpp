#include "Tracking.h"
#include "Preprocessing.h"

double ** create_gaussian_filter(float sigma, int size)
{
	double **G=dmatrix(0,size-1,0,size-1);
	float x,y;
	double sum=0;
	
	for (int i=0;i<size;i++){
		for(int j=0;j<size;j++){
			x=i-size/2;
			y=j-size/2;
			G[i][j]=exp(- (pow(x,2)+pow(y,2))/(2*pow(sigma,2)) )/(2*PI*pow(sigma,2));
			sum+=G[i][j];
		}
	}
	// printf("Gaussian filter :\n");
	for (int i=0;i<size;i++){
		for(int j=0;j<size;j++){
			G[i][j]/=sum;
			// printf("%lf ",G[i][j]);

		}
		// printf("\n");
	}

	return G;
}

double ** harris(byte **I, double **Ix, double **Iy, double **C, double** filter, int filter_size, float lambda, long nrl, long nrh, long ncl, long nch, int start, int end)
{
	double Ix_square, Iy_square, IxIy;
	double filterValue, IxValue, IyValue;

	int max=filter_size/2;

	for (int x=start+max; x<=end-max; x++)
	{
		for(int y=ncl+max; y<=nch-max; y++)
		{
			Ix_square=0;
			Iy_square=0;
			IxIy=0;

			for (int i=-max;i<=max;i++)
			{
				for(int j=-max;j<=max;j++)
				{
					filterValue = filter[i+max][j+max];
					IxValue = Ix[x+i][y+j];
					IyValue = Iy[x+i][y+j];

					Ix_square += filterValue*IxValue*IxValue;
					Iy_square += filterValue*IyValue*IyValue;
					IxIy += filterValue*(IxValue*IyValue);

				}
			}

			C[x][y]=(Ix_square*Iy_square)-(IxIy)-(lambda*(Ix_square+Iy_square)*(Ix_square+Iy_square));
		}
	}
}

double ** gradient_direction_interest_points(byte **I, double** filter, int filter_size, long nrl, long nrh, long ncl, long nch)
{
	double ** C;
	double ** Ix;
	double ** Iy;
	double Ix_square,Ix_square_filtered, Iy_square,Iy_square_filtered, IxIy, IxIy_filtered;

	int max=filter_size/2;
	double GradFilter[3][3]={ {1,1,1},{1,0,1},{1,1,1} };

	C= dmatrix(nrl, nrh, ncl, nch);
	Ix=dmatrix(nrl,nrh,ncl,nch);
	Iy=dmatrix(nrl,nrh,ncl,nch);

	sobel(I,Ix,Iy,nrl,nrh,ncl,nch);

	for (int x=nrl+max; x<nrh-max; x++)
		{
			for(int y=ncl+max; y<nch-max; y++)
			{
				Ix_square=0;
				Ix_square_filtered=0;
				Iy_square=0;
				Iy_square_filtered=0;
				IxIy=0;
				IxIy_filtered=0;
				
				for (int i=-max;i<=max;i++)
				{
					for(int j=-max;j<=max;j++)
					{
						Ix_square+= pow(Ix[x+i][y+j],2);
						Ix_square_filtered+= GradFilter[i+max][j+max]*pow(Ix[x+i][y+j],2);
						Iy_square+= pow(Iy[x+i][y+j],2);
						Iy_square_filtered+= GradFilter[i+max][j+max]*pow(Iy[x+i][y+j],2);
						IxIy+=Ix[x+i][y+j]*Iy[x+i][y+j];
						IxIy_filtered+= GradFilter[i+max][j+max]*Ix[x+i][y+j]*Iy[x+i][y+j];
					}
				}

				C[x][y]=(Ix_square*Iy_square_filtered)+(Iy_square*Ix_square_filtered)-(2*IxIy*IxIy_filtered);
			}
		}

	free_dmatrix(Ix,nrl, nrh, ncl, nch);
	free_dmatrix(Iy,nrl, nrh, ncl, nch);
	return C;
}

byte ** convolve(byte **I, double** filter, int filter_size, long nrl, long nrh, long ncl, long nch)
{	
	byte ** out;
	out=bmatrix(nrl,nrh,ncl,nch);

	double ** C;
	C= dmatrix(nrl, nrh, ncl, nch);
	int max=filter_size/2;

	for (int x=nrl+max; x<nrh-max; x++)
			{
				for(int y=ncl+max; y<nch-max; y++)
				{
					
					for (int i=-max;i<=max;i++)
					{
						for(int j=-max;j<=max;j++)
						{
							C[x][y]+= filter[i+max][j+max]*I[x+i][y+i];					
						}
					}
				}
			}

	double max_C=max_dmatrix(C,nrl, nrh, ncl, nch);
	printf("%f\n",max_C);
	for (int i=nrl; i<=nrh; i++)
	{
		for(int j=ncl; j<=nch; j++)
		{
			out[i][j]=(C[i][j]*255)/max_C;
		}
	}
	free_dmatrix(C, nrl, nrh, ncl, nch);
	return out;
}

void sobel(byte **I, double **Ix, double **Iy, long nrl, long nrh, long ncl, long nch)
{
	int i,j;
	long total;
	int SobelH[3][3]={{-1,0,1},{-2,0,2},{-1,0,1} };
	int SobelV[3][3]={{-1,-2,-1},{0,0,0},{1,2,1} };
	byte I1, I2, I3, I4, I5, I6, I7, I8, I9;

	for(i=nrl+1;i<nrh;i++)
	{	
		for(j=ncl+1;j<nch;j++)
		{
			I1 = I[i-1][j-1];
			I2 = I[i][j-1];
			I3 = I[i+1][j-1];
			I4 = I[i-1][j];
			I6 = I[i+1][j];
			I7 = I[i-1][j+1];
			I8 = I[i][j+1];
			I9 = I[i+1][j+1];

			//convolution Sobel horizontal
			total=-I1 + I3 - I4*2 + I6*2 - I7 + I9;
			Ix[i][j]=total/4;

			//convolution Sobel horizontal
			total=-I1 - I2*2 - I3 + I7 + I8*2 + I9;
			Iy[i][j]=total/4;
		}
	}

}

void norm_gradient(double **SobelX, double **SobelY, double **Sobel, long nrl, long nrh, long ncl, long nch)
{
	for(int i=nrl;i<nrh;i++)
	{	
		for(int j=ncl;j<nch;j++)
		{
			Sobel[i][j]=sqrt(pow(SobelX[i][j],2) + pow(SobelY[i][j],2));
		}
	}	
}

void binarize(byte **I, byte **B, int treshold, long nrl, long nrh, long ncl, long nch)
{
	int i, j;
	for (i = nrl; i < nrh; i++)
	{
		for (j = ncl; j < nch; j++)
		{
			if (I[i][j] > treshold)
			{
				B[i][j] = 255;
			}
			else
			{
				B[i][j] = 0;
			}
		}
	}
}

void convert_rgb8_to_byte(rgb8 **I, byte **B, long nrl, long nrh, long ncl, long nch)
{
	int i, j;
	int moy;
	for (i = nrl; i < nrh; i++)
	{
		for (j = ncl; j < nch; j++)
		{
			moy= (I[i][j].r +I[i][j].g + I[i][j].b)/3;
			B[i][j] = (byte)moy;
		}
	}
}

void convert_dmatrix_bmatrix(double **D, byte **B, long nrl, long nrh, long ncl, long nch)
{

	for(int i=nrl;i<nrh;i++)
	{	
		for(int j=ncl;j<nch;j++)
		{
			B[i][j]=abs(D[i][j]);
		}
	}
}

void getBarycenterColor(rgb8 **I, long nrl, long nrh, long ncl, long nch, int *x, int *y){
	*x = 0;
	*y = 0;
	double total = 1*10e-8;
	for(int i = nrl; i < nrh; i++){
		for(int j = ncl; j < nch; j++){
			if(isOfColor(I[i][j], 64, 12, 14) == 1){
				*y += i;
				*x += j;

				++total;
			}
		}
	}
	// printf("x:%d y:%d total:%d\n", *x, *y, total);
	*x /= total;
	*y /= total;
}