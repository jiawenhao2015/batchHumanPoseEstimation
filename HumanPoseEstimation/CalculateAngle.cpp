
#include"CalculateAngle.h"
#include<math.h>

#include<iostream>
using namespace std;

////注意cout一个string字符串变量时候的写法

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
///已知三点求平面
void CalculateAngle::getPlane(Data a, Data b, Data c, Surface &sur)
{
	sur.a = (b.Ty - a.Ty)*(c.Tz - a.Tz) - (b.Tz - a.Tz)*(c.Ty - a.Ty);
	sur.b = ((b.Tz - a.Tz)*(c.Tx - a.Tx) - (b.Tx - a.Tx)*(c.Tz - a.Tz));
	sur.c = ((b.Tx - a.Tx)*(c.Ty - a.Ty) - (b.Ty - a.Ty)*(c.Tx - a.Tx));
	sur.d = (0 - (sur.a*a.Tx + sur.b*a.Ty + sur.c*a.Tz));
}

// 已知三点坐标，求法向量   
MyPoint CalculateAngle::getNormal(MyPoint p1, MyPoint p2, MyPoint p3)
{
	MyPoint normal;
	normal.x = ((p2.y - p1.y)*(p3.z - p1.z) - (p2.z - p1.z)*(p3.y - p1.y));
	normal.y = ((p2.z - p1.z)*(p3.x - p1.x) - (p2.x - p1.x)*(p3.z - p1.z));
	normal.z = ((p2.x - p1.x)*(p3.y - p1.y) - (p2.y - p1.y)*(p3.x - p1.x));

	return normal;
}
//已知两向量 求夹角
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
//计算斜肩的角度 定义为 找到A B 两肩中y轴最小的那个 
//比如A y轴小 将B的y值调整为A的y值 记为b 求AB与bB两直线的角度
//这样做合理吗？ 感觉不合理 应该加上平面的信息....但是平面如何得到？
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
//转身角度计算 找到A B肩膀中最小的z值 不妨为A 然后定义点C 与A相同z值 
//C点的x y值取自B 计算AB与AC的角度 记为转身角度 angle  类似求斜肩的角度
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
//球坐标系初始化函数 两个参数分别为原点与端点 此处的点数据坐标
//为左手坐标系 已知直角坐标系与球坐标系转换的公式为右手坐标系下的
//求角度信息不知道有没有影响  ???
//要把左右肩 左右髋作为4个原点么？
//现在转换一下试试 将左手坐标系的y值当做右手坐标系下的z值
//acos 返回 0 到 180 atan 返回 -90 到 90
void CalculateAngle::SphericalCoordinate(MyPoint origin, MyPoint point)
{
	MyPoint normal(point.x - origin.x, point.z - origin.z, point.y - origin.y);////////注意此时已经把y z坐标转换过来 在右手坐标系下计算

	printPoint("SphericalCoordinate:normal", normal);

	this->radius = sqrt(normal.x*normal.x + normal.y*normal.y + normal.z*normal.z);

	if ((normal.x > 0) && (normal.y > 0) && (normal.z > 0))//点在第一象限
	{
		cout << "第一象限" << endl;
		this->phi = atan(normal.y / normal.x)*180.0 / 3.141593;
		this->theta = acos(normal.z / (sqrt(normal.x*normal.x + normal.y*normal.y + normal.z*normal.z)))*180.0 / 3.141593;
	}
	else if ((normal.x < 0) && (normal.y >0) && (normal.z > 0))//第二象限
	{
		cout << "第er象限" << endl;
		this->phi = atan(abs(normal.y / normal.x))*180.0 / 3.141593 + 90;
		this->theta = acos(normal.z / (sqrt(normal.x*normal.x + normal.y*normal.y + normal.z*normal.z)))*180.0 / 3.141593;
	}
	else if ((normal.x < 0) && (normal.y < 0) && (normal.z > 0))
	{
		cout << "第san象限" << endl;
		this->phi = atan(abs(normal.y / normal.x))*180.0 / 3.141593 + 180;
		this->theta = acos(normal.z / (sqrt(normal.x*normal.x + normal.y*normal.y + normal.z*normal.z)))*180.0 / 3.141593;
	}
	else if ((normal.x >0) && (normal.y < 0) && (normal.z > 0))///第四象限
	{
		cout << "第si象限" << endl;
		this->phi = atan(abs(normal.y / normal.x))*180.0 / 3.141593 + 270;
		this->theta = acos(normal.z / (sqrt(normal.x*normal.x + normal.y*normal.y + normal.z*normal.z)))*180.0 / 3.141593;
	}
	else if ((normal.x > 0) && (normal.y > 0) && (normal.z < 0))//5
	{
		cout << "第wu象限" << endl;
		this->phi = atan(abs(normal.y / normal.x))*180.0 / 3.141593;
		this->theta = acos(normal.z / (sqrt(normal.x*normal.x + normal.y*normal.y + normal.z*normal.z)))*180.0 / 3.141593;
	}
	else if ((normal.x < 0) && (normal.y > 0) && (normal.z < 0))//6
	{
		cout << "第liu象限" << endl;
		this->phi = atan(abs(normal.y / normal.x))*180.0 / 3.141593 + 90;
		this->theta = acos(normal.z / (sqrt(normal.x*normal.x + normal.y*normal.y + normal.z*normal.z)))*180.0 / 3.141593;
	}
	else if ((normal.x < 0) && (normal.y < 0) && (normal.z < 0))//7
	{
		cout << "第qi象限" << endl;
		this->phi = atan(abs(normal.y / normal.x))*180.0 / 3.141593 + 180;
		this->theta = acos(normal.z / (sqrt(normal.x*normal.x + normal.y*normal.y + normal.z*normal.z)))*180.0 / 3.141593;
	}
	else if ((normal.x > 0) && (normal.y < 0) && (normal.z < 0))//8
	{
		cout << "第ba象限" << endl;
		this->phi = atan(abs(normal.y / normal.x))*180.0 / 3.141593 + 270;
		this->theta = acos(normal.z / (sqrt(normal.x*normal.x + normal.y*normal.y + normal.z*normal.z)))*180.0 / 3.141593;
	}
	else
	{
		this->phi = 0.0; ////////////瞎写的 待确定 
		this->theta = 0.0;
		cout << "其他异常情况  x y z中有等于0的情况 等待处理。" << endl;
	}

	cout << "radius:" << this->radius << " " << "theta:" << this->theta << " " << "phi:" << this->phi << endl;
	//cout<<"theta:"<<this->theta<<endl;	
	//cout<<"phi:"<<this->phi<<endl;
}
