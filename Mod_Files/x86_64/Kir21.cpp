/* Created by Language version: 7.7.0 */
/* VECTORIZED */
#define NRN_VECTORIZED 1
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "mech_api.h"
#undef PI
#define nil 0
#define _pval pval
// clang-format off
#include "md1redef.h"
#include "section_fwd.hpp"
#include "nrniv_mf.h"
#include "md2redef.h"
#include "nrnconf.h"
// clang-format on
#include "neuron/cache/mechanism_range.hpp"
static constexpr auto number_of_datum_variables = 3;
static constexpr auto number_of_floating_point_variables = 24;
namespace {
template <typename T>
using _nrn_mechanism_std_vector = std::vector<T>;
using _nrn_model_sorted_token = neuron::model_sorted_token;
using _nrn_mechanism_cache_range = neuron::cache::MechanismRange<number_of_floating_point_variables, number_of_datum_variables>;
using _nrn_mechanism_cache_instance = neuron::cache::MechanismInstance<number_of_floating_point_variables, number_of_datum_variables>;
using _nrn_non_owning_id_without_container = neuron::container::non_owning_identifier_without_container;
template <typename T>
using _nrn_mechanism_field = neuron::mechanism::field<T>;
template <typename... Args>
void _nrn_mechanism_register_data_fields(Args&&... args) {
  neuron::mechanism::register_data_fields(std::forward<Args>(args)...);
}
}
 
#if !NRNGPU
#undef exp
#define exp hoc_Exp
#if NRN_ENABLE_ARCH_INDEP_EXP_POW
#undef pow
#define pow hoc_pow
#endif
#endif
 
#define nrn_init _nrn_init__Kir21
#define _nrn_initial _nrn_initial__Kir21
#define nrn_cur _nrn_cur__Kir21
#define _nrn_current _nrn_current__Kir21
#define nrn_jacob _nrn_jacob__Kir21
#define nrn_state _nrn_state__Kir21
#define _net_receive _net_receive__Kir21 
#define kin kin__Kir21 
#define rate rate__Kir21 
 
#define _threadargscomma_ _ml, _iml, _ppvar, _thread, _globals, _nt,
#define _threadargsprotocomma_ Memb_list* _ml, size_t _iml, Datum* _ppvar, Datum* _thread, double* _globals, NrnThread* _nt,
#define _internalthreadargsprotocomma_ _nrn_mechanism_cache_range* _ml, size_t _iml, Datum* _ppvar, Datum* _thread, double* _globals, NrnThread* _nt,
#define _threadargs_ _ml, _iml, _ppvar, _thread, _globals, _nt
#define _threadargsproto_ Memb_list* _ml, size_t _iml, Datum* _ppvar, Datum* _thread, double* _globals, NrnThread* _nt
#define _internalthreadargsproto_ _nrn_mechanism_cache_range* _ml, size_t _iml, Datum* _ppvar, Datum* _thread, double* _globals, NrnThread* _nt
 	/*SUPPRESS 761*/
	/*SUPPRESS 762*/
	/*SUPPRESS 763*/
	/*SUPPRESS 765*/
	 extern double *hoc_getarg(int);
 
#define t _nt->_t
#define dt _nt->_dt
#define gkbar _ml->template fpfield<0>(_iml)
#define gkbar_columnindex 0
#define ik _ml->template fpfield<1>(_iml)
#define ik_columnindex 1
#define gk _ml->template fpfield<2>(_iml)
#define gk_columnindex 2
#define O _ml->template fpfield<3>(_iml)
#define O_columnindex 3
#define BS _ml->template fpfield<4>(_iml)
#define BS_columnindex 4
#define B1 _ml->template fpfield<5>(_iml)
#define B1_columnindex 5
#define B2 _ml->template fpfield<6>(_iml)
#define B2_columnindex 6
#define B3 _ml->template fpfield<7>(_iml)
#define B3_columnindex 7
#define BB _ml->template fpfield<8>(_iml)
#define BB_columnindex 8
#define DO _ml->template fpfield<9>(_iml)
#define DO_columnindex 9
#define DBS _ml->template fpfield<10>(_iml)
#define DBS_columnindex 10
#define DB1 _ml->template fpfield<11>(_iml)
#define DB1_columnindex 11
#define DB2 _ml->template fpfield<12>(_iml)
#define DB2_columnindex 12
#define DB3 _ml->template fpfield<13>(_iml)
#define DB3_columnindex 13
#define DBB _ml->template fpfield<14>(_iml)
#define DBB_columnindex 14
#define ek _ml->template fpfield<15>(_iml)
#define ek_columnindex 15
#define alpha1 _ml->template fpfield<16>(_iml)
#define alpha1_columnindex 16
#define beta1 _ml->template fpfield<17>(_iml)
#define beta1_columnindex 17
#define alphas _ml->template fpfield<18>(_iml)
#define alphas_columnindex 18
#define betas _ml->template fpfield<19>(_iml)
#define betas_columnindex 19
#define alphas2 _ml->template fpfield<20>(_iml)
#define alphas2_columnindex 20
#define betas2 _ml->template fpfield<21>(_iml)
#define betas2_columnindex 21
#define v _ml->template fpfield<22>(_iml)
#define v_columnindex 22
#define _g _ml->template fpfield<23>(_iml)
#define _g_columnindex 23
#define _ion_ek *(_ml->dptr_field<0>(_iml))
#define _p_ion_ek static_cast<neuron::container::data_handle<double>>(_ppvar[0])
#define _ion_ik *(_ml->dptr_field<1>(_iml))
#define _p_ion_ik static_cast<neuron::container::data_handle<double>>(_ppvar[1])
#define _ion_dikdv *(_ml->dptr_field<2>(_iml))
 /* Thread safe. No static _ml, _iml or _ppvar. */
 static int hoc_nrnpointerindex =  -1;
 static _nrn_mechanism_std_vector<Datum> _extcall_thread;
 static Prop* _extcall_prop;
 /* _prop_id kind of shadows _extcall_prop to allow validity checking. */
 static _nrn_non_owning_id_without_container _prop_id{};
 /* external NEURON variables */
 /* declaration of user functions */
 static void _hoc_rate(void);
 static int _mechtype;
