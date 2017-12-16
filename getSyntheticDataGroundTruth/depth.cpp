#include <Windows.h>
#include <osgViewer/Viewer>
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>
#include <osgGA/TrackballManipulator>
#include <osg/Image>
#include <fstream>
#include<string>
#include <map>

using namespace std;

int _winWidth = 640;
int _winHeight = 480;

static int index = 0;
static int frameIndex = 0;//��ʾ��ʼ֡ ֡��
static int frameCount = 49;//��ʾ����֡֡��
static int action = 3; ///��ʾ�ڼ�����������
static int people = 1;//��ʾ�ڼ�����

string pathPrefix = "E:/laboratory/dataset/synthesisdata/bvhtransformdepthacquistion/";
bool __bReplacedNode = false;
bool __bStartRepalce = false;
map<string, int> joint;
string joints[15] = { "hip", "chest", "neck", "lShldr", "lForeArm", "lHand", "rShldr", "rForeArm", "rHand","lThigh","lShin","lFoot","rThigh","rShin","rFoot" };

void initData()
{
	joint["hip"] = 1;
	joint["chest"] = 1;
	joint["neck"] = 1;
	joint["lShldr"] = 1;
	joint["lForeArm"] = 1;
	joint["lHand"] = 1;
	joint["rShldr"] = 1;
	joint["rForeArm"] = 1;
	joint["rHand"] = 1;
	joint["lThigh"] = 1;
	joint["lShin"] = 1;
	joint["lFoot"] = 1;
	joint["rThigh"] = 1;
	joint["rShin"] = 1;
	joint["rFoot"] = 1;
}

osg::Vec3Array* vectices;

