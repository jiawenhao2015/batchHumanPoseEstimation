#include "BvhReader.h"
#include <stdio.h>
#include "BvhData.h"
#include <iostream>
#include <Windows.h>
#include <osg/Matrix>
#include <osgViewer/Viewer>
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>
#include <osgGA/TrackballManipulator>
#include <osg/Image>
#include <osgText/Text>
#include <osg/MatrixTransform>
#include <osg/Point>


using namespace std;
using namespace BVHViewer_DataAccess;
using namespace BVHViewer_DataObjects;

//static int action = 3; ///表示第几个动作序列
//static int people = 1;//表示第几个人
//
//static int frameIndex = 0;
static int frameCount = 199;

string bvhFilePrefix = "E:/laboratory/dataset/synthesisdata/bvhobjposer/";
string objFilePrefix = "E:/laboratory/dataset/synthesisdata/bvhobjposer/";
string tempFilePrefix = "E:/laboratory/dataset/synthesisdata/bvhtransformdepthacquistion/";
void transPosition(osg::Matrix &matrix1)
{
	osg::Matrix matrix2 = matrix1;
	for (int i=0;i<4;i++)
	{
		for (int j=0;j<4;j++)
		{
			matrix1(i,j) = matrix2(j,i);
		}
	}
}

osg::Matrix getTransformMatrix(BvhData &bvhData, BvhHierarchyNode &node, int iFrame)
{
	osg::Matrix mat;
	mat.makeIdentity();

	float *channelData = bvhData.getMotionData() 
		+ iFrame * bvhData.getMotionDataStride() 
		+ node.FirstChannelIndex;

	float xposition,yposition,zposition;
	float xrotation,yrotation,zrotation;
	float xscale,yscale,zscale;

	xposition = yposition = zposition = 0;
	xrotation = yrotation = zrotation = 0;
	xscale = yscale = zscale = 1;

	static int index=0;
	cout<<"--"<<index++<<"--:\t"<<node.NodeName<<":--\t";


	////jwh 添加  找到感兴趣的结点20161117

	//string  temp = node.NodeName;

	//if (temp.compare("hip") == 0)
	//{
	//	cout<<"hip hip  found "<<endl;
	//	
	//}


	osg::Matrix rotateMatrix;

	for (int i=0;i<node.ChannelCount;i++)
	{
		switch ( node.channels[i] )
		{
		case E_BVH_TOKEN_TKW_XPOSITION:
			xposition = channelData[i];
			cout<<"xposition:"<<xposition<<" \t";
			break;
		case E_BVH_TOKEN_TKW_YPOSITION:
			yposition = channelData[i];
			cout<<"yposition:"<<yposition<<" \t";
			break;
		case E_BVH_TOKEN_TKW_ZPOSITION:
			zposition = channelData[i];
			cout<<"zposition:"<<zposition<<" \t";
			break;
		case E_BVH_TOKEN_TKW_XROTATION:
			xrotation = channelData[i];
			rotateMatrix.makeRotate(osg::inDegrees(xrotation),osg::Vec3(1,0,0));
			//osg::Matrix::inverse(rotateMatrix);
			transPosition(rotateMatrix);
			mat.postMult(rotateMatrix);//osg::Quat(osg::inDegrees(xrotation),1,0,0));
			cout<<"xrotation:"<<xrotation<<" \t";
			break;
		case E_BVH_TOKEN_TKW_YROTATION:
			yrotation = channelData[i];
			rotateMatrix.makeRotate(osg::inDegrees(yrotation),osg::Vec3(0,1,0));
			transPosition(rotateMatrix);
			mat.postMult(rotateMatrix);//osg::Quat(osg::inDegrees(yrotation),0,1,0));
			cout<<"yrotation:"<<yrotation<<" \t";
			break;
		case E_BVH_TOKEN_TKW_ZROTATION:
			zrotation = channelData[i];
			rotateMatrix.makeRotate(osg::inDegrees(zrotation),osg::Vec3(0,0,1));
			transPosition(rotateMatrix);
			mat.postMult(rotateMatrix);//osg::Quat(osg::inDegrees(zrotation),0,0,1));
			cout<<"zrotation:"<<zrotation<<" \t";
			break;
		case E_BVH_TOKEN_TKW_XSCALE:
			xscale = channelData[i];
			cout<<"xscale:"<<xscale<<" \t";
			break;
		case E_BVH_TOKEN_TKW_YSCALE:
			yscale = channelData[i];
			cout<<"yscale:"<<yscale<<" \t";
			break;
		case E_BVH_TOKEN_TKW_ZSCALE:
			zscale = channelData[i];
			cout<<"zscale:"<<zscale<<" \t";
			break;
		}
	}
	cout<<endl;
	mat(0,3) = xposition;
	mat(1,3) = yposition;
	mat(2,3) = zposition;
		
	mat.postMultScale(osg::Vec3(xscale,yscale,zscale));

	return mat;
}


