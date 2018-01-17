////////�µĹؽڵ���������
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
	//��
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
	//����
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
//�����ֵ�����4ֱ֫��
vector<int> guanjielianXian =
{
	rHand8, rShldr6,
	lShldr3, lHand5,
	rThigh12, rFoot14,
	lThigh9, lFoot11,
};
//��Ҫ�����ĵ�
vector<vector<int>> guanjiexuyaoqiujulidedian =
{
	{ rForeArm7 },
	{ lForeArm4 },
	{ rShin13 },
	{ lShin10 }
};
//���岿λ 4֫
vector<vector<int>> guanjiepart =
{
	{ rHand8, rForeArm7,rShldr6, },
	{ lShldr3, lForeArm4,lHand5, },
	{ rThigh12, rShin13, rFoot14 },
	{ lThigh9, lShin10, lFoot11 }
};





///////////
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
	//��
	rHand, chest,
	neck, chest,
	chest, lHand,
	chest, hip,
	hip, rFoot,
	hip, lFoot,
	//����
	rHand, rForeArm,
	rForeArm, rShldr,
	rShldr, neck,
	rShldr, chest,
	neck, lShldr,
	chest, lShldr,
	lShldr, lForeArm,
	lForeArm, lHand,
	chest, rThigh,
	chest, lThigh,
	rThigh, rShin,
	lThigh, lShin,
	lShin, lFoot,
	rShin, rFoot
};

//-------------------------------------------------------------------3��������

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
	l11 = 10,//����ò�Ʊ��Ҹ���ˡ�����
	l12 = 11,
	l13 = 12,
	r14 = 13,
	r15 = 14,
	r16 = 15
};

JULEI_Joint julei_line[JULEI_line_Num * 2] =
{
	//��
	l5, hip1,
	hip1, r8,
	head2, hip1,
	hip1, l11,
	hip1, r14,
	//����

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

//-------------------------------------------------------------------4��������

enum JULEI4_Joint_Num
{
	JULEI4_Num = 21,
	JULEI4_line_Num = 27
};
enum JULEI4_Joint
{
	hip0 = 0,
	h1 = 1,
	h2 = 2,
	h3 = 3,
	h4 = 4,
	left5 = 5,
	left6 = 6,
	left7 = 7,
	left8 = 8,
	right9 = 9,
	right10 = 10,
	right11 = 11,
	right12 = 12,
	leg13 = 13,
	leg14 = 14,
	leg15 = 15,
	leg16 = 16,
	leg17 = 17,
	leg18 = 18,
	leg19 = 19,
	leg20 = 20
};

JULEI4_Joint julei4_line[JULEI4_line_Num * 2] =
{
	//��
	left5, hip0,
	h1, hip0,
	right9, hip0,
	leg13, hip0,
	leg17, hip0,
	//����

	left5, left6,
	left6, left7,
	left7, left8,
	left8, h3,

	h1, h2,
	h2, h3,
	h2, h4,
	h3, h4,

	h3, hip0,
	h4, hip0,

	h4, right12,
	right12, right11,
	right11, right10,
	right10, right9,

	hip0, leg16,
	leg16, leg15,
	leg15, leg14,
	leg14, leg13,

	hip0, leg20,
	leg20, leg19,
	leg19, leg18,
	leg18, leg17

};
//��ӵ������ �����ֶ˵����ľ���
vector<int> lianXian =
{	//��
	left5, hip0,
	h1, hip0,
	right9, hip0,
	leg13, hip0,
	leg17, hip0,

	left5,left8,
	right9,right12,
	leg13,leg16,
	leg20,leg17,//�ֽŵ��ֱ� ��ֱ��

	//����

	left5, left6,
	left6, left7,
	left7, left8,
	left8, h3,

	h1, h2,
	h2, h3,
	h2, h4,
	h3, h4,

	h3, hip0,
	h4, hip0,

	h4, right12,
	right12, right11,
	right11, right10,
	right10, right9,

	hip0, leg16,
	leg16, leg15,
	leg15, leg14,
	leg14, leg13,

	hip0, leg20,
	leg20, leg19,
	leg19, leg18,
	leg18, leg17
};
//�����ֵ�����4ֱ֫��
vector<int> zhiduanlianXian =
{
	left5, left8,
	right9, right12,
	leg13, leg16,
	leg20, leg17,//�ֽ�
};
//��Ҫ�����ĵ�
vector<vector<int>> xuyaoqiujulidedian =
{
	{ left6, left7},
	{ right11, right10},
	{ leg15, leg14 },
	{ leg19, leg18}
};
//���岿λ 4֫
vector<vector<int>> bodypart =
{
	{ left5, left6, left7, left8 },
	{ right9, right10, right11, right12},
	{ leg13, leg14, leg15, leg16},
	{ leg17, leg18, leg19, leg20 }
};