class DepthHld: public osgGA::GUIEventHandler
{
public:
	DepthHld()
	{
	}
	~DepthHld()
	{
	}
	bool handle( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa ) 
	{
		osgViewer::Viewer* viewer = dynamic_cast<osgViewer::Viewer*>( &aa ); 
		if (!viewer) return false; 

		switch(ea.getEventType()) 
		{ 
		case(osgGA::GUIEventAdapter::KEYDOWN): 
			{
				if(ea.getKey() == 'a')
				{ 
					viewer->setCameraManipulator(NULL);
					viewer->getCamera()->setViewMatrixAsLookAt(osg::Vec3(-0.00138688, -1.67498, 0.358249),osg::Vec3(-0.00138688, 10.674977, 0.358249),osg::Vec3(0,0,1));
					return true;
				}
				if(ea.getKey() == 'z')
				{ 
					viewer->setCameraManipulator(new osgGA::TrackballManipulator);
					return true;
				}
				if(ea.getKey() == 'p')
				{
					{
						/*********************RGBͼƬ*********************************/
						int x, y, width, height;

						x = (int) viewer->getCamera()->getViewport ()->x() ;// + 150;   
						y = (int) viewer->getCamera()->getViewport()->y()  ;// + 100;
						width = (int) viewer->getCamera()->getViewport()->width();// - 300; 
						height = (int) viewer->getCamera()->getViewport()->height();// - 100;

						std::ostringstream outputName2;
						osg::Image * imageBuffer = new osg::Image();
						imageBuffer->readPixels(x, y, width, height,GL_RGBA,GL_UNSIGNED_BYTE); 
						osgDB::writeImageFile(*imageBuffer,"f:\\test.png");
						/****************************************************/
					}

					{
						//std::cout<<ostr.str()<<std::endl;
						osg::Vec3 vPosEye, vCenter, vUp;
						viewer->getCamera()->getViewMatrixAsLookAt( vPosEye, vCenter, vUp);//��ȡ��������ϵ�µ��ӵ�ֵ
						std::cout<<"eye:"<<vPosEye.x()<<" "<<vPosEye.y()<<" "<<vPosEye.z()<<std::endl;
						std::cout<<"center:"<<vCenter.x()<<" "<<vCenter.y()<<" "<<vCenter.z()<<std::endl;
						std::cout<<"up:"<<vUp.x()<<" "<<vUp.y()<<" "<<vUp.z()<<std::endl;

						//Matrix to change world coordinates in windows coordinates
						double x=0,y=0,z=0;

						osg::Matrix modelView = viewer->getCamera()->getViewMatrix();
						osg::Matrix projection = viewer->getCamera()->getProjectionMatrix();
						osg::Matrix window = viewer->getCamera()->getViewport()->computeWindowMatrix();
						osg::Matrix MVPW = modelView * projection * window;//window * projection * modelView;

						double **depth;

						depth = new double *[_winHeight];
						for (int i=0;i<_winHeight;i++)
						{
							depth[i] = new double [_winWidth];
							memset(depth[i],0,_winWidth*sizeof(double));
						}
						//memset(depth,0,_winWidth*_winHeight*sizeof(double));
						//memset(vectZ,0,_winWidth*_winHeight*sizeof(double));

						double maxZ,minZ;
						maxZ = 0;minZ = 10000000;

						for (int i=0;i<vectices->size();i++)
						{
							osg::Vec3 xyz = (vectices->at(i)) * MVPW;

							int sx = (int)xyz.x();
							int sy = _winHeight - (int)xyz.y();
							if (sx<=_winWidth && sy<=_winHeight
								&& sx>=1 && sy>=1)
							{
								if (depth[sy-1][sx-1]==0 || depth[sy-1][sx-1] > xyz.z())
								{
									depth[sy-1][sx-1] = xyz.z();
									//vectZ[(int)xyz.y()][(int)xyz.x()] = vectices->at(i).z();

									if (maxZ<xyz.z())
									{
										maxZ = xyz.z();
									}

									if (minZ>xyz.z())
									{
										minZ = xyz.z();
									}
								}
							}
						}

						std::cout<<"minZ:"<<minZ<<"  maxZ:"<<maxZ<<std::endl;

						std::ofstream output("f:\\test.txt");
						for (int i=0;i<_winHeight;i++)
						{
							for (int j=0;j<_winWidth;j++)
							{
								output<<depth[i][j]<<" ";
							}
							output<<std::endl;
						}
						for (int i=0;i<_winHeight;i++)
						{
							delete depth[i];
						}
						delete depth;

					}

				}
				if(ea.getKey() == 's')/////��ȡȫ����
				{ 
					//���ļ�
					std::ostringstream ostr;
					//ostr<<"E:/TrainData/90002/output";

					ostr<<"C:/Users/jwh/Desktop/test/output";
					ostr<<index++;
					ostr<<".txt";
					std::ofstream output(ostr.str());
					std::cout<<ostr.str()<<std::endl;
					/*
					eye���������λ��(Ҳ����˵�۲��ߵ�λ��)
					center���������viewer�е�ģ�͵�λ�ã�Ҳ����˵�ǹ۲쵽�ĵ��λ�ã�
					up����ֱ������������ϵķ���������ͨ��˵�۲���ͷ���ķ���
					���磺up������ֵΪ��0,0,1�����Կ���up.z()=1����ֱ������������ϣ�Ҳ����˵����Ǵ�Y��ĸ�������Y���������۲졣
					ע��osg������ϵΪ��������ϵ x������ y����ǰ z������
					
					*/
					osg::Vec3 vPosEye, vCenter, vUp;
					viewer->getCamera()->getViewMatrixAsLookAt( vPosEye, vCenter, vUp);//��ȡ��������ϵ�µ��ӵ�ֵ
					std::cout<<"eye:"<<vPosEye.x()<<" "<<vPosEye.y()<<" "<<vPosEye.z()<<std::endl;
					std::cout<<"center:"<<vCenter.x()<<" "<<vCenter.y()<<" "<<vCenter.z()<<std::endl;
					std::cout<<"up:"<<vUp.x()<<" "<<vUp.y()<<" "<<vUp.z()<<std::endl;

					float lamda = 1;
					float maxX=0,minX=100000;
					float maxY=0,minY=100000;
					float maxZ=0,minZ=100000;
					//ѭ��������������
					static int ii=0;
					for (int x=0;x<_winWidth;x++)
					{
						for (int y=_winHeight;y>0;y--)
						{
							float cloudX,cloudY,cloudZ;

							osg::ref_ptr< osgUtil::LineSegmentIntersector > picker = new osgUtil::LineSegmentIntersector(
								//osgUtil::Intersector::WINDOW, ea.getX(), ea.getY());
								osgUtil::Intersector::WINDOW, x, y);

							osgUtil::IntersectionVisitor iv( picker.get());
							viewer->getCamera()->accept(iv);//(��������±���)

							if (picker->containsIntersections())
							{  //��ȡ��ǰ�Ľ��㡣

								osg::Vec3 ptWorldIntersectPointFirst= picker->getFirstIntersection().getWorldIntersectPoint();

								cloudX=lamda*(ptWorldIntersectPointFirst.x()-vPosEye.x());								
								cloudZ=lamda*(ptWorldIntersectPointFirst.y()-vPosEye.y());
								cloudY=lamda*(ptWorldIntersectPointFirst.z()-vPosEye.z());

								output
									<<x<<" "
									<<_winHeight - y<<" "
									<<cloudX<<" "
									<<cloudY<<" "
									<<cloudZ<<std::endl;
								if (ii++==0)
								{
									std::cout<<"x:"<<x<<" y:"<<y<<" z1:"<<std::endl;
									std::cout<<"x1:"<<ptWorldIntersectPointFirst.x()<<" y1:"<<ptWorldIntersectPointFirst.y()<<" z1:"<<ptWorldIntersectPointFirst.z()<<std::endl;
								}

								//std::cout<<x<<" "<<y<<"("<< ptWorldIntersectPointFirst.x()<<","
								//	<< ptWorldIntersectPointFirst.y()<< ","<< ptWorldIntersectPointFirst.z()<<")"<< std::endl;

								maxX = max(maxX,cloudX);
								minX = min(minX,cloudX);

								maxY = max(maxY,cloudY);
								minY = min(minY,cloudY);

								maxZ = max(maxZ,cloudZ);
								minZ = min(minZ,cloudZ);

							}else
							{

							}

						}

					}
					std::cout<<"X:"<<minX<<" "<<maxX<<std::endl;
					std::cout<<"Y:"<<minY<<" "<<maxY<<std::endl;
					std::cout<<"Z:"<<minZ<<" "<<maxZ<<std::endl;

					output.close();

				return true;
				}
				if(ea.getKey() == 'o')
				{ 
					osg::Image *image = new osg::Image();
					//image->allocateImage(_winWidth, _winHeight, 1, GL_LUMINANCE, GL_FLOAT);
					image->readPixels(0, 0, _winWidth, _winHeight, GL_DEPTH_COMPONENT, GL_FLOAT);
					
					osgDB::writeImageFile(*image,"f:/test2.bmp");	//������ͼ

					float *bufferZ = reinterpret_cast<float*>(image->data()); 

					std::ofstream output("f:\\test3.txt");
					for (int i=0;i<_winHeight;i++)
					{
						for (int j=0;j<_winWidth;j++)
						{
							output<<*(bufferZ++)<<" ";
						}
						output<<std::endl;
					}

					return true;
				}
				
				if(ea.getKey() == 'd')//BVH��ȡ��
				{ 
					cout<<"key down:d"<<endl;
					//Matrix to change world coordinates in windows coordinates
					double x=0,y=0,z=0;


					osg::Matrix modelView = viewer->getCamera()->getViewMatrix();
					osg::Matrix projection = viewer->getCamera()->getProjectionMatrix();
					osg::Matrix window = viewer->getCamera()->getViewport()->computeWindowMatrix();
					osg::Matrix MVPW = modelView * projection * window;//window * projection * modelView;

					osg::Vec3 vPosEye, vCenter, vUp;
					viewer->getCamera()->getViewMatrixAsLookAt( vPosEye, vCenter, vUp);//��ȡ��������ϵ�µ��ӵ�ֵ
					
					
					std::ifstream inputFile("C:/Users/jwh/Desktop/test/0.txt");


					string name;
					
					//int index[15] = {0,2,3,10,11,12,34,35,36,57,58,59,62,63,64};
					//int index[15] = {6,8,9,16,17,18,40,41,42,63,64,65,68,69,70};////////������ʦ�ֵ�poser pro 2012 ����bvh����73������
					/*
					 2 hip		 4 chest        5 neck   
					 6head		 7leye			9reye
					 12 lshldr 	 13 lForeArm    14 lHand
					 36 rShldr	 37 rForeArm	38 rHand
					 59 lThigh	 60 lShin		61 lFoot
					 64 rThigh	 65 rShin       66 rFoot					
					*/
					int index[15] = {2,4,5,12,13,14,36,37,38,59,60,61,64,65,66};//////////jwh��ӵ� bvh���68������ 
					int indexnew[] = {2,4,5,6,7,9,12,13,14,36,37,38,59,60,61,64,65,66};///jwh �����head6  7leye 9reye 
					
					//for (int i=0;i<73;i++) /// ������ʦ��֮ǰ�� 
					for(int i=0;i<69;i++)
					{
						//inputFile>>name>>maohao>>x>>z>>y;//////////////////
						//x/=100;
						//y/=-100;
						//z/=100;						

						inputFile>>x>>z>>y;
						y=0-y;

						osg::Vec3 xyz = (osg::Vec3(x,y,z)) * MVPW;

						std::cout<<i<<"xyz:"<<x<<" "<<y<<" "<<z<<std::endl;
						std::cout<<i<<"XYZ:"<<xyz.x()<<" "<<_winHeight - xyz.y()<<" "<<xyz.z()<<std::endl;
						for(int j=0;j<15;j++)//------
						{
							if(index[j]==i)
							{
								//std::ofstream output("E:/TrainData/90002/PoserGUJIADIAN.txt",ios::app); ʦ�ֵ�·�� ����һ����·��
								std::ofstream output("C:/Users/jwh/Desktop/test/PoserGUJIADIAN.txt",ios::app); 

								std::cout<<i<<"xyz:"<<x<<" "<<y<<" "<<z<<std::endl;
								output<<int(xyz.x())<<" "<<int(_winHeight - xyz.y())<<" "<<int(xyz.z())<<std::endl;
								output.close();
							}
						}
						/////jwh ��ӵ� ��δ�Ż�
						for(int j=0;j<18;j++)//------
						{
							if(indexnew[j]==i)
							{
								//std::ofstream output("E:/TrainData/90002/PoserGUJIADIAN.txt",ios::app); ʦ�ֵ�·�� ����һ����·��
								std::ofstream output("C:/Users/jwh/Desktop/test/PoserGUJIADIAN2.txt",ios::app); 

								std::cout<<i<<"xyz:"<<x<<" "<<y<<" "<<z<<std::endl;
								output<<int(xyz.x())<<" "<<int(_winHeight - xyz.y())<<" "<<int(xyz.z())<<std::endl;
								output.close();
							}
						}
					}					
					return true;
				}
				if (ea.getKey() == 't')
				{
					__bStartRepalce = true;
					cout<<"tttttttt pressed"<<endl;
					return true;
				}
			}
			break;
		case(osgGA::GUIEventAdapter::DOUBLECLICK):
			{
				osg::ref_ptr< osgUtil::LineSegmentIntersector > picker = new osgUtil::LineSegmentIntersector(
					osgUtil::Intersector::WINDOW, ea.getX(), ea.getY());
				osgUtil::IntersectionVisitor iv( picker.get());
				viewer->getCamera()->accept(iv);//(��������±���)

				if (picker->containsIntersections())

				{  //��ȡ��ǰ�Ľ��㡣
					osg::Vec3 ptWorldIntersectPointFirst= picker->getFirstIntersection().getWorldIntersectPoint();

					std::cout<<ea.getX()<<" "<<ea.getY()<<std::endl;
					std::cout<<"world coords vertex("<< ptWorldIntersectPointFirst.x()<<","
						<< ptWorldIntersectPointFirst.y()<< ","<< ptWorldIntersectPointFirst.z()<<")"<< std::endl;
				}
			}
			break;
		case (osgGA::GUIEventAdapter::CLOSE_WINDOW):
			{
				cout<<"close window"<<endl;
			}
			break;
		default: /////jwh��ӵ�  ����һ��
			{
				cout<<"default"<<endl;
			}
			return false; 
		}
	}
};

