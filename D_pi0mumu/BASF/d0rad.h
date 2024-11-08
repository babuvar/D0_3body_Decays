// -*- C++ -*-
//

#include "belle.h"
#include <iostream>
#include <string>
#include <iomanip>
#include <cmath>
#include <cstdio>
#include <cfloat>

#include "basf/module.h"
#include "particle/Particle.h"
#if defined(BELLE_NAMESPACE)
namespace Belle {
#endif

class BelleEvent;
class BasfOutput;
class BelleHistogram;

class d0rad: public Module {

public:

  d0rad( void );
  ~d0rad( void ) {};
  void init( int* );
  void term( void );
  void disp_stat( const char* );
  void hist_def( void );
  void event( BelleEvent*, int* );
  void begin_run( BelleEvent*, int* );
  void end_run( BelleEvent*, int* );
  void other( int*, BelleEvent*, int* );

void FillTuple(Particle& DstCand, BelleTuple *nt);
//void ConstrainedFit(Particle& DstCand);
void ConstrainedFit(Particle& DstCandi, bool &m_usable, HepPoint3D &m_pos, HepSymMatrix &m_err, double &m_chi2);//Constr.-Fit
int Find_MC_D();


  void with_imp_cut(std::vector<Particle> &list);
  HepVector param_at_ip(Particle &p);

  /// Filename for output
  char m_SkimFileName[256];

private:
    int dataType;

  //Particle Types
  static const Ptype m_ptypeD0;
  static const Ptype m_ptypeD0B;
  static const Ptype m_ptypeDstarP;
  static const Ptype m_ptypeDstarM;

  BasfOutput* m_SkimFile;

  // Event histograms
    BelleTuple *nt;


};



#if defined(BELLE_NAMESPACE)
} // namespace Belle
#endif
