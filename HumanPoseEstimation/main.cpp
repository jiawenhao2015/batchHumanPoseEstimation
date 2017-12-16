#include <iostream>
#include "highgui.h"
#include<windows.h>
#include "slic.h"
#include "ImageShow.h"
#include "Loading.h"
#include "Dijkstra.h"
#include "Cluster.h"
#include "Kmeans.h"
#include <ctime>
#include "Skeleton.h"
#include<winbase.h>
#include<direct.h>
using namespace std;
using namespace cv;

int HEIGHT = 480;
int  WIDTH = 640;
#define NUM_SUPERPIXEL 2500
//callback function  
void mouseEvent(int evt, int x, int y, int flags, void* param)
{
	IplImage* img, *img1;
	img = (IplImage*)param;
	img1 = cvCloneImage(img);

	char  label[128];
	CvFont font, font2;;
	cvInitFont(&font, CV_FONT_HERSHEY_PLAIN, 1, 1, 0, 1, 1);
	cvInitFont(&font2, CV_FONT_HERSHEY_PLAIN, 1, 1, 0, 1, 1);

	if (evt == CV_EVENT_LBUTTONDOWN){
		//printf("%d %d\n", x, y);
	}
	if (evt == CV_EVENT_MOUSEMOVE)
	{
		//cout << x << " " << y << endl;
		sprintf(label, " %d, %d", x, y);
		cvPutText(img1, label, cvPoint(x, y), &font, CV_RGB(255, 0, 0));
		cvShowImage("show", img1);
	}
}
////�ж��ļ�·���Ƿ����
bool dirExists(const std::string& dirName_in)
{
	DWORD ftyp = GetFileAttributesA(dirName_in.c_str());
	if (ftyp == INVALID_FILE_ATTRIBUTES)
		return false;  //something is wrong with your path!  

	if (ftyp & FILE_ATTRIBUTE_DIRECTORY)
		return true;   // this is a directory!  
	return false;    // this is not a directory!  
}
void run(int action, int people, int frame);
int main()
{
	for (int aciton = 7; aciton <= 7; aciton++)
	{
		for (int people = 1; people <= 1; people++)
		{
			int start;
			cin >> start;
			for (int frame = start; frame <= 299; frame++)
			{
				run(aciton, people, frame);
			}
		}
	}
	getchar();
	return 0;
}


