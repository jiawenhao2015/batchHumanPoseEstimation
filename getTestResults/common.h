//
//enum Kinect_Joint_Num
//{
//	Joints_Num = 25,
//	Skeleton_line_Num = 20
//};
//
//enum Kinect_Joint
//{
//	AnkleLeft = 14,// 	Left ankle
//	AnkleRight = 18,// 	Right ankle
//	ElbowLeft = 5,// 	Left elbow
//	ElbowRight = 9,// 	Right elbow
//	FootLeft = 15,// 	Left foot
//	FootRight = 19,// 	Right foot
//	HandLeft = 7,// 	Left hand
//	HandRight = 11,// 	Right hand
//	HandTipLeft = 21,// 	Tip of the left hand
//	HandTipRight = 23,// 	Tip of the right hand
//	Head = 3,// 	Head
//	HipLeft = 12,// 	Left hip
//	HipRight = 16,// 	Right hip
//	KneeLeft = 13,// 	Left knee
//	KneeRight = 17,// 	Right knee
//	Neck = 2,// 	Neck
//	ShoulderLeft = 4,// 	Left shoulder
//	ShoulderRight = 8,// 	Right shoulder
//	SpineBase = 0,// 	Base of the spine
//	SpineMid = 1,// 	Middle of the spine
//	SpineShoulder = 20,// 	Spine at the shoulder
//	ThumbLeft = 22,// 	Left thumb
//	ThumbRight = 24,// 	Right thumb
//	WristLeft = 6, //	Left wrist
//	WristRight = 10// 	Right wrist
//};
//Kinect_Joint skeleton_line[Skeleton_line_Num * 2] =
//{
//	SpineMid, SpineBase,
//
//	SpineBase, HipLeft,
//	HipLeft, KneeLeft,
//	KneeLeft, AnkleLeft,
//	AnkleLeft, FootLeft,
//
//	SpineBase, HipRight,
//	HipRight, KneeRight,
//	KneeRight, AnkleRight,
//	AnkleRight, FootRight,
//
//	SpineMid, SpineShoulder,
//
//	SpineShoulder, ShoulderLeft,
//	ShoulderLeft, ElbowLeft,
//	ElbowLeft, WristLeft,
//	WristLeft, HandLeft,
//
//	SpineShoulder, ShoulderRight,
//	ShoulderRight, ElbowRight,
//	ElbowRight, WristRight,
//	WristRight, HandRight,
//
//	SpineMid, Neck,
//	Neck, Head
//};
//
//
//�������� ���Լ��궨������ �ؽ�����
vector<string> JOINTS = { "hip", "chest", "neck", "lShldr", "lForeArm", "lHand", "rShldr", "rForeArm", "rHand", "lThigh", "lShin", "lFoot", "rThigh", "rShin", "rFoot" };

//enum BIAODING_Joint_Num
//{
//	BIAODING_Num = 15,
//	BIAODING_line_Num = 14
//};
//enum BIAODING_Joint
//{
//	hip = 0,
//	chest = 1,
//	neck = 2,
//	lShldr = 3,
//	lForeArm = 4,
//	lHand = 5,
//	rShldr = 6,
//	rForeArm = 7,
//	rHand = 8,
//	lThigh = 9,
//	lShin = 10,
//	lFoot = 11,
//	rThigh = 12,
//	rShin = 13,
//	rFoot = 14
//};
//
//BIAODING_Joint biaoding_line[BIAODING_line_Num * 2] =
//{
//	hip, chest,
//	chest, neck,
//
//	lShldr, lForeArm,
//	lForeArm, lHand,
//
//	rShldr, rForeArm,
//	rForeArm, rHand,
//
//	lThigh, lShin,
//	lShin, lFoot,
//
//	rThigh, rShin,
//	rShin, rFoot,
//
//	hip, lThigh,
//	hip, rThigh,
//
//	neck, lShldr,
//	neck, rShldr
//};


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
	head2,hip1,
	hip1,l11,
	hip1,r14,
	//����
	
	l5,l6,
	l6,l7,
	l7,head3,
	head3,head4,
	head4,r10,
	r10,r9,
	r9,r8,
	head2,head3,
	head2,head4,
	head3,hip1,
	head4,hip1,
	hip1,l13,
	hip1,r16,
	l13,l12,
	l12,l11,
	r16,r15,
	r15,r14	
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
	h2,h3,
	h2,h4,
	h3,h4,
	
	h3,hip0,
	h4,hip0,

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