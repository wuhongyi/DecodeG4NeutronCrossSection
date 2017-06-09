// main.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 四 6月  8 21:58:34 2017 (+0800)
// Last-Updated: 五 6月  9 10:32:08 2017 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 22
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

// 这个还未写完！！！
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


  
  int dummy;
  ss >> dummy>>dummy;
  int total;
  ss >> total;

  output<<"dummy:"<<dummy<<"   dummy:"<<dummy<<endl;
  output<<"total:"<<total<<endl;
  
  double x,y;
  for (int i=0;i<total;i++)
    {
      ss >> x >> y;//G4ParticleHPVector  Init
      output<<"x:"<<x<<"  y:"<<y<<endl;
    }

  if (!(ss >> dummy))
    {
      return 1;
    }
  output<<"dummy:"<<dummy<<endl;
  ss >> dummy;
  output<<"dummy:"<<dummy<<endl;

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  
  // G4ParticleHPAngular::Init
  int theAngularDistributionType;
  double targetMass;
  int frameFlag; // 1=Lab, 2=CMS
  ss >> theAngularDistributionType >> targetMass;
  ss >> frameFlag;
  output<<"theAngularDistributionType:"<<theAngularDistributionType<<"   targetMass:"<<targetMass<<"   frameFlag:"<<frameFlag<<endl;

  
  if(theAngularDistributionType == 0 )
  {
  }
  
  if(theAngularDistributionType==1)
    {
      int nEnergy;
      ss >> nEnergy;
      output<<"nEnergy:"<<nEnergy<<endl;
      
      // G4ParticleHPLegendreStore
      int range;
      int nRanges;
      ss >> nRanges;
      output<<"nRanges:"<<nRanges<<endl;
      int it;
      for(int i=0; i<nRanges; i++)
	{
	  ss>>range;
	  ss>>it;
	  output<<"range:"<<range<<"   it:"<<it<<endl;
	}


      double temp, energy;
      int tempdep, nLegendre;
      int i, ii;
      for (i=0; i<nEnergy; i++)
	{
	  ss >> temp >> energy >> tempdep >> nLegendre;
	  output<<"temp:"<<temp<<"  energy:"<<energy<<"  tempdep:"<<tempdep<<"  nLegendre:"<<nLegendre<<endl;
	  double coeff=0;
	  for(ii=0; ii<nLegendre; ii++)
	    {
	      ss >> coeff;
	      output<<"coeff:"<<coeff<<endl;
	    }
	}
    }


  if (theAngularDistributionType==2)
    {
      int nEnergy;
      ss >> nEnergy;
      output<<"nEnergy:"<<nEnergy<<endl;
      
      // G4InterpolationManager::Init
      int range;
      int nRanges;
      ss >> nRanges;
      output<<"nRanges:"<<nRanges<<endl;
      
      int it;
      for(int i=0; i<nRanges; i++)
	{
	  ss>>range;
	  ss>>it;
	  output<<"range:"<<range<<"   it:"<<it<<endl;
	}


      double temp, energy;
      int tempdep;
      for(int i=0; i<nEnergy; i++)
	{
	  ss >> temp >> energy >> tempdep;
	  output<<"temp:"<<temp<<"  energy:"<<energy<<"  tempdep:"<<tempdep<<endl;
	  
	  // G4ParticleHPPartial::InitData
	  int ii;
	  double eg, pg;
	  int neg;
	  ss >> neg;
	  output<<"neg:"<<neg<<endl;
	  
	  // G4InterpolationManager::Init
	  int range2;
	  int nRanges2;
	  ss >> nRanges2;
	  output<<"nRanges:"<<nRanges2<<endl;
	  int it2;
	  for(int k=0; k<nRanges; k++)
	    {
	      ss>>range2;
	      ss>>it2;
	      output<<"range:"<<range2<<"   it:"<<it2<<endl;
	    }
	  
	  for (ii=0; ii<neg; ii++)
	    { 
	      ss >> eg >> pg;
	      output<<"eg:"<<eg<<"  pg:"<<pg<<endl;
	    }          
	}
    }

  if(theAngularDistributionType!=0 && theAngularDistributionType!=1 && theAngularDistributionType!=2) cout << "unknown distribution found for Angular: "<< theAngularDistributionType << endl;

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
  ss >> dummy >> dummy;
  output<<"dummy:"<<dummy<<"   dummy:"<<dummy<<endl;

  // G4ParticleHPEnergyDistribution::Init
  double dummy1;
  int theNumberOfPartials;
  int theRepresentationType;
  ss >> dummy1 >> theNumberOfPartials;

  for(G4int iiiii=0; iiiii<theNumberOfPartials; iiiii++) 
    {
      ss >> theRepresentationType;

      // TODO
      // switch(theRepresentationType)
      // {
      // 	case 1:
      //     theEnergyDistribution[i] = new G4ParticleHPArbitaryTab;
      //     break;
      // 	case 5:        
      //     theEnergyDistribution[i] = new G4ParticleHPEvapSpectrum;
      //     break;
      // 	case 7:
      //     theEnergyDistribution[i] = new G4ParticleHPFissionSpectrum;
      //     break;
      // 	case 9:
      //     theEnergyDistribution[i] = new G4ParticleHPSimpleEvapSpectrum;
      //     break;
      // 	case 11:
      //     theEnergyDistribution[i] = new G4ParticleHPWattSpectrum;
      //     break;
      // 	case 12:
      //     theEnergyDistribution[i] = new G4ParticleHPMadlandNixSpectrum;
      //     break;
      // }
      // theEnergyDistribution[i]->Init(theData);

      
    }

  
  
  return 0;
}

// 
// main.cc ends here
