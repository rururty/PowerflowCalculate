struct CLine           // 支路数据
{
	int    i;          // 首端母线号(k侧)
	int    j;          // 末端母线号
	double r;          // 电阻(在1侧)
	double x;          // 电抗(在1侧)
	double yk;         // B/2(或变比)
};

struct CBus             // 节点数据
{
	int    i;           // 母线号
	int    Type;        // 节点类型
	double pg;          // 有功出力
	double qg;          // 无功出力
	double pl;          // 有功负荷(负值)
	double ql;          // 无功负荷(负值)
	double v0;          // 电压给定值(PQ节点给定值为1.0)
};

struct CCp              // 电容器(或电抗器)
{
	int    i;           // 所在母线号
	double b;           // 电纳值
};

struct CYij             // 互导纳
{
	double g;           // 实部
	double b;           // 虚部
};

struct CVp              // 电压
{
	double v;           // 幅值
	double va;          // 相角
};

struct CVxy				// 直角坐标电压
{
	double ve;			// 实部
	double vf;			// 虚部
};

const int NS = 1000;    // 节点数据数组大小
const int ZS = 2000;    // 支路数据数组大小
const int NS2 = NS + NS;    // 节点数据数组大小 * 2 

CLine Line[ZS];
CBus  Bus[NS];
CCp   Cp[NS];
CVp   Vp[NS];
CVxy  Vxy[NS];
CYij  Yij[NS][NS];
double Jacb[NS2][NS2];	// 雅可比矩阵
double w[NS2];			// 节点功率数组
double b[NS2];			// 功率不平衡量数组、解向量

int n;					// 节点数
int n2;					// 
int zls;                // 支路数
int ncp;				// 电容器个数

int dsd;                // 最大不平衡量的节点号
int niter;				// 迭代次数
int maxiter;			// 最大迭代次数
double dsm;             // 最大不平衡量
double eps;				// 收敛精度

const int VA = -2;		// 节点类型：平衡节点
const int PV = -1;		// 节点类型：PV节点
const int PQ = 0;		// 节点类型：PQ节点

FILE* fp1, * fp2;

void dataio(char* InputFile);	// 从文件中读取数据
void vini();			// 平启动电压初值
void Pr_nod(void);		// 输出节点计算结果
void branch(void);      // 生成支路计算结果
void y2(void);          // 形成导纳矩阵
void solution(void);    // 牛顿法解方程

void newtonp(void);   // 极坐标牛顿法
void jcbp(void);		// 雅可比矩阵
void bbhl_p(void);		// 不平衡量及平衡节点功率、PV节点无功功率

