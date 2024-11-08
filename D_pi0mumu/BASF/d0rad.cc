//
#include "particle/Particle.h"
#include "particle/PID.h"
#include "particle/utility.h"
#include "particle/combination.h"
#include "event/BelleEvent.h"
#include "tuple/BelleTupleManager.h"
#include "basf/module.h"
#include "basf/module_descr.h"
#include "kid/atc_pid.h"
#include "panther/panther.h"
#include "mdst/mdst.h"
#include "mdst/findKs.h"
#include "ip/IpProfile.h"
#include "belle.h"
#include "d0rad.h"
#include "benergy/BeamEnergy.h"
#include "eid/eid.h"

// system include files

#include <string>
#include <iostream>
#include <fstream>

#include "basf/basfshm.h"
#include "basf/basfout.h"
#include "tuple/BelleTupleManager.h"
#include "panther/panther.h"
#include "geninfo.h"
//#include "kfitter/ExKFitter.h"
#include RUN_INFO_H


#include HEPEVT_H
#include BELLETDF_H
#include MDST_H
#include EVTCLS_H
#include "belleutil/debugout.h"

using namespace std;
#if defined(BELLE_NAMESPACE)
namespace Belle {
#endif


  extern "C" Module_descr *mdcl_d0rad()
  {
    d0rad *module = new d0rad;
    Module_descr *dscr = new Module_descr ( "d0rad", module );
    BeamEnergy::define_global(dscr);

    return dscr;
  }



d0rad::d0rad( void ) {

//  std::strcpy( m_SkimFileName,   "d0rad.index" );

}

void d0rad::disp_stat( const char* ) {
}

void d0rad::end_run( BelleEvent*, int* ) {
}

void d0rad::other( int*, BelleEvent*, int* ) {
}

//Particle Types
  const Ptype d0rad::m_ptypeD0("D0");
  const Ptype d0rad::m_ptypeD0B("D0B");
  const Ptype d0rad::m_ptypeDstarP("D*+");
  const Ptype d0rad::m_ptypeDstarM("D*-");

void d0rad::init ( int * ) {
//  extern BasfOutputManager* BASF_Output;
//  m_SkimFile = BASF_Output->open ( m_SkimFileName );

}

void d0rad::begin_run( BelleEvent*, int* ) {
  
  IpProfile::begin_run();
  BeamEnergy::begin_run();
        eid::init_data();
//  m_SkimFile->write();
// Data or MC?
    Belle_runhead_Manager &runhead_m = Belle_runhead_Manager::get_manager();
    Belle_runhead &runhead = runhead_m( ( Panther_ID) 1 );

    if(runhead)
      {
        if(runhead.ExpMC() == 1)
          {
            dataType = 0;
            std::cout << ">>>User_ana[begin_run]: running on real data" << std::endl;
          }
        else
          {
            dataType = 1;
            std::cout << ">>>User_ana[begin_run]: running on Monte Carlo" << std::endl;
          }
      }

}

void d0rad::term( void ) {

//  delete m_SkimFile;

}

void d0rad::hist_def(void)
{
  // no histograms
  extern BelleTupleManager *BASF_Histogram;
  std::string title;
  title = "information on D candidates";
  nt = BASF_Histogram->ntuple( title.c_str(),"Exp Run Evt DstChar DMass PstDst DeltaM CThetDst PstD KsMass KsMom PiMom  m_chi2 DstID PisID DID KsID PiID DstF PisF DF KsF PiF res_x res_y res_z mc_x mc_y mc_z  mc_m_x mc_m_y mc_m_z fit_x fit_y fit_z fit_m_x fit_m_y fit_m_z", 1);


}

void d0rad::event( BelleEvent *, int *status ) {

  *status = 0;
  // ---------------- particle reconstruction and selection 

  vector<Particle>   mu_p, mu_m, k_p, k_m, pi_p, pi_m;
  makeKPi(k_p, k_m, pi_p, pi_m, 0);

  with_imp_cut(mu_p);
  with_imp_cut(mu_m);
  with_imp_cut(pi_p);
  with_imp_cut(pi_m);

//  withKaonId(k_p,0.6,3,1,5);
//  withKaonId(k_m,0.6,3,1,5);

     withMuId(mu_p,2,0.9,2);
     withMuId(mu_m,2,0.9,2);


  vector<Particle>  pi0;
  makePi0(pi0);

  for(vector<Particle>::iterator i=pi0.begin(); i!=pi0.end();++i)
    if(i->mdstPi0().gamma(0).ecl().energy()<0.05||
       i->mdstPi0().gamma(1).ecl().energy()<0.05||
       fabs(i->mdstPi0().mass()-.135)>0.025 || i->ptot()<0.3) {
      pi0.erase(i); 
      --i;
    }
  

  vector<Particle> k_s;
  Mdst_vee2_Manager &m = Mdst_vee2_Manager::get_manager();
  for (Mdst_vee2_Manager::iterator it = m.begin(); it != m.end(); it++) {
    Mdst_vee2* k0s = &(*it);
    if(k0s->kind() == 1)  // kind == 1 are K0s                                                                                                             
      {
	FindKs ffangks;
	
	ffangks.candidates(*k0s, IpProfile::position());
	
        Particle Kshort(*it);
	
	if (ffangks.goodKs()==1)
          k_s.push_back(Kshort);
      }
  }

/*  

  vector<Particle> eta;
  combination( eta, Ptype(221),gamma_low,gamma_low);
  withMassCut( eta, 0.45, 0.65);
  withPCut( eta, 0.3);
*/

  vector<Particle> D0;

  // D0 -> gamma gamma
//  combination(D0, m_ptypeD0, gamma, gamma , 0.2);
  // D0 -> phi pi0
//  combination(D0, m_ptypeD0, phi, pi0 , 0.2);
  // D0 -> phi eta
//  combination(D0, m_ptypeD0, phi, eta, 0.2);
  // D0 -> phi gamma
//  combination(D0, m_ptypeD0, phi, gamma, 0.2);
  // D0 -> K0s gamma
//  combination(D0, m_ptypeD0, k_s, gamma, 0.2);
  // D0 -> K0s pi0
  combination(D0, m_ptypeD0, pi0, mu_p, mu_m, 0.2);
  // D0 -> pi0 pi0
//  combination(D0, m_ptypeD0, pi0, pi0, 0.2);

  // D*+ -> D0 pi-
  vector<Particle> DstP;
  vector<Particle> DstM;

  combination( DstP, m_ptypeDstarP, D0, pi_p);
  combination( DstM, m_ptypeDstarM, D0, pi_m);
  withMassDifCut( DstP, 0.13, 0.165, 0);
  withMassDifCut( DstM, 0.13, 0.165, 0);
  withPSCut( DstP, 2.0);
  withPSCut( DstM, 2.0);

double massdiff;

//cout<<"Outside FillTuple loop"<<endl;

  if(DstP.size()+DstM.size()>0) {
//    m_SkimFile->write();
    *status = 1;
  for (int i=0;i<DstM.size();i++) {
//cout<<"Inside FillTuple loop"<<endl;
FillTuple(DstM[i],nt);
//ConstrainedFit(DstM[i]);
}
  for (int i=0;i<DstP.size();i++) {
//cout<<"Inside FillTuple loop"<<endl;
FillTuple(DstP[i],nt);
//ConstrainedFit(DstP[i]);
}

  }//if(DstP.size()+DstM.size()>0)

  
}



void d0rad::FillTuple(Particle& DstCand, BelleTuple *nt){

//cout<<"Inside FillTuple"<<endl;

//Fill stuff
    Belle_event_Manager& EvtMgr=Belle_event_Manager::get_manager();
    Belle_event& Evt = *EvtMgr.begin();

    int ExpNo = Evt.ExpNo();
    int RunNo = Evt.RunNo();
    int EvtNo = Evt.EvtNo();

    nt->column("Exp",(float)ExpNo);
    nt->column("Run",(float)RunNo);
    nt->column("Evt",(float)EvtNo);

    nt->column("DstChar",DstCand.charge());
    nt->column("DMass",DstCand.child(0).mass());
    nt->column("PstDst",pStar(DstCand).vect().mag());
    double massdiff=DstCand.mass()-DstCand.child(0).mass();
    nt->column("DeltaM",massdiff);
    nt->column("CThetDst",pStar(DstCand).cosTheta());//*
    nt->column("PstD",pStar(DstCand.child(0)).vect().mag());//*
/*
    nt->column("KsMass",DstCand.child(0).child(0).mass());
    nt->column("KsMom",DstCand.child(0).child(0).ptot());
    nt->column("PiMom",DstCand.child(0).child(1).ptot());

//Constrained fits
bool m_usable; HepPoint3D m_pos; HepSymMatrix m_err; double m_chi2;

//cout<<"--------------------start--------------------"<<endl;
ConstrainedFit(DstCand, m_usable, m_pos, m_err, m_chi2);
//cout<<"m_pos is "<<m_pos.x()<<"\t"<<m_pos.y()<<"\t"<<m_pos.z()<<endl;
//cout<<"--------------------end--------------------"<<endl;



nt->column("m_chi2",m_chi2);


if(dataType){


      setMCtruth(DstCand);
      nt->column("DstID",IDhep(DstCand));
      nt->column("PisID",IDhep(DstCand.child(1)));//*
      nt->column("DID",IDhep(DstCand.child(0)));
      nt->column("KsID",IDhep(DstCand.child(0).child(0)));
      nt->column("PiID",IDhep(DstCand.child(0).child(1)));

      nt->column("DstF",getMCtruthFlag(DstCand));
      nt->column("PisF",getMCtruthFlag(DstCand.child(1)));//*
      nt->column("DF",getMCtruthFlag(DstCand.child(0)));
      nt->column("KsF",getMCtruthFlag(DstCand.child(0).child(0)));
      nt->column("PiF",getMCtruthFlag(DstCand.child(0).child(1)));

Gen_hepevt_Manager& GenMgr = Gen_hepevt_Manager::get_manager();

if(getMCtruthFlag(DstCand)==1){
cout<<"-------i--------------------------------------------------"<<endl;
//cout<<"signal event"<<endl;
int Guy_ID=Find_MC_D();
//cout<<"id returned = "<<Guy_ID<<endl;
if(Guy_ID != 0){
const Gen_hepevt& Guy = GenMgr(Panther_ID(Guy_ID));
cout<<"Guy.idhep() = "<<Guy.idhep()<<endl;
const Gen_hepevt& Mother = GenMgr(Panther_ID(Guy.mo(1)));
cout<<"Mother.idhep() = "<<Mother.idhep()<<endl;
//cout<<"MC vertex is at "<<Guy.V(1)<<" "<<Guy.V(2)<<" "<<Guy.V(3)<<endl;

cout<<"---------------------------------------------------------"<<endl;

//Residuals
HepPoint3D m_pos_v(Guy.V(1),Guy.V(2),Guy.V(3));
HepPoint3D D_residual=m_pos_v-m_pos;
      nt->column("res_x",D_residual.x());
      nt->column("res_y",D_residual.y());
      nt->column("res_z",D_residual.z());

      nt->column("mc_x",Guy.V(1));
      nt->column("mc_y",Guy.V(2));
      nt->column("mc_z",Guy.V(3));

      nt->column("fit_x",m_pos.x());
      nt->column("fit_y",m_pos.y());
      nt->column("fit_z",m_pos.z());

      nt->column("mc_m_x",Mother.V(1));
      nt->column("mc_m_y",Mother.V(2));
      nt->column("mc_m_z",Mother.V(3));

}
else{
      nt->column("res_x",-999);
      nt->column("res_y",-999);
      nt->column("res_z",-999);

      nt->column("mc_x",-999);
      nt->column("mc_y",-999);
      nt->column("mc_z",-999);

      nt->column("fit_x",-999);
      nt->column("fit_y",-999);
      nt->column("fit_z",-999);

      nt->column("mc_m_x",-999);
      nt->column("mc_m_y",-999);
      nt->column("mc_m_z",-999);
}


}//if(getMCtruthFlag(DstCand)==1)
else{
      nt->column("res_x",-999);
      nt->column("res_y",-999);
      nt->column("res_z",-999);

      nt->column("mc_x",-999);
      nt->column("mc_y",-999);
      nt->column("mc_z",-999);

      nt->column("fit_x",-999);
      nt->column("fit_y",-999);
      nt->column("fit_z",-999);

      nt->column("mc_m_x",-999);
      nt->column("mc_m_y",-999);
      nt->column("mc_m_z",-999);

}


}
*/
      nt->dumpData();

}

int d0rad::Find_MC_D()
{
int maxdau=1;
Gen_hepevt_Manager& GenMgr = Gen_hepevt_Manager::get_manager();

for(int itr=1;itr<=500;itr++){
if(itr>maxdau){break;}
const Gen_hepevt& Guy = GenMgr(Panther_ID(itr));
if(fabs( Guy.idhep() )==421){
//cout<<"got the real guy, idhep() = "<<Guy.idhep()<<endl;
int da1=Guy.daFirst();
int da2= da1+1;
//cout<<"Guy_ID = "<<itr<<"\t daughter1_ID = "<<da1<<"\t daughter2_ID = "<<da2<<endl; 
if(da1 != 0 && da2 != 0){
const Gen_hepevt& Daughter1=GenMgr(Panther_ID(da1));
const Gen_hepevt& Daughter2=GenMgr(Panther_ID(da2));
//right combination is +/- 311, 111
//cout<<"Daughter-1 idhep() = "<<Daughter1.idhep()<<endl;
//cout<<"Daughter-2 idhep() = "<<Daughter2.idhep()<<endl;
int id1=Daughter1.idhep(), id2=Daughter2.idhep();
if( (id1==311 && id2==111) || (id1==-311 && id2==111) || (id1==111 && id2==311) || (id1==111 && id2==-311) ){
//cout<<"Viola! Got him!"<<endl;
return itr;

}
//Guy.idhep()<<"\t"<<Guy.daFirst()<<"\t"<<Guy.daLast()
}}

if(Guy.daLast()>maxdau){maxdau=Guy.daLast();}


}//loop over genhepevt table

return 0;
}

/*

void d0rad::ConstrainedFit(Particle& DstCand, bool &m_usable, HepPoint3D &m_pos, HepSymMatrix &m_err, double &m_chi2){//Constr.-Fit

//IP stuff
const int use_nevtdep_ip    = 1;
HepPoint3D   ip_pos     = IpProfile::position(use_nevtdep_ip);
HepSymMatrix ip_err    = IpProfile::position_err(use_nevtdep_ip);



//D daughters (1 Ks and 2 photons)
ExKFitterParticle ExK_Ks(DstCand.child(0).child(0).mdstVee2(),1);
ExKFitterParticle ExK_gam1(DstCand.child(0).child(1).child(0).mdstGamma());
ExKFitterParticle ExK_gam2(DstCand.child(0).child(1).child(1).mdstGamma());


//D vertex
 HepPoint3D D_init;
// Here we assigned it to be one 100 microns away from the Dst vertex(nominal IP)
D_init.setX(ip_pos.x() + (0.01* DstCand.child(0).p().px()/DstCand.child(0).p().rho() ));
D_init.setY(ip_pos.y() + (0.01* DstCand.child(0).p().py()/DstCand.child(0).p().rho() ));
D_init.setZ(ip_pos.z() + (0.01* DstCand.child(0).p().pz()/DstCand.child(0).p().rho() ));
ExKFitterVertex D_Vertex(D_init);


//Virtual D
	ExKFitterParticle ExK_D; // Create a virtual particle D
	ExK_D.LinkParticle(&ExK_Ks);
	ExK_D.LinkParticle(&ExK_gam1);
	ExK_D.LinkParticle(&ExK_gam2);
	ExK_D.LinkVertex(&D_Vertex);


// D vertex constraint
	ExKFitterConstrain con1; 
	con1.SetVertexConstrain();
	con1.LinkParticle(&ExK_Ks);
	con1.LinkParticle(&ExK_gam1);
	con1.LinkParticle(&ExK_gam2);
	con1.LinkVertex(&D_Vertex);

	ExKFitterMass D_Mass(1.8645);
	ExKFitterConstrain con2; // D mass constraint
	con2.SetMassConstrain();
	con2.LinkParticle(&ExK_Ks);
	con2.LinkParticle(&ExK_gam1);
	con2.LinkParticle(&ExK_gam2);
	con2.LinkVertex(&D_Vertex);
	con2.LinkMass(&D_Mass);


	ExKFitterMass Pi0_Mass(0.134976); 
	ExKFitterConstrain con3; // pi0 mass constraint
	con3.SetMassConstrain();
	con3.LinkParticle(&ExK_gam1);
	con3.LinkParticle(&ExK_gam2);
	con3.LinkVertex(&D_Vertex);
	con3.LinkMass(&Pi0_Mass);


	ExKFitter Core;
	Core.LinkConstrain(&con1);
	Core.LinkConstrain(&con2);
	Core.LinkConstrain(&con3);



	const int vtx_fit_error = Core.Minimize();
	if (!vtx_fit_error) {
		m_usable   = true;
		m_pos      = D_Vertex.Vertex();
		m_err      = D_Vertex.ErrVertex();
		m_chi2     = Core.Chisq();
		//m_effxi	   = Core.EffectiveXi(&con2);
		//int m_ndf      = Core.N_DegreeOfFreedom();
		//m_effndf   = Core.N_EffectiveDOF(&con2);
		//m_ntrk     = Core.N_VertexingTracks();

//cout<<"m_pos is "<<m_pos.x()<<"\t"<<m_pos.y()<<"\t"<<m_pos.z()<<endl;
	}



}//Constr.-Fit

*/

