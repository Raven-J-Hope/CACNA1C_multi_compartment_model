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
static constexpr auto number_of_datum_variables = 8;
static constexpr auto number_of_floating_point_variables = 18;
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
 
#define nrn_init _nrn_init__Cav13
#define _nrn_initial _nrn_initial__Cav13
#define nrn_cur _nrn_cur__Cav13
#define _nrn_current _nrn_current__Cav13
#define nrn_jacob _nrn_jacob__Cav13
#define nrn_state _nrn_state__Cav13
#define _net_receive _net_receive__Cav13 
#define rates rates__Cav13 
#define state state__Cav13 
 
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
#define gbar _ml->template fpfield<0>(_iml)
#define gbar_columnindex 0
#define g _ml->template fpfield<1>(_iml)
#define g_columnindex 1
#define m _ml->template fpfield<2>(_iml)
#define m_columnindex 2
#define h _ml->template fpfield<3>(_iml)
#define h_columnindex 3
#define h2 _ml->template fpfield<4>(_iml)
#define h2_columnindex 4
#define ilca _ml->template fpfield<5>(_iml)
#define ilca_columnindex 5
#define ica _ml->template fpfield<6>(_iml)
#define ica_columnindex 6
#define eca _ml->template fpfield<7>(_iml)
#define eca_columnindex 7
#define cai _ml->template fpfield<8>(_iml)
#define cai_columnindex 8
#define mInf _ml->template fpfield<9>(_iml)
#define mInf_columnindex 9
#define hInf _ml->template fpfield<10>(_iml)
#define hInf_columnindex 10
#define h2Inf _ml->template fpfield<11>(_iml)
#define h2Inf_columnindex 11
#define mTau _ml->template fpfield<12>(_iml)
#define mTau_columnindex 12
#define Dm _ml->template fpfield<13>(_iml)
#define Dm_columnindex 13
#define Dh _ml->template fpfield<14>(_iml)
#define Dh_columnindex 14
#define Dh2 _ml->template fpfield<15>(_iml)
#define Dh2_columnindex 15
#define v _ml->template fpfield<16>(_iml)
#define v_columnindex 16
#define _g _ml->template fpfield<17>(_iml)
#define _g_columnindex 17
#define _ion_cai *(_ml->dptr_field<0>(_iml))
#define _p_ion_cai static_cast<neuron::container::data_handle<double>>(_ppvar[0])
#define _ion_eca *(_ml->dptr_field<1>(_iml))
#define _p_ion_eca static_cast<neuron::container::data_handle<double>>(_ppvar[1])
#define _ion_cao *(_ml->dptr_field<2>(_iml))
#define _p_ion_cao static_cast<neuron::container::data_handle<double>>(_ppvar[2])
#define _ion_ica *(_ml->dptr_field<3>(_iml))
#define _p_ion_ica static_cast<neuron::container::data_handle<double>>(_ppvar[3])
#define _ion_dicadv *(_ml->dptr_field<4>(_iml))
#define _ion_ilca *(_ml->dptr_field<5>(_iml))
#define _p_ion_ilca static_cast<neuron::container::data_handle<double>>(_ppvar[5])
#define _ion_dilcadv *(_ml->dptr_field<6>(_iml))
#define diam	(*(_ml->dptr_field<7>(_iml)))
 /* Thread safe. No static _ml, _iml or _ppvar. */
 static int hoc_nrnpointerindex =  -1;
 static _nrn_mechanism_std_vector<Datum> _extcall_thread;
 static Prop* _extcall_prop;
 /* _prop_id kind of shadows _extcall_prop to allow validity checking. */
 static _nrn_non_owning_id_without_container _prop_id{};
 /* external NEURON variables */
 /* declaration of user functions */
 static void _hoc_rates(void);
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
 {"setdata_Cav13", _hoc_setdata},
 {"rates_Cav13", _hoc_rates},
 {0, 0}
};
 
/* Direct Python call wrappers to density mechanism functions.*/
 static double _npy_rates(Prop*);
 