extern void _nrn_cacheloop_reg(int, int);
extern void hoc_register_limits(int, HocParmLimits*);
extern void hoc_register_units(int, HocParmUnits*);
extern void nrn_promote(Prop*, int, int);
 
#define NMODL_TEXT 1
#if NMODL_TEXT
static void register_nmodl_text_and_filename(int mechtype);
#endif
 static void _hoc_setdata();
 /* connect user functions to hoc names */
 static VoidFunc hoc_intfunc[] = {
 {"setdata_Kir21", _hoc_setdata},
 {"rate_Kir21", _hoc_rate},
 {0, 0}
};
 
/* Direct Python call wrappers to density mechanism functions.*/
 static double _npy_rate(Prop*);
 
static NPyDirectMechFunc npy_direct_func_proc[] = {
 {"rate", _npy_rate},
 {0, 0}
};
 /* declare global and static user variables */
 #define gind 0
 #define _gth 0
#define As As_Kir21
 double As = 1;
#define b b_Kir21
 double b = 0.1;
#define cas cas_Kir21
 double cas = 1;
#define fac fac_Kir21
 double fac = 0.0005;
#define gsub gsub_Kir21
 double gsub = 0.1;
#define mg_i mg_i_Kir21
 double mg_i = 4;
#define spm_i spm_i_Kir21
 double spm_i = 1;
#define shiftmg shiftmg_Kir21
 double shiftmg = 1;
#define vshiftbs vshiftbs_Kir21
 double vshiftbs = 0;
 /* some parameters have upper and lower limits */
 static HocParmLimits _hoc_parm_limits[] = {
 {0, 0, 0}
};
 static HocParmUnits _hoc_parm_units[] = {
 {"mg_i_Kir21", "mM"},
 {"spm_i_Kir21", "uM"},
 {"vshiftbs_Kir21", "mV"},
 {"gkbar_Kir21", "S/cm2"},
 {"ik_Kir21", "mA/cm2"},
 {"gk_Kir21", "S/cm2"},
 {0, 0}
};
 static double BB0 = 0;
 static double B30 = 0;
 static double B20 = 0;
 static double B10 = 0;
 static double BS0 = 0;
 static double O0 = 0;
 static double delta_t = 0.01;
 /* connect global user variables to hoc */
 static DoubScal hoc_scdoub[] = {
 {"mg_i_Kir21", &mg_i_Kir21},
 {"spm_i_Kir21", &spm_i_Kir21},
 {"As_Kir21", &As_Kir21},
 {"vshiftbs_Kir21", &vshiftbs_Kir21},
 {"b_Kir21", &b_Kir21},
 {"fac_Kir21", &fac_Kir21},
 {"gsub_Kir21", &gsub_Kir21},
 {"shiftmg_Kir21", &shiftmg_Kir21},
 {"cas_Kir21", &cas_Kir21},
 {0, 0}
};
 static DoubVec hoc_vdoub[] = {
 {0, 0, 0}
};
 static double _sav_indep;
 extern void _nrn_setdata_reg(int, void(*)(Prop*));
 static void _setdata(Prop* _prop) {
 _extcall_prop = _prop;
 _prop_id = _nrn_get_prop_id(_prop);
 }
 static void _hoc_setdata() {
 Prop *_prop, *hoc_getdata_range(int);
 _prop = hoc_getdata_range(_mechtype);
   _setdata(_prop);
 hoc_retpushx(1.);
}
 static void nrn_alloc(Prop*);
static void nrn_init(_nrn_model_sorted_token const&, NrnThread*, Memb_list*, int);
static void nrn_state(_nrn_model_sorted_token const&, NrnThread*, Memb_list*, int);
 static void nrn_cur(_nrn_model_sorted_token const&, NrnThread*, Memb_list*, int);
static void nrn_jacob(_nrn_model_sorted_token const&, NrnThread*, Memb_list*, int);
 
static int _ode_count(int);
static void _ode_map(Prop*, int, neuron::container::data_handle<double>*, neuron::container::data_handle<double>*, double*, int);
static void _ode_spec(_nrn_model_sorted_token const&, NrnThread*, Memb_list*, int);
static void _ode_matsol(_nrn_model_sorted_token const&, NrnThread*, Memb_list*, int);
 
#define _cvode_ieq _ppvar[3].literal_value<int>()
 static void _ode_matsol_instance1(_internalthreadargsproto_);
 /* connect range variables in _p that hoc is supposed to know about */
 static const char *_mechanism[] = {
 "7.7.0",
"Kir21",
 "gkbar_Kir21",
 0,
 "ik_Kir21",
 "gk_Kir21",
 0,
 "O_Kir21",
 "BS_Kir21",
 "B1_Kir21",
 "B2_Kir21",
 "B3_Kir21",
 "BB_Kir21",
 0,
 0};
 static Symbol* _k_sym;
 
 /* Used by NrnProperty */
 static _nrn_mechanism_std_vector<double> _parm_default{
     0.00015, /* gkbar */
 }; 
 
 
