#include<math.h>
#include<stdio.h>
#include "PowerFlow.h"
void printy2();
void main()
{
	dataio("Bus51.txt");
	n2 = n + n;
	y2();
	printy2();
	newtonp();
}
void dataio(char* InputFile)
{
	int i;
	fp1 = fopen(InputFile, "r");
	fp2 = fopen("Flowout.txt", "w");
	fscanf(fp1, "%d%d%d%lf%d", &n, &zls, &ncp, &eps, &maxiter);
	for (i = 1; i <= zls; i++)
	{
		fscanf(fp1, "%d%d%lf%lf%lf", &Line[i].i, &Line[i].j, &Line[i].r, &Line[i].x, &Line[i].yk);
	}
	for (i = 1; i <= n; i++)
	{
		fscanf(fp1, "%d%d%lf%lf%lf%lf%lf",
			&Bus[i].i, &Bus[i].Type, &Bus[i].pg,
			&Bus[i].qg, &Bus[i].pl, &Bus[i].ql,
			&Bus[i].v0);
	}
	for (i = 1; i <= ncp; i++)
	{
		fscanf(fp1, "%d%lf", &Cp[i].i, &Cp[i].b);
	}
	fclose(fp1);
	fprintf(fp2, "\n *******ZLB*******\n");
	for (i = 1; i <= zls; i++)
	{
		fprintf(fp2, "\n%8d%8d%12.7f%12.7f%12.7f",
			Line[i].i, Line[i].j, Line[i].r,
			Line[i].x, Line[i].yk);
	}
	fprintf(fp2, "\n\n *******BUS*******\n");
	for (i = 1; i <= n; i++)
	{
		fprintf(fp2, "\n%8d%8d%9.4f%9.4f%9.4f%9.4f%9.4f",
			Bus[i].i, Bus[i].Type, Bus[i].pg
			, Bus[i].qg, Bus[i].pl, Bus[i].ql
			, Bus[i].v0);
	}
	fprintf(fp2, "\n\n ******CP******\n");
	for (i = 1; i <= ncp; i++)
	{
		fprintf(fp2, "\n%8d%7.4f", Cp[i].i, Cp[i].b);
	}
}

void vini(void)
{
	int i = 1;
	for (i = 1; i <= n; i++)
	{
		int j = 1;
		for (j = 1; j <= n; j++)
		{
			if (Bus[j].i == i) {
				break;
			}
		}
		Vp[i].v = Bus[j].v0;
		Vp[i].va = 0.0;
		Vxy[i].ve = Bus[j].v0;
		Vxy[i].vf = 0;
	}
}
void y2(void) {
	int i = 1;
	int j = 1;
	for (i = 1; i <= n; i++)
	{
		for (j = 1; j <= n; j++)
		{
		    
			int k = 1;
			CYij Temp;
			Temp.b = 0;
			Temp.g = 0;
			//����֧·����
			for (k = 1; k <= zls; k++)
			{
				if (((i == j) && (abs(Line[k].i) == i || abs(Line[k].j) == i ))
					|| (abs(Line[k].i) == i && abs(Line[k].j) == j) 
					|| (abs(Line[k].i) == j && abs(Line[k].j) == i)) // �ж�֧·������ڵ��Ƿ��й�
				{
					if (Line[k].i > 0 && Line[k].j > 0) // �ж��ǲ�����·
					{
						if (i == j) // �ǲ����Ե���
						{
							Temp.g += Line[k].r / (pow(Line[k].r, 2) + pow(Line[k].x, 2));
							Temp.b += Line[k].yk - Line[k].x / (pow(Line[k].r, 2) + pow(Line[k].x, 2));
						}
						else // �����Ե����ǻ����� 
						{
							Temp.g -= Line[k].r / (pow(Line[k].r, 2) + pow(Line[k].x, 2));
							Temp.b -= 0 - Line[k].x / (pow(Line[k].r, 2) + pow(Line[k].x, 2));
						}
					}
					else // ���Ǳ�ѹ��ʱ 
					{
						if (i == j) //�ǲ����Ե���
						{
							if (Line[k].i > 0 && Line[k].i == i) // �ǲ��ǵ�ѹ�� ����Ƚϸ�������Ϊǰ��Ƚϼ�
							{
								Temp.g += Line[k].r / (pow(Line[k].r, 2) + pow(Line[k].x, 2));
								Temp.b -= Line[k].x / (pow(Line[k].r, 2) + pow(Line[k].x, 2));
							}
							else //���ǵ�ѹ���Ǹ�ѹ��
							{
								Temp.g += Line[k].r / (pow(Line[k].r, 2) + pow(Line[k].x, 2)) / pow(Line[k].yk, 2);
								Temp.b -= Line[k].x / (pow(Line[k].r, 2) + pow(Line[k].x, 2)) / pow(Line[k].yk, 2);
							}
							
						}
						else // �����Ե����ǻ�����
						{
							Temp.g -= Line[k].r / (pow(Line[k].r, 2) + pow(Line[k].x, 2)) / Line[k].yk;
							Temp.b += Line[k].x / (pow(Line[k].r, 2) + pow(Line[k].x, 2)) / Line[k].yk;
						}
					}
				}
			}
			//�������������
			for (k = 1; k <= ncp; k++)
			{
				if (Cp[k].i == i && i == j) {
					Temp.b += Cp[k].b;
				}
			}
			Yij[i][j].g = Temp.g;
			Yij[i][j].b = Temp.b;
		}
	}
}
void printy2()
{
	int i = 1;
	int j = 1;
	printf("\n *******ZLB*******\n");
	for (i = 1; i <= zls; i++)
	{
		printf("\n%8d%8d%12.7f%12.7f%12.7f",
			Line[i].i, Line[i].j, Line[i].r,
			Line[i].x, Line[i].yk);
	}
	printf("\n\n *******BUS*******\n");
	for (i = 1; i <= n; i++)
	{
		printf("\n%8d%8d%9.4f%9.4f%9.4f%9.4f%9.4f",
			Bus[i].i, Bus[i].Type, Bus[i].pg
			, Bus[i].qg, Bus[i].pl, Bus[i].ql
			, Bus[i].v0);
	}
	printf("\n\n ******CP******\n");
	for (i = 1; i <= ncp; i++)
	{
		printf("\n%8d%7.4f", Cp[i].i, Cp[i].b);
	}
	for (i = 1; i <= n; i++)
	{
		for (j = 1; j <= n; j++)
		{
			printf("%lf %lf  ", Yij[i][j].g, Yij[i][j].b);
		}
		printf("\n");
	}
}