void run(int action, int people, int frame)
{	
	char inputPath[128];
	sprintf_s(inputPath, "E:/laboratory/dataset/synthesisdata/bvhtransformdepthacquistion/action%d/people%d/output%d.txt",action,people,frame);
	//sprintf_s(inputPath, "E:/laboratory/dataset/realdata/model%d/action%d/%04dpointscloud.txt", model, action, frame);
	
	//sprintf_s(inputPath, "E:/laboratory/RecordProcess/RecordProcess/output/model%d/action%d/%04dpointscloud.txt", model, action, frame);
	
	//sprintf_s(inputPath, "C:\\Users\\jwh\\Desktop\\test\\output0.txt");

	ifstream isexist(inputPath);
	if (!isexist.is_open())return;//��ǰ������
	cout << inputPath << endl;
	
	char outputPath[128];
	sprintf_s(outputPath, "E:/laboratory/dataset/synthesisdata/mypartresults/action%d/people%d/newframe%d", action, people, frame);
	//sprintf_s(outputPath, "E:/laboratory/dataset/realdataresults/model%d/action%d/frame%d", model, action, frame);
	//sprintf_s(outputPath, "E:/laboratory/RecordProcess/RecordProcess/output/model%d/action%d/frame%d", model, action, frame);


	string prefix = outputPath;
	if (!dirExists(outputPath))////·�������ڴ����ļ���  Ӧ�÷�װһ�¡�����
	{
		if (_mkdir(outputPath) == 0)
		{
			sprintf_s(outputPath, "%s/1", prefix);
			_mkdir(outputPath);
			sprintf_s(outputPath, "%s/2", prefix);
			_mkdir(outputPath);
			sprintf_s(outputPath, "%s/2", prefix);
			_mkdir(outputPath);
			sprintf_s(outputPath, "%s/4", prefix);
			_mkdir(outputPath);
			sprintf_s(outputPath, "%s/5", prefix);
			_mkdir(outputPath);
			cout << "success to mkdir" << endl;
		}
		else
		{
			cout << "fail to mkdir!!!!" << endl;
			getchar();
		}
	}
	sprintf_s(outputPath, "E:/laboratory/dataset/synthesisdata/mypartresults/action%d/people%d/newframe%d", action, people, frame);
	//sprintf_s(outputPath, "E:/laboratory/dataset/realdataresults/model%d/action%d/frame%d", model, action, frame);
	//sprintf_s(outputPath, "E:/laboratory/RecordProcess/RecordProcess/output/model%d/action%d/frame%d", model, action, frame);
	

	char tempPath[128];
	DWORD dwLast;
	DWORD dwCurrent;
	Mat  slicResult;//��ų����ش�����ͼ �����ӵ�
	Mat  slicBody;//��ʾ���峬���ص�
	Mat  slicseeds;//�������泬�������ӵ�
	SLIC slic;
	slic.outputPath = outputPath;
	slic.HEIGHT = HEIGHT;
	slic.WIDTH = WIDTH;
	Mat photo(HEIGHT, WIDTH, CV_8UC3);
	Mat depthData(HEIGHT, WIDTH, CV_64FC3);

	const int downSampling = 1;//downsampling=1
	const int clusterNumber = 4; //�������ھ������
	CvPoint2D32f gujiadian[5][clusterNumber];//�洢,�����ֺ����ҽż�ͷ��������������ĵ�3
	int gujiadian2[5][clusterNumber];
	ImageShow ShowTool;
	int CenterX, CenterY;
	vector<Data> src;
	vector<Data> originalsrc;//������ʼ��������
	Loading Load;
	clock_t clock_begin, clock_end;


	//�����ܵ������ݶ�������src��
	Load.DownLoad_Info(inputPath, src, downSampling);
	ShowTool.ShowPhoto(photo, src, depthData);//����������ݵ�depthData     ��ʾͼƬ
	originalsrc = src;
	//�����طָ�
	
	
	clock_begin = clock();//�������Ե�ʱ����Ҫ�ѳ�����д�ļ���ʱ��ȥ��
	
	slic.GenerateSuperpixels(photo, NUM_SUPERPIXEL, depthData);
	if (photo.channels() == 3)
		slicResult = slic.GetImgWithContours(cv::Scalar(0, 0, 255), slicBody);
	else
		slicResult = slic.GetImgWithContours(cv::Scalar(128), slicBody);
	

	clock_end = clock();
	printf("�����ش���time elapsed: %f (ms), for img size: %dx%d\n", (float)(clock_end - clock_begin) / CLOCKS_PER_SEC * 1000, slicResult.rows, slicResult.cols);

	cv::imshow("slic", slicResult);
	waitKey(1);
	slicseeds = slicBody;

	sprintf_s(tempPath, "%s/slicResult.jpg", outputPath);
	imwrite(tempPath, slicResult);	

	//����src����  ��ʱsrc�е������ǳ����ش����ĵ���
	src.clear();
	sprintf_s(tempPath, "%s/slic_body.txt", outputPath);
	Load.Load_Info(tempPath, src, 1);//downsampling=1
	double step = sqrt((WIDTH*HEIGHT) / NUM_SUPERPIXEL);//�����ز��� ����Ѱ�����ĵ�	
	Load.Generate_Center_Point(src, CenterX, CenterY, (int)step, slicBody);//�����ص����ĵ���п��ܲ���slic_body.txt��  Ҫ����!!!!!	

	//ShowTool.ShowPoint(slicBody, CenterX, CenterY, 0, 0, 255);//����ʾһ�����ĵ�λ��
	circle(slicBody, Point(CenterX, CenterY), 3, Scalar(0, 0, 255), -1);
		
	
	Dijkstra dijkstra(src.size(), CenterX, CenterY, 1, (int)step);
	dijkstra.Make_Matrix(src);

	int v0 = Load.Search(CenterX, CenterY, src);	//����о�û��Ҫ���Ż� ��ΪslicBody��϶���


	clock_begin = clock();

	//��ʹ��ʦ�־ɷ������� 
	int dingdian = v0;
	for (int i = 1; i <= 5; i++)
	{
		dijkstra.MakeDJ(v0, true, src);
		dijkstra.Vertex[i][0] = dijkstra.Xx;
		dijkstra.Vertex[i][1] = dijkstra.Yy;
		dingdian = Load.Search(dijkstra.Vertex[i][0], dijkstra.Vertex[i][1], src);		

		circle(slicBody, Point(dijkstra.Xx, dijkstra.Yy), 4, Scalar(0, 0, 255), -1);//���ӵ���ʾ	
		imshow("show", slicBody);
	}
	/*cvNamedWindow("show");
	IplImage iplimg = IplImage(slicBody);
	cvSetMouseCallback("show", mouseEvent, (void*)&iplimg);
	cvShowImage("show", &iplimg);
	cvWaitKey(1);*/


	clock_end = clock();
	printf("��֫�˵� elapsed: %f (ms), for img size: %dx%d\n", (float)(clock_end - clock_begin) / CLOCKS_PER_SEC * 1000, slicResult.rows, slicResult.cols);


	sprintf_s(tempPath, "%s/֫�˵�.jpg", outputPath);
	imwrite(tempPath, slicBody);

	

	sprintf_s(tempPath, "%s/zhiduandian.txt", outputPath);
	ofstream of(tempPath);
	for (int jishu = 1; jishu < 6; jishu++)
	{
		of << dijkstra.Vertex[jishu][0] << " " << dijkstra.Vertex[jishu][1] << endl;
	}
	of.close();

	



	dijkstra.Make_Matrix(src);
	//	train.Adjust();  ʦ�ֵĳ�����ע����
	Cluster CL;
	CL.outputPath = outputPath;
	CL.Chek(dijkstra.Vertex);

	CL.pixelBelongToSeedX = slic.pixelBelongToSeedsX;
	CL.pixelBelongToSeedY = slic.pixelBelongToSeedsY;
	CL.label = slic.label;
	
	




	clock_begin = clock();

	for (int j = 0; j < 6; j++)
	{
		dijkstra.MakeDJ(Load.Search(dijkstra.Vertex[j][0], dijkstra.Vertex[j][1], src), false, src);
		dijkstra.Reflash(j);
	}
	
	dijkstra._Check(dijkstra.Vertex);
	//cout << dijkstra.lefthand << " " << dijkstra.leftfoot << " " << dijkstra.righthand << " " << dijkstra.rightfoot << endl;

	//CL.Classify(dijkstra.Dist,dijkstra.count,src);/////Ҳ���õ���dijkstra�Ĳ�ؾ���
	CL.DisClassify(dijkstra.Dist, dijkstra.count, src, v0);


	clock_end = clock();
	printf("�ֲ�λ elapsed: %f (ms), for img size: %dx%d\n", (float)(clock_end - clock_begin) / CLOCKS_PER_SEC * 1000, slicResult.rows, slicResult.cols);




	vector<CvPoint2D32f> tuples[6];

	//��ʾ��Щ���ʱ�� class .txt ������cluster.cpp��д���ʱ����app��ʽд��� 
	
	ShowTool.ShowPhoto(photo, src);
	sprintf_s(tempPath, "%s/Class0.txt", outputPath);
	ShowTool.ShowCluster(photo, 255, 255, 0, tempPath, tuples[0]);
	sprintf_s(tempPath, "%s/Class1.txt", outputPath);
	ShowTool.ShowCluster(photo, 0, 255, 0, tempPath, tuples[1]);
	sprintf_s(tempPath, "%s/Class2.txt", outputPath);
	ShowTool.ShowCluster(photo, 255, 0, 0, tempPath, tuples[2]);
	sprintf_s(tempPath, "%s/Class3.txt", outputPath);
	ShowTool.ShowCluster(photo, 0, 0, 255, tempPath, tuples[3]);
	sprintf_s(tempPath, "%s/Class4.txt", outputPath);
	ShowTool.ShowCluster(photo, 255, 0, 255, tempPath, tuples[4]);
	sprintf_s(tempPath, "%s/Class5.txt", outputPath);
	ShowTool.ShowCluster(photo, 0, 255, 255, tempPath, tuples[5]);
	sprintf_s(tempPath, "%s/13.jpg", outputPath);
	imwrite(tempPath, photo);

	Kmeans k(clusterNumber, downSampling);
	k.outputPath = outputPath;
	clock_begin = clock();
	for (int nn = 1; nn < 6; nn++)//tuples1234Ϊ��֫5Ϊͷ��
	{
		//k.K_solveSuperPixel(tuples[nn], nn, gujiadian[nn - 1], clusterNumber, true);///////kmeans����
		k.K_solveSuperPixel4(tuples[nn], nn, gujiadian[nn - 1], clusterNumber, true);///////kmeans����

	}
	clock_end = clock();
	printf("��λ����: %f (ms), for img size: %dx%d\n", (float)(clock_end - clock_begin) / CLOCKS_PER_SEC * 1000, slicResult.rows, slicResult.cols);

	for (int kkk = 0; kkk < 5; kkk++)
	{
		for (int lll = 0; lll < clusterNumber; lll++)
		{//gujiadian2���������������src�������� ���ܲ���src����������������ĵ�ı�Ŵ���
			//gujiadian2[kkk][lll] = Load.Search(gujiadian[kkk][lll].x, gujiadian[kkk][lll].y, src);//����Ӧ����ԭʼ���� ��Ϊ����㲻һ�����ڳ����ؽ����			
			gujiadian2[kkk][lll] = Load.SearchInSlic(gujiadian[kkk][lll].x, gujiadian[kkk][lll].y, slicseeds);
			circle(photo, Point(gujiadian[kkk][lll].x, gujiadian[kkk][lll].y), 5, Scalar(0, 0, 0), -1);
		    imshow("show", photo);
			waitKey(1);
		}
		cout << endl;
	}
	sprintf_s(tempPath, "%s/14.jpg", outputPath);
	imwrite(tempPath, photo);
	

	CvPoint2D32f Skeleton_point[5*clusterNumber+1];
	Skeleton_point[0].x = CenterX;
	Skeleton_point[0].y = CenterY;

	Skeleton abc;
	int vvv = Load.Search(dijkstra.Vertex[0][0], dijkstra.Vertex[0][1], src);
	 
//	abc.ProcessingSkeleton(photo, dijkstra.Dist, gujiadian, gujiadian2, dijkstra.Vertex, vvv, dijkstra.lefthand, dijkstra.leftfoot, dijkstra.righthand, dijkstra.rightfoot, Skeleton_point, clusterNumber);//$$$$$$$$$$$$$$$

	abc.ProcessingSkeleton4(photo, dijkstra.Dist, gujiadian, gujiadian2, dijkstra.Vertex, vvv, dijkstra.lefthand, dijkstra.leftfoot, dijkstra.righthand, dijkstra.rightfoot, Skeleton_point, clusterNumber);//$$$$$$$$$$$$$$$


	clock_end = clock();
	printf("time elapsed: %f (ms), for img size: %dx%d\n", (float)(clock_end - clock_begin) / CLOCKS_PER_SEC * 1000, slicResult.rows, slicResult.cols);




	//�������ȡ��21���㰴������˳������ļ���
	//�������ĵ㣬ͷ����ͼ���󲿼磬���ӣ�ͼ���Ҳ��磬ͼ�����֣�ͼ���Ҳ��֣�ͼ�����ȣ�ͼ���Ҳ���
	sprintf_s(tempPath, "%s/featurePoints.txt", outputPath);
	ofstream Cwriter(tempPath);//��ž��������������
	 
	for (int jishu = 0; jishu < 5*clusterNumber+1; jishu++)
	{
		Cwriter << Skeleton_point[jishu].x << " " << Skeleton_point[jishu].y << endl;
		cout << Skeleton_point[jishu].x << " " << Skeleton_point[jishu].y << endl;

		circle(slicResult, Point(Skeleton_point[jishu].x, Skeleton_point[jishu].y), 5, Scalar(255, 0, 0), -1);
		char str[10];//////////////////////////////////////////////////////////////////////////////////////////////////���Դ�ɾ��
		sprintf_s(str, "%d", jishu);
		putText(slicResult, str, Point(Skeleton_point[jishu].x, Skeleton_point[jishu].y), CV_FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 255));
	}
	 
	Cwriter.close();
	sprintf_s(tempPath, "%s/000.jpg", outputPath);
	imwrite(tempPath, slicResult);
	
}