extern Prop* need_memb(Symbol*);
static void nrn_alloc(Prop* _prop) {
  Prop *prop_ion{};
  Datum *_ppvar{};
   _ppvar = nrn_prop_datum_alloc(_mechtype, 4, _prop);
    _nrn_mechanism_access_dparam(_prop) = _ppvar;
     _nrn_mechanism_cache_instance _ml_real{_prop};
    auto* const _ml = &_ml_real;
    size_t const _iml{};
    assert(_nrn_mechanism_get_num_vars(_prop) == 24);
 	/*initialize range parameters*/
 	gkbar = _parm_default[0]; /* 0.00015 */
 	 assert(_nrn_mechanism_get_num_vars(_prop) == 24);
 	_nrn_mechanism_access_dparam(_prop) = _ppvar;
 	/*connect ionic variables to this model*/
 prop_ion = need_memb(_k_sym);
 nrn_promote(prop_ion, 0, 1);
 	_ppvar[0] = _nrn_mechanism_get_param_handle(prop_ion, 0); /* ek */
 	_ppvar[1] = _nrn_mechanism_get_param_handle(prop_ion, 3); /* ik */
 	_ppvar[2] = _nrn_mechanism_get_param_handle(prop_ion, 4); /* _ion_dikdv */
 
}
 static void _initlists();
  /* some states have an absolute tolerance */
 static Symbol** _atollist;
 static HocStateTolerance _hoc_state_tol[] = {
 {0, 0}
};
 static void _thread_cleanup(Datum*);
 extern Symbol* hoc_lookup(const char*);
extern void _nrn_thread_reg(int, int, void(*)(Datum*));
void _nrn_thread_table_reg(int, nrn_thread_table_check_t);
extern void hoc_register_tolerance(int, HocStateTolerance*, Symbol***);
extern void _cvode_abstol( Symbol**, double*, int);

 extern "C" void _Kir21_reg() {
	int _vectorized = 1;
  _initlists();
 	ion_reg("k", -10000.);
 	_k_sym = hoc_lookup("k_ion");
 	register_mech(_mechanism, nrn_alloc,nrn_cur, nrn_jacob, nrn_state, nrn_init, hoc_nrnpointerindex, 3);
  _extcall_thread.resize(2);
 _mechtype = nrn_get_mechtype(_mechanism[1]);
 hoc_register_parm_default(_mechtype, &_parm_default);
         hoc_register_npy_direct(_mechtype, npy_direct_func_proc);
     _nrn_setdata_reg(_mechtype, _setdata);
     _nrn_thread_reg(_mechtype, 0, _thread_cleanup);
 #if NMODL_TEXT
  register_nmodl_text_and_filename(_mechtype);
#endif
   _nrn_mechanism_register_data_fields(_mechtype,
                                       _nrn_mechanism_field<double>{"gkbar"} /* 0 */,
                                       _nrn_mechanism_field<double>{"ik"} /* 1 */,
                                       _nrn_mechanism_field<double>{"gk"} /* 2 */,
                                       _nrn_mechanism_field<double>{"O"} /* 3 */,
                                       _nrn_mechanism_field<double>{"BS"} /* 4 */,
                                       _nrn_mechanism_field<double>{"B1"} /* 5 */,
                                       _nrn_mechanism_field<double>{"B2"} /* 6 */,
                                       _nrn_mechanism_field<double>{"B3"} /* 7 */,
                                       _nrn_mechanism_field<double>{"BB"} /* 8 */,
                                       _nrn_mechanism_field<double>{"DO"} /* 9 */,
                                       _nrn_mechanism_field<double>{"DBS"} /* 10 */,
                                       _nrn_mechanism_field<double>{"DB1"} /* 11 */,
                                       _nrn_mechanism_field<double>{"DB2"} /* 12 */,
                                       _nrn_mechanism_field<double>{"DB3"} /* 13 */,
                                       _nrn_mechanism_field<double>{"DBB"} /* 14 */,
                                       _nrn_mechanism_field<double>{"ek"} /* 15 */,
                                       _nrn_mechanism_field<double>{"alpha1"} /* 16 */,
                                       _nrn_mechanism_field<double>{"beta1"} /* 17 */,
                                       _nrn_mechanism_field<double>{"alphas"} /* 18 */,
                                       _nrn_mechanism_field<double>{"betas"} /* 19 */,
                                       _nrn_mechanism_field<double>{"alphas2"} /* 20 */,
                                       _nrn_mechanism_field<double>{"betas2"} /* 21 */,
                                       _nrn_mechanism_field<double>{"v"} /* 22 */,
                                       _nrn_mechanism_field<double>{"_g"} /* 23 */,
                                       _nrn_mechanism_field<double*>{"_ion_ek", "k_ion"} /* 0 */,
                                       _nrn_mechanism_field<double*>{"_ion_ik", "k_ion"} /* 1 */,
                                       _nrn_mechanism_field<double*>{"_ion_dikdv", "k_ion"} /* 2 */,
                                       _nrn_mechanism_field<int>{"_cvode_ieq", "cvodeieq"} /* 3 */);
  hoc_register_prop_size(_mechtype, 24, 4);
  hoc_register_dparam_semantics(_mechtype, 0, "k_ion");
  hoc_register_dparam_semantics(_mechtype, 1, "k_ion");
  hoc_register_dparam_semantics(_mechtype, 2, "k_ion");
  hoc_register_dparam_semantics(_mechtype, 3, "cvodeieq");
 	hoc_register_cvode(_mechtype, _ode_count, _ode_map, _ode_spec, _ode_matsol);
 	hoc_register_tolerance(_mechtype, _hoc_state_tol, &_atollist);
 
    hoc_register_var(hoc_scdoub, hoc_vdoub, hoc_intfunc);
 	ivoc_help("help ?1 Kir21 /home/raven/PycharmProjects/Masters_model/Mod_Files_Beining_2017/Kir21.mod\n");
 hoc_register_limits(_mechtype, _hoc_parm_limits);
 hoc_register_units(_mechtype, _hoc_parm_units);
 }
static int _reset;
static const char *modelname = "Kir potassium current";

static int error;
static int _ninits = 0;
static int _match_recurse=1;
static void _modl_cleanup(){ _match_recurse=1;}
static int rate(_internalthreadargsprotocomma_ double);
 
#define _MATELM1(_row,_col) *(_nrn_thread_getelm(static_cast<SparseObj*>(_so), _row + 1, _col + 1))
 
#define _RHS1(_arg) _rhs[_arg+1]
 static int _cvspth1 = 1;
 
