struct Segment
{
int	x1,y1,h1,x2,y2,h2;
Segment* nearby[60];
int zondCameHere;
};
struct tylesPathMapping
{
	Segment segment[800];
	int countSegs;
}pathMap[5][5];


void remapGrid (int gx, int gy)
{
//mapmatrix[5][5][phArSize][phArSize][matrdepth+matrhig+1];
for (int i=0; i<phArSize; i++)
	for (int j=0; j<phArSize; j++)
		for (int h=0; h<matrdepth+matrhig+1; h++)
			if (!mapmatrix[gx][gy][i][j][h]) mapmatrix[gx][gy][i][j][h]=-1;
pathMap[gx][gy].countSegs=0;
// This part will be done prior
for (int i=0; i<phArSize; i++)
	for (int j=0; j<phArSize; j++)
		for (int h=0; h<matrdepth+matrhig+1; h++)
		{
			if ((mapmatrix[gx][gy][i][j][h]==-1) && ((h==0)||(mapmatrix[gx][gy][i][j][h-1]>0)))
			//start growing segment 
			{
				int px=0;
				int py=0;
				int ph=0;
				bool pxexp=1;// To know if theres point to try expend
				bool pyexp=1;
				bool phexp=1;
				while ((pxexp) || (pyexp) || (phexp))
				{
					if (pxexp)	{
						if ((i+px+1)<phArSize)	{
							px++;
							bool collision=false;
							for (int jj=j; jj<=j+py; jj++)
								for (int hh=h; hh<=h+ph; hh++)
									if (mapmatrix[gx][gy][i+px][jj][hh]!=-1)
							{collision=1;goto stopx;}
stopx:
							if (collision) {pxexp=0; px--;}
						}
						else
							pxexp=0;
					}

					if (pyexp)	{
						if ((j+py+1)<phArSize)	{
							py++;
							bool collision=false;
							for (int ii=i; ii<=i+px; ii++)
								for (int hh=h; hh<=h+ph; hh++)
									if (mapmatrix[gx][gy][ii][j+py][hh]!=-1)
							{collision=1;goto stopy;}
stopy:
							if (collision) {pyexp=0; py--;}
						}
						else
							pyexp=0;
					}

						if (phexp)	{
						if ((h+ph+1)<matrdepth+matrhig)	{
							ph++;
							bool collision=false;
							for (int ii=i; ii<=i+px; ii++)
								for (int jj=j; jj<=j+py; jj++)
									if (mapmatrix[gx][gy][ii][jj][h+ph]!=-1)
							{collision=1;goto stoph;}
stoph:
							if (collision) {phexp=0; ph--;}
						}
						else
							phexp=0;
					}

				}
				pathMap[gx][gy].segment[pathMap[gx][gy].countSegs].x1=i;
				pathMap[gx][gy].segment[pathMap[gx][gy].countSegs].y1=j;
				pathMap[gx][gy].segment[pathMap[gx][gy].countSegs].h1=h;
				pathMap[gx][gy].segment[pathMap[gx][gy].countSegs].x2=i+px;
				pathMap[gx][gy].segment[pathMap[gx][gy].countSegs].y2=j+py;
				pathMap[gx][gy].segment[pathMap[gx][gy].countSegs].h2=h+ph;

for (int ii=i; ii<=i+px; ii++)
	for (int jj=j; jj<=j+py; jj++)
		for (int hh=h; hh<=h+ph; hh++)
			mapmatrix[gx][gy][ii][jj][hh]=0;

				pathMap[gx][gy].countSegs++;


			}
			//end growing segment
			//if (mapmatrix[gx][gy][i][j][h]==-1) mapmatrix[gx][gy][i][j][h]=0;
		}








}