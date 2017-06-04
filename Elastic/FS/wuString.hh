// wuString.hh --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 六 3月 22 13:47:33 2014 (+0800)
// Last-Updated: 六 3月 22 19:48:47 2014 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 7
// URL: http://wuhongyi.cn 

#ifndef _WUSTRING_H_
#define _WUSTRING_H_

#include <cstring>
using namespace std;

class wuString : public std::string
{

public: 

  enum caseCompare { exact, ignoreCase };
  enum stripType { leading, trailing, both };

  inline wuString ();
  inline wuString ( char );
  inline wuString ( const char * );
  inline wuString ( const char *, std::string::size_type );
  inline wuString ( const wuString& );
  inline wuString ( const std::string & );
  ~wuString () {}

  inline wuString& operator=(const wuString&);
  inline wuString& operator=(const std::string &);
  inline wuString& operator=(const char*);

  inline char operator () (std::string::size_type) const; 
  inline char& operator () (std::string::size_type);

  inline wuString& operator+=(const char*);
  inline wuString& operator+=(const std::string &);
  inline wuString& operator+=(const char&);
  inline bool operator==(const wuString&) const;
  inline bool operator==(const char*) const;
  inline bool operator!=(const wuString&) const;
  inline bool operator!=(const char*) const;

  inline operator const char*() const;

  inline int compareTo(const char*, caseCompare mode=exact) const;
  inline int compareTo(const wuString&, caseCompare mode=exact) const;//第一个不同的字符ASCII的差值。

  inline wuString& prepend (const char*);
  inline wuString& append (const wuString&);

  inline std::istream& readLine (std::istream&, bool skipWhite=true);
  
  inline wuString& replace (unsigned int, unsigned int, 
                             const char*, unsigned int );
  inline wuString& replace(std::string::size_type, std::string::size_type, const char*);

  inline wuString& remove(std::string::size_type);//移除第size_type个字符之后所有字符。
  inline wuString& remove(std::string::size_type, std::string::size_type);//从第size_type个字符开始移除，并且之后size_type个字符也移除。

  inline int first(char) const;
  inline int last(char) const;

  inline bool contains(const std::string&) const;
  inline bool contains(char) const;

  // stripType = 0 beginning
  // stripType = 1 end
  // stripType = 2 both
  //
  inline wuString strip (int strip_Type=trailing, char c=' ');

  inline void toLower ();
  inline void toUpper ();

  inline bool isNull() const;

  inline std::string::size_type index (const char*, int pos=0) const; 
  inline std::string::size_type index (char, int pos=0) const; 
  inline std::string::size_type index (const wuString&, std::string::size_type, std::string::size_type, caseCompare) const;

  inline const char* data() const;

  inline int strcasecompare(const char*, const char*) const;

  inline unsigned int hash( caseCompare cmp = exact ) const;
  inline unsigned int stlhash() const;
};

#endif /* _WUSTRING_H_ */
// 
// wuString.hh ends here