static int _ode_spec1(_internalthreadargsproto_);
/*static int _ode_matsol1(_internalthreadargsproto_);*/
 
#define _MATELM1(_row,_col) *(_nrn_thread_getelm(static_cast<SparseObj*>(_so), _row + 1, _col + 1))
 
#define _RHS1(_arg) _rhs[_arg+1]
  
#define _linmat1  1
 static int _spth1 = 0;
 static neuron::container::field_index _slist1[6], _dlist1[6]; static double *_temp1;
 static int kin (void* _so, double* _rhs, _internalthreadargsproto_);
 
static int kin (void* _so, double* _rhs, _internalthreadargsproto_)
 {int _reset=0;
 {
   double _lalpha2 , _lalpha3 , _lbeta2 , _lbeta3 ;
 double b_flux, f_flux, _term; int _i;
 {int _i; double _dt1 = 1.0/dt;
for(_i=1;_i<6;_i++){
  	_RHS1(_i) = -_dt1*(_ml->data(_iml, _slist1[_i]) - _ml->data(_iml, _dlist1[_i]));
	_MATELM1(_i, _i) = _dt1;
      
} }
 rate ( _threadargscomma_ v ) ;
   _lalpha2 = 2.0 * alpha1 ;
   _lbeta2 = 2.0 * beta1 ;
   _lalpha3 = 3.0 * alpha1 ;
   _lbeta3 = 3.0 * beta1 ;
   /* ~ BS <-> O ( alphas , betas )*/
 f_flux =  alphas * BS ;
 b_flux =  betas * O ;
 _RHS1( 4) -= (f_flux - b_flux);
 _RHS1( 5) += (f_flux - b_flux);
 
 _term =  alphas ;
 _MATELM1( 4 ,4)  += _term;
 _MATELM1( 5 ,4)  -= _term;
 _term =  betas ;
 _MATELM1( 4 ,5)  -= _term;
 _MATELM1( 5 ,5)  += _term;
 /*REACTION*/
  /* ~ B1 <-> O ( alpha1 , _lbeta3 )*/
 f_flux =  alpha1 * B1 ;
 b_flux =  _lbeta3 * O ;
 _RHS1( 3) -= (f_flux - b_flux);
 _RHS1( 5) += (f_flux - b_flux);
 
 _term =  alpha1 ;
 _MATELM1( 3 ,3)  += _term;
 _MATELM1( 5 ,3)  -= _term;
 _term =  _lbeta3 ;
 _MATELM1( 3 ,5)  -= _term;
 _MATELM1( 5 ,5)  += _term;
 /*REACTION*/
  /* ~ B2 <-> B1 ( _lalpha2 , _lbeta2 )*/
 f_flux =  _lalpha2 * B2 ;
 b_flux =  _lbeta2 * B1 ;
 _RHS1( 2) -= (f_flux - b_flux);
 _RHS1( 3) += (f_flux - b_flux);
 
 _term =  _lalpha2 ;
 _MATELM1( 2 ,2)  += _term;
 _MATELM1( 3 ,2)  -= _term;
 _term =  _lbeta2 ;
 _MATELM1( 2 ,3)  -= _term;
 _MATELM1( 3 ,3)  += _term;
 /*REACTION*/
  /* ~ B3 <-> B2 ( _lalpha3 , beta1 )*/
 f_flux =  _lalpha3 * B3 ;
 b_flux =  beta1 * B2 ;
 _RHS1( 2) += (f_flux - b_flux);
 
 _term =  _lalpha3 ;
 _MATELM1( 2 ,0)  -= _term;
 _term =  beta1 ;
 _MATELM1( 2 ,2)  += _term;
 /*REACTION*/
  /* ~ BB <-> BS ( alphas2 , betas2 )*/
 f_flux =  alphas2 * BB ;
 b_flux =  betas2 * BS ;
 _RHS1( 1) -= (f_flux - b_flux);
 _RHS1( 4) += (f_flux - b_flux);
 
 _term =  alphas2 ;
 _MATELM1( 1 ,1)  += _term;
 _MATELM1( 4 ,1)  -= _term;
 _term =  betas2 ;
 _MATELM1( 1 ,4)  -= _term;
 _MATELM1( 4 ,4)  += _term;
 /*REACTION*/
   /* O + BS + BB + B1 + B2 + B3 = 1.0 */
 _RHS1(0) =  1.0;
 _MATELM1(0, 0) = 1;
 _RHS1(0) -= B3 ;
 _MATELM1(0, 2) = 1;
 _RHS1(0) -= B2 ;
 _MATELM1(0, 3) = 1;
 _RHS1(0) -= B1 ;
 _MATELM1(0, 1) = 1;
 _RHS1(0) -= BB ;
 _MATELM1(0, 4) = 1;
 _RHS1(0) -= BS ;
 _MATELM1(0, 5) = 1;
 _RHS1(0) -= O ;
 /*CONSERVATION*/
   } return _reset;
 }
 
static int  rate ( _internalthreadargsprotocomma_ double _lv ) {
   double _la , _ld ;
 alpha1 = 12.0 * exp ( - 0.025 * ( _lv - ( shiftmg * ( ek ) ) ) ) ;
   beta1 = mg_i / 8.0 * 28.0 * exp ( 0.025 * ( _lv - ( shiftmg * ( ek ) ) ) ) ;
   alphas = As * 0.17 * exp ( cas * - 0.07 * ( _lv - ( ek ) + 8.0 / 8.0 * mg_i ) ) ;
   betas = As * spm_i * 0.28 * exp ( 0.15 * ( _lv - ( ek ) + 8.0 / 8.0 * mg_i ) ) ;
   _la = - 1.0 / 9.1 + b ;
   alphas2 = fac * 40.0 * exp ( _la * ( _lv - ( ek + vshiftbs ) ) ) ;
   betas2 = spm_i * fac * exp ( b * ( _lv - ( ek + vshiftbs ) ) ) ;
    return 0; }
 