static NPyDirectMechFunc npy_direct_func_proc[] = {
 {"rates", _npy_rates},
 {0, 0}
};
 /* declare global and static user variables */
 #define gind 0
 #define _gth 0
#define VDI VDI_Cav13
 double VDI = 1;
#define hTau hTau_Cav13
 double hTau = 44.3;
#define h2Tau h2Tau_Cav13
 double h2Tau = 0.5;
#define kf kf_Cav13
 double kf = 0.0005;
#define vshift vshift_Cav13
 double vshift = 0;
 /* some parameters have upper and lower limits */
 static HocParmLimits _hoc_parm_limits[] = {
 {0, 0, 0}
};
 static HocParmUnits _hoc_parm_units[] = {
 {"hTau_Cav13", "ms"},
 {"h2Tau_Cav13", "ms"},
 {"vshift_Cav13", "mV"},
 {"kf_Cav13", "mM"},
 {"gbar_Cav13", "S/cm2"},
 {"g_Cav13", "S/cm2"},
 {0, 0}
};
 static double delta_t = 0.01;
 static double h20 = 0;
 static double h0 = 0;
 static double m0 = 0;
 /* connect global user variables to hoc */
 static DoubScal hoc_scdoub[] = {
 {"hTau_Cav13", &hTau_Cav13},
 {"h2Tau_Cav13", &h2Tau_Cav13},
 {"vshift_Cav13", &vshift_Cav13},
 {"kf_Cav13", &kf_Cav13},
 {"VDI_Cav13", &VDI_Cav13},
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
 
#define _cvode_ieq _ppvar[8].literal_value<int>()
 static void _ode_matsol_instance1(_internalthreadargsproto_);
 /* connect range variables in _p that hoc is supposed to know about */
 static const char *_mechanism[] = {
 "7.7.0",
"Cav13",
 "gbar_Cav13",
 0,
 "g_Cav13",
 0,
 "m_Cav13",
 "h_Cav13",
 "h2_Cav13",
 0,
 0};
 static Symbol* _morphology_sym;
 static Symbol* _ca_sym;
 static Symbol* _lca_sym;
 
 /* Used by NrnProperty */
 static _nrn_mechanism_std_vector<double> _parm_default{
     0, /* gbar */
 }; 
 
 
extern Prop* need_memb(Symbol*);
static void nrn_alloc(Prop* _prop) {
  Prop *prop_ion{};
  Datum *_ppvar{};
   _ppvar = nrn_prop_datum_alloc(_mechtype, 9, _prop);
    _nrn_mechanism_access_dparam(_prop) = _ppvar;
     _nrn_mechanism_cache_instance _ml_real{_prop};
    auto* const _ml = &_ml_real;
    size_t const _iml{};
    assert(_nrn_mechanism_get_num_vars(_prop) == 18);
 	/*initialize range parameters*/
 	gbar = _parm_default[0]; /* 0 */
 	 assert(_nrn_mechanism_get_num_vars(_prop) == 18);
 	_nrn_mechanism_access_dparam(_prop) = _ppvar;
 	/*connect ionic variables to this model*/
 prop_ion = need_memb(_morphology_sym);
 	_ppvar[7] = _nrn_mechanism_get_param_handle(prop_ion, 0); /* diam */
 prop_ion = need_memb(_ca_sym);
 nrn_promote(prop_ion, 1, 1);
 	_ppvar[0] = _nrn_mechanism_get_param_handle(prop_ion, 1); /* cai */
 	_ppvar[1] = _nrn_mechanism_get_param_handle(prop_ion, 0); /* eca */
 	_ppvar[2] = _nrn_mechanism_get_param_handle(prop_ion, 2); /* cao */
 	_ppvar[3] = _nrn_mechanism_get_param_handle(prop_ion, 3); /* ica */
 	_ppvar[4] = _nrn_mechanism_get_param_handle(prop_ion, 4); /* _ion_dicadv */
 prop_ion = need_memb(_lca_sym);
 	_ppvar[5] = _nrn_mechanism_get_param_handle(prop_ion, 3); /* ilca */
 	_ppvar[6] = _nrn_mechanism_get_param_handle(prop_ion, 4); /* _ion_dilcadv */
 
}
 static void _initlists();
  /* some states have an absolute tolerance */
 static Symbol** _atollist;
 static HocStateTolerance _hoc_state_tol[] = {
 {0, 0}
};
 extern Symbol* hoc_lookup(const char*);
extern void _nrn_thread_reg(int, int, void(*)(Datum*));
void _nrn_thread_table_reg(int, nrn_thread_table_check_t);
extern void hoc_register_tolerance(int, HocStateTolerance*, Symbol***);
extern void _cvode_abstol( Symbol**, double*, int);

 extern "C" void _Cav13_reg() {
	int _vectorized = 1;
  _initlists();
 	ion_reg("ca", -10000.);
 	ion_reg("lca", 0.0);
 	_morphology_sym = hoc_lookup("morphology");
 	_ca_sym = hoc_lookup("ca_ion");
 	_lca_sym = hoc_lookup("lca_ion");
 	register_mech(_mechanism, nrn_alloc,nrn_cur, nrn_jacob, nrn_state, nrn_init, hoc_nrnpointerindex, 1);
 _mechtype = nrn_get_mechtype(_mechanism[1]);
 hoc_register_parm_default(_mechtype, &_parm_default);
         hoc_register_npy_direct(_mechtype, npy_direct_func_proc);
     _nrn_setdata_reg(_mechtype, _setdata);
 #if NMODL_TEXT
  register_nmodl_text_and_filename(_mechtype);
#endif
   _nrn_mechanism_register_data_fields(_mechtype,
                                       _nrn_mechanism_field<double>{"gbar"} /* 0 */,
                                       _nrn_mechanism_field<double>{"g"} /* 1 */,
                                       _nrn_mechanism_field<double>{"m"} /* 2 */,
                                       _nrn_mechanism_field<double>{"h"} /* 3 */,
                                       _nrn_mechanism_field<double>{"h2"} /* 4 */,
                                       _nrn_mechanism_field<double>{"ilca"} /* 5 */,
                                       _nrn_mechanism_field<double>{"ica"} /* 6 */,
                                       _nrn_mechanism_field<double>{"eca"} /* 7 */,
                                       _nrn_mechanism_field<double>{"cai"} /* 8 */,
                                       _nrn_mechanism_field<double>{"mInf"} /* 9 */,
                                       _nrn_mechanism_field<double>{"hInf"} /* 10 */,
                                       _nrn_mechanism_field<double>{"h2Inf"} /* 11 */,
                                       _nrn_mechanism_field<double>{"mTau"} /* 12 */,
                                       _nrn_mechanism_field<double>{"Dm"} /* 13 */,
                                       _nrn_mechanism_field<double>{"Dh"} /* 14 */,
                                       _nrn_mechanism_field<double>{"Dh2"} /* 15 */,
                                       _nrn_mechanism_field<double>{"v"} /* 16 */,
                                       _nrn_mechanism_field<double>{"_g"} /* 17 */,
                                       _nrn_mechanism_field<double*>{"_ion_cai", "ca_ion"} /* 0 */,
                                       _nrn_mechanism_field<double*>{"_ion_eca", "ca_ion"} /* 1 */,
                                       _nrn_mechanism_field<double*>{"_ion_cao", "ca_ion"} /* 2 */,
                                       _nrn_mechanism_field<double*>{"_ion_ica", "ca_ion"} /* 3 */,
                                       _nrn_mechanism_field<double*>{"_ion_dicadv", "ca_ion"} /* 4 */,
                                       _nrn_mechanism_field<double*>{"_ion_ilca", "lca_ion"} /* 5 */,
                                       _nrn_mechanism_field<double*>{"_ion_dilcadv", "lca_ion"} /* 6 */,
                                       _nrn_mechanism_field<double*>{"diam", "diam"} /* 7 */,
                                       _nrn_mechanism_field<int>{"_cvode_ieq", "cvodeieq"} /* 8 */);
  hoc_register_prop_size(_mechtype, 18, 9);
  hoc_register_dparam_semantics(_mechtype, 0, "ca_ion");
  hoc_register_dparam_semantics(_mechtype, 1, "ca_ion");
  hoc_register_dparam_semantics(_mechtype, 2, "ca_ion");
  hoc_register_dparam_semantics(_mechtype, 3, "ca_ion");
  hoc_register_dparam_semantics(_mechtype, 4, "ca_ion");
  hoc_register_dparam_semantics(_mechtype, 5, "lca_ion");
  hoc_register_dparam_semantics(_mechtype, 6, "lca_ion");
  hoc_register_dparam_semantics(_mechtype, 8, "cvodeieq");
  hoc_register_dparam_semantics(_mechtype, 7, "diam");
 	hoc_register_cvode(_mechtype, _ode_count, _ode_map, _ode_spec, _ode_matsol);
 	hoc_register_tolerance(_mechtype, _hoc_state_tol, &_atollist);
 
    hoc_register_var(hoc_scdoub, hoc_vdoub, hoc_intfunc);
 	ivoc_help("help ?1 Cav13 /home/raven/PycharmProjects/Masters_model/Mod_Files_Beining_2017/Cav13.mod\n");
 hoc_register_limits(_mechtype, _hoc_parm_limits);
 hoc_register_units(_mechtype, _hoc_parm_units);
 }
static int _reset;
static const char *modelname = "";

static int error;
static int _ninits = 0;
static int _match_recurse=1;
static void _modl_cleanup(){ _match_recurse=1;}
static int rates(_internalthreadargsproto_);
 
static int _ode_spec1(_internalthreadargsproto_);
/*static int _ode_matsol1(_internalthreadargsproto_);*/
 static neuron::container::field_index _slist1[3], _dlist1[3];
 static int state(_internalthreadargsproto_);
 
/*CVODE*/
 static int _ode_spec1 (_internalthreadargsproto_) {int _reset = 0; {
   Dm = ( mInf - m ) / mTau ;
   Dh = ( hInf - h ) / hTau ;
   Dh2 = ( h2Inf - h2 ) / h2Tau ;
   }
 return _reset;
}
 static int _ode_matsol1 (_internalthreadargsproto_) {
 Dm = Dm  / (1. - dt*( ( ( ( - 1.0 ) ) ) / mTau )) ;
 Dh = Dh  / (1. - dt*( ( ( ( - 1.0 ) ) ) / hTau )) ;
 Dh2 = Dh2  / (1. - dt*( ( ( ( - 1.0 ) ) ) / h2Tau )) ;
  return 0;
}
 /*END CVODE*/
 static int state (_internalthreadargsproto_) { {
    m = m + (1. - exp(dt*(( ( ( - 1.0 ) ) ) / mTau)))*(- ( ( ( mInf ) ) / mTau ) / ( ( ( ( - 1.0 ) ) ) / mTau ) - m) ;
    h = h + (1. - exp(dt*(( ( ( - 1.0 ) ) ) / hTau)))*(- ( ( ( hInf ) ) / hTau ) / ( ( ( ( - 1.0 ) ) ) / hTau ) - h) ;
    h2 = h2 + (1. - exp(dt*(( ( ( - 1.0 ) ) ) / h2Tau)))*(- ( ( ( h2Inf ) ) / h2Tau ) / ( ( ( ( - 1.0 ) ) ) / h2Tau ) - h2) ;
   }
  return 0;
}
 
static int  rates ( _internalthreadargsproto_ ) {
   double _lmA , _lmB ;
 _lmA = ( 39800.0 * ( v + 67.24 ) ) / ( exp ( ( v + 67.24 ) / 15.005 ) - 1.0 ) ;
   _lmB = 3500.0 * exp ( v / 31.4 ) ;
   mTau = ( 1.0 / ( _lmA + _lmB ) ) ;
   mInf = 1.0 / ( ( exp ( ( v - ( - 40.0 ) ) / ( - 5.0 ) ) ) + 1.0 ) ;
   hInf = VDI / ( ( exp ( ( v - ( - 37.0 ) ) / ( 5.0 ) ) ) + 1.0 ) + ( 1.0 - VDI ) ;
   h2Inf = kf / ( kf + cai ) ;
    return 0; }
 
static void _hoc_rates(void) {
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
 rates ( _threadargs_ );
 hoc_retpushx(_r);
}
 
static double _npy_rates(Prop* _prop) {
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
 rates ( _threadargs_ );
 return(_r);
}
 
static int _ode_count(int _type){ return 3;}
 
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
  cai = _ion_cai;
  eca = _ion_eca;
     _ode_spec1 (_threadargs_);
   }}
 
