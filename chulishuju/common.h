
enum XUNI_Joint_Num
{
	XUNI_Num = 15,
	XUNI_line_Num = 20
};
enum XUNI_Joint
{
	hip = 0,
	chest = 1,
	neck = 2,
	lShldr = 3,
	lForeArm = 4,
	lHand = 5,
	rShldr = 6,
	rForeArm = 7,
	rHand = 8,
	lThigh = 9,
	lShin = 10,
	lFoot = 11,
	rThigh = 12,
	rShin = 13,
	rFoot = 14
};

XUNI_Joint xuni_line[XUNI_line_Num * 2] =
{
	//主
	rHand,chest,
	neck,chest,
	chest,lHand,
	chest,hip,
	hip,rFoot,
	hip,lFoot,
	//整体
	rHand,rForeArm,
	rForeArm,rShldr,
	rShldr,neck,
	rShldr,chest,
	neck,lShldr,
	chest,lShldr,
	lShldr,lForeArm,
	lForeArm,lHand,
	chest,rThigh,
	chest,lThigh,
	rThigh,rShin,
	lThigh,lShin,
	lShin,lFoot,
	rShin,rFoot
};


enum JULEI_Joint_Num
{
	JULEI_Num = 16,
	JULEI_line_Num = 22
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
	//整体

	l5, l6,
	l6, l7,
	l7, head3,
	head3, head4,
	head4, r10,
	r10, r9,
	r9, r8,
	head2, head3,
	head2, head4,
	head3, hip1,
	head4, hip1,
	hip1, l13,
	hip1, r16,
	l13, l12,
	l12, l11,
	r16, r15,
	r15, r14
};



