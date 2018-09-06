//load, save and rank players people.  Meant to be overridden
#pragma once

#include "all.h"
   
#include <list>
#include <string>

using namespace std;


class ScoreSaveObject
{
public:
    
    ScoreSaveObject()
    {
        memset(this, 0, sizeof(ScoreSaveObject()));
    }
    int m_i_level;
    long m_l_points;
    int m_i_dragons_slain;
    char m_st_name[50];
    char m_st_buffer[50];
};

class CScoreObject
{

public:

    CScoreObject()
    {
      m_i_level = 0;
      m_l_points = 0;
    }

    virtual long GetSort(){return m_l_points;}

    string m_st_name;
    int m_i_level;
    long m_l_points;
    int m_i_dragons_slain;
    
    
};


class CRanking
{
                                         
public:

    bool IsHighScore(long l_score, int i_rank_required);
    bool LoadScores(char *p_fname);
    bool SaveScores(char *p_fname, int i_how_many_to_save);
    int Count(){return m_list_scores.size();}
    
protected:

    virtual ~CRanking(); //kill it all automatically 

    void SortList();
    CScoreObject* AddRecord();
    void Clear(); //kill all data
  
    list<CScoreObject*> m_list_scores; //hold all our scores
};