int main()
{	
	for (int action = 9; action <= 9;action++)
	{
		for (int people = 1; people <= 1;people++)
		{
			char bvhFileName[256];
			sprintf(bvhFileName, "action%d/people%d/Untitled.bvh", action, people);

			for (int frameIndex = 0; frameIndex <= frameCount; frameIndex++) 
			{
				BvhReader bvhReader;
				BvhData bvhData;

				char objFileName[256];
				sprintf(objFileName, "%saction%d/people%d/Untitled_0_%d.obj", objFilePrefix.c_str(), action, people, frameIndex);//可能有不存在的风险
				cout << objFileName << endl;
				FILE* file;
				if (NULL == (file = fopen(objFileName, "r")))
				{
					cout << "文件不存在。" << objFileName << endl;
					sprintf(objFileName, "%saction%d/people%d/Untitled_%d.obj", objFilePrefix.c_str(), action, people, frameIndex);
					cout << objFileName << endl;
				}

				int iFrame = 0;
				//osg
				osg::ref_ptr<osg::Group> root = new osg::Group;

				bvhReader.read(&bvhData, bvhFilePrefix + bvhFileName);
				vector<BvhHierarchyNode> rootNodes = bvhData.getRootNodes();

				osg::ref_ptr<osg::MatrixTransform> objNode = new osg::MatrixTransform;
				objNode->setMatrix(osg::Matrix::rotate(osg::DegreesToRadians(-90.0), 1, 0, 0));

				objNode->addChild(osgDB::readNodeFile(objFilePrefix + objFileName));
				root->addChild(objNode);
				{
					//coodinate
					osg::ref_ptr<osg::Vec3Array> coords = new osg::Vec3Array;
					osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array;
					osg::ref_ptr<osg::Geometry> geom = new osg::Geometry;
					osg::ref_ptr<osg::Geode> geode = new osg::Geode;

					coords->push_back(osg::Vec3(0, 0, 0));
					coords->push_back(osg::Vec3(1, 0, 0));
					coords->push_back(osg::Vec3(0, 0, 0));
					coords->push_back(osg::Vec3(0, 1, 0));
					coords->push_back(osg::Vec3(0, 0, 0));
					coords->push_back(osg::Vec3(0, 0, 1));

					colors->push_back(osg::Vec4(1, 0, 0, 1));
					colors->push_back(osg::Vec4(1, 0, 0, 1));
					colors->push_back(osg::Vec4(0, 1, 0, 1));
					colors->push_back(osg::Vec4(0, 1, 0, 1));
					colors->push_back(osg::Vec4(0, 0, 1, 1));
					colors->push_back(osg::Vec4(0, 0, 1, 1));

					geom->setVertexArray(coords);
					geom->setColorArray(colors);
					geom->setColorBinding(osg::Geometry::BIND_PER_VERTEX);
					geom->addPrimitiveSet(new  osg::DrawArrays(osg::PrimitiveSet::LINES, 0, coords->size()));
					geode->addDrawable(geom.get());

					root->addChild(geode.get());
				}
				{
					//skeleton
					osg::ref_ptr<osg::Vec3Array> coords = new osg::Vec3Array;
					osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array;
					osg::ref_ptr<osg::Geometry> geom = new osg::Geometry;
					osg::ref_ptr<osg::Geode> geode = new osg::Geode;

					osg::ref_ptr<osg::Vec3Array> coordsPoint = new osg::Vec3Array;
					osg::ref_ptr<osg::Vec4Array> colorsPoint = new osg::Vec4Array;
					osg::ref_ptr<osg::Geometry> geomPoint = new osg::Geometry;
					osg::ref_ptr<osg::Geode> geodePoint = new osg::Geode;


					int maxParentIndex = bvhData.getMaxParentIndex();
					osg::ref_ptr<osg::Vec3Array> nodesPosition
						= new osg::Vec3Array(bvhData.getNodesCount() + 1);
					osg::ref_ptr<osg::Vec3Array> framePosition
						= new osg::Vec3Array(bvhData.getNodesCount() + 1);

					osg::ref_ptr<osg::MatrixfArray> matrixArr
						= new osg::MatrixfArray(bvhData.getNodesCount() + 1);


					char tempFile[256];
					sprintf(tempFile, "action%d/people%d/%d.txt", action, people, frameIndex);//

					std::ofstream output(tempFilePrefix + tempFile, ios::out);///////me
					for (int i = 1; i <= bvhData.getNodesCount(); i++)
					{
						BvhHierarchyNode currNode = bvhData.getNode(i);
						if (currNode.ParentIndex == -1)
						{
							nodesPosition->at(i) = osg::Vec3(0, 0, 0);
							framePosition->at(i) = osg::Vec3(0, 0, 0);
							//framePosition->at(i) = osg::Vec3(1,1,1);
						}

						{
							osg::Matrix transMatrix = getTransformMatrix(bvhData, currNode, frameIndex);///////////important

							transMatrix(0, 3) += currNode.Offset[0];
							transMatrix(1, 3) += currNode.Offset[1];
							transMatrix(2, 3) += currNode.Offset[2];

							osg::Vec3 parentPosition;
							if (currNode.ParentIndex == -1)
							{
								parentPosition = osg::Vec3(0, 0, 0);
							}
							else
							{
								parentPosition = framePosition->at(currNode.ParentIndex);
								transMatrix.preMult(matrixArr->at(currNode.ParentIndex));
							}
							matrixArr->at(i) = transMatrix;

							framePosition->at(i) = osg::Vec3(transMatrix(0, 3), transMatrix(1, 3), transMatrix(2, 3));

							coords->push_back(parentPosition);
							coords->push_back(framePosition->at(i));

							coordsPoint->push_back(framePosition->at(i));

							std::cout << "**" << framePosition->at(i).x() << " " << framePosition->at(i).y() << " " << framePosition->at(i).z() << std::endl;
							//output<<framePosition->at(i).x()<<" "<<framePosition->at(i).y()<<" "<<framePosition->at(i).z()<<std::endl;//---------------------
							output << framePosition->at(i).x() << " "
								<< framePosition->at(i).y() << " "
								<< framePosition->at(i).z() << " "
								<< currNode.NodeName <<
								std::endl;//---------------------


						}

						osg::ref_ptr<osgText::Text> text = new osgText::Text;
						text->setFont("fonts/arial.ttf");
						text->setAxisAlignment(osgText::Text::SCREEN);
						text->setText(currNode.NodeName);
						text->setPosition(framePosition->at(i));
						text->setCharacterSize(0.01);
						geode->addDrawable(text);
					}
					output.close();///

					colors->push_back(osg::Vec4(1.0f, 0.0f, 0.0f, 1.0f));
					colorsPoint->push_back(osg::Vec4(0.0f, 1.0f, 0.0f, 1.0f));

					geom->setVertexArray(coords);
					geom->setColorArray(colors);
					geom->setColorBinding(osg::Geometry::BIND_OVERALL);
					geom->addPrimitiveSet(new  osg::DrawArrays(osg::PrimitiveSet::LINES, 0, coords->size()));
					geode->addDrawable(geom.get());

					geomPoint->setVertexArray(coordsPoint);
					geomPoint->setColorArray(colorsPoint);
					geomPoint->setColorBinding(osg::Geometry::BIND_OVERALL);
					geomPoint->addPrimitiveSet(new  osg::DrawArrays(osg::PrimitiveSet::POINTS, 0, coordsPoint->size()));
					geodePoint->addDrawable(geomPoint.get());
					osg::ref_ptr<osg::Point> ptSize = new osg::Point;
					ptSize->setSize(5.0);
					geodePoint->getOrCreateStateSet()->setAttributeAndModes(ptSize.get(), osg::StateAttribute::ON);

					root->addChild(geode);
					root->addChild(geodePoint);
				}

				////被jwh注释了   调试。。。
				//osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer;
				//viewer->setUpViewInWindow(100,100,640,480);
				//viewer->setCameraManipulator(new osgGA::TrackballManipulator);
				//viewer->setSceneData(root);

				//while(!viewer->done())
				//{
				//	viewer->frame();
				//}
				
			}
		}
	}	
	
	getchar();


	return 0;
}