static void _hoc_rate(void) {
  double _r;
 Datum* _ppvar; Datum* _thread; NrnThread* _nt;
 
  Prop* _local_prop = _prop_id ? _extcall_prop : nullptr;
  _nrn_mechanism_cache_instance _ml_real{_local_prop};
auto* const _ml = &_ml_real;
size_t const _iml{};
_ppvar = _local_prop ? _nrn_mechanism_access_dparam(_local_prop) : nullptr;
_thread = _extcall_thread.data();
double* _globals = nullptr;
if (gind != 0 && _thread != nullptr) { _globals = _thread[_gth].get<double*>(); }
_nt = nrn_threads;
 _r = 1.;
 rate ( _threadargscomma_ *getarg(1) );
 hoc_retpushx(_r);
}
 
static double _npy_rate(Prop* _prop) {
    double _r{0.0};
 Datum* _ppvar; Datum* _thread; NrnThread* _nt;
 _nrn_mechanism_cache_instance _ml_real{_prop};
auto* const _ml = &_ml_real;
size_t const _iml{};
_ppvar = _nrn_mechanism_access_dparam(_prop);
_thread = _extcall_thread.data();
double* _globals = nullptr;
if (gind != 0 && _thread != nullptr) { _globals = _thread[_gth].get<double*>(); }
_nt = nrn_threads;
 _r = 1.;
 rate ( _threadargscomma_ *getarg(1) );
 return(_r);
}
 
/*CVODE ode begin*/
 static int _ode_spec1(_internalthreadargsproto_) {
  int _reset=0;
  {
 double _lalpha2 , _lalpha3 , _lbeta2 , _lbeta3 ;
 double b_flux, f_flux, _term; int _i;
 {int _i; for(_i=0;_i<6;_i++) _ml->data(_iml, _dlist1[_i]) = 0.0;}
 rate ( _threadargscomma_ v ) ;
 _lalpha2 = 2.0 * alpha1 ;
 _lbeta2 = 2.0 * beta1 ;
 _lalpha3 = 3.0 * alpha1 ;
 _lbeta3 = 3.0 * beta1 ;
 /* ~ BS <-> O ( alphas , betas )*/
 f_flux =  alphas * BS ;
 b_flux =  betas * O ;
 DBS -= (f_flux - b_flux);
 DO += (f_flux - b_flux);
 
 /*REACTION*/
  /* ~ B1 <-> O ( alpha1 , _lbeta3 )*/
 f_flux =  alpha1 * B1 ;
 b_flux =  _lbeta3 * O ;
 DB1 -= (f_flux - b_flux);
 DO += (f_flux - b_flux);
 
 /*REACTION*/
  /* ~ B2 <-> B1 ( _lalpha2 , _lbeta2 )*/
 f_flux =  _lalpha2 * B2 ;
 b_flux =  _lbeta2 * B1 ;
 DB2 -= (f_flux - b_flux);
 DB1 += (f_flux - b_flux);
 
 /*REACTION*/
  /* ~ B3 <-> B2 ( _lalpha3 , beta1 )*/
 f_flux =  _lalpha3 * B3 ;
 b_flux =  beta1 * B2 ;
 DB3 -= (f_flux - b_flux);
 DB2 += (f_flux - b_flux);
 
 /*REACTION*/
  /* ~ BB <-> BS ( alphas2 , betas2 )*/
 f_flux =  alphas2 * BB ;
 b_flux =  betas2 * BS ;
 DBB -= (f_flux - b_flux);
 DBS += (f_flux - b_flux);
 
 /*REACTION*/
   /* O + BS + BB + B1 + B2 + B3 = 1.0 */
 /*CONSERVATION*/
   } return _reset;
 }
 
/*CVODE matsol*/
 static int _ode_matsol1(void* _so, double* _rhs, _internalthreadargsproto_) {int _reset=0;{
 double _lalpha2 , _lalpha3 , _lbeta2 , _lbeta3 ;
 double b_flux, f_flux, _term; int _i;
   b_flux = f_flux = 0.;
 {int _i; double _dt1 = 1.0/dt;
for(_i=0;_i<6;_i++){
  	_RHS1(_i) = _dt1*(_ml->data(_iml, _dlist1[_i]));
	_MATELM1(_i, _i) = _dt1;
      
} }
 rate ( _threadargscomma_ v ) ;
 _lalpha2 = 2.0 * alpha1 ;
 _lbeta2 = 2.0 * beta1 ;
 _lalpha3 = 3.0 * alpha1 ;
 _lbeta3 = 3.0 * beta1 ;
 /* ~ BS <-> O ( alphas , betas )*/
 _term =  alphas ;
 _MATELM1( 4 ,4)  += _term;
 _MATELM1( 5 ,4)  -= _term;
 _term =  betas ;
 _MATELM1( 4 ,5)  -= _term;
 _MATELM1( 5 ,5)  += _term;
 /*REACTION*/
  /* ~ B1 <-> O ( alpha1 , _lbeta3 )*/
 _term =  alpha1 ;
 _MATELM1( 3 ,3)  += _term;
 _MATELM1( 5 ,3)  -= _term;
 _term =  _lbeta3 ;
 _MATELM1( 3 ,5)  -= _term;
 _MATELM1( 5 ,5)  += _term;
 /*REACTION*/
  /* ~ B2 <-> B1 ( _lalpha2 , _lbeta2 )*/
 _term =  _lalpha2 ;
 _MATELM1( 2 ,2)  += _term;
 _MATELM1( 3 ,2)  -= _term;
 _term =  _lbeta2 ;
 _MATELM1( 2 ,3)  -= _term;
 _MATELM1( 3 ,3)  += _term;
 /*REACTION*/
  /* ~ B3 <-> B2 ( _lalpha3 , beta1 )*/
 _term =  _lalpha3 ;
 _MATELM1( 0 ,0)  += _term;
 _MATELM1( 2 ,0)  -= _term;
 _term =  beta1 ;
 _MATELM1( 0 ,2)  -= _term;
 _MATELM1( 2 ,2)  += _term;
 /*REACTION*/
  /* ~ BB <-> BS ( alphas2 , betas2 )*/
 _term =  alphas2 ;
 _MATELM1( 1 ,1)  += _term;
 _MATELM1( 4 ,1)  -= _term;
 _term =  betas2 ;
 _MATELM1( 1 ,4)  -= _term;
 _MATELM1( 4 ,4)  += _term;
 /*REACTION*/
   /* O + BS + BB + B1 + B2 + B3 = 1.0 */
 /*CONSERVATION*/
   } return _reset;
 }
 
