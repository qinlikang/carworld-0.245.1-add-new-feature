#include "WorldBlock.h"
#include <boost/foreach.hpp>
using namespace boost;
void printHelp()
{
	cout<<"usage: \n\tTransformWorldBlock [landscapefile] [dstfile]"<<endl;
	cout<<"synopsis: \n\tTransformed [landscapefile] into a whole 3d OFF file [dstfile]"<<endl;
	cout<<"notice!!!!: \n\tthe [landscapefile] should contain path, like './landscape.txt' but not 'landscape.txt'"<<endl;
	cout<<" \tThe file name should not contain spaces"<<endl;
	cout<<"SAMPLE: \n\tTransformWorldBlock ./landscape.txt wholeland.off"<<endl;

}

int main(int argc, char* argv[])
{
	if(argc!=3)
	{
		printHelp();
		return -1;
	}
	
	const char* file_name=argv[1];
	const char* output_file_name=argv[2];
	list<WorldBlock> MyWorldBlocks;
	try
	{
		// read as usual
		char BlockName[1024];

		Ref CurrentRef;
		CurrentRef.reset();
		ifstream infile;
		OpenDataFile(infile,file_name);

		unsigned int NbWorldBlocks;
		infile >> NbWorldBlocks;
		infile.getline(BlockName, 1024);
		for (unsigned int i=0 ; i<NbWorldBlocks ; i++)
		{
			infile.getline(BlockName, 1024);
			string ThisPath = PathOf(file_name);
			string BlockFile = ThisPath+'/'+BlockName;
			MyWorldBlocks.push_back(WorldBlock());
			MyWorldBlocks.back().ReadFile(BlockFile.c_str());
			MyWorldBlocks.back().Place(CurrentRef);
			CurrentRef = MyWorldBlocks.back().EndPosition;
		}

		// write them into a obj file
		typedef Point<3,int> IndexTriangle;
		
		int index_offset=0;

		vector<Point3D> all_vertex_position;
		vector<Point3D> all_vertex_normal;
		vector<Point2D> all_vertex_texture;
		vector<IndexTriangle> all_triangles;
		BOOST_FOREACH(const WorldBlock& block,MyWorldBlocks)
		{
			BOOST_FOREACH(const OFFObject::Vertex& ver,block.MyOFFVertexes)
			{
				all_vertex_position.push_back(ver.Position);
				all_vertex_normal.push_back(ver.Normal);
				all_vertex_texture.push_back(ver.TexCoord);
			}
			BOOST_FOREACH(const WorldBlock::MyTriangle& tri,block.Triangles)
			{
				all_triangles.push_back(IndexTriangle(tri.MyI+index_offset,tri.MyJ+index_offset,tri.MyK+index_offset));
			}
			index_offset=all_vertex_position.size();
		}

		// write to file
		ofstream outfile(output_file_name);
		if(!outfile)
		{
			cout<<"can't open "<<output_file_name<<" to write!!"<<endl;
			return -1;
		}
		outfile<<"OFF"<<endl;
		outfile<<all_vertex_position.size()<<" "<<all_triangles.size()<<" "<<0<<endl;
		BOOST_FOREACH(const Point3D& ver,all_vertex_position)
		{
			outfile<<ver<<endl;
		}
		BOOST_FOREACH(const IndexTriangle& tri,all_triangles)
		{
			outfile<<"3 "<<tri<<" 0.2 0.6 0.1"<<endl;
		}
		outfile.close();
	}
	catch (const HException &E)
	{
		MyWorldBlocks.clear();
		cout<<(string("problem with CWLandscape object \"")+TitleOf(file_name)+("\":\n")+E.what())<<endl;
	}
	return 0;
}