class ObjCallBack :public osg::NodeCallback
{
public:
	ObjCallBack()
		:_group(NULL)
	{

	}
	ObjCallBack(osg::Group *r)
		:_group(r)
	{

	}
	virtual void operator()(osg::Node* node, osg::NodeVisitor* nv)
	{
		cout<<"run call back function"<<endl;

		if (__bStartRepalce && !__bReplacedNode)
		{
			_group->removeChild((unsigned int)0);
			
			char objName[256];

			while((frameIndex <= frameCount) && !__bReplacedNode)
			{			
				sprintf(objName,"E:/laboratory/dataset/synthesisdata/bvhobjposer/action%d/people%d/Untitled_%d.obj",action,people,frameIndex);
				std::ifstream inputFile(objName);
				if (!inputFile.is_open())
				{
					cout << "���ļ�ʧ��" << objName<< endl;
					sprintf(objName, "E:/laboratory/dataset/synthesisdata/bvhobjposer/action%d/people%d/Untitled_0_%d.obj", action, people, frameIndex);
				}
				osg::ref_ptr<osg::Node> node = osgDB::readNodeFile(objName);
				_group->addChild(node);

				cout << objName << endl;

				__bReplacedNode = true;				
			}
		}
		//��������
		traverse(node, nv);		
	}
public:
	osg::Group *_group;

};

