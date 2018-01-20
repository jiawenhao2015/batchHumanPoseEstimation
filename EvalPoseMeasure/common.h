////////新的关节点特征构造
enum NewJoint_Num
{
	NewJointNum = 12,
	Newline_Num = 10
};
enum NewJoint
{	
	j1 = 1,
	j2 = 2,
	j3 = 3,
	j4 = 4,
	j5 = 5,
	j6 = 6,
	j7 = 7,
	j8 = 8,
	j9 = 9,
	j10 = 10,
	j11 = 11,
	j12 = 12	
};
NewJoint newline[Newline_Num * 2] =
{
	 j5,j4,
	 j4,j3,
	 j3,j2,
	 j2,j6,
	 j6,j7,
	 j7,j8,
	 j9,j10,
	 j11,j12,
	 j5,j3,
	 j6,j8
	 
};
//类似手到肩膀等4肢直线
vector<int> guanjielianXian =
{
	j5, j3,
	j6, j8
};
//需要求距离的点
vector<vector<int>> guanjiexuyaoqiujulidedian =
{
	{ j4 },
	{ j7 }	
};
//身体部位 4肢
vector<vector<int>> guanjiepart =
{
	{ j5,j4,j3},
	{ j6,j7,j8}
};

//-------------------------------------------------------------------3个特征点

enum JULEI_Joint_Num
{
	JULEI_Num = 16,
	JULEI_line_Num = 23
};
enum JULEI_Joint
{
	hip1 = 0,
	head2 = 1,
	head3 = 2,
	head4 = 3,
	l5 = 4,
	l6 = 5,
	l7 = 6,
	r8 = 7,
	r9 = 8,
	r10 = 9,
	l11 = 10,//左右貌似被我搞混了。。。
	l12 = 11,
	l13 = 12,
	r14 = 13,
	r15 = 14,
	r16 = 15
};

JULEI_Joint julei_line[JULEI_line_Num * 2] =
{
	//主
	l5, hip1,
	hip1, r8,
	head2, hip1,
	hip1, l11,
	hip1, r14,

	l5,l7,
	r10,r8,
	l13,l11,
	r16,r14,

	//整体
	l5, l6,
	l6, l7,
	l7, head2,	
	head2, r10,
	r10, r9,
	r9, r8,	 
	l7, hip1,
	r10, hip1,
	hip1, l13,
	hip1, r16,
	l13, l12,
	l12, l11,
	r16, r15,
	r15, r14
};
//类似手到肩膀等4肢直线
vector<int> juleilianXian =
{
	l5, l7,
	r10, r8,
	l13, l11,
	r16, r14
};
//需要求距离的点
vector<vector<int>> juleixuyaoqiujulidedian =
{
	{ l6 },
	{ r9 },
	{ l12 },
	{ r15 }
};
//身体部位 4肢
vector<vector<int>> juleipart =
{
	{ l5, l6,l7 },
	{ r8, r9,r10 },
	{ l13, l11, l12 },
	{ r16, r14, r15 }
};