static void _ode_map(Prop* _prop, int _ieq, neuron::container::data_handle<double>* _pv, neuron::container::data_handle<double>* _pvdot, double* _atol, int _type) { 
  Datum* _ppvar;
  _ppvar = _nrn_mechanism_access_dparam(_prop);
  _cvode_ieq = _ieq;
  for (int _i=0; _i < 3; ++_i) {
    _pv[_i] = _nrn_mechanism_get_param_handle(_prop, _slist1[_i]);
    _pvdot[_i] = _nrn_mechanism_get_param_handle(_prop, _dlist1[_i]);
    _cvode_abstol(_atollist, _atol, _i);
  }
 }
 
static void _ode_matsol_instance1(_internalthreadargsproto_) {
 _ode_matsol1 (_threadargs_);
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
  cai = _ion_cai;
  eca = _ion_eca;
 _ode_matsol_instance1(_threadargs_);
 }}

static void initmodel(_internalthreadargsproto_) {
  int _i; double _save;{
  h2 = h20;
  h = h0;
  m = m0;
 {
   rates ( _threadargs_ ) ;
   m = mInf ;
   h = hInf ;
   h2 = h2Inf ;
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
  cai = _ion_cai;
  eca = _ion_eca;
 initmodel(_threadargs_);
  }
}

static double _nrn_current(_internalthreadargsprotocomma_ double _v) {
double _current=0.; v=_v;
{ {
   rates ( _threadargs_ ) ;
   g = gbar * m * h * h2 ;
   ica = ( g ) * ( v - eca ) ;
   ilca = ica ;
   }
 _current += ica;
 _current += ilca;

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
  cai = _ion_cai;
  eca = _ion_eca;
 auto const _g_local = _nrn_current(_threadargscomma_ _v + .001);
 	{ double _dilca;
 double _dica;
  _dica = ica;
  _dilca = ilca;
 _rhs = _nrn_current(_threadargscomma_ _v);
  _ion_dicadv += (_dica - ica)/.001 ;
  _ion_dilcadv += (_dilca - ilca)/.001 ;
 	}
 _g = (_g_local - _rhs)/.001;
  _ion_ica += ica ;
  _ion_ilca += ilca ;
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
  cai = _ion_cai;
  eca = _ion_eca;
 {   state(_threadargs_);
  }  }}

}