  void d0rad::with_imp_cut(std::vector<Particle> &list) {
    for(int i=0;i<(int)list.size();++i){
      if(list[i].mdstCharged()){
	HepVector a(param_at_ip(list[i]));
	if (!(abs(a[0])<1.0 && abs(a[3])<3.0)){
	  list.erase(list.begin()+i);
	  --i;
	}
      }
    }
  }


  HepVector d0rad::param_at_ip(Particle &p){

    const Mdst_charged charged(p.mdstCharged());

    double thisMass = p.mass();

    int hyp = 4;
    if(thisMass < 0.005){ // e = 0.000511                                                                                         
      hyp = 0;
    }else if(thisMass < 0.110){ // mu = 0.1056                                                                                               
      hyp = 1;
    }else if(thisMass < 0.200){ // pi = 0.13956                                                                                                 
      hyp = 2;
    }else if(thisMass < 0.5){ // K = 0.4936                                                                                                          
      hyp = 3;
    }
    const HepPoint3D pivot(charged.trk().mhyp(hyp).pivot_x(),
			   charged.trk().mhyp(hyp).pivot_y(),
			   charged.trk().mhyp(hyp).pivot_z());

    HepVector  a(5);
    a[0] = charged.trk().mhyp(hyp).helix(0);
    a[1] = charged.trk().mhyp(hyp).helix(1);
    a[2] = charged.trk().mhyp(hyp).helix(2);
    a[3] = charged.trk().mhyp(hyp).helix(3);
    a[4] = charged.trk().mhyp(hyp).helix(4);
    HepSymMatrix Ea(5,0);
    Ea[0][0] = charged.trk().mhyp(hyp).error(0);
    Ea[1][0] = charged.trk().mhyp(hyp).error(1);
    Ea[1][1] = charged.trk().mhyp(hyp).error(2);
    Ea[2][0] = charged.trk().mhyp(hyp).error(3);
    Ea[2][1] = charged.trk().mhyp(hyp).error(4);
    Ea[2][2] = charged.trk().mhyp(hyp).error(5);
    Ea[3][0] = charged.trk().mhyp(hyp).error(6);
    Ea[3][1] = charged.trk().mhyp(hyp).error(7);
    Ea[3][2] = charged.trk().mhyp(hyp).error(8);
    Ea[3][3] = charged.trk().mhyp(hyp).error(9);
    Ea[4][0] = charged.trk().mhyp(hyp).error(10);
    Ea[4][1] = charged.trk().mhyp(hyp).error(11);
    Ea[4][2] = charged.trk().mhyp(hyp).error(12);
    Ea[4][3] = charged.trk().mhyp(hyp).error(13);
    Ea[4][4] = charged.trk().mhyp(hyp).error(14);
    Helix helix(pivot, a, Ea);

    const Hep3Vector&   IP     = IpProfile::position();
    if (IP.mag())
      helix.pivot(IP);
    return helix.a();
  }



#if defined(BELLE_NAMESPACE)
} // namespace Belle
#endif
