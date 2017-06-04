// main.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 六 6月 28 14:38:43 2014 (+0800)
// Last-Updated: 三 3月 25 15:55:55 2015 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 44
// URL: http://wuhongyi.cn 

#include<iostream>
#include<fstream>
#include<cstdlib>
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
  filename="82_208_Lead.z";//这里修改要读取数据的文件名
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

  //G4NeutronHPElasticFS

  int repFlag;    // Legendre coeff(1), or probability array(2), or isotropic(0). add 3: for Legendre (Low Energy) + Probability (High Energy)
  double targetMass; // in neutronmass units.
  int frameFlag;  // CMS or Lab system.

  ss>> repFlag >> targetMass >> frameFlag;
  output<<"repFlag:"<<repFlag<<"  targetMass:"<<targetMass<<"  frameFlag:"<<frameFlag<<endl;

  if(repFlag==1)//来自G4NeutronHPElasticFS
    {
      int nEnergy;
      ss >> nEnergy; 
      output<<"nEnergy:"<<nEnergy<<endl;
      //theCoefficients = new G4NeutronHPLegendreStore(nEnergy);
      //theCoefficients->InitInterpolation(theData);


      //G4InterpolationManager 
      int nRanges;
      ss>> nRanges;
      output<<"nRanges:"<<nRanges<<endl;
      int * range; 
      range = new int[nRanges];
      int it;
     for(int i=0; i<nRanges; i++)
     {
       ss>>range[i];
       ss>>it;
       output<<range[i]<<" "<<it<<endl;
     }


      double temp, energy;
      int tempdep, nLegendre;
      for (int i=0; i<nEnergy; i++)
      {
        ss >> temp >> energy >> tempdep >> nLegendre;
	output<<"temp:"<<temp<<"  energy:"<<energy<<"  tempdep:"<<tempdep<<"  nLegendre:"<<nLegendre<<endl;
        //energy *=eV;
        //theCoefficients->Init(i, energy, nLegendre);
        //theCoefficients->SetTemperature(i, temp);
        double coeff=0;
        for(int ii=0; ii<nLegendre; ii++)
        {
          // load legendre coefficients.
          ss >> coeff;
	  output<<coeff<<endl;
          //theCoefficients->SetCoeff(i, ii+1, coeff); // @@@HPW@@@
        }
      }
    }
    else if (repFlag==2)
    {
      int nEnergy;
      ss >> nEnergy;
      output<<"nEnergy:"<<nEnergy<<endl;
      //theProbArray = new G4NeutronHPPartial(nEnergy, nEnergy);
      //theProbArray->InitInterpolation(theData);

      //G4InterpolationManager 
      int nRanges;
      ss>> nRanges;
      output<<"nRanges:"<<nRanges<<endl;
      int * range; 
      range = new int[nRanges];
      int it;
     for(int i=0; i<nRanges; i++)
     {
       ss>>range[i];
       ss>>it;
       output<<range[i]<<" "<<it<<endl;
     }


      double temp, energy;
      int tempdep, nPoints;
      for(int i=0; i<nEnergy; i++)
      {
        ss >> temp >> energy >> tempdep >> nPoints;
	output<<"temp:"<<temp<<"  energy:"<<energy<<"  tempdep:"<<tempdep<<"  nPoints:"<<nPoints<<endl;
        //energy *= eV;
        //theProbArray->InitInterpolation(i, theData);
	//G4InterpolationManager 
	int nRanges;
	ss>> nRanges;
	output<<"nRanges:"<<nRanges<<endl;
	int * range; 
	range = new int[nRanges];
	int it;
	for(int i=0; i<nRanges; i++)
	  {
	    ss>>range[i];
	    ss>>it;
	    output<<range[i]<<" "<<it<<endl;
	  }
        //theProbArray->SetT(i, temp);
        //theProbArray->SetX(i, energy);
        double prob, costh;
        for(int ii=0; ii<nPoints; ii++)
        {
          // fill probability arrays.
	  ss >> costh >> prob;
	  output<<"costh:"<<costh<<"  prob:"<<prob<<endl;
          //theProbArray->SetX(i, ii, costh);
          //theProbArray->SetY(i, ii, prob);
        }
        //theProbArray->DoneSetXY( i );
      }
    }
    else if ( repFlag==3 )
    {
       int nEnergy_Legendre;
       ss >> nEnergy_Legendre;
       output<<"nEnergy_Legendre:"<<nEnergy_Legendre<<endl;

       //theCoefficients = new G4NeutronHPLegendreStore( nEnergy_Legendre );
       //theCoefficients->InitInterpolation( theData );
       //G4InterpolationManager 
       int nRanges;
       ss>> nRanges;
       output<<"nRanges:"<<nRanges<<endl;
       int * range; 
       range = new int[nRanges];
       int it;
       for(int i=0; i<nRanges; i++)
	 {
	   ss>>range[i];
	   ss>>it;
	   output<<range[i]<<" "<<it<<endl;
	 }

       double temp, energy;
       int tempdep, nLegendre;

       for ( int i = 0 ; i < nEnergy_Legendre ; i++ )
       {
          ss >> temp >> energy >> tempdep >> nLegendre;
	  output<<"temp:"<<temp<<"  energy:"<<energy<<"  tempdep:"<<tempdep<<"  nLegendre:"<<nLegendre<<endl;
          //energy *=eV;
          //theCoefficients->Init( i , energy , nLegendre );
          //theCoefficients->SetTemperature( i , temp );
          double coeff = 0;
          for (int ii = 0 ; ii < nLegendre ; ii++ )
          {
             // load legendre coefficients.
             ss >> coeff;
	     output<<coeff<<endl;
             //theCoefficients->SetCoeff(i, ii+1, coeff); // @@@HPW@@@
          }
       } 

       double tE_of_repFlag3 = energy; 

       int nEnergy_Prob;
       ss >> nEnergy_Prob;
       output<<"nEnergy_Prob:"<<nEnergy_Prob<<endl;
       //theProbArray = new G4NeutronHPPartial( nEnergy_Prob , nEnergy_Prob );
       //theProbArray->InitInterpolation( theData );
       //G4InterpolationManager 

       ss>> nRanges;
       output<<"nRanges:"<<nRanges<<endl;

       range = new int[nRanges];

       for(int i=0; i<nRanges; i++)
	 {
	   ss>>range[i];
	   ss>>it;
	   output<<range[i]<<" "<<it<<endl;
	 }
       int nPoints;
       for ( int i=0 ; i < nEnergy_Prob ; i++ )
       {
          ss >> temp >> energy >> tempdep >> nPoints;
	  output<<"temp:"<<temp<<"  energy:"<<energy<<"  tempdep:"<<tempdep<<"  nPoints:"<<nPoints<<endl;
          //energy *= eV;

//        consistency check
          if ( i == 0 )
 
             if ( std::abs( energy - tE_of_repFlag3 ) / tE_of_repFlag3 > 1.0e-15 )
                cout << "Warning Transition Energy of repFlag3 is not consistent." << endl; 

          //theProbArray->InitInterpolation( i , theData );
	  //G4InterpolationManager 
	  int nRanges;
	  ss>> nRanges;
	  output<<"nRanges:"<<nRanges<<endl;
	  int * range; 
	  range = new int[nRanges];
	  int it;
	  for(int i=0; i<nRanges; i++)
	    {
	      ss>>range[i];
	      ss>>it;
	      output<<range[i]<<" "<<it<<endl;
	    }
          //theProbArray->SetT( i , temp );
          //theProbArray->SetX( i , energy );
          double prob, costh;
          for( int ii = 0 ; ii < nPoints ; ii++ )
          {
             // fill probability arrays.
             ss >> costh >> prob;
	     output<<"costh:"<<costh<<"  prob:"<<prob<<endl;
	     //theProbArray->SetX( i , ii , costh );
             //theProbArray->SetY( i , ii , prob );
          }
          //theProbArray->DoneSetXY( i );
       }
    }
    else if (repFlag==0)
    {
      ss >> frameFlag;
      output<<"frameFlag:"<<frameFlag<<endl;
    }
    else
    {
      cout << "unusable number for repFlag: repFlag="<<repFlag<<endl;
      //throw G4HadronicException(__FILE__, __LINE__, "G4NeutronHPElasticFS::Init -- unusable number for repFlag");
    }

  return 0;
}


// 
// main.cc ends here
