
#include"Skeleton.h"

void addTextToPhoto(Mat photo,int x,int y,char *str)
{
	cv::Point position(x,y);///为了显示出编号
	putText(photo,str,position, CV_FONT_HERSHEY_SIMPLEX, 1, Scalar(0,0,0));
}

void Skeleton::ProcessingSkeleton4(Mat photo,float** Dist,CvPoint2D32f gujiadian[5][4],int gujiadian2[5][4],int Vertex[6][2],int vvv,int lefthand,int leftfoot,int righthand,int rightfoot,CvPoint2D32f Skeleton_point[21],const int Num)
{
	float min,max;
	int _min,_max,_second;
	int sign1,sign2,sign3;
	CvPoint2D32f minP,maxP;
	if(Num==2)
	{
		for(int i=0;i<5;i++)
		{
			min=Dist[i+1][gujiadian2[i][0]];
			minP=gujiadian[i][0];
			max=Dist[i+1][gujiadian2[i][0]];
			maxP=gujiadian[i][0];
			for(int j=0;j<Num;j++)
			{
				if(min>Dist[i+1][gujiadian2[i][j]])
				{
					min=Dist[i+1][gujiadian2[i][j]];
					minP=gujiadian[i][j];
				}
				if(max<Dist[i+1][gujiadian2[i][j]])
				{
					max=Dist[i+1][gujiadian2[i][j]];
					maxP=gujiadian[i][j];
				}
			}
			//将最近和最远的特征点存入数组
			if ((i+1)==5)
			{
				Skeleton_point[1].x=minP.x;
				Skeleton_point[1].y=minP.y;

				//circle(photo,Skeleton_point[1],5,Scalar(0,255,0),-1);
				imshow("show",photo);
				waitKey(1);

				Skeleton_point[2].x=maxP.x;
				Skeleton_point[2].y=maxP.y;

				//circle(photo,Skeleton_point[2],5,Scalar(0,255,0),-1);
				imshow("show",photo);
				waitKey(1);
			}
			else if ((i+1)==lefthand)
			{
				Skeleton_point[3].x=minP.x;
				Skeleton_point[3].y=minP.y;

				//circle(photo,Skeleton_point[3],5,Scalar(0,255,0),-1);
				imshow("show",photo);
				waitKey(1);

				Skeleton_point[4].x=maxP.x;
				Skeleton_point[4].y=maxP.y;

				//circle(photo,Skeleton_point[4],5,Scalar(0,255,0),-1);
				imshow("show",photo);
				waitKey(1);
			}
			else if ((i+1)==righthand)
			{
				Skeleton_point[5].x=minP.x;
				Skeleton_point[5].y=minP.y;

				//circle(photo,Skeleton_point[5],5,Scalar(0,255,0),-1);
				imshow("show",photo);
				waitKey(1);

				Skeleton_point[6].x=maxP.x;
				Skeleton_point[6].y=maxP.y;

				//circle(photo,Skeleton_point[6],5,Scalar(0,255,0),-1);
				imshow("show",photo);
				waitKey(1);
			}
			else if ((i+1)==leftfoot)
			{
				Skeleton_point[7].x=minP.x;
				Skeleton_point[7].y=minP.y;

				//circle(photo,Skeleton_point[7],5,Scalar(0,255,0),-1);
				imshow("show",photo);
				waitKey(1);

				Skeleton_point[8].x=maxP.x;
				Skeleton_point[8].y=maxP.y;

				//circle(photo,Skeleton_point[8],5,Scalar(0,255,0),-1);
				imshow("show",photo);
				waitKey(1);
			}
			else if ((i+1)==rightfoot)
			{
				Skeleton_point[9].x=minP.x;
				Skeleton_point[9].y=minP.y;

				//circle(photo,Skeleton_point[9],5,Scalar(0,255,0),-1);
				imshow("show",photo);
				waitKey(1);

				Skeleton_point[10].x=maxP.x;
				Skeleton_point[10].y=maxP.y;

				//circle(photo,Skeleton_point[10],5,Scalar(0,255,0),-1);
				imshow("show",photo);
				waitKey(1);
			}
			//上述代码
		}
	}
	else if (Num==3)
	{
		for(int i=0;i<4;i++)
		{
			sign1=0;sign3=0;
			min=Dist[i+1][gujiadian2[i][0]];
			minP=gujiadian[i][0];
			max=Dist[i+1][gujiadian2[i][0]];
			maxP=gujiadian[i][0];
			for(int j=0;j<Num;j++)
			{
				if(min>Dist[i+1][gujiadian2[i][j]])
				{
					min=Dist[i+1][gujiadian2[i][j]];
					minP=gujiadian[i][j];
					sign1=j;
				}
				if(max<Dist[i+1][gujiadian2[i][j]])
				{
					max=Dist[i+1][gujiadian2[i][j]];
					maxP=gujiadian[i][j];
					sign3=j;
				}
			}
			//将最近和最远的特征点存入数组
			if ((i+1)==lefthand)
			{
				for (int t=0;t<Num;t++)
				{
					if((t!=sign1)&&(t!=sign3))
					{
						Skeleton_point[5].x=gujiadian[i][t].x;
						Skeleton_point[5].y=gujiadian[i][t].y;

						//circle(photo,Skeleton_point[5],5,Scalar(0,255,0),-1);
						imshow("show",photo);
						waitKey(1);
					}
				}
				Skeleton_point[4].x=minP.x;
				Skeleton_point[4].y=minP.y;

				//circle(photo,Skeleton_point[4],5,Scalar(0,255,0),-1);
				imshow("show",photo);
				waitKey(1);
				
				Skeleton_point[6].x=maxP.x;
				Skeleton_point[6].y=maxP.y;

				//circle(photo,Skeleton_point[6],5,Scalar(0,255,0),-1);
				imshow("show",photo);
				waitKey(1);
			}
			else if ((i+1)==righthand)
			{
				for (int t=0;t<Num;t++)
				{
					if((t!=sign1)&&(t!=sign3))
					{
						Skeleton_point[8].x=gujiadian[i][t].x;
						Skeleton_point[8].y=gujiadian[i][t].y;

						//circle(photo,Skeleton_point[8],5,Scalar(0,255,0),-1);
						imshow("show",photo);
						waitKey(1);
					}
				}
				Skeleton_point[7].x=minP.x;
				Skeleton_point[7].y=minP.y;

				//circle(photo,Skeleton_point[7],5,Scalar(0,255,0),-1);
				imshow("show",photo);
				waitKey(1);

				Skeleton_point[9].x=maxP.x;
				Skeleton_point[9].y=maxP.y;

				//circle(photo,Skeleton_point[9],5,Scalar(0,255,0),-1);
				imshow("show",photo);
				waitKey(1);
			}
			else if ((i+1)==leftfoot)
			{
				for (int t=0;t<Num;t++)
				{
					if((t!=sign1)&&(t!=sign3))
					{
						Skeleton_point[11].x=gujiadian[i][t].x;
						Skeleton_point[11].y=gujiadian[i][t].y;

						//circle(photo,Skeleton_point[11],5,Scalar(0,255,0),-1);
						imshow("show",photo);
						waitKey(1);
					}
				}
				Skeleton_point[10].x=minP.x;
				Skeleton_point[10].y=minP.y;

				//circle(photo,Skeleton_point[10],5,Scalar(0,255,0),-1);
				imshow("show",photo);
				waitKey(1);

				Skeleton_point[12].x=maxP.x;
				Skeleton_point[12].y=maxP.y;

				//circle(photo,Skeleton_point[12],5,Scalar(0,255,0),-1);
				imshow("show",photo);
				waitKey(1);
			}
			else if ((i+1)==rightfoot)
			{
				for (int t=0;t<Num;t++)
				{
					if((t!=sign1)&&(t!=sign3))
					{
						Skeleton_point[14].x=gujiadian[i][t].x;
						Skeleton_point[14].y=gujiadian[i][t].y;

						//circle(photo,Skeleton_point[14],5,Scalar(0,255,0),-1);
						imshow("show",photo);
						waitKey(1);
					}
				}
				Skeleton_point[13].x=minP.x;
				Skeleton_point[13].y=minP.y;

				//circle(photo,Skeleton_point[13],5,Scalar(0,255,0),-1);
				imshow("show",photo);
				waitKey(1);

				Skeleton_point[15].x=maxP.x;
				Skeleton_point[15].y=maxP.y;

				//circle(photo,Skeleton_point[15],5,Scalar(0,255,0),-1);
				imshow("show",photo);
				waitKey(1);
			}
			
		}//循环左右手脚

	//第五个是头部区域
		min=Dist[5][gujiadian2[4][0]];
		minP=gujiadian[4][0];
		sign1=0;
		/*max=Dist[5][gujiadian2[4][0]];
		maxP=gujiadian[4][0];*/
		for(int j=0;j<Num;j++)//寻找最近的点
		{
			if(min>Dist[5][gujiadian2[4][j]])
			{
				min=Dist[5][gujiadian2[4][j]];
				minP=gujiadian[4][j];
				sign1=j;
			}
		}
		Skeleton_point[1].x=minP.x;
		Skeleton_point[1].y=minP.y;

		//circle(photo,Skeleton_point[1],5,Scalar(0,255,0),-1);
		imshow("show",photo);
		waitKey(1);

		min=Dist[lefthand][gujiadian2[4][0]];
		minP=gujiadian[4][0];
		sign2=0;
		/*max=Dist[5][gujiadian2[4][0]];
		maxP=gujiadian[4][0];*/
		for(int j=0;j<Num;j++)//寻找图左手最近的点
		{
			if(min>Dist[lefthand][gujiadian2[4][j]])
			{
				min=Dist[lefthand][gujiadian2[4][j]];
				minP=gujiadian[4][j];
				sign2=j;
			}
		}
		Skeleton_point[2].x=minP.x;
		Skeleton_point[2].y=minP.y;

		//circle(photo,Skeleton_point[2],5,Scalar(0,255,0),-1);
		imshow("show",photo);
		waitKey(1);
	
		for (int t=0;t<Num;t++)
		{
			if((t!=sign1)&&(t!=sign2))
			{
				Skeleton_point[3].x=gujiadian[4][t].x;
				Skeleton_point[3].y=gujiadian[4][t].y;
			}
		}

		//circle(photo,Skeleton_point[3],5,Scalar(0,255,0),-1);
		imshow("show",photo);
		waitKey(1);

	}


if (Num==4)
{

	////假设数组前四个是手脚，最后一个是头
	for(int i=0;i<4;i++)//处理左右手脚
	{
		int sign1,sign2,sign3;
		min=Dist[i+1][gujiadian2[i][0]];
		minP=gujiadian[i][0];
		max=Dist[i+1][gujiadian2[i][0]];
		maxP=gujiadian[i][0];

		for(int j=0;j<Num;j++)
		{
			cout<<min<<":"<<max<<":"<<Dist[i+1][gujiadian2[i][j]]<<endl;
			if(min>Dist[i+1][gujiadian2[i][j]])
			{
				min = Dist[i + 1][gujiadian2[i][j]];
				minP = gujiadian[i][j];
				sign3 = j;
			}
			if(max<Dist[i+1][gujiadian2[i][j]])
			{
				max = Dist[i + 1][gujiadian2[i][j]];
				maxP = gujiadian[i][j];
				sign1 = j;
			}
		}

		cout<<endl;
		line(photo,minP,maxP,Scalar(255,255,255),2,8,0);
		namedWindow("show");
		imshow("show",photo);
		waitKey(1);

		float min_min=10000;
		CvPoint2D32f min_min_p;//&&&&&&&&
		for(int m=0;m<5;m++)
		{
			if(m!=i)
			{
				for(int n=0;n<4;n++)
				{
					if(Dist[i+1][gujiadian2[m][n]]<min_min)
					{
						min_min=Dist[i+1][gujiadian2[m][n]];
						min_min_p=gujiadian[m][n];
					}
				}
			}
		}
		if(Dist[i+1][vvv]<min_min)
			min_min_p=cvPoint2D32f(Vertex[0][0],Vertex[0][1]);

		line(photo,min_min_p,maxP,Scalar(255,255,255),2,8,0);
		namedWindow("show");
		imshow("show",photo);
		waitKey(1);
	}
	//第五个是头部区域
	min=Dist[5][gujiadian2[4][0]];
	minP=gujiadian[4][0];
	max=Dist[5][gujiadian2[4][0]];
	maxP=gujiadian[4][0];
	_min=0;
	_max=0;
	for(int j=0;j<Num;j++)
	{
		
		if(min>Dist[5][gujiadian2[4][j]])
			{
				min=Dist[5][gujiadian2[4][j]];
				minP=gujiadian[4][j];
				_min=j;
			}
		if(max<Dist[5][gujiadian2[4][j]])
			{
				max=Dist[5][gujiadian2[4][j]];
				maxP=gujiadian[4][j];
				_max=j;
			}
	}
	int p2=0,p4=0;
	while((p2==_min)  ||  (p2==_max))
	{
		p2++;
	}
	while((p4==_min)  ||  (p4==_max) ||(p4==p2))
	{
		p4++;
	}
	if(Dist[5][gujiadian2[4][p2]]<Dist[5][gujiadian2[4][p4]])
		_second=p2;
	else _second=p4;
	for(int n=0;n<4;n++)
	{
		if(n!=_second)
		{
			line(photo,gujiadian[4][n],gujiadian[4][_second],Scalar(255,255,255),2,8,0);
		}
	
	}
	float x=Vertex[0][0],y=Vertex[0][1];
	CvPoint2D32f _point=cvPoint2D32f(x,y);
  	line(photo,_point,gujiadian[4][_second],Scalar(255,255,255),2,8,0);//连接几何中心点和脖子
	namedWindow("show");
	imshow("show",photo);
	waitKey(1);


	////////////////////////////////////////////////////////////////////////////////////////////
	bool _sign[4];//处理图片中左边的手
	for(int i=0;i<4;i++)
	{
		_sign[i]=false;
	}
	float _temp=0;
	int _temp_sign=0;
	for(int i=0;i<4;i++)
	{
		for(int j=0;j<4;j++)
		{
			if(_sign[j]==false)
				if(_temp <Dist[lefthand][gujiadian2[lefthand-1][j]])
				{
					_temp=Dist[lefthand][gujiadian2[lefthand-1][j]];
					_temp_sign=j;
				}
		}
		_sign[_temp_sign]=true;
		_temp=0;
		Skeleton_point[8-i].x=gujiadian[lefthand-1][_temp_sign].x;
		Skeleton_point[8-i].y=gujiadian[lefthand-1][_temp_sign].y;
	}
	//处理图片中右边的手部
	for(int i=0;i<4;i++)
	{
		_sign[i]=false;
	}
	_temp=0;
	_temp_sign=0;
	for(int i=0;i<4;i++)
	{
		for(int j=0;j<4;j++)
		{
			if(_sign[j]==false)
				if(_temp <Dist[righthand][gujiadian2[righthand-1][j]])
				{
					_temp=Dist[righthand][gujiadian2[righthand-1][j]];
					_temp_sign=j;
				}
		}
		_sign[_temp_sign]=true;
		_temp=0;
		Skeleton_point[12-i].x=gujiadian[righthand-1][_temp_sign].x;
		Skeleton_point[12-i].y=gujiadian[righthand-1][_temp_sign].y;
	}
	//处理图片中左边的脚
	for(int i=0;i<4;i++)
	{
		_sign[i]=false;
	}
	_temp=0;
	_temp_sign=0;
	for(int i=0;i<4;i++)
	{
		for(int j=0;j<4;j++)
		{
			if(_sign[j]==false)
				if(_temp <Dist[leftfoot][gujiadian2[leftfoot-1][j]])
				{
					_temp=Dist[leftfoot][gujiadian2[leftfoot-1][j]];
					_temp_sign=j;
				}
		}
		_sign[_temp_sign]=true;
		_temp=0;
		Skeleton_point[16-i].x=gujiadian[leftfoot-1][_temp_sign].x;
		Skeleton_point[16-i].y=gujiadian[leftfoot-1][_temp_sign].y;
	}
	//处理图片中右边的脚
	for(int i=0;i<4;i++)
	{
		_sign[i]=false;
	}
	_temp=0;
	_temp_sign=0;
	for(int i=0;i<4;i++)
	{
		for(int j=0;j<4;j++)
		{
			if(_sign[j]==false)
				if(_temp <Dist[rightfoot][gujiadian2[rightfoot-1][j]])
				{
					_temp=Dist[rightfoot][gujiadian2[rightfoot-1][j]];
					_temp_sign=j;
				}
		}
		_sign[_temp_sign]=true;
		_temp=0;
		Skeleton_point[20-i].x=gujiadian[rightfoot-1][_temp_sign].x;
		Skeleton_point[20-i].y=gujiadian[rightfoot-1][_temp_sign].y;
	}
	//处理图片中的头部，比较复杂
	for(int i=0;i<4;i++)
	{
		_sign[i]=false;
	}
	_temp=Dist[5][gujiadian2[4][0]];
	_temp_sign=0;
	for(int j=0;j<4;j++)
	{
		if(_sign[j]==false)
			if(_temp >Dist[5][gujiadian2[4][j]])
			{
				_temp=Dist[5][gujiadian2[4][j]];
				_temp_sign=j;
			}
	}
	_sign[_temp_sign]=true;
	Skeleton_point[1].x=gujiadian[4][_temp_sign].x;
	Skeleton_point[1].y=gujiadian[4][_temp_sign].y;//头

	_temp=0;
	for(int j=0;j<4;j++)
	{
		if(_sign[j]==false)
			if(_temp <Dist[righthand][gujiadian2[4][j]])
			{
				_temp=Dist[righthand][gujiadian2[4][j]];
				_temp_sign=j;
			}
	}
	_sign[_temp_sign]=true;
	Skeleton_point[2].x=gujiadian[4][_temp_sign].x;
	Skeleton_point[2].y=gujiadian[4][_temp_sign].y;//图左边的手部

	_temp=0;
	for(int j=0;j<4;j++)
	{
		if(_sign[j]==false)
			if(_temp <Dist[lefthand][gujiadian2[4][j]])
			{
				_temp=Dist[lefthand][gujiadian2[4][j]];
				_temp_sign=j;
			}
	}
	_sign[_temp_sign]=true;
	Skeleton_point[4].x=gujiadian[4][_temp_sign].x;
	Skeleton_point[4].y=gujiadian[4][_temp_sign].y;//图右边的手部

	for(int j=0;j<4;j++)
	{
		if(_sign[j]==false)
		{
			Skeleton_point[3].x=gujiadian[4][j].x;
			Skeleton_point[3].y=gujiadian[4][j].y;//脖子
		}
	}

}
	
}