static void terminal(){}

static void _initlists(){
 int _i; static int _first = 1;
  if (!_first) return;
 _slist1[0] = {m_columnindex, 0};  _dlist1[0] = {Dm_columnindex, 0};
 _slist1[1] = {h_columnindex, 0};  _dlist1[1] = {Dh_columnindex, 0};
 _slist1[2] = {h2_columnindex, 0};  _dlist1[2] = {Dh2_columnindex, 0};
_first = 0;
}

#if NMODL_TEXT
static void register_nmodl_text_and_filename(int mech_type) {
    const char* nmodl_filename = "/home/raven/PycharmProjects/Masters_model/Mod_Files_Beining_2017/Cav13.mod";
    const char* nmodl_file_text = 
  ": model from Evans et al 2013, transferred from GENESIS to NEURON by Beining et al (2016), \"A novel comprehensive and consistent electrophysiologcal model of dentate granule cells\"\n"
  ": also added Calcium dependent inactivation\n"
  "\n"
  "NEURON {\n"
  "	SUFFIX Cav13\n"
  "	USEION ca READ cai, eca WRITE ica   :,cai,cao...., cai, cao\n"
  "	USEION lca WRITE ilca VALENCE 0\n"
  "	RANGE gbar, g\n"
  "	GLOBAL kf, h2Tau, VDI\n"
  "}\n"
  "\n"
  "UNITS {\n"
  "	(molar) = (1/liter)\n"
  "	(mM) = (millimolar)\n"
  "	(mV) = (millivolt)\n"
  "	(mA) = (milliamp)\n"
  "	(S) = (siemens)\n"
  "	(um) = (micrometer)\n"
  "}\n"
  "\n"
  "ASSIGNED {\n"
  "	ilca		(mA/cm2) : instantaneous calcium current of l-type calcium channel\n"
  "	v			(mV)\n"
  "	ica		(mA/cm2)\n"
  "	g		(S/cm2)\n"
  "	eca 		(mV)\n"
  "	diam		(um)\n"
  "	cai 		(mM)\n"
  "	mInf  (1)\n"
  "	hInf  (1)\n"
  "	h2Inf (1)\n"
  "	mTau (ms)\n"
  "}\n"
  "\n"
  "PARAMETER {\n"
  "	hTau 	= 44.3 (ms)\n"
  "	h2Tau = 0.5 (ms)\n"
  "	gbar = 0	(S/cm2)\n"
  "		vshift = 0 		(mV)\n"
  "		\n"
  "		:parameters for calcium-dep inactivation (CDI) \n"
  "			:f= (0.001/(0.001+[Ca]))Poirazi CA1  2003\n"
  "			:f= (0.0005/(0.0005+[Ca])) Rhodes and Llinas 2001 Cort Pyr\n"
  "	kf		=			0.0005 (mM)  : factor in inactivation, the higher the less sensitive. others uses 0.0002.. standen and stanfield use 0.001mM in original paper	\n"
  "	VDI = 1\n"
  "}\n"
  "\n"
  "STATE {m h h2}  :a b  :cai (mM) cao (mM)\n"
  "\n"
  "INITIAL {\n"
  "	rates()\n"
  "	m = mInf\n"
  "	h = hInf\n"
  "	h2 = h2Inf\n"
  "}\n"
  "\n"
  "BREAKPOINT {\n"
  "	rates()\n"
  "	SOLVE state METHOD cnexp\n"
  "	g = gbar*m*h*h2 : h2 calcium dependent inactivation is taken from santhakumar 05.. tjos assumes instantaneous calcium inactivation\n"
  "	ica = (g)*(v - eca) : \n"
  "	ilca = ica\n"
  "	\n"
  "}\n"
  "\n"
  "DERIVATIVE state {	: exact when v held constant integrates over dt step\n"
  "	m' = (mInf-m) / mTau\n"
  "	h' = (hInf-h) / hTau\n"
  "	h2' = (h2Inf-h2)/h2Tau\n"
  "}\n"
  "\n"
  "PROCEDURE rates(){\n"
  "		LOCAL mA,mB\n"
  "		mA = (39800*( v + 67.24))/( exp ( (v + 67.24)/15.005) - 1.0)\n"
  "		mB = 3500* exp(v/31.4) \n"
  "		mTau = (1/(mA + mB))\n"
  "		\n"
  "		mInf = 1.0/((exp ( (v - (-40.0))/(-5))) + 1.0)\n"
  "\n"
  "		hInf = VDI/( (exp ( (v - (-37))/(5))) + 1.0) + (1-VDI)\n"
  "		:h2 = caIn(cai)\n"
  "		h2Inf = kf/(kf+cai)\n"
  "}\n"
  ;
    hoc_reg_nmodl_filename(mech_type, nmodl_filename);
    hoc_reg_nmodl_text(mech_type, nmodl_file_text);
}
#endif
