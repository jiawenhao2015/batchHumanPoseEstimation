//#include "BvhReader.h"
//
//#include "BvhData.h"
//
//#include <iostream>
//#include <Windows.h>
//#include <osg/Matrix>
//#include <osgViewer/Viewer>
//#include <osgDB/ReadFile>
//#include <osgDB/WriteFile>
//#include <osgGA/TrackballManipulator>
//#include <osg/Image>
//#include <osgText/Text>
//#include <osg/MatrixTransform>
//#include <osg/Point>
//#include <fstream>
//
//using namespace std;
//using namespace BVHViewer_DataAccess;
//using namespace BVHViewer_DataObjects;
//
//osg::Matrix getTransformMatrix(BvhData &bvhData, BvhHierarchyNode &node, int iFrame)
//{
//	osg::Matrix mat;
//	mat.makeIdentity();
//
//	float *channelData = bvhData.getMotionData() 
//		+ iFrame * bvhData.getMotionDataStride() 
//		+ node.FirstChannelIndex;
//	
//	////cout<<node.NodeName<<":";
//	////if (node.ChannelCount==3)
//	////{
//	////	cout<<channelData[0]<<" "<<channelData[1]<<" "<<channelData[2]<<" "
//	////		<<endl;
//	////	mat.setTrans(channelData[0], channelData[1], channelData[2]);
//	////}else if (node.ChannelCount==9)
//	////{
//	////	cout<<channelData[0]<<" "<<channelData[1]<<" "<<channelData[2]<<" "
//	////		<<channelData[3]<<" "<<channelData[4]<<" "<<channelData[5]<<" "
//	////		<<channelData[6]<<" "<<channelData[7]<<" "<<channelData[8]<<" "
//	////		<<endl;
//
//	////	//mat.makeScale(channelData[6], channelData[7], channelData[8]);
//	////	//mat.setTrans(channelData[0], channelData[1], channelData[2]);
//	////	mat.makeRotate(
//	////		channelData[5], osg::Vec3d(1,0,0),
//	////		channelData[4], osg::Vec3d(0,1,0),
//	////		channelData[3], osg::Vec3d(0,0,1)
//	////		);
//	////}
//
//
//	float xposition,yposition,zposition;
//	float xrotation,yrotation,zrotation;
//	float xscale,yscale,zscale;
//
//	xposition = yposition = zposition = 0;
//	xrotation = yrotation = zrotation = 0;
//	xscale = yscale = zscale = 1;
//
//	static int index=0;
//	cout<<"--"<<index++<<"--:\t"<<node.NodeName<<":--\t";
//	osg::Matrix rotateMatrix;
//
//
//
//	osg::Matrix m1;
//	osg::Matrix m2;
//
//	rotateMatrix.makeRotate(osg::inDegrees(20.0),osg::Vec3(1,0,0));
//	m1.postMult(rotateMatrix);
//	rotateMatrix.makeRotate(osg::inDegrees(30.0),osg::Vec3(0,1,0));
//	m1.postMult(rotateMatrix);
//	rotateMatrix.makeRotate(osg::inDegrees(40.0),osg::Vec3(0,0,1));
//	m1.postMult(rotateMatrix);
//
//	rotateMatrix.makeRotate(osg::inDegrees(20.0),osg::Vec3(1,0,0));
//	m2.preMult(rotateMatrix);
//	rotateMatrix.makeRotate(osg::inDegrees(30.0),osg::Vec3(0,1,0));
//	m2.preMult(rotateMatrix);
//	rotateMatrix.makeRotate(osg::inDegrees(40.0),osg::Vec3(0,0,1));
//	m2.preMult(rotateMatrix);
//
//
//
//
//	for (int i=0;i<node.ChannelCount;i++)
//	{
//		switch ( node.channels[i] )
//		{
//		case E_BVH_TOKEN_TKW_XPOSITION:
//			xposition = channelData[i];
//			cout<<"xposition:"<<xposition<<" \t";
//			break;
//		case E_BVH_TOKEN_TKW_YPOSITION:
//			yposition = channelData[i];
//			cout<<"yposition:"<<yposition<<" \t";
//			break;
//		case E_BVH_TOKEN_TKW_ZPOSITION:
//			zposition = channelData[i];
//			cout<<"zposition:"<<zposition<<" \t";
//			break;
//		case E_BVH_TOKEN_TKW_XROTATION:
//			xrotation = channelData[i];
//			rotateMatrix.makeRotate(osg::inDegrees(xrotation),osg::Vec3(1,0,0));
//			mat.preMult(rotateMatrix);//osg::Quat(osg::inDegrees(xrotation),1,0,0));
//			cout<<"xrotation:"<<xrotation<<" \t";
//			break;
//		case E_BVH_TOKEN_TKW_YROTATION:
//			yrotation = channelData[i];
//			rotateMatrix.makeRotate(osg::inDegrees(yrotation),osg::Vec3(0,1,0));
//			mat.preMult(rotateMatrix);//osg::Quat(osg::inDegrees(yrotation),0,1,0));
//			cout<<"yrotation:"<<yrotation<<" \t";
//			break;
//		case E_BVH_TOKEN_TKW_ZROTATION:
//			zrotation = channelData[i];
//			rotateMatrix.makeRotate(osg::inDegrees(zrotation),osg::Vec3(0,0,1));
//			mat.preMult(rotateMatrix);//osg::Quat(osg::inDegrees(zrotation),0,0,1));
//			cout<<"zrotation:"<<zrotation<<" \t";
//			break;
//		case E_BVH_TOKEN_TKW_XSCALE:
//			xscale = channelData[i];
//			cout<<"xscale:"<<xscale<<" \t";
//			break;
//		case E_BVH_TOKEN_TKW_YSCALE:
//			yscale = channelData[i];
//			cout<<"yscale:"<<yscale<<" \t";
//			break;
//		case E_BVH_TOKEN_TKW_ZSCALE:
//			zscale = channelData[i];
//			cout<<"zscale:"<<zscale<<" \t";
//			break;
//		}
//	}
//	cout<<endl;
//
//	////mat.makeRotate(
//	////	-xrotation/180.0 * osg::PI, osg::Vec3d(1,0,0),
//	////	-yrotation/180.0 * osg::PI, osg::Vec3d(0,1,0),
//	////	-zrotation/180.0 * osg::PI, osg::Vec3d(0,0,1)
//	////	);
//	mat.preMultTranslate(osg::Vec3(xposition,yposition,zposition));
//	mat.postMultScale(osg::Vec3(xscale,yscale,zscale));
//
//	return mat;
//}
//
//
//int main()
//{
//	BvhReader bvhReader;
//	BvhData bvhData;
//
//	char bvhFileName[] = "C:/Users/Administrator/Desktop/data/Untitled3.bvh";
//	char objFileName[] = "C:/Users/Administrator/Desktop/data/Untitled3.obj";
//	int iFrame = 0;
//
//	//osg
//	osg::ref_ptr<osg::Group> root = new osg::Group;
//
//	bvhReader.read(&bvhData,bvhFileName);
//	vector<BvhHierarchyNode> rootNodes = bvhData.getRootNodes();
//
//	osg::ref_ptr<osg::MatrixTransform> objNode = new osg::MatrixTransform;
//	objNode->setMatrix(osg::Matrix::rotate(osg::DegreesToRadians(-90.0),1,0,0));
//
//	objNode->addChild(osgDB::readNodeFile(objFileName));
//	root->addChild(objNode);
//
//
//	{
//		//coodinate
//		osg::ref_ptr<osg::Vec3Array> coords = new osg::Vec3Array;
//		osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array;
//		osg::ref_ptr<osg::Geometry> geom = new osg::Geometry; 
//		osg::ref_ptr<osg::Geode> geode = new osg::Geode;
//		
//		coords->push_back(osg::Vec3(0,0,0));
//		coords->push_back(osg::Vec3(1,0,0));
//		coords->push_back(osg::Vec3(0,0,0));
//		coords->push_back(osg::Vec3(0,1,0));
//		coords->push_back(osg::Vec3(0,0,0));
//		coords->push_back(osg::Vec3(0,0,1));
//
//		colors->push_back(osg::Vec4(1,0,0,1));
//		colors->push_back(osg::Vec4(1,0,0,1));
//		colors->push_back(osg::Vec4(0,1,0,1));
//		colors->push_back(osg::Vec4(0,1,0,1));
//		colors->push_back(osg::Vec4(0,0,1,1));
//		colors->push_back(osg::Vec4(0,0,1,1));
//
//		geom->setVertexArray(coords);
//		geom->setColorArray(colors);
//		geom->setColorBinding(osg::Geometry::BIND_PER_VERTEX);
//		geom->addPrimitiveSet(new  osg::DrawArrays(osg::PrimitiveSet::LINES,0,coords->size()));
//		geode->addDrawable(geom.get());
//
//		root->addChild(geode.get()); 
//	}
//
//	{
//		//skeleton
//		osg::ref_ptr<osg::Vec3Array> coords = new osg::Vec3Array;
//		osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array;
//		osg::ref_ptr<osg::Geometry> geom = new osg::Geometry; 
//		osg::ref_ptr<osg::Geode> geode = new osg::Geode;
//
//		osg::ref_ptr<osg::Vec3Array> coordsPoint = new osg::Vec3Array;
//		osg::ref_ptr<osg::Vec4Array> colorsPoint = new osg::Vec4Array;
//		osg::ref_ptr<osg::Geometry> geomPoint = new osg::Geometry; 
//		osg::ref_ptr<osg::Geode> geodePoint = new osg::Geode;
//
//
//		int maxParentIndex = bvhData.getMaxParentIndex();
//		osg::ref_ptr<osg::Vec3Array> nodesPosition 
//			= new osg::Vec3Array(bvhData.getNodesCount() + 1);
//		osg::ref_ptr<osg::Vec3Array> framePosition 
//			= new osg::Vec3Array(bvhData.getNodesCount() + 1);
//
//		osg::ref_ptr<osg::MatrixfArray> matrixArr
//			= new osg::MatrixfArray(bvhData.getNodesCount() + 1);
//
//		for (int i = 1; i <= bvhData.getNodesCount(); i++)
//		{
//			//vector<BvhHierarchyNode> childNodes = bvhData.getChildNodes(i);
//			//int childNodesSize = childNodes.size();
//
//			BvhHierarchyNode currNode = bvhData.getNode(i);
//			if (currNode.ParentIndex == -1)
//			{
//				nodesPosition->at(i) = osg::Vec3(0,0,0);
//				framePosition->at(i) = osg::Vec3(0,0,0);
//				//framePosition->at(i) = osg::Vec3(1,1,1);
//			}else
//			{
//				osg::Vec3 parentPosition = nodesPosition->at(currNode.ParentIndex);
//				nodesPosition->at(i) = parentPosition + osg::Vec3(currNode.Offset[0],currNode.Offset[1],currNode.Offset[2]);
//
//				if (currNode.ParentIndex != 1)
//				{
//					//original skeleton
//					////coords->push_back(parentPosition);
//					////coords->push_back(nodesPosition->at(i));
//				}
//
//				osg::Matrix tt;
//				tt.makeTranslate(osg::Vec3(currNode.Offset[0],currNode.Offset[1],currNode.Offset[2]));
//
//				//Mposition * Mrotation * Mscale = Mprs
//				osg::Matrix transMatrix = getTransformMatrix(bvhData, currNode, iFrame);
//				//Moffset * Mprs = Moprs
//				transMatrix.preMultTranslate(osg::Vec3(currNode.Offset[0],currNode.Offset[1],currNode.Offset[2]));
//				
//				parentPosition = framePosition->at(currNode.ParentIndex);
//
//				//Mparent * Moprs = Mpoprs
//				transMatrix.postMult(matrixArr->at(currNode.ParentIndex));
//				matrixArr->at(i) = transMatrix;
//
//				framePosition->at(i) = //parentPosition + 
//					//matrixArr->at(currNode.ParentIndex) * osg::Vec3(currNode.Offset[0],currNode.Offset[1],currNode.Offset[2]);
//					osg::Vec3(transMatrix(3,0),transMatrix(3,1),transMatrix(3,2));
//
//				if (currNode.ParentIndex != 1)
//				{
//					//coords->push_back(osg::Vec3(parentPosition.x(),parentPosition.z(),parentPosition.y()));
//					//coords->push_back(osg::Vec3(framePosition->at(i).x(),framePosition->at(i).z(),framePosition->at(i).y()));
//					coords->push_back(parentPosition);
//					coords->push_back(framePosition->at(i));
//
//					coordsPoint->push_back(framePosition->at(i));
//
//					std::cout<<"**"<<framePosition->at(i).x()<<" "<<framePosition->at(i).y()<<" "<<framePosition->at(i).z()<<std::endl;
//				}
//			}
//
//			osg::ref_ptr<osgText::Text> text = new osgText::Text;
//			text->setFont("fonts/arial.ttf");
//			text->setAxisAlignment(osgText::Text::SCREEN);
//			text->setText(currNode.NodeName);
//			text->setPosition(framePosition->at(i));
//			text->setCharacterSize(0.01);
//			geode->addDrawable(text);
//
//
//		}
//		//for (int i=0;i<bvhData.n;i++)
//		//{
//		//	BvhHierarchyNode rootNodes = bvhData.getNode(i);
//		//	coords->push_back(osg::Vec3(x,y,z));
//		//	coords->push_back(osg::Vec3(0,0,0));
//		//	getTransformMatrix(bvhData, rootNodes[i], 0);
//		//}
//
//		colors->push_back(osg::Vec4(1.0f,0.0f,0.0f,1.0f));
//		colorsPoint->push_back(osg::Vec4(0.0f,1.0f,0.0f,1.0f));
//
//		geom->setVertexArray(coords);
//		geom->setColorArray(colors);
//		geom->setColorBinding(osg::Geometry::BIND_OVERALL);
//		geom->addPrimitiveSet(new  osg::DrawArrays(osg::PrimitiveSet::LINES,0,coords->size()));
//		geode->addDrawable(geom.get());
//
//		geomPoint->setVertexArray(coordsPoint);
//		geomPoint->setColorArray(colorsPoint);
//		geomPoint->setColorBinding(osg::Geometry::BIND_OVERALL);
//		geomPoint->addPrimitiveSet(new  osg::DrawArrays(osg::PrimitiveSet::POINTS,0,coordsPoint->size()));
//		geodePoint->addDrawable(geomPoint.get());
//		osg::ref_ptr<osg::Point> ptSize = new osg::Point;
//		ptSize->setSize(5.0) ;      
//		geodePoint->getOrCreateStateSet()->setAttributeAndModes( ptSize.get (),osg::StateAttribute::ON); 
//
//		root->addChild(geode); 
//		root->addChild(geodePoint);
//	}
//
//
//	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer;
//	viewer->setUpViewInWindow(100,100,640,480);
//	viewer->setCameraManipulator(new osgGA::TrackballManipulator);
//	viewer->setSceneData(root);
//
//	while(!viewer->done())
//	{
//		viewer->frame();
//	}
//
//	return 0;
//}