/*CVODE end*/
 
static int _ode_count(int _type){ return 6;}
 
static void _ode_spec(_nrn_model_sorted_token const& _sorted_token, NrnThread* _nt, Memb_list* _ml_arg, int _type) {
   Datum* _ppvar;
   size_t _iml;   _nrn_mechanism_cache_range* _ml;   Node* _nd{};
  double _v{};
  int _cntml;
  _nrn_mechanism_cache_range _lmr{_sorted_token, *_nt, *_ml_arg, _type};
  _ml = &_lmr;
  _cntml = _ml_arg->_nodecount;
  Datum *_thread{_ml_arg->_thread};
  double* _globals = nullptr;
  if (gind != 0 && _thread != nullptr) { _globals = _thread[_gth].get<double*>(); }
  for (_iml = 0; _iml < _cntml; ++_iml) {
    _ppvar = _ml_arg->_pdata[_iml];
    _nd = _ml_arg->_nodelist[_iml];
    v = NODEV(_nd);
  ek = _ion_ek;
     _ode_spec1 (_threadargs_);
  }}
 
static void _ode_map(Prop* _prop, int _ieq, neuron::container::data_handle<double>* _pv, neuron::container::data_handle<double>* _pvdot, double* _atol, int _type) { 
  Datum* _ppvar;
  _ppvar = _nrn_mechanism_access_dparam(_prop);
  _cvode_ieq = _ieq;
  for (int _i=0; _i < 6; ++_i) {
    _pv[_i] = _nrn_mechanism_get_param_handle(_prop, _slist1[_i]);
    _pvdot[_i] = _nrn_mechanism_get_param_handle(_prop, _dlist1[_i]);
    _cvode_abstol(_atollist, _atol, _i);
  }
 }
 
static void _ode_matsol_instance1(_internalthreadargsproto_) {
 _cvode_sparse_thread(&(_thread[_cvspth1].literal_value<void*>()), 6, _dlist1, neuron::scopmath::row_view{_ml, _iml}, _ode_matsol1, _threadargs_);
 }
 
static void _ode_matsol(_nrn_model_sorted_token const& _sorted_token, NrnThread* _nt, Memb_list* _ml_arg, int _type) {
   Datum* _ppvar;
   size_t _iml;   _nrn_mechanism_cache_range* _ml;   Node* _nd{};
  double _v{};
  int _cntml;
  _nrn_mechanism_cache_range _lmr{_sorted_token, *_nt, *_ml_arg, _type};
  _ml = &_lmr;
  _cntml = _ml_arg->_nodecount;
  Datum *_thread{_ml_arg->_thread};
  double* _globals = nullptr;
  if (gind != 0 && _thread != nullptr) { _globals = _thread[_gth].get<double*>(); }
  for (_iml = 0; _iml < _cntml; ++_iml) {
    _ppvar = _ml_arg->_pdata[_iml];
    _nd = _ml_arg->_nodelist[_iml];
    v = NODEV(_nd);
  ek = _ion_ek;
 _ode_matsol_instance1(_threadargs_);
 }}
 
static void _thread_cleanup(Datum* _thread) {
   _nrn_destroy_sparseobj_thread(static_cast<SparseObj*>(_thread[_spth1].get<void*>()));
   _nrn_destroy_sparseobj_thread(static_cast<SparseObj*>(_thread[_cvspth1].get<void*>()));
 }

static void initmodel(_internalthreadargsproto_) {
  int _i; double _save;{
  BB = BB0;
  B3 = B30;
  B2 = B20;
  B1 = B10;
  BS = BS0;
  O = O0;
 {
   rate ( _threadargscomma_ v ) ;
    _ss_sparse_thread(&(_thread[_spth1].literal_value<void*>()), 6, _slist1, _dlist1, neuron::scopmath::row_view{_ml, _iml}, &t, dt, kin, _linmat1, _threadargs_);
     if (secondorder) {
    int _i;
    for (_i = 0; _i < 6; ++_i) {
      _ml->data(_iml, _slist1[_i]) += dt*_ml->data(_iml, _dlist1[_i]);
    }}
 }
 
}
}

static void nrn_init(_nrn_model_sorted_token const& _sorted_token, NrnThread* _nt, Memb_list* _ml_arg, int _type){
_nrn_mechanism_cache_range _lmr{_sorted_token, *_nt, *_ml_arg, _type};
auto* const _vec_v = _nt->node_voltage_storage();
auto* const _ml = &_lmr;
Datum* _ppvar; Datum* _thread;
Node *_nd; double _v; int* _ni; int _iml, _cntml;
_ni = _ml_arg->_nodeindices;
_cntml = _ml_arg->_nodecount;
_thread = _ml_arg->_thread;
double* _globals = nullptr;
if (gind != 0 && _thread != nullptr) { _globals = _thread[_gth].get<double*>(); }
for (_iml = 0; _iml < _cntml; ++_iml) {
 _ppvar = _ml_arg->_pdata[_iml];
   _v = _vec_v[_ni[_iml]];
 v = _v;
  ek = _ion_ek;
 initmodel(_threadargs_);
 }
}

static double _nrn_current(_internalthreadargsprotocomma_ double _v) {
double _current=0.; v=_v;
{ {
   gk = ( gkbar ) * ( O + 1.0 / 3.0 * B2 + 2.0 / 3.0 * B1 ) + ( gkbar * gsub ) * BS ;
   ik = gk * ( v - ek ) ;
   }
 _current += ik;

} return _current;
}

