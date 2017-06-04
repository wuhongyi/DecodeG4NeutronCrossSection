// main.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 六 6月 28 14:38:43 2014 (+0800)
// Last-Updated: 三 3月 25 13:25:57 2015 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 27
// URL: http://wuhongyi.cn 

#include<iostream>
#include<fstream>
#include<cstdlib>
#include <cstring>
#include <sstream>
#include <string>
#include <zlib.h>
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
  filename="1_1_Hydrogen.z";//这里修改要读取数据的文件名
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

  //解压缩后的数据读取
  stringstream ss(*data);//将string型转为double
  int temp,number;
  double Ek,CrossSection;
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

  ss>>temp;//前两个数没用读取掉
  //cout<<temp<<endl;
  output<<temp<<endl;
  ss>>temp;//前两个数没用读取掉
  output<<temp<<endl;
  //cout<<temp<<endl;
  ss>>number;//数据行数
  output<<number<<endl;
  //cout<<"number:"<<number<<endl;
  for (int i = 0; i<number; ++i)
    {
      ss>>Ek>>CrossSection;
      //cout<<Ek<<" "<<CrossSection<<endl;
      output<<Ek<<" "<<CrossSection<<endl;
    }  

  return 0;
}


// 
// main.cc ends here
