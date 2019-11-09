#include<math.h>
#include<stdio.h>
#include "PowerFlow.h"
int locateBus[NS]; //����������ڶ�λ��Ӧ�ڵ�
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
	/*������Ҫ��λ�ڵ��ˣ�����������*/
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
	

	/*������ͻȻ���ֹ��ʷ��̵������� 2*n-2 */
	/*�����Ҵ���ѽڵ��˳�����*/
	/*int i = 1;
	for (i = 1; i <= n - 1; i++)
	{
		Vp[i].v = 1.0;
		Vp[i].va = 0.0;
		Vxy[i].ve = 1;
		Vxy[i].vf = 0;
	}
	*/
	/*�Ҵ��ˣ���ʦ�ķ���Ҳ��2*n������ҽ����սڵ����Ƶ�˳������ ��Ȼ��δ���û�����ֳ���*/
	for (i = 1; i <= n; i++)
	{
		Vp[i].v = 1.0;
		Vp[i].va = 0.0;
		Vxy[i].ve = 1;
		Vxy[i].vf = 0;
	}
}
void y2(void) {
	/* �����������ĵ��ɾ���������������ǽڵ�������������·������˳���޹��Ѿ��ź�����o(*������*)�� */
	int i = 1;
	int j = 1;
	for (i = 1; i <= n; i++)
	{
		for (j = 1; j <= n; j++)
		{ 
			int k = 1;
			CYij Temp;//��������
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
void newtonp()
{
	for (niter = 0; niter < maxiter; niter++)
	{
		/*��������������������Ҫ����������������������*/
		/*����Ĺ��ʷ��̵�˳��͵��������˳���ǰ��սڵ����Ƶ�˳��*/
		int i;
		dsm = 0;
		/*���㹦������*/
		
		for (i = 1; i <= n; i++) // (ע�������ѭ����������Ϊƽ��ڵ������)�Ҵ�����ʦ���Ÿ��Ⱦ���Ҳ��2*n��
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
		/*����Ƿ�������������*/
		if (dsm < eps)
		{
			break;
		}
		/*�����Ÿ��Ⱦ���*/
		jcbp();
	}
}
void jcbp()
{
	/*������Ÿ��Ⱦ����˳�򽫲��ýڵ�����˳��*/
	/*���ﲻ��Ҫpoint��ԭ�����Ѿ�����ҪBus���������*/
	int i;
	for (i = 1; i <= n; i++)
	{
		int j;
		for (j = 1; j <= n; j++)
		{
			/*����ע����Ҫ��һ�½ڵ�����*/
			/*oh my god�һ��ǰ��Ǹ��ڵ�������Ÿ���������Ļ��ҾͲ���Ҫдѭ����ѯ�����ˣ�*/
			/*������ֱ�Ӵ���һ����������Ҷ�Ӧ���λ�þ�ok��ѽ�һ�������������ȫ�ֱ����������̽������dataio������*/
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
