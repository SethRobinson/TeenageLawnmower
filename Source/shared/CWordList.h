/* Copyright (C) Seth A. Robinson, 2003. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) Seth A. Robinson, 2003"
 */


/*

A class to load a text file with a ton of words in either straight cr
deliminated text or an optimized pre-sorted format.

Once the list is in memory, you can quickly check to see if a word
is on the list, or generate a list of possible words that can be made
with certain letters, while accepting a list of words to ignore. (in a word
game, this list may be words they've already used that round, for instance)

CWordsUsed is a helper class that is convient for storing a list of words
and doing some simple operatations such as "Get the longest word".

Check www.codedojo.com for info on how to use this class to make a word game.

Last updated 7-21-2003

*/


#pragma once

#include "all.h" //generic utility library, we use the replace function and some others
#include "CWordsUsed.h"
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

class CWord
{
public:
    string s_sorted_word;
    string s_word;
};


typedef vector<CWord> WordVec;
typedef WordVec::iterator WordItor;

class CWordList
{
public:

    CWordList(); //init defaults

    //functions to load and process a word list to create the optimized version (probably not used in the actual game)

    int GetMinWordLength(){return m_i_min_word_length;}
    int GetMaxWordLength(){return m_i_max_word_length;}
    void SetMaxWordLength(int i_new){m_i_max_word_length = i_new;}
    void SetMinWordLength(int i_new){m_i_min_word_length = i_new;}
    void SetStripMiscOnLoad(bool b_new) {m_b_strip_misc_on_load = b_new;} //strip out hypens and apostrophes?
    void SetRemoveAccentsOnLoad(bool b_new) {m_b_remove_accents = b_new;} //convert umlauts and things to normal versions
    void SetConvertToLowerCase(bool b_new) {m_b_convert_to_lower_case = b_new;} 
    bool ReadInWordList(const char *fname); //load words from a text file, cr deliminated
    void SortAndAddSortedWord(); //must call this if an unsorted list is loaded before using it
    void PrintList(); //debug thing
    bool SaveOutWordList(const char *fname); //saves in our snazzy format only
    int GetCountOfWordsLoaded() {return a_words.size();}

    //Functions to use the optimize word list (in the game)

    bool ReadInOptimizedWordlist(const char *fname); //read in our snazzy format
    void SetIgnoreMiscOnLoad(bool b_new){m_b_ignore_misc_on_load = b_new;}
    bool IsWordInDictionary(char *p_word); //very fast yes/no lookup
    int GenerateAnagrams(char *p_word, const CWordsUsed *p_words_to_ignore);
    int GetCountOfAnagramsFound(){return m_anagram_list.Count();}
    CWordsUsed * GetAnagramsFound(){return &m_anagram_list;}
    void GetRandomWord(char *p_word_out);
    void RemoveAccentsWord(char *p_word);
    void ConvertToLowerCaseWord(char *p_word);
    string GetWord(unsigned int ui_index);
private:

    void BuildSortedWordFromWord(CWord &new_word);
    void AddWord(char *p_word);
    void AddWordPair(char *p_word, char *p_word_optimized);
    void StripMiscWord(char *p_word);
    bool CheckThisForAnagrams(char *p_word);
    void AnagramCombination(char *str, int i_start_length);
        
    WordVec a_words; //our vector of words
    int m_i_min_word_length;
    int m_i_max_word_length;
    bool m_b_strip_misc_on_load; //if true, we'll take out apostrophies, dashes and spaces as we load
    bool m_b_ignore_misc_on_load; //only applicable to reading unoptomized lists
    bool m_b_remove_accents; //only aplicable to reading unoptomized lists
    bool m_b_convert_to_lower_case; //only aplicable to reading unoptomized lists
    char st_buffer[256]; //for misc work we need to do
    CWordsUsed m_anagram_list; //hold generated words here
    CWordsUsed *m_p_words_to_ignore_list; //optional for our anagram maker
};