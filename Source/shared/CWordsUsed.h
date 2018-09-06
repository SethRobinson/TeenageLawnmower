/* Copyright (C) Seth A. Robinson, 2003. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) Seth A. Robinson, 2003"
 */

//super simple class to add words to a list.  Used by CWordList
//Last updated 7-21-2003


#pragma once

#include "..\\shared\\all.h"

#include <string>
#include <vector>

using namespace std;

class CUsedWord
{
public:
    string s_word;

};


typedef vector<CUsedWord> UsedWordVec;
typedef UsedWordVec::iterator UsedWordItor;


class CWordsUsed
{
public:
    
    void AddWord(char *p_word);
    bool IsWordOnList(char *p_word);
    bool IsWordOnList(string p_word);
    int Count(){return a_word.size();}
    const string GetLastWordAdded();
    void AddWord(string p_word);
    void PrintWords();
    void Clear();
    const string GetLongestWord();
    const string GetWord(int i_index){return a_word[i_index].s_word;}
   bool Save(FILE *fp);
   bool Load(FILE *fp);
 
private:
   
    UsedWordVec a_word; //word list

};