void Skeleton::ProcessingSkeleton(Mat photo,float** Dist,CvPoint2D32f gujiadian[5][3],int gujiadian2[5][3],int Vertex[6][2],int vvv,int lefthand,int leftfoot,int righthand,int rightfoot,CvPoint2D32f Skeleton_point[16],const int Num)
{
	float min,max;
	int _min,_max,_second;
	int sign1,sign2,sign3;
	CvPoint2D32f minP,maxP;
	
		for(int i=0;i<4;i++)
		{
			sign1=0;sign3=0;
			min=Dist[i+1][gujiadian2[i][0]];
			minP=gujiadian[i][0];
			max=Dist[i+1][gujiadian2[i][0]];
			maxP=gujiadian[i][0];
			for(int j=0;j<Num;j++)
			{
				if(min>Dist[i+1][gujiadian2[i][j]])
				{
					min=Dist[i+1][gujiadian2[i][j]];
					minP=gujiadian[i][j];
					sign1=j;
				}
				if(max<Dist[i+1][gujiadian2[i][j]])
				{
					max=Dist[i+1][gujiadian2[i][j]];
					maxP=gujiadian[i][j];
					sign3=j;
				}
			}
			//将最近和最远的特征点存入数组
			if ((i+1)==lefthand)
			{
				for (int t=0;t<Num;t++)
				{
					if((t!=sign1)&&(t!=sign3))
					{
						Skeleton_point[5].x=gujiadian[i][t].x;
						Skeleton_point[5].y=gujiadian[i][t].y;

						
						//addTextToPhoto(photo,Skeleton_point[5].x,Skeleton_point[5].y,"5");
						//circle(photo,Skeleton_point[5],5,Scalar(0,255,0),-1);
						imshow("show",photo);
						waitKey(1);
					}
				}
				Skeleton_point[4].x=minP.x;
				Skeleton_point[4].y=minP.y;

				//addTextToPhoto(photo,Skeleton_point[4].x,Skeleton_point[4].y,"4");
				//circle(photo,Skeleton_point[4],5,Scalar(0,255,0),-1);
				imshow("show",photo);
				waitKey(1);
				
				Skeleton_point[6].x=maxP.x;
				Skeleton_point[6].y=maxP.y;

				//addTextToPhoto(photo,Skeleton_point[6].x,Skeleton_point[6].y,"6");
				//circle(photo,Skeleton_point[6],5,Scalar(0,255,0),-1);
				imshow("show",photo);
				waitKey(1);
			}
			else if ((i+1)==righthand)
			{
				for (int t=0;t<Num;t++)
				{
					if((t!=sign1)&&(t!=sign3))
					{
						Skeleton_point[8].x=gujiadian[i][t].x;
						Skeleton_point[8].y=gujiadian[i][t].y;

						//addTextToPhoto(photo,Skeleton_point[8].x,Skeleton_point[8].y,"8");
						//circle(photo,Skeleton_point[8],5,Scalar(0,255,0),-1);
						imshow("show",photo);
						waitKey(1);
					}
				}
				Skeleton_point[7].x=minP.x;
				Skeleton_point[7].y=minP.y;

				//addTextToPhoto(photo,Skeleton_point[7].x,Skeleton_point[7].y,"7");
				//circle(photo,Skeleton_point[7],5,Scalar(0,255,0),-1);
				imshow("show",photo);
				waitKey(1);

				
				Skeleton_point[9].x=maxP.x;
				Skeleton_point[9].y=maxP.y;

				//addTextToPhoto(photo,Skeleton_point[9].x,Skeleton_point[9].y,"9");
				//circle(photo,Skeleton_point[9],5,Scalar(0,255,0),-1);
				imshow("show",photo);
				waitKey(1);
			}
			else if ((i+1)==leftfoot)
			{
				for (int t=0;t<Num;t++)
				{
					if((t!=sign1)&&(t!=sign3))
					{
						Skeleton_point[11].x=gujiadian[i][t].x;
						Skeleton_point[11].y=gujiadian[i][t].y;

						//addTextToPhoto(photo,Skeleton_point[11].x,Skeleton_point[11].y,"11");
						//circle(photo,Skeleton_point[11],5,Scalar(0,255,0),-1);
						imshow("show",photo);
						waitKey(1);
					}
				}
				Skeleton_point[10].x=minP.x;
				Skeleton_point[10].y=minP.y;

				//addTextToPhoto(photo,Skeleton_point[10].x,Skeleton_point[10].y,"10");
				//circle(photo,Skeleton_point[10],5,Scalar(0,255,0),-1);
				imshow("show",photo);
				waitKey(1);

				
				Skeleton_point[12].x=maxP.x;
				Skeleton_point[12].y=maxP.y;

				//addTextToPhoto(photo,Skeleton_point[12].x,Skeleton_point[12].y,"12");
				//circle(photo,Skeleton_point[12],5,Scalar(0,255,0),-1);
				imshow("show",photo);
				waitKey(1);
			}
			else if ((i+1)==rightfoot)
			{
				for (int t=0;t<Num;t++)
				{
					if((t!=sign1)&&(t!=sign3))
					{
						Skeleton_point[14].x=gujiadian[i][t].x;
						Skeleton_point[14].y=gujiadian[i][t].y;

						//addTextToPhoto(photo,Skeleton_point[14].x,Skeleton_point[14].y,"14");
						//circle(photo,Skeleton_point[14],5,Scalar(0,255,0),-1);
						imshow("show",photo);
						waitKey(1);
					}
				}
				Skeleton_point[13].x=minP.x;
				Skeleton_point[13].y=minP.y;

				//addTextToPhoto(photo,Skeleton_point[13].x,Skeleton_point[13].y,"13");
				//circle(photo,Skeleton_point[13],5,Scalar(0,255,0),-1);
				imshow("show",photo);
				waitKey(1);

				Skeleton_point[15].x=maxP.x;
				Skeleton_point[15].y=maxP.y;

				//addTextToPhoto(photo,Skeleton_point[15].x,Skeleton_point[15].y,"15");
				//circle(photo,Skeleton_point[15],5,Scalar(0,255,0),-1);
				imshow("show",photo);
				waitKey(1);
			}
			
		}//循环左右手脚

	//第五个是头部区域
		min=Dist[5][gujiadian2[4][0]];
		minP=gujiadian[4][0];
		sign1=0;
		/*max=Dist[5][gujiadian2[4][0]];
		maxP=gujiadian[4][0];*/
		for(int j=0;j<Num;j++)//寻找最近的点
		{
			if(min>Dist[5][gujiadian2[4][j]])
			{
				min=Dist[5][gujiadian2[4][j]];
				minP=gujiadian[4][j];
				sign1=j;
			}
		}
		Skeleton_point[1].x=minP.x;
		Skeleton_point[1].y=minP.y;

		//addTextToPhoto(photo,Skeleton_point[1].x,Skeleton_point[1].y,"1");
		//circle(photo,Skeleton_point[1],5,Scalar(0,255,0),-1);
		imshow("show",photo);
		waitKey(1);

		min=Dist[lefthand][gujiadian2[4][0]];
		minP=gujiadian[4][0];
		sign2=0;
		/*max=Dist[5][gujiadian2[4][0]];
		maxP=gujiadian[4][0];*/
		for(int j=0;j<Num;j++)//寻找图左手最近的点
		{
			if(min>Dist[lefthand][gujiadian2[4][j]])
			{
				min=Dist[lefthand][gujiadian2[4][j]];
				minP=gujiadian[4][j];
				sign2=j;
			}
		}
		Skeleton_point[2].x=minP.x;
		Skeleton_point[2].y=minP.y;

		//addTextToPhoto(photo,Skeleton_point[2].x,Skeleton_point[2].y,"2");
		//circle(photo,Skeleton_point[2],5,Scalar(0,255,0),-1);
		imshow("show",photo);
		waitKey(1);
	
		for (int t=0;t<Num;t++)
		{
			if((t!=sign1)&&(t!=sign2))
			{
				Skeleton_point[3].x=gujiadian[4][t].x;
				Skeleton_point[3].y=gujiadian[4][t].y;
			}
		}

		//addTextToPhoto(photo,Skeleton_point[3].x,Skeleton_point[3].y,"3");
		//circle(photo,Skeleton_point[3],5,Scalar(0,255,0),-1);

		//addTextToPhoto(photo,Skeleton_point[0].x,Skeleton_point[0].y,"0");
		//circle(photo,Skeleton_point[0],5,Scalar(0,255,0),-1);

		imshow("show",photo);
		waitKey(1);

	
}