// wuString.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 六 3月 22 14:22:55 2014 (+0800)
// Last-Updated: 六 3月 22 15:53:01 2014 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 4
// URL: http://wuhongyi.cn 

#include "wuString.hh"

// **************************************************************
// wuString
// **************************************************************

wuString::wuString () {}

wuString::wuString ( const char * astring )
 : std::string ( astring ) {}

wuString::wuString ( const char * astring, std::string::size_type len )
 : std::string ( astring, len ) {} 

wuString::wuString ( char ch )
{
  char str[2];
  str[0]=ch;
  str[1]='\0';
  std::string::operator=(str);
}

wuString::wuString ( const wuString& str )
 : std::string(str) {}

wuString::wuString ( const std::string& str )
 : std::string(str) {}

wuString& wuString::operator=(const wuString& str)
{
  if (&str == this) { return *this; }
  std::string::operator=(str);
  return *this;
}

wuString& wuString::operator=(const std::string& str)
{
  std::string::operator=(str);
  return *this;
}

wuString& wuString::operator=(const char* str)
{
  std::string::operator=(str);
  return *this;
}

// "cmp" optional parameter is NOT implemented ! 
// N.B.: The hash value returned is generally DIFFERENT from the
//       one returned by the original RW function.
//       Users should not rely on the specific return value.
//
char wuString::operator () (std::string::size_type i) const
{
  return operator[](i);
}

char& wuString::operator () (std::string::size_type i)
{
  return std::string::operator[](i);
}

wuString& wuString::operator+=(const char* str)
{
  std::string::operator+=(str);
  return *this;
}

wuString& wuString::operator+=(const std::string& str)
{
  std::string::operator+=(str);
  return *this;
}

wuString& wuString::operator+=(const char& ch)
{
  std::string::operator+=(ch);
  return *this;
}

bool wuString::operator==(const wuString& str) const
{
  return (std::string::compare(str) == 0);
}

bool wuString::operator==(const char* str) const
{
  return (std::string::compare(str) == 0);
}

bool wuString::operator!=(const wuString& str) const
{
  return !(*this == str);
}

bool wuString::operator!=(const char* str) const
{
  return !(*this == str);
}

wuString::operator const char*() const
{
  return c_str();
}

int wuString::strcasecompare(const char* s1, const char* s2) const
{
  char* buf1 = new char[strlen(s1)+1];
  char* buf2 = new char[strlen(s2)+1];

  for (std::string::size_type i=0; i<=strlen(s1); i++)
    { buf1[i] = tolower(char(s1[i])); }
  for (std::string::size_type j=0; j<=strlen(s2); j++)
    { buf2[j] = tolower(char(s2[j])); }

  int res = strcmp(buf1, buf2);
  delete [] buf1;
  delete [] buf2;
  return res;
}

int wuString::compareTo(const char* str, caseCompare mode) const
{
  return (mode==exact) ? strcmp(c_str(),str)
                       : strcasecompare(c_str(),str);
}

int wuString::compareTo(const wuString& str, caseCompare mode) const
{
  return compareTo(str.c_str(), mode);
}

wuString& wuString::prepend (const char* str)
{
  insert(0,str);
  return *this;
}

wuString& wuString::append(const wuString& str)
{
  std::string::operator+=(str);
  return *this;
}

std::istream&
wuString::readLine (std::istream& strm, bool skipWhite)
{
  char tmp[1024];
  if ( skipWhite )
  {
    strm >> std::ws;
    strm.getline(tmp,1024);
    *this=tmp;
  }
  else
  {
    strm.getline(tmp,1024);    
    *this=tmp;
  } 
  return strm;
}

wuString& wuString::replace (unsigned int start, unsigned int nbytes, 
                             const char* buff, unsigned int n2 )
{
  std::string::replace ( start, nbytes, buff, n2 ); 
  return *this;                                                              
}                                                                          

wuString& wuString::replace(std::string::size_type pos, std::string::size_type n, const char* str)
{
  std::string::replace(pos,n,str);
  return *this;
}

wuString& wuString::remove(std::string::size_type n)
{
  if(n<size()) { erase(n,size()-n); }
  return *this;
}

wuString& wuString::remove(std::string::size_type pos, std::string::size_type N)
{
  erase(pos,N+pos);
  return *this;
}

int wuString::first(char ch) const
{
  return find(ch);
}

int wuString::last(char ch) const
{
  return rfind(ch);
}

bool wuString::contains(const std::string& str) const
{
  return (std::string::find(str) != std::string::npos);
}

bool wuString::contains(char ch) const
{
  return (std::string::find(ch) != std::string::npos);
}

wuString wuString::strip (int strip_Type, char ch)
{
  wuString retVal = *this;
  if(length()==0) { return retVal; }
  std::string::size_type i=0;
  switch ( strip_Type ) {
  case leading: 
    {
      for(i=0;i<length();i++)
	{ if (std::string::operator[](i) != ch) { break; } }
      retVal = substr(i,length()-i);
    }
    break;
  case trailing:
    {
      int j=0;
      for(j=length()-1;j>=0;j--)
	{ if (std::string::operator[](j) != ch) { break; } }
      retVal = substr(0,j+1);
    }
    break;
  case both:
    { 
      for(i=0;i<length();i++)
	{ if (std::string::operator[](i) != ch) { break; } }
      wuString tmp(substr(i,length()-i));
      int k=0;
      for(k=tmp.length()-1;k>=0;k--)
	{ if (tmp.std::string::operator[](k) != ch) { break; } }
      retVal = tmp.substr(0,k+1);
    }
    break;
  default:
    break;
  }
  return retVal;
}

void wuString::toLower ()
{
  for (std::string::size_type i=0; i<size();i++)
  {
    //GB:HP-UX-aCC,Linux-KCC 
    std::string::operator[](i) = tolower(char(std::string::operator[](i)));
    //at(i) = tolower(at(i)); 
  } 
}

void wuString::toUpper ()
{
  for (std::string::size_type i=0; i<size();i++)
  {
    //GB:HP-UX-aCC,Linux-KCC 
    std::string::operator[](i) = toupper(char(std::string::operator[](i)));
    //at(i) = toupper(at(i)); 
  }
}

bool wuString::isNull() const
{
  return empty ();
}

// "caseCompare" optional parameter is NOT implemented !
//
std::string::size_type wuString::index( const wuString& str, std::string::size_type ln, 
                          std::string::size_type st, wuString::caseCompare ) const
{
  return std::string::find( str.c_str(), st, ln );
}

std::string::size_type wuString::index (const char* str, int pos) const
{
  return std::string::find(str,pos);
}

std::string::size_type wuString::index (char ch, int pos) const
{
  return std::string::find(ch,pos);
}

unsigned int wuString::hash( caseCompare ) const
{
  const char* str=c_str();
  unsigned long h = 0;
  for ( ; *str; ++str)
    { h = 5*h + *str; }

  return std::string::size_type(h);
}

unsigned int wuString::stlhash() const
{
  const char* str=c_str();
  unsigned long h = 0;
  for ( ; *str; ++str)
    { h = 5*h + *str; }

  return std::string::size_type(h);
}

// 
// wuString.cc ends here
