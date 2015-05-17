#ifndef StPicoKFVertexFitter_h
#define StPicoKFVertexFitter_h

/* **************************************************
 *  Class to fit primary vertex using KF vertex maker
 *  
 *  Usage:
 *
 * **************************************************
 *  Authors:  
 *            **Liang He(he202@purdue.edu)
 *            Mustafa Mustafa (mmustafa@lbl.gov)
 *
 * ** Code Maintainer
 *
 * **************************************************
 */

#include <vector>
#include "StThreeVectorF.hh"
class StPicoDst;
class StPicoD0Event;
class StPicoDstMaker;
class StPicoTrack;
class StDcaGeometry;


class StPicoKFVertexFitter
{
  public:
    StPicoKFVertexFitter(StThreeVectorF *kfVertex,StPicoDstMaker* picoDstMaker);
    StPicoKFVertexFitter(StThreeVectorF *kfVertex, vector<int>& daughter, StPicoDstMaker* picoDstMaker);
    StPicoKFVertexFitter() {}
    virtual ~StPicoKFVertexFitter();
//User's function
    int primaryVertexRefit();
  private:
    StPicoDstMaker *mPicoDstMaker;
    StThreeVectorF *mKFVertex;
    StPicoDst *picoDst;
    vector<int> d0Daughters;
    StDcaGeometry *dcaG;

    ClassDef(StPicoKFVertexFitter, 1)
};

#endif