class CaptureCallback : public osg::Camera::DrawCallback
{
public:
	osg::ref_ptr<osg::Camera>	_camera;
	osg::ref_ptr<osg::Image>	_depthbufferimg;
	osg::ref_ptr<osg::Image>	_colorbufferimg;
	
public:
	CaptureCallback(osg::Camera *camera,
		osg::Image* depthbufferimg,
		osg::Image* colorbufferimg
		)
		:_camera(camera),
		_depthbufferimg(depthbufferimg),
		_colorbufferimg(colorbufferimg)
	{

	}

	~CaptureCallback()
	{

	}
	void ComputeDepth() const
	{

		osg::View *viewer = _camera->getView();
	
		std::ostringstream ostr;				
					
		ostr<<"E:/laboratory/dataset/synthesisdata/bvhtransformdepthacquistion/action";
		ostr<<action<<"/people"<<people<<"/output"<<frameIndex<<".txt";


		std::ofstream output(ostr.str());
		std::cout<<ostr.str()<<std::endl;
					
		osg::Vec3 vPosEye, vCenter, vUp;
		viewer->getCamera()->getViewMatrixAsLookAt( vPosEye, vCenter, vUp);//��ȡ��������ϵ�µ��ӵ�ֵ
		std::cout<<"eye:"<<vPosEye.x()<<" "<<vPosEye.y()<<" "<<vPosEye.z()<<std::endl;
		std::cout<<"center:"<<vCenter.x()<<" "<<vCenter.y()<<" "<<vCenter.z()<<std::endl;
		std::cout<<"up:"<<vUp.x()<<" "<<vUp.y()<<" "<<vUp.z()<<std::endl;

		float lamda = 1;
		float maxX=0,minX=100000;
		float maxY=0,minY=100000;
		float maxZ=0,minZ=100000;
		//ѭ��������������
		static int ii=0;
		for (int x=0;x<_winWidth;x++)
		{
			for (int y=_winHeight;y>0;y--)
			{
				float cloudX,cloudY,cloudZ;
				osg::ref_ptr< osgUtil::LineSegmentIntersector > picker = new osgUtil::LineSegmentIntersector(
					//osgUtil::Intersector::WINDOW, ea.getX(), ea.getY());
					osgUtil::Intersector::WINDOW, x, y);

				osgUtil::IntersectionVisitor iv( picker.get());
				viewer->getCamera()->accept(iv);//(��������±���)

				if (picker->containsIntersections())
				{  //��ȡ��ǰ�Ľ��㡣

					osg::Vec3 ptWorldIntersectPointFirst= picker->getFirstIntersection().getWorldIntersectPoint();

					cloudX=lamda*(ptWorldIntersectPointFirst.x()-vPosEye.x());								
					cloudZ=lamda*(ptWorldIntersectPointFirst.y()-vPosEye.y());
					cloudY=lamda*(ptWorldIntersectPointFirst.z()-vPosEye.z());

					output
						<<x<<" "
						<<_winHeight - y<<" "
						<<cloudX<<" "
						<<cloudY<<" "
						<<cloudZ<<std::endl;

					if (ii++==0)
					{
						std::cout<<"x:"<<x<<" y:"<<y<<" z1:"<<std::endl;
						std::cout<<"x1:"<<ptWorldIntersectPointFirst.x()<<" y1:"<<ptWorldIntersectPointFirst.y()<<" z1:"<<ptWorldIntersectPointFirst.z()<<std::endl;
					}
							
					maxX = max(maxX,cloudX);
					minX = min(minX,cloudX);
					maxY = max(maxY,cloudY);
					minY = min(minY,cloudY);
					maxZ = max(maxZ,cloudZ);
					minZ = min(minZ,cloudZ);

				}else
				{
				}
			}
		}
		std::cout<<"X:"<<minX<<" "<<maxX<<std::endl;
		std::cout<<"Y:"<<minY<<" "<<maxY<<std::endl;
		std::cout<<"Z:"<<minZ<<" "<<maxZ<<std::endl;

		output.close();		
	}
	void SaveSkeleton()const
	{
		initData();		
		double x = 0, y = 0, z = 0;
		string nodeName;//��Ҫ����ؽڵ�����
		osg::View *viewer = _camera->getView();
		osg::Matrix modelView = viewer->getCamera()->getViewMatrix();
		osg::Matrix projection = viewer->getCamera()->getProjectionMatrix();
		osg::Matrix window = viewer->getCamera()->getViewport()->computeWindowMatrix();
		osg::Matrix MVPW = modelView * projection * window;

		osg::Vec3 vPosEye, vCenter, vUp;
		viewer->getCamera()->getViewMatrixAsLookAt(vPosEye, vCenter, vUp);//��ȡ��������ϵ�µ��ӵ�ֵ
		ostringstream ostr;
		ostr << pathPrefix << "action" << action << "/people" << people << "/" << frameIndex << ".txt";
		std::ifstream inputFile(ostr.str());
		if (!inputFile.is_open())
		{
			cout << "���ļ�ʧ��" << endl;
			return;
		}
		
		/*
		2 hip		 4 chest        5 neck
		6head		 7leye			9reye
		12 lshldr 	 13 lForeArm    14 lHand
		36 rShldr	 37 rForeArm	38 rHand
		59 lThigh	 60 lShin		61 lFoot
		64 rThigh	 65 rShin       66 rFoot
		*/
		char gtPath[128];
		sprintf_s(gtPath, "action%d/people%d/groundTruth%d.txt",action,people,frameIndex);
		std::ofstream output(pathPrefix+gtPath, ios::out);
		
		while (!inputFile.eof())
		{
			inputFile >> x >> z >> y >> nodeName;
			y = 0 - y;
			osg::Vec3 xyz = (osg::Vec3(x, y, z)) * MVPW;

			if (joint.count(nodeName))//�ҵ��ؽڵ�
			{
				std::cout <<nodeName << "xyz:" << x << " " << y << " " << z << std::endl;
				output << int(xyz.x()) << " " << int(_winHeight - xyz.y()) << " " << int(xyz.z()) << " " << nodeName << std::endl;
			}
		}		
		output.close();
	}
	virtual void operator () (osg::RenderInfo& renderInfo) const
	{
		static bool bDrawSwith = false;
		if (__bReplacedNode)
		{
			if (bDrawSwith)
			{
				ComputeDepth();
				SaveSkeleton();
				__bReplacedNode = false;
				frameIndex++;
				if (frameIndex == frameCount+1)//����һ���� ������һ������
				{
					frameIndex = 0;
					people++;
					if (people == 5)
					{
						people = 1;
						//action++;
						action = 67;
					}
				}
			}
			bDrawSwith = !bDrawSwith;
		}
		std::cout<<"capture  draw call back"<<std::endl;
	}
};