static void nrn_cur(_nrn_model_sorted_token const& _sorted_token, NrnThread* _nt, Memb_list* _ml_arg, int _type) {
_nrn_mechanism_cache_range _lmr{_sorted_token, *_nt, *_ml_arg, _type};
auto const _vec_rhs = _nt->node_rhs_storage();
auto const _vec_sav_rhs = _nt->node_sav_rhs_storage();
auto const _vec_v = _nt->node_voltage_storage();
auto* const _ml = &_lmr;
Datum* _ppvar; Datum* _thread;
Node *_nd; int* _ni; double _rhs, _v; int _iml, _cntml;
_ni = _ml_arg->_nodeindices;
_cntml = _ml_arg->_nodecount;
_thread = _ml_arg->_thread;
double* _globals = nullptr;
if (gind != 0 && _thread != nullptr) { _globals = _thread[_gth].get<double*>(); }
for (_iml = 0; _iml < _cntml; ++_iml) {
 _ppvar = _ml_arg->_pdata[_iml];
   _v = _vec_v[_ni[_iml]];
  ek = _ion_ek;
 auto const _g_local = _nrn_current(_threadargscomma_ _v + .001);
 	{ double _dik;
  _dik = ik;
 _rhs = _nrn_current(_threadargscomma_ _v);
  _ion_dikdv += (_dik - ik)/.001 ;
 	}
 _g = (_g_local - _rhs)/.001;
  _ion_ik += ik ;
	 _vec_rhs[_ni[_iml]] -= _rhs;
 
}
 
}

static void nrn_jacob(_nrn_model_sorted_token const& _sorted_token, NrnThread* _nt, Memb_list* _ml_arg, int _type) {
_nrn_mechanism_cache_range _lmr{_sorted_token, *_nt, *_ml_arg, _type};
auto const _vec_d = _nt->node_d_storage();
auto const _vec_sav_d = _nt->node_sav_d_storage();
auto* const _ml = &_lmr;
Datum* _ppvar; Datum* _thread;
Node *_nd; int* _ni; int _iml, _cntml;
_ni = _ml_arg->_nodeindices;
_cntml = _ml_arg->_nodecount;
_thread = _ml_arg->_thread;
double* _globals = nullptr;
if (gind != 0 && _thread != nullptr) { _globals = _thread[_gth].get<double*>(); }
for (_iml = 0; _iml < _cntml; ++_iml) {
  _vec_d[_ni[_iml]] += _g;
 
}
 
}

static void nrn_state(_nrn_model_sorted_token const& _sorted_token, NrnThread* _nt, Memb_list* _ml_arg, int _type) {
_nrn_mechanism_cache_range _lmr{_sorted_token, *_nt, *_ml_arg, _type};
auto* const _vec_v = _nt->node_voltage_storage();
auto* const _ml = &_lmr;
Datum* _ppvar; Datum* _thread;
Node *_nd; double _v = 0.0; int* _ni;
double _dtsav = dt;
if (secondorder) { dt *= 0.5; }
_ni = _ml_arg->_nodeindices;
size_t _cntml = _ml_arg->_nodecount;
_thread = _ml_arg->_thread;
double* _globals = nullptr;
if (gind != 0 && _thread != nullptr) { _globals = _thread[_gth].get<double*>(); }
for (size_t _iml = 0; _iml < _cntml; ++_iml) {
 _ppvar = _ml_arg->_pdata[_iml];
 _nd = _ml_arg->_nodelist[_iml];
   _v = _vec_v[_ni[_iml]];
 v=_v;
{
  ek = _ion_ek;
 {  sparse_thread(&(_thread[_spth1].literal_value<void*>()), 6, _slist1, _dlist1, neuron::scopmath::row_view{_ml, _iml}, &t, dt, kin, _linmat1, _threadargs_);
     if (secondorder) {
    int _i;
    for (_i = 0; _i < 6; ++_i) {
      _ml->data(_iml, _slist1[_i]) += dt*_ml->data(_iml, _dlist1[_i]);
    }}
 } }}
 dt = _dtsav;
}

static void terminal(){}

static void _initlists(){
 int _i; static int _first = 1;
  if (!_first) return;
 _slist1[0] = {B3_columnindex, 0};  _dlist1[0] = {DB3_columnindex, 0};
 _slist1[1] = {BB_columnindex, 0};  _dlist1[1] = {DBB_columnindex, 0};
 _slist1[2] = {B2_columnindex, 0};  _dlist1[2] = {DB2_columnindex, 0};
 _slist1[3] = {B1_columnindex, 0};  _dlist1[3] = {DB1_columnindex, 0};
 _slist1[4] = {BS_columnindex, 0};  _dlist1[4] = {DBS_columnindex, 0};
 _slist1[5] = {O_columnindex, 0};  _dlist1[5] = {DO_columnindex, 0};
_first = 0;
}

