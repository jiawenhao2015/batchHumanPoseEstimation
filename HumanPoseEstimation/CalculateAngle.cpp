
#include"CalculateAngle.h"
#include<math.h>

#include<iostream>
using namespace std;

////ע��coutһ��string�ַ�������ʱ���д��

void CalculateAngle::printPoint(string pointname, MyPoint point)
{
	cout << pointname.c_str() << ":" << point.x << " " << point.y << " " << point.z << endl;
}

MyPoint::MyPoint(float a, float b, float c)
{
	this->x = a;
	this->y = b;
	this->z = c;
}
///��֪������ƽ��
void CalculateAngle::getPlane(Data a, Data b, Data c, Surface &sur)
{
	sur.a = (b.Ty - a.Ty)*(c.Tz - a.Tz) - (b.Tz - a.Tz)*(c.Ty - a.Ty);
	sur.b = ((b.Tz - a.Tz)*(c.Tx - a.Tx) - (b.Tx - a.Tx)*(c.Tz - a.Tz));
	sur.c = ((b.Tx - a.Tx)*(c.Ty - a.Ty) - (b.Ty - a.Ty)*(c.Tx - a.Tx));
	sur.d = (0 - (sur.a*a.Tx + sur.b*a.Ty + sur.c*a.Tz));
}

