// main.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 四 6月  8 21:58:34 2017 (+0800)
// Last-Updated: 四 6月  8 22:48:03 2017 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 18
// URL: http://wuhongyi.cn 

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include <sstream>
#include <string>
#include <zlib.h>
#include <cmath>
#include "wuString.hh"
#include "wuString.cc"
using namespace std;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//使用说明：
//使用g++编译时候需要加上 -lz 调用 zlib 库
//例如 g++ main.cc -lz -o 123
//
//wuString.hh 是 G4 中的 G4String.hh
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
int main(int argc, char *argv[])
{

  wuString filename;
  if(argc == 1)
    {
      filename="92_235_Uranium.z";//这里修改要读取数据的文件名
    }
  if(argc == 2)
    {
      filename=argv[1];
    }
  string::size_type idx = filename.find('.');
  wuString OutPutFile=filename.substr(0,idx);//输出文件名
  OutPutFile+=".txt";
  ofstream output;
  output.open(OutPutFile); 

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  wuString* data=NULL;//解压缩的数据暂存在这
  wuString compfilename(filename);
  std::ifstream* in = new std::ifstream ( compfilename , std::ios::binary | std::ios::ate );//二进制、打开定位到文件尾
  if ( in->good() )
    {
      //这些是G4的源码剥离出来的
      // Use the compressed file 
      int file_size = in->tellg();//返回当前指针的位置。因为打开定位在文件结尾，所以代表数据流的大小
      cout<<"file_size:"<<file_size<<endl;
      in->seekg( 0 , std::ios::beg );//基地址为文件头，偏移量为0，于是定位在文件头
      Bytef* compdata = new Bytef[ file_size ];//要解压缩的数据存放在这

      while ( *in ) {
	in->read( (char*)compdata , file_size );//读取要解压缩的数据
      }

      uLongf complen = (uLongf) ( file_size*4 );//变量，用来存放解压缩后数据的大小
      Bytef* uncompdata = new Bytef[complen];//解压缩后的数据

      while ( Z_OK != uncompress ( uncompdata , &complen , compdata , file_size ) )
	{
	  //uncompress ( uncompdata , &complen , compdata , file_size )进行解压缩，成功返回Z_OK
	  //如果解压缩没成功执行下面
	  delete[] uncompdata;
	  complen *= 2;
	  uncompdata = new Bytef[complen];
	}

      delete [] compdata;
      //Now "complen" has uncomplessed size
      data = new wuString ( (char*)uncompdata , (long)complen );//解压缩的内容在这
      delete [] uncompdata;
    }

  std::cout<<*data<<std::endl;
  
  //解压缩后的数据读取
  stringstream ss(*data);//将string型转为double
  wuString id;
  ss>>id;
  output<<id<<endl;

  if ( id == "G4NDL" ) {
    //Register information of file
    wuString source;
    ss >> source;
    output<<source<<endl;
    //register_data_file(filename,source);
  } else {
    ss.seekg( 0 , std::ios::beg );
  }

  while ( ss.good() ) 
    {
      double dummy;
      int iMT, iMF;
      int imax;
      //         MT       MF       AWR
      ss >> iMT >> iMF >> dummy;
      //         nBlock 
      ss >> imax;

      output<<"MT:"<<iMT<<"  MF:"<<iMF<<"  AWR:"<<dummy<<std::endl;
      output<<"nBlock:"<<imax<<std::endl;
     
      for ( int i = 0 ; i <= imax ; i++ ) 
	{
       
	  double YY=0.0; 
	  double Ei;
	  int jmax;
	  int ip;
	  //        energy of incidence neutron
	  ss >> Ei;//eV
	  //        Number of data set followings 
	  ss >> jmax;
	  //         interpolation scheme
	  ss >> ip;

	  output<<"energy of incidence neutron:"<<Ei<<"          Number of data set followings:"<<jmax<<"          interpolation scheme:"<<ip<<std::endl;

	  for ( int j = 0 ; j < jmax ; j++ ) 
	    {
	      int FSP;
	      int mFSP;
	      double Y;
	      ss >> FSP >> mFSP >> Y;
	      int k = FSP*100+mFSP;
	      YY = YY + Y;
	      //if ( iMT == 454 )G4cout << iMT << " " << i << " " << j << " " <<  k << " " << Y << " " << YY << G4endl;
	    
	      output<<"FSP:"<<FSP<<"  mFSP:"<<mFSP<<"  Y:"<<Y<<std::endl;
	    }
	}
	 
    }

  
  return 0;
}

// 
// main.cc ends here
