////////新的关节点特征构造
enum NewJoint_Num
{
	NewJointNum = 15,
	Newline_Num = 24
};
enum NewJoint
{
	hi0 = 0,
	chest1 = 1,
	neck2 = 2,
	lShldr3 = 3,
	lForeArm4 = 4,
	lHand5 = 5,
	rShldr6 = 6,
	rForeArm7 = 7,
	rHand8 = 8,
	lThigh9 = 9,
	lShin10 = 10,
	lFoot11 = 11,
	rThigh12 = 12,
	rShin13 = 13,
	rFoot14 = 14
};
NewJoint newline[Newline_Num * 2] =
{
	//主
	rHand8, chest1,
	neck2, chest1,
	chest1, lHand5,
	chest1, hi0,
	hi0, rFoot14,
	hi0, lFoot11,

	rHand8,rShldr6,
	lShldr3,lHand5,
	rThigh12,rFoot14,
	lThigh9,lFoot11,
	//整体
	rHand8, rForeArm7,
	rForeArm7, rShldr6,
	rShldr6, neck2,
	rShldr6, chest1,
	neck2, lShldr3,
	chest1, lShldr3,
	lShldr3, lForeArm4,
	lForeArm4, lHand5,
	chest1, rThigh12,
	chest1, lThigh9,
	rThigh12, rShin13,
	lThigh9, lShin10,
	lShin10, lFoot11,
	rShin13, rFoot14
};
//类似手到肩膀等4肢直线
vector<int> guanjielianXian =
{
	rHand8, rShldr6,
	lShldr3, lHand5,
	rThigh12, rFoot14,
	lThigh9, lFoot11
};
//需要求距离的点
vector<vector<int>> guanjiexuyaoqiujulidedian =
{
	{ rForeArm7 },
	{ lForeArm4 },
	{ rShin13 },
	{ lShin10 }
};
//身体部位 4肢
vector<vector<int>> guanjiepart =
{
	{ rHand8, rForeArm7,rShldr6 },
	{ lShldr3, lForeArm4,lHand5 },
	{ rThigh12, rShin13, rFoot14 },
	{ lThigh9, lShin10, lFoot11 }
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