// ��֪�������꣬������   
MyPoint CalculateAngle::getNormal(MyPoint p1, MyPoint p2, MyPoint p3)
{
	MyPoint normal;
	normal.x = ((p2.y - p1.y)*(p3.z - p1.z) - (p2.z - p1.z)*(p3.y - p1.y));
	normal.y = ((p2.z - p1.z)*(p3.x - p1.x) - (p2.x - p1.x)*(p3.z - p1.z));
	normal.z = ((p2.x - p1.x)*(p3.y - p1.y) - (p2.y - p1.y)*(p3.x - p1.x));

	return normal;
}
//��֪������ ��н�
float CalculateAngle::getTwoNormalAngle(MyPoint normal1, MyPoint normal2)
{
	double angle;
	double cosTheta;
	cosTheta = abs(normal1.x*normal2.x + normal1.y*normal2.y + normal1.z*normal2.z) / (sqrt(normal1.x*normal1.x + normal1.y*normal1.y + normal1.z*normal1.z)*sqrt(normal2.x*normal2.x + normal2.y*normal2.y + normal2.z*normal2.z));

	if (cosTheta >= 1.0)
	{
		return 0.0;
	}
	cout << "cosTheta:" << cosTheta << " " << "acos(cosTheta):" << acos(cosTheta) << endl;
	angle = acos(cosTheta)*180.0 / 3.141593;

	return angle;
}
//����б��ĽǶ� ����Ϊ �ҵ�A B ������y����С���Ǹ� 
//����A y��С ��B��yֵ����ΪA��yֵ ��Ϊb ��AB��bB��ֱ�ߵĽǶ�
//������������ �о������� Ӧ�ü���ƽ�����Ϣ....����ƽ����εõ���
float CalculateAngle::getCrookedShoulderAngle(MyPoint lShldr, MyPoint rShldr)
{
	float angle;
	MyPoint tempPoint;

	CalculateAngle calAngle;

	MyPoint lrshoulderNormal(rShldr.x - lShldr.x, rShldr.y - lShldr.y, rShldr.z - lShldr.z);
	MyPoint tempNormal;

	if (lShldr.y < rShldr.y)
	{
		MyPoint point1(rShldr.x, lShldr.y, rShldr.z);
		tempPoint = point1;
		MyPoint normal(tempPoint.x - lShldr.x, tempPoint.y - lShldr.y, tempPoint.z - lShldr.z);
		tempNormal = normal;
	}
	else
	{
		MyPoint point2(lShldr.x, rShldr.y, lShldr.z);
		tempPoint = point2;
		MyPoint normal(tempPoint.x - rShldr.x, tempPoint.y - rShldr.y, tempPoint.z - rShldr.z);
		tempNormal = normal;
	}

	angle = calAngle.getTwoNormalAngle(lrshoulderNormal, tempNormal);


	cout << "lshlder:" << lShldr.x << " " << lShldr.y << " " << lShldr.z << endl;
	cout << "rshlder:" << rShldr.x << " " << rShldr.y << " " << rShldr.z << endl;
	cout << "YtempPoint:" << tempPoint.x << " " << tempPoint.y << " " << tempPoint.z << endl;
	cout << "crooked shoulder angle:" << angle << endl;

	return angle;
}
//ת��Ƕȼ��� �ҵ�A B�������С��zֵ ����ΪA Ȼ�����C ��A��ͬzֵ 
//C���x yֵȡ��B ����AB��AC�ĽǶ� ��Ϊת��Ƕ� angle  ������б��ĽǶ�
float CalculateAngle::getTurnRoundAngle(MyPoint lShldr, MyPoint rShldr)
{
	float angle;
	MyPoint tempPoint;

	CalculateAngle calAngle;

	MyPoint lrshoulderNormal(rShldr.x - lShldr.x, rShldr.y - lShldr.y, rShldr.z - lShldr.z);
	MyPoint tempNormal;

	if (lShldr.z < rShldr.z)
	{
		MyPoint point1(rShldr.x, rShldr.y, lShldr.z);
		tempPoint = point1;
		MyPoint normal(tempPoint.x - lShldr.x, tempPoint.y - lShldr.y, tempPoint.z - lShldr.z);
		tempNormal = normal;
	}
	else
	{
		MyPoint point2(lShldr.x, lShldr.y, rShldr.z);
		tempPoint = point2;
		MyPoint normal(tempPoint.x - rShldr.x, tempPoint.y - rShldr.y, tempPoint.z - rShldr.z);
		tempNormal = normal;
	}

	angle = calAngle.getTwoNormalAngle(lrshoulderNormal, tempNormal);


	cout << "lshlder:" << lShldr.x << " " << lShldr.y << " " << lShldr.z << endl;
	cout << "rshlder:" << rShldr.x << " " << rShldr.y << " " << rShldr.z << endl;
	cout << "ZtempPoint:" << tempPoint.x << " " << tempPoint.y << " " << tempPoint.z << endl;
	cout << "turn round shoulder angle:" << angle << endl;

	return angle;
}
//������ϵ��ʼ������ ���������ֱ�Ϊԭ����˵� �˴��ĵ���������
//Ϊ��������ϵ ��ֱ֪������ϵ��������ϵת���Ĺ�ʽΪ��������ϵ�µ�
//��Ƕ���Ϣ��֪����û��Ӱ��  ???
//Ҫ�����Ҽ� ��������Ϊ4��ԭ��ô��
//����ת��һ������ ����������ϵ��yֵ������������ϵ�µ�zֵ
//acos ���� 0 �� 180 atan ���� -90 �� 90
void CalculateAngle::SphericalCoordinate(MyPoint origin, MyPoint point)
{
	MyPoint normal(point.x - origin.x, point.z - origin.z, point.y - origin.y);////////ע���ʱ�Ѿ���y z����ת������ ����������ϵ�¼���

	printPoint("SphericalCoordinate:normal", normal);

	this->radius = sqrt(normal.x*normal.x + normal.y*normal.y + normal.z*normal.z);

	if ((normal.x > 0) && (normal.y > 0) && (normal.z > 0))//���ڵ�һ����
	{
		cout << "��һ����" << endl;
		this->phi = atan(normal.y / normal.x)*180.0 / 3.141593;
		this->theta = acos(normal.z / (sqrt(normal.x*normal.x + normal.y*normal.y + normal.z*normal.z)))*180.0 / 3.141593;
	}
	else if ((normal.x < 0) && (normal.y >0) && (normal.z > 0))//�ڶ�����
	{
		cout << "��er����" << endl;
		this->phi = atan(abs(normal.y / normal.x))*180.0 / 3.141593 + 90;
		this->theta = acos(normal.z / (sqrt(normal.x*normal.x + normal.y*normal.y + normal.z*normal.z)))*180.0 / 3.141593;
	}
	else if ((normal.x < 0) && (normal.y < 0) && (normal.z > 0))
	{
		cout << "��san����" << endl;
		this->phi = atan(abs(normal.y / normal.x))*180.0 / 3.141593 + 180;
		this->theta = acos(normal.z / (sqrt(normal.x*normal.x + normal.y*normal.y + normal.z*normal.z)))*180.0 / 3.141593;
	}
	else if ((normal.x >0) && (normal.y < 0) && (normal.z > 0))///��������
	{
		cout << "��si����" << endl;
		this->phi = atan(abs(normal.y / normal.x))*180.0 / 3.141593 + 270;
		this->theta = acos(normal.z / (sqrt(normal.x*normal.x + normal.y*normal.y + normal.z*normal.z)))*180.0 / 3.141593;
	}
	else if ((normal.x > 0) && (normal.y > 0) && (normal.z < 0))//5
	{
		cout << "��wu����" << endl;
		this->phi = atan(abs(normal.y / normal.x))*180.0 / 3.141593;
		this->theta = acos(normal.z / (sqrt(normal.x*normal.x + normal.y*normal.y + normal.z*normal.z)))*180.0 / 3.141593;
	}
	else if ((normal.x < 0) && (normal.y > 0) && (normal.z < 0))//6
	{
		cout << "��liu����" << endl;
		this->phi = atan(abs(normal.y / normal.x))*180.0 / 3.141593 + 90;
		this->theta = acos(normal.z / (sqrt(normal.x*normal.x + normal.y*normal.y + normal.z*normal.z)))*180.0 / 3.141593;
	}
	else if ((normal.x < 0) && (normal.y < 0) && (normal.z < 0))//7
	{
		cout << "��qi����" << endl;
		this->phi = atan(abs(normal.y / normal.x))*180.0 / 3.141593 + 180;
		this->theta = acos(normal.z / (sqrt(normal.x*normal.x + normal.y*normal.y + normal.z*normal.z)))*180.0 / 3.141593;
	}
	else if ((normal.x > 0) && (normal.y < 0) && (normal.z < 0))//8
	{
		cout << "��ba����" << endl;
		this->phi = atan(abs(normal.y / normal.x))*180.0 / 3.141593 + 270;
		this->theta = acos(normal.z / (sqrt(normal.x*normal.x + normal.y*normal.y + normal.z*normal.z)))*180.0 / 3.141593;
	}
	else
	{
		this->phi = 0.0; ////////////Ϲд�� ��ȷ�� 
		this->theta = 0.0;
		cout << "�����쳣���  x y z���е���0����� �ȴ�����" << endl;
	}

	cout << "radius:" << this->radius << " " << "theta:" << this->theta << " " << "phi:" << this->phi << endl;
	//cout<<"theta:"<<this->theta<<endl;	
	//cout<<"phi:"<<this->phi<<endl;
}
