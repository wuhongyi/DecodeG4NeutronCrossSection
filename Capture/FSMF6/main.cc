// main.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 六 6月 28 14:38:43 2014 (+0800)
// Last-Updated: 三 3月 25 20:58:52 2015 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 79
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
  filename="92_232_Uranium.z";//这里修改要读取数据的文件名
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

  //G4NeutronHPCaptureFS
  if ( ss.good() == true ) 
    { 
    //theMF6FinalState.Init(theData);

    double targetMass;
    int frameFlag; // =1: Target rest frame; =2: CMS system; incident always in lab
    int nProducts;
    ss>>targetMass>>frameFlag>>nProducts;
    output<<"targetMass:"<<targetMass<<"  frameFlag:"<<frameFlag<<"  nProducts:"<<nProducts<<endl;
    //theProducts = new G4NeutronHPProduct[nProducts];
    for(int i=0; i<nProducts; i++)
      {
	//theProducts[i].Init(aDataFile);
	double theMassCode;
	double theMass;
	int theIsomerFlag;
	double theGroundStateQValue;
	double theActualStateQValue;
	int theDistLaw;  // redundant
	ss >> theMassCode>>theMass>>theIsomerFlag>>theDistLaw>> theGroundStateQValue>>theActualStateQValue;
	output<<"theMassCode:"<<theMassCode<<"  theMass:"<<theMass<<"  theIsomerFlag:"<<theIsomerFlag<<"  theDistLaw:"<<theDistLaw<<"  theGroundStateQValue:"<<theGroundStateQValue<<"  theActualStateQValue:"<<theActualStateQValue<<endl;
	// theYield.Init(aDataFile, CLHEP::eV);
	//G4NeutronHPVector
	int total;
	ss >> total;
	output<<"total:"<<total<<endl;
	//G4InterpolationManager 
	int nRanges;
	ss>> nRanges;
	output<<"nRanges:"<<nRanges<<endl;
	int range; 
	int it;
	for(int i=0; i<nRanges; i++)
	  {
	    ss>>range;
	    ss>>it;
	    output<<"range:"<<range<<" it:"<<it<<endl;
	  }
	//G4NeutronHPVector
	double x,y;
	for (int i=0;i<total;i++)
	  {
	    ss >> x >> y;
	    output<<"x:"<<x<<"  y:"<<y<<endl;
	  }

	// theYield.Hash();
	if(theDistLaw==0)
	  {
	    // distribution not known, use E-independent, isotropic angular distribution
	    //theDist = new G4NeutronHPIsotropic;//这是一个空函数
	  }
	else if(theDistLaw == 1)
	  {
	    // Continuum energy-angular distribution
	    //theDist = new G4NeutronHPContEnergyAngular;
	    double theTargetCode;
	    int theAngularRep;
	    int nEnergy;
	    int theInterpolation;
	    ss >> theTargetCode >> theAngularRep >> theInterpolation >> nEnergy;
	    output<<"theTargetCode:"<<theTargetCode<<"  theAngularRep:"<<theAngularRep<<"  theInterpolation:"<<theInterpolation<<"  nEnergy:"<<nEnergy<<endl;
	    // theAngular = new G4NeutronHPContAngularPar[nEnergy];
	    //theManager.Init(aDataFile);
	    //G4InterpolationManager 
	    int nRanges;
	    ss>> nRanges;
	    output<<"nRanges:"<<nRanges<<endl;
	    int range; 
	    int it;
	    for(int i=0; i<nRanges; i++)
	      {
		ss>>range;
		ss>>it;
		output<<"range:"<<range<<" it:"<<it<<endl;
	      }
	    for(int i=0; i<nEnergy; i++)
	      {
		// theAngular[i].Init(aDataFile);
		double theEnergy;// incoming particle 
		int nEnergies;// number of exit channel energies
		int nDiscreteEnergies;
		int nAngularParameters;// number of angular paramerers per channel
		ss >> theEnergy >> nEnergies >> nDiscreteEnergies >> nAngularParameters;
		output<<"theEnergy:"<<theEnergy<<"  nEnergies:"<<nEnergies<<"  nDiscreteEnergies:"<<nDiscreteEnergies<<"  nAngularParameters:"<<nAngularParameters<<endl;
		// theEnergy *= eV;
		// theAngular = new G4NeutronHPList [nEnergies];
		for(int i=0; i<nEnergies; i++)
		  {
		    double sEnergy;
		    ss >> sEnergy;
		    output<<"sEnergy:"<<sEnergy<<endl;
		    // sEnergy*=eV;
		    // theAngular[i].SetLabel(sEnergy);
		    //theAngular[i].Init(aDataFile, nAngularParameters, 1.);
		    double y;
		    for (int iii=0; iii< nAngularParameters; iii++)
		      {
			ss >> y;
			output<<"y:"<<y<<endl;
			// SetValue(i,y*unit);
		      }
		  }
		// theAngular[i].SetInterpolation(theInterpolation);
	      }
	  }
	else if(theDistLaw == 2)
	  {
	    // Discrete 2-body scattering
	    //theDist = new G4NeutronHPDiscreteTwoBody;
	    int nEnergy;
	    ss >> nEnergy;
	    output<<"nEnergy:"<<nEnergy<<endl;
	    //theManager.Init(aDataFile);
	    //G4InterpolationManager 
	    int nRanges;
	    ss>> nRanges;
	    output<<"nRanges:"<<nRanges<<endl;
	    int range; 
	    int it;
	    for(int i=0; i<nRanges; i++)
	      {
		ss>>range;
		ss>>it;
		output<<"range:"<<range<<" it:"<<it<<endl;
	      }
	    //theCoeff = new G4NeutronHPLegendreTable[nEnergy];
	    for(int i=0; i<nEnergy; i++)
	      {
		double energy;
		int aRep, nCoeff;
		ss >> energy >> aRep >> nCoeff;
		output<<"energy:"<<energy<<"  aRep:"<<aRep<<"  nCoeff:"<<nCoeff<<endl;
		// energy*=CLHEP::eV;
		int nPoints=nCoeff;
		if(aRep>0) nPoints*=2;
		//theCoeff[i].Init(energy, nPoints);
		// theCoeff[i].Init(energy, nPoints-1);
		// theCoeff[i].SetRepresentation(aRep);
		for(int ii=0; ii<nPoints; ii++)
		  {
		    double y;
		    ss >> y;
		    output<<"y:"<<y<<endl;
		    // theCoeff[i].SetCoeff(ii, y);
		  }
	      }
	  }
	else if(theDistLaw == 3)
	  {
	    // Isotropic emission
	    //theDist = new G4NeutronHPIsotropic;这个是空函数
	  }
	else if(theDistLaw == 4)
	  {
	    // Discrete 2-body recoil modification
	    // not used for now. @@@@
	    //theDist = new G4NeutronHPDiscreteTwoBody; 
	    // the above is only temporary;
	    // recoils need to be addressed
	    // properly
	    // delete theDist;
	    // theDist = 0;
	    //这个暂时没用
	  }
	else if(theDistLaw == 5)
	  {
	    // charged particles only, to be used in a later stage. @@@@
	  }
	else if(theDistLaw == 6)
	  {
	    // N-Body phase space
	    //theDist = new G4NeutronHPNBodyPhaseSpace;
	    double theTotalMass; 
	    int theTotalCount;
	    ss >> theTotalMass >> theTotalCount;
	    output<<"theTotalMass:"<<theTotalMass<<"  theTotalCount:"<<endl;
	  }
	else if(theDistLaw == 7)
	  {
	    // Laboratory angular energy paraetrisation
	    //theDist = new G4NeutronHPLabAngularEnergy;
	    int nEnergies;// number of incoming neutron energies
	    double theEnergies;// Incoming neutron energies
	    int * nCosTh;// number of directioncosines; parallel to theEnergies
	    ss >> nEnergies;
	    output<<"nEnergies:"<<nEnergies<<endl;
	    //theManager.Init(aDataFile);
	    //G4InterpolationManager 
	    int nRanges;
	    ss>> nRanges;
	    output<<"nRanges:"<<nRanges<<endl;
	    int range; 
	    int it;
	    for(int i=0; i<nRanges; i++)
	      {
		ss>>range;
		ss>>it;
		output<<"range:"<<range<<" it:"<<it<<endl;
	      }
	    // theEnergies = new G4double[nEnergies];
	    nCosTh = new int[nEnergies];
	    // theData = new G4NeutronHPVector * [nEnergies];
	    // theSecondManager = new G4InterpolationManager [nEnergies];
	    for(int i=0; i<nEnergies; i++)
	      {
		ss >> theEnergies;
		// theEnergies[i]*=eV;
		ss >> nCosTh[i];
		output<<"theEnergies:"<<theEnergies<<"  nCosTh:"<<nCosTh[i]<<endl;
		//theSecondManager[i].Init(aDataFile);
		//G4InterpolationManager 
		int nRanges;
		ss>> nRanges;
		output<<"nRanges:"<<nRanges<<endl;
		int range; 
		int it;
		for(int iii=0; iii<nRanges; iii++)
		  {
		    ss>>range;
		    ss>>it;
		    output<<"range:"<<range<<" it:"<<it<<endl;
		  } 
		// theData[i] = new G4NeutronHPVector[nCosTh[i]];
		double label;
		for(int ii=0; ii<nCosTh[i]; ii++)
		  {
		    ss >> label;
		    output<<"label:"<<label<<endl;
		    // theData[i][ii].SetLabel(label);
		    // theData[i][ii].Init(aDataFile, eV);
		    //G4NeutronHPVector
		    int total;
		    ss >> total;
		    output<<"total:"<<total<<endl;
		    //G4InterpolationManager 
		    int nRanges;
		    ss>> nRanges;
		    output<<"nRanges:"<<nRanges<<endl;
		    int range; 
		    int it;
		    for(int iiii=0; iiii<nRanges; iiii++)
		      {
			ss>>range;
			ss>>it;
			output<<"range:"<<range<<" it:"<<it<<endl;
		      }
		    //G4NeutronHPVector
		    double x,y;
		    for (int iiiii=0;iiiii<total;iiiii++)
		      {
			ss >> x >> y;
			output<<"x:"<<x<<"  y:"<<y<<endl;
		      }
		  }
	      }
	  }
	else
	  {
	    //throw G4HadronicException(__FILE__, __LINE__, "distribution law unknown to G4NeutronHPProduct");
	    cout<<"distribution law unknown to G4NeutronHPProduct"<<endl;
	  }
	// if(theDist!=0)
	//   {
	//     theDist->SetQValue(theActualStateQValue);      
	//     theDist->Init(aDataFile);
	//   }
      }
    }



  return 0;
}


// 
// main.cc ends here