int main(int argc,char* argv[])
{

	osg::ref_ptr<osg::Group> root = new osg::Group;
	osg::ref_ptr<osg::Node> node;
	char buf[256];
	sprintf(buf, "C:/Users/jwh/Desktop/Untitled_0_0.obj",action,people,frameIndex);	
		
	node = osgDB::readNodeFile(buf);
	root->addChild(node);

	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer;
	viewer->setUpViewInWindow(100,100,_winWidth,_winHeight);
	viewer->setCameraManipulator(new osgGA::TrackballManipulator);
	viewer->addEventHandler(new DepthHld());
	
	viewer->setSceneData(root);

	osg::ref_ptr<osg::Camera> camera = viewer->getCamera();	
	osg::ref_ptr<osg::Image> depthbufferimg = new osg::Image;
	depthbufferimg->allocateImage(_winWidth, _winHeight, 1, GL_DEPTH_COMPONENT, GL_FLOAT);
	osg::ref_ptr<osg::Image> colorbufferimg = new osg::Image;
	colorbufferimg->allocateImage(_winWidth, _winHeight, 1, GL_RGBA, GL_UNSIGNED_BYTE);

	//-------------------call back----------------------
	cout<<"before  callback "<<endl;
	root->setUpdateCallback(new ObjCallBack(root));

	camera->setFinalDrawCallback(new CaptureCallback(camera, depthbufferimg, colorbufferimg));//�����viewer�������

	cout<<"after setSceneData "<<endl;

	while(!viewer->done())
	{
		cout<<"viewer"<<endl;
		//__bReplacedNode = false;
		viewer->frame();		
	}	
	
	getchar();
}	