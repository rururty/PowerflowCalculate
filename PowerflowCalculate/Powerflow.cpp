#include<math.h>
#include<stdio.h>
#include "PowerFlow.h"
int locateBus[NS]; //这个函数用于定位对应节点
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
	/*这里我要定位节点了！！！！！！*/
	for (i = 1; i <= n; i++)
	{
		locateBus[i] = Bus[i].i;
	}

}

void vini(void)
{	

	int i = 1;
	/*
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
	*/
	

	/*这里我突然发现功率方程的数量是 2*n-2 */
	/*所以我打算把节点的顺序打乱*/
	/*int i = 1;
	for (i = 1; i <= n - 1; i++)
	{
		Vp[i].v = 1.0;
		Vp[i].va = 0.0;
		Vxy[i].ve = 1;
		Vxy[i].vf = 0;
	}
	*/
	/*我错了，老师的方程也是2*n，这会我将按照节点名称的顺序排序 虽然这段代码没有体现出来*/
	for (i = 1; i <= n; i++)
	{
		Vp[i].v = 1.0;
		Vp[i].va = 0.0;
		Vxy[i].ve = 1;
		Vxy[i].vf = 0;
	}
}
void y2(void) {
	/* 这个函数算出的导纳矩阵横坐标纵坐标是节点名，与输入线路参数的顺序无关已经排好序了o(*￣▽￣*)ブ */
	int i = 1;
	int j = 1;
	for (i = 1; i <= n; i++)
	{
		for (j = 1; j <= n; j++)
		{ 
			int k = 1;
			CYij Temp;//被操作数
			Temp.b = 0;
			Temp.g = 0;
			//计算支路导纳
			for (k = 1; k <= zls; k++)
			{
				if (((i == j) && (abs(Line[k].i) == i || abs(Line[k].j) == i ))
					|| (abs(Line[k].i) == i && abs(Line[k].j) == j) 
					|| (abs(Line[k].i) == j && abs(Line[k].j) == i)) // 判断支路与这个节点是否有关
				{
					if (Line[k].i > 0 && Line[k].j > 0) // 判断是不是线路
					{
						if (i == j) // 是不是自导纳
						{
							Temp.g += Line[k].r / (pow(Line[k].r, 2) + pow(Line[k].x, 2));
							Temp.b += Line[k].yk - Line[k].x / (pow(Line[k].r, 2) + pow(Line[k].x, 2));
						}
						else // 不是自导纳是互导纳 
						{
							Temp.g -= Line[k].r / (pow(Line[k].r, 2) + pow(Line[k].x, 2));
							Temp.b -= 0 - Line[k].x / (pow(Line[k].r, 2) + pow(Line[k].x, 2));
						}
					}
					else // 当是变压器时 
					{
						if (i == j) //是不是自导纳
						{
							if (Line[k].i > 0 && Line[k].i == i) // 是不是低压侧 这里比较复杂是因为前面比较简单
							{
								Temp.g += Line[k].r / (pow(Line[k].r, 2) + pow(Line[k].x, 2));
								Temp.b -= Line[k].x / (pow(Line[k].r, 2) + pow(Line[k].x, 2));
							}
							else //不是低压侧是高压侧
							{
								Temp.g += Line[k].r / (pow(Line[k].r, 2) + pow(Line[k].x, 2)) / pow(Line[k].yk, 2);
								Temp.b -= Line[k].x / (pow(Line[k].r, 2) + pow(Line[k].x, 2)) / pow(Line[k].yk, 2);
							}
							
						}
						else // 不是自导纳是互导纳
						{
							Temp.g -= Line[k].r / (pow(Line[k].r, 2) + pow(Line[k].x, 2)) / Line[k].yk;
							Temp.b += Line[k].x / (pow(Line[k].r, 2) + pow(Line[k].x, 2)) / Line[k].yk;
						}
					}
				}
			}
			//计算电容器导纳
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
void newtonp()
{
	for (niter = 0; niter < maxiter; niter++)
	{
		/*！！！！！！！！！重要！！！！！！！！！！！*/
		/*这里的功率方程的顺序和迭代矩阵的顺序是按照节点名称的顺序*/
		int i;
		dsm = 0;
		/*计算功率数组*/
		
		for (i = 1; i <= n; i++) // (注意这里的循环次数，因为平衡节点的问题)我错了老师的雅各比矩阵也是2*n个
		{
			int point = Bus[i].i;
			int j;
			if (Bus[i].Type == PQ)
			{
				int ge_bf, gf_plus_be, gf_be;
				ge_bf = 0;
				gf_plus_be = 0;
				gf_be = 0;
				for (j = 1; j <= n; j++)
				{
					ge_bf += Yij[point][j].g * Vxy[j].ve - Yij[point][j].b * Vxy[j].vf;
					gf_plus_be += Yij[point][j].g * Vxy[j].vf + Yij[point][j].b * Vxy[j].ve;
					gf_be += Yij[point][j].g * Vxy[j].vf + Yij[point][j].b * Vxy[j].ve;
				}
				w[2 * point - 1] = Bus[i].pg - Bus[i].pl - (Vxy[point].ve * ge_bf + Vxy[point].vf * gf_plus_be);
				w[2 * point] = Bus[i].qg - Bus[i].ql - (Vxy[point].vf * ge_bf - Vxy[point].ve * gf_be);
				if (w[2 * point - 1] > dsm)
				{
					dsm = w[2 * point - 1];
				}
				else if (w[2 * point] > dsm)
				{
					dsm = w[2 * point];
				}
			}
			else if (Bus[i].Type == PV)
			{
				int ge_bf, gf_plus_be;
				ge_bf = 0;
				gf_plus_be = 0;
				for (j = 1; j <= n; j++)
				{
					ge_bf += Yij[point][j].g * Vxy[j].ve - Yij[point][j].b * Vxy[j].vf;
					gf_plus_be += Yij[point][j].g * Vxy[j].vf + Yij[point][j].b * Vxy[j].ve;
				}
				w[2 * point - 1] = Bus[i].pg - Bus[i].pl - (Vxy[point].ve * ge_bf + Vxy[point].vf * gf_plus_be);
				w[2 * point] = pow(Bus[i].v0, 2) - (pow(Vxy[point].ve, 2) + pow(Vxy[point].vf, 2));
				if (w[2 * point - 1] > dsm)
				{
					dsm = w[2 * point - 1];
				}
			}
			else if (Bus[i].Type == VA)
			{
				w[2 * point - 1] = 0;
				w[2 * point] = 0;
			}
		}
		/*检测是否满足收敛条件*/
		if (dsm < eps)
		{
			break;
		}
		/*计算雅各比矩阵*/
		jcbp();
	}
}
void jcbp()
{
	/*这里的雅各比矩阵的顺序将采用节点名称顺序*/
	/*这里不需要point的原因是已经不需要Bus里的数据了*/
	int i;
	for (i = 1; i <= n; i++)
	{
		int j;
		for (j = 1; j <= n; j++)
		{
			/*这里注意需要查一下节点类型*/
			/*oh my god我还是把那个节点的数组排个序把这样的话我就不需要写循环查询（悲伤）*/
			/*算啦我直接创建一个数组告诉我对应点的位置就ok了呀我会把这个数组整成全局变量创建过程将会放在dataio函数中*/
			if (Bus[locateBus[i]].Type == PQ)
			{
				if (i == j)
				{
					int ge_bf, gf_plus_be, gf_be;
				}
			}
		}
	}
}