#if NMODL_TEXT
static void register_nmodl_text_and_filename(int mech_type) {
    const char* nmodl_filename = "/home/raven/PycharmProjects/Masters_model/Mod_Files_Beining_2017/Kir21.mod";
    const char* nmodl_file_text = 
  "TITLE Kir potassium current\n"
  "\n"
  "COMMENT\n"
  "\n"
  "Kir 2.1 (Mg high-affinity) model\n"
  "from Beining et al (2016), \"A novel comprehensive and consistent electrophysiologcal model of dentate granule cells\"\n"
  "\n"
  "based on\n"
  "Yan & Ishihara (2005): Two Kir2.1 channel populations with different sensitivities to Mg(2+) and polyamine block: a model for the cardiac strong inward rectifier K(+) channel. , Journal of physiology\n"
  "and Liu 2012\n"
  "\n"
  "ENDCOMMENT\n"
  "\n"
  "NEURON {\n"
  "	SUFFIX Kir21\n"
  "	USEION k READ ek WRITE ik\n"
  "    RANGE  ik, gk, gkbar:, O, BS, B1, B2, B3\n"
  "	GLOBAL mg_i, As, shiftmg, cas,fac, gsub, b, spm_i, vshiftbs\n"
  "}\n"
  "\n"
  "UNITS {\n"
  "	(mA) = (milliamp)\n"
  "	(mV) = (millivolt)\n"
  "        (S)  = (siemens)\n"
  "	\n"
  "	(molar) = (1/liter)\n"
  "	(mM) = (millimolar)\n"
  "	(uM) = (micromolar)\n"
  "	\n"
  "}\n"
  "\n"
  "PARAMETER {\n"
  "	v 		(mV)\n"
  "	gkbar  = 0.00015                (S/cm2) :     	\n"
  "	mg_i = 4 (mM)  : in Mongiat 2009\n"
  "	spm_i = 1 (uM) : intracellular polyamine concentration (Yan&Ishihara 2005) Liu 2012 says physiologic is 5-10uM bzw 0.1-1uM spmd\n"
  "	As = 1\n"
  "	vshiftbs = 0 (mV)\n"
  "	b= 0.1  : close to 0 makes tau big and shifts to right, b=0.1099 makes boltzmann tau to the right\n"
  "	:c = -100 (mV)   : seems plausible\n"
  "	\n"
  "	fac = 0.0005  : this influences tau a lot! make it smaller for bigger tau\n"
  "	gsub = 0.1  : factor of sub state conductance 0.05-0.055 fuer spermin und 0.15-0.155 fuer spermidin\n"
  "	shiftmg = 1 : 0 for normal 1 for shift to ek\n"
  "	cas = 1  \n"
  "}\n"
  "\n"
  "STATE {\n"
  "        O BS B1 B2 B3 BB\n"
  "}\n"
  "\n"
  "ASSIGNED {\n"
  "        : ki                              (mM)\n"
  "        : ko                              (mM)\n"
  "        ik                             (mA/cm2)\n"
  "        gk                            (S/cm2)\n"
  "        ek                            (mV)\n"
  "		alpha1   					(/ms)\n"
  "		beta1						(/ms)\n"
  "		alphas   					(/ms)\n"
  "		betas   					(/ms)\n"
  "		alphas2 					(/ms)\n"
  "		betas2						(/ms)\n"
  "}\n"
  "\n"
  "INITIAL {\n"
  "	rate(v)\n"
  "	SOLVE kin STEADYSTATE sparse\n"
  "}\n"
  "\n"
  "BREAKPOINT {\n"
  "	SOLVE kin METHOD sparse\n"
  "	gk = (gkbar ) * (O + 1/3 * B2 + 2/3 * B1) + (gkbar * gsub ) * BS \n"
  "    ik = gk * ( v - ek )\n"
  "}\n"
  "\n"
  "\n"
  "KINETIC kin {\n"
  "LOCAL alpha2, alpha3, beta2, beta3\n"
  "rate(v)\n"
  "\n"
  "alpha2 = 2*alpha1\n"
  "beta2 = 2 * beta1\n"
  "alpha3 = 3*alpha1\n"
  "beta3 = 3*beta1\n"
  "\n"
  "\n"
  "~ BS <-> O (alphas,betas)\n"
  "~ B1 <-> O (alpha1,beta3)\n"
  "~ B2 <-> B1 (alpha2,beta2)\n"
  "~ B3 <-> B2 (alpha3,beta1)\n"
  "~ BB <-> BS (alphas2,betas2)\n"
  "\n"
  "CONSERVE O + BS + BB + B1 + B2 + B3 = 1\n"
  "\n"
  "}\n"
  "\n"
  "\n"
  "PROCEDURE rate(v (mV)) { :callable from hoc\n"
  "	LOCAL a,d\n"
  "	\n"
  "	: Mg block\n"
  "	alpha1 = 12 * exp(-0.025 * (v - (shiftmg * (ek))))			: this is exactly as in paper\n"
  "	beta1 = mg_i/8 * 28 * exp(0.025 * (v - (shiftmg * (ek))) ) : this is exactly as in paper  \n"
  "	\n"
  "	: high-affinity polyamine block\n"
  "	alphas = As * 0.17 * exp(cas*-0.07 * (v - (ek) +8/8 (mV/mM) * mg_i)) :/  (1 + 0.01 * exp(0.12 * (v - (ek+vshiftbs)  +8  (mV/mM) * mg_i)))   : this is exactly as in paper, except denominator was omitted because it did not change anything in kinetics\n"
  "	\n"
  "	betas =  As * spm_i * 0.28 * exp(0.15 * (v - (ek) +8/8  (mV/mM) * mg_i)) :/ (1 + 0.01 * exp(0.13 * (v - (ek+vshiftbs)  + 8  (mV/mM) * mg_i)))    : this is exactly as in paper, except denominator was omitted because it did not change anything in kinetics\n"
  "	\n"
  "	: this is to fit two rate functions to the  kd of the paper kdd = 40 .* exp( - (v - (ek+vshiftbs)) / 9.1) \n"
  "	: b zwischen 0 und 1/9.1 ( 0.1099)\n"
  "\n"
  "	a = - 1/9.1 + b\n"
  "	\n"
  "	:d = (ek  - c)/(9.1 * b) + c  : d reduces to ek if c is ek\n"
  "\n"
  "	: low-affinity (second) polyamine block\n"
  "	alphas2 = fac* 40 * exp(a*(v-(ek+vshiftbs)))  : formerly v-c..... formula is turned around compared to matlab!\n"
  "	betas2 = spm_i * fac * exp(b*(v-(ek+vshiftbs))) : formerly v-d\n"
  "\n"
  "}\n"
  ;
    hoc_reg_nmodl_filename(mech_type, nmodl_filename);
    hoc_reg_nmodl_text(mech_type, nmodl_file_text);
}
#endif
