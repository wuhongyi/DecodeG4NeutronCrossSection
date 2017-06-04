// main.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 六 6月 28 14:38:43 2014 (+0800)
// Last-Updated: 三 3月 25 19:28:18 2015 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 65
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
  filename="92_238_Uranium.z";//这里修改要读取数据的文件名
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
  

  //G4NeutronHPPhotonDist::InitMean(std::istream & aDataFile)
  int repFlag;  //representation as multiplicities or transition probability arrays.
double targetMass;
  int nDiscrete;  //number of discrete photons
  int * disType;  // discrete, or continuum photons
  double  energy;  // photon energies
  int theInternalConversionFlag;
  double theBaseEnergy;
  int nGammaEnergies;
  double theLevelEnergies;
  double theTransitionProbabilities;
  double thePhotonTransitionFraction;
  int isoFlag; // isotropic or not?
  int tabulationType;
  int nDiscrete2;
  int nIso;
  double theShells;
  double theGammas;
  int * nNeu;
  int nPartials;
  int nData;

  bool result = true;
  if(ss >> repFlag)
  {
    output<<"repFlag:"<<repFlag<<endl;
    ss >> targetMass;
    output<<"targetMass:"<<targetMass<<endl;
    if(repFlag==1)
    {
    // multiplicities
      ss >> nDiscrete;
      output<<"nDiscrete:"<<nDiscrete<<endl;
      disType = new int[nDiscrete];
      //theYield = new G4NeutronHPVector[nDiscrete];
      for (int i=0; i<nDiscrete; i++)
      {
	ss >> disType[i]>>energy;
	output<<"disType:"<<disType[i]<<"  energy:"<<energy<<endl;
	//energy[i]*=eV;
	//theYield[i].Init(aDataFile, eV);
	//theYield[i]   multiplicity as a function of neutron energy.
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
      }
    }
    else if(repFlag == 2)
    {
       ss >> theInternalConversionFlag;
       output<<"theInternalConversionFlag:"<<theInternalConversionFlag<<endl;
       ss >> theBaseEnergy;
       output<<"theBaseEnergy:"<<theBaseEnergy<<endl;
       //theBaseEnergy*=eV;
       ss >> theInternalConversionFlag;
       output<<"theInternalConversionFlag:"<<theInternalConversionFlag<<endl;
 
       ss >> nGammaEnergies;
       output<<"nGammaEnergies:"<<nGammaEnergies<<endl;
       //theLevelEnergies = new G4double[nGammaEnergies];
       //theTransitionProbabilities = new G4double[nGammaEnergies];
       //if(theInternalConversionFlag == 2) thePhotonTransitionFraction = new G4double[nGammaEnergies];
       for(int  ii=0; ii<nGammaEnergies; ii++)
       {
	 if(theInternalConversionFlag == 1)
	 {
           ss >> theLevelEnergies >> theTransitionProbabilities;
	   output<<theLevelEnergies<<"  "<<theTransitionProbabilities<<endl;
	   //theLevelEnergies[ii]*=eV;
	 }
	 else if(theInternalConversionFlag == 2)
	 {
           ss >> theLevelEnergies >> theTransitionProbabilities >> thePhotonTransitionFraction;
	   output<<theLevelEnergies<<"  "<<theTransitionProbabilities<<"  "<<thePhotonTransitionFraction<<endl;
	   //theLevelEnergies[ii]*=eV;
	 }
	 else
	 {
           //throw G4HadronicException(__FILE__, __LINE__, "G4NeutronHPPhotonDist: Unknown conversion flag");
	 }
      }
       // Note, that this is equivalent to using the 'Gamma' classes.
      // throw G4HadronicException(__FILE__, __LINE__, "G4NeutronHPPhotonDist: Transition probability array not sampled for the moment.");
       cout<<"G4NeutronHPPhotonDist: Transition probability array not sampled for the moment."<<endl;
    }
    else
    {
      cout << "Data representation in G4NeutronHPPhotonDist: "<<repFlag<<endl;
      //throw G4HadronicException(__FILE__, __LINE__, "G4NeutronHPPhotonDist: This data representation is not implemented.");
    }
  }
  else
  {
    result = false;
  }

  output<<endl<<endl;

  if(result)
    {
      //G4NeutronHPPhotonDist::InitAngular(std::istream & aDataFile)
      //angular distributions
      ss >> isoFlag;
      output<<"isoFlag:"<<isoFlag<<endl;
      if (isoFlag != 1)
	{
	  if ( repFlag == 2 ) cout << "G4NeutronHPPhotonDist: repFlag == 2 && isoFlag != 1 is unexpected! If you use G4ND3.x, then please report to Geant4 Hyper News. Thanks." << endl;
	  ss >> tabulationType >> nDiscrete2 >> nIso;
	  output<<"tabulationType:"<<tabulationType<<"  nDiscrete2:"<<nDiscrete2<<"  nIso:"<<nIso<<endl;
	  //if ( theGammas != NULL && nDiscrete2 != nDiscrete ) G4cout << "080731c G4NeutronHPPhotonDist nDiscrete2 != nDiscrete, It looks like something wrong in your NDL files. Please update the latest. If you still have this messages after the update, then please report to Geant4 Hyper News." << G4endl;

	  // The order of cross section (InitPartials) and distribution (InitAngular here) data are different, we have to re-coordinate consistent data order.
	  // std::vector < G4double > vct_gammas_par; 
	  // std::vector < G4double > vct_shells_par; 
	  // std::vector < G4int > vct_primary_par; 
	  // std::vector < G4int > vct_distype_par; 
	  // std::vector < G4NeutronHPVector* > vct_pXS_par;
	  // if ( theGammas != NULL ) 
	  //   {
	  //     //copy the cross section data 
	  //     for ( i = 0 ; i < nDiscrete ; i++ )
	  // 	{
	  // 	  vct_gammas_par.push_back( theGammas[ i ] );
	  // 	  vct_shells_par.push_back( theShells[ i ] );
	  // 	  vct_primary_par.push_back( isPrimary[ i ] );
	  // 	  vct_distype_par.push_back( disType[ i ] );
	  // 	  G4NeutronHPVector* hpv = new G4NeutronHPVector;
	  // 	  *hpv = thePartialXsec[ i ];
	  // 	  vct_pXS_par.push_back( hpv );
	  // 	}
	  //   }
	  // if ( theGammas == NULL ) theGammas = new G4double[nDiscrete2];
	  // if ( theShells == NULL ) theShells = new G4double[nDiscrete2];

	  for (int i=0; i< nIso; i++) // isotropic photons
	    {
	      ss >> theGammas >> theShells;
	      output<<"theGammas:"<<theGammas<<"  theShells:"<<theShells<<endl;
	      // theGammas[i]*=eV;
	      // theShells[i]*=eV;
	    }
	  nNeu = new int [nDiscrete2-nIso];
	  // if(tabulationType==1)theLegendre=new G4NeutronHPLegendreTable *[nDiscrete2-nIso];
	  // if(tabulationType==2)theAngular =new G4NeutronHPAngularP *[nDiscrete2-nIso];
	  for(int i=nIso; i< nDiscrete2; i++)
	    {
	      if(tabulationType==1) 
		{
		  ss >> theGammas >> theShells >> nNeu[i-nIso];
		  output<<"theGammas:"<<theGammas<<"  theShells:"<<theShells<<"  nNeu:"<<nNeu[i-nIso]<<endl;
		  // theGammas[i]*=eV;
		  // theShells[i]*=eV;
		  // theLegendre[i-nIso]=new G4NeutronHPLegendreTable[nNeu[i-nIso]];
		  //theLegendreManager.Init(aDataFile); 
		  //G4InterpolationManager 
		  int nRanges;
		  ss>> nRanges;
		  output<<"nRanges:"<<nRanges<<endl;
		  int  range; 
		  int it;
		  for(int iiii=0; iiii<nRanges; iiii++)
		    {
		      ss>>range;
		      ss>>it;
		      output<<range<<" "<<it<<endl;
		    }

		  for (int ii=0; ii<nNeu[i-nIso]; ii++)
		    {
		      //theLegendre[i-nIso][ii].Init(aDataFile);
		      //
		      double eNeu, coeff;
		      int nPoly;
		      ss >> eNeu >> nPoly;
		      output<<eNeu<<"  "<<nPoly<<endl;

		      //Init(eNeu, nPoly);
		      for(int l=0; l<nPoly; l++)
			{
			   ss >> coeff;
			   output<<coeff<<endl;
			}
		    }
		}
	      else if(tabulationType==2)
		{
		  ss >> theGammas >> theShells >> nNeu[i-nIso];
		  output<<"theGammas:"<<theGammas<<"  theShells:"<<theShells<<"  nNeu:"<<nNeu[i-nIso]<<endl;
		  //theAngular[i-nIso]=new G4NeutronHPAngularP[nNeu[i-nIso]];

		  for (int ii=0; ii<nNeu[i-nIso]; ii++)
		    {
		      //theAngular[i-nIso][ii].Init(aDataFile);
		      double eNeu, cosTheta, probDist;
		      int nProb;
		      ss >> eNeu >> nProb;
		      output<<"eNeu:"<<eNeu<<"  nProb:"<<nProb<<endl;
		      //theManager.Init(aDataFile);
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
			  output<<range<<" "<<it<<endl;
			}

		      // eNeu *= CLHEP::eV;
		      // Init(eNeu, nProb);
		      for (int iii=0; iii<nProb; iii++)
			{
			  ss >> cosTheta >> probDist;
			  output<<"cosTheta:"<<cosTheta<<"  probDist:"<<probDist<<endl;
			  // SetCosTh(iii, cosTheta);
			  // SetProb(iii,probDist);
			}  
		    }
		}
	      else
		{
		  cout << "tabulation type: tabulationType"<<endl;
		  //throw G4HadronicException(__FILE__, __LINE__, "cannot deal with this tabulation type for angular distributions.");
		  cout<<"cannot deal with this tabulation type for angular distributions."<<endl;
		}
	    }
	  // if ( vct_gammas_par.size() > 0 ) 
	  //   {
	  //     //Reordering cross section data to corrsponding distribution data 
	  //     for ( i = 0 ; i < nDiscrete ; i++ ) 
	  // 	{
	  // 	  for ( G4int j = 0 ; j < nDiscrete ; j++ )  
	  // 	    {
	  // 	      // Checking gamma and shell to identification 
	  // 	      if ( theGammas[ i ] == vct_gammas_par [ j ] && theShells [ i ] == vct_shells_par[ j ] )
	  // 		{
	  // 		  isPrimary [ i ] = vct_primary_par [ j ];
	  // 		  disType [ i ] = vct_distype_par [ j ];
	  // 		  thePartialXsec[ i ] = ( *( vct_pXS_par[ j ] ) );
	  // 		}
	  // 	    }
	  // 	}
	  //     //Garbage collection 
	  //     for ( std::vector < G4NeutronHPVector* >::iterator 
	  // 	      it = vct_pXS_par.begin() ; it != vct_pXS_par.end() ; it++ )
	  // 	{
	  // 	  delete *it;
	  // 	}
	  //   }

	}


      output<<endl<<endl;

      //G4NeutronHPPhotonDist::InitEnergies(std::istream & aDataFile)
      int energyDistributionsNeeded = 0;
      for (int i=0; i<nDiscrete; i++)
	{
	  if( disType[i]==1) energyDistributionsNeeded =1;
	}
      if(!energyDistributionsNeeded) return 0;
      ss >>  nPartials;
      output<<"nPartials:"<<nPartials<<endl;
      // distribution = new G4int[nPartials];
      // probs = new G4NeutronHPVector[nPartials];
      // partials = new G4NeutronHPPartial * [nPartials];
      nData=nPartials;
      int nen;
      int dummy;
      for (int i=0; i<nPartials; i++)
	{
	  ss >> dummy;
	  output<<"dummy:"<<dummy<<endl;
	  //probs[i].Init(aDataFile, eV);
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
	  for (int i=0;i<total;i++)
	    {
	      ss >> x >> y;
	      output<<"x:"<<x<<"  y:"<<y<<endl;
	    }
	  ss >> nen;
	  output<<"nen:"<<nen<<endl;
	  //partials[i] = new G4NeutronHPPartial(nen);
	  //partials[i]->InitInterpolation(aDataFile);
	  //G4InterpolationManager 
	  ss>> nRanges;
	  output<<"nRanges:"<<nRanges<<endl;
	  for(int i=0; i<nRanges; i++)
	    {
	      ss>>range;
	      ss>>it;
	      output<<"range:"<<range<<" it:"<<it<<endl;
	    }

	  //partials[i]->Init(aDataFile);
	  double e;
	  for(int  iiiii=0; iiiii<nData; iiiii++)
	    {
	      ss >> e;
	      output<<"e:"<<e<<endl;
	      //InitData(i, aDataFile, CLHEP::eV); // energy and probability for gammas
	      double eg, pg;
	      int neg;
	      ss >> neg;
	      output<<"neg:"<<neg<<endl;
	      //data[i].InitInterpolation(aDataFile);
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
		  output<<range<<" "<<it<<endl;
		}

	      for (int ii=0; ii<neg; ii++)
		{ 
		  ss >> eg >> pg;
		  output<<"eg:"<<eg<<"  pg:"<<pg<<endl;
		  // eg *= unit;
		  // SetX(i,ii,eg);
		  // SetY(i,ii,pg);
		}          
	      // data[i].Hash();

	    }

  
	}

    }




  return 0;
}


// 
// main.cc ends here
