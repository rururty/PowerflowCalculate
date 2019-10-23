struct CLine           // ֧·����
{
	int    i;          // �׶�ĸ�ߺ�(k��)
	int    j;          // ĩ��ĸ�ߺ�
	double r;          // ����(��1��)
	double x;          // �翹(��1��)
	double yk;         // B/2(����)
};

struct CBus             // �ڵ�����
{
	int    i;           // ĸ�ߺ�
	int    Type;        // �ڵ�����
	double pg;          // �й�����
	double qg;          // �޹�����
	double pl;          // �й�����(��ֵ)
	double ql;          // �޹�����(��ֵ)
	double v0;          // ��ѹ����ֵ(PQ�ڵ����ֵΪ1.0)
};

struct CCp              // ������(��翹��)
{
	int    i;           // ����ĸ�ߺ�
	double b;           // ����ֵ
};

struct CYij             // ������
{
	double g;           // ʵ��
	double b;           // �鲿
};

struct CVp              // ��ѹ
{
	double v;           // ��ֵ
	double va;          // ���
};

struct CVxy				// ֱ�������ѹ
{
	double ve;			// ʵ��
	double vf;			// �鲿
};

const int NS = 1000;    // �ڵ����������С
const int ZS = 2000;    // ֧·���������С
const int NS2 = NS + NS;    // �ڵ����������С * 2 

CLine Line[ZS];
CBus  Bus[NS];
CCp   Cp[NS];
CVp   Vp[NS];
CVxy  Vxy[NS];
CYij  Yij[NS][NS];
double Jacb[NS2][NS2];	// �ſɱȾ���
double w[NS2];			// �ڵ㹦������
double b[NS2];			// ���ʲ�ƽ�������顢������

int n;					// �ڵ���
int n2;					// 
int zls;                // ֧·��
int ncp;				// ����������

int dsd;                // ���ƽ�����Ľڵ��
int niter;				// ��������
int maxiter;			// ����������
double dsm;             // ���ƽ����
double eps;				// ��������

const int VA = -2;		// �ڵ����ͣ�ƽ��ڵ�
const int PV = -1;		// �ڵ����ͣ�PV�ڵ�
const int PQ = 0;		// �ڵ����ͣ�PQ�ڵ�

FILE* fp1, * fp2;

void dataio(char* InputFile);	// ���ļ��ж�ȡ����
void vini();			// ƽ������ѹ��ֵ
void Pr_nod(void);		// ����ڵ������
void branch(void);      // ����֧·������
void y2(void);          // �γɵ��ɾ���
void solution(void);    // ţ�ٷ��ⷽ��

void newtonp(void);   // ������ţ�ٷ�
void jcbp(void);		// �ſɱȾ���
void bbhl_p(void);		// ��ƽ������ƽ��ڵ㹦�ʡ�PV�ڵ��޹�����

