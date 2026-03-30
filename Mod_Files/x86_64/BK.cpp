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
static constexpr auto number_of_datum_variables = 7;
static constexpr auto number_of_floating_point_variables = 20;
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
 
#define nrn_init _nrn_init__BK
#define _nrn_initial _nrn_initial__BK
#define nrn_cur _nrn_cur__BK
#define _nrn_current _nrn_current__BK
#define nrn_jacob _nrn_jacob__BK
#define nrn_state _nrn_state__BK
#define _net_receive _net_receive__BK 
#define rates rates__BK 
#define state state__BK 
 
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
#define diff _ml->template fpfield<0>(_iml)
#define diff_columnindex 0
#define gakbar _ml->template fpfield<1>(_iml)
#define gakbar_columnindex 1
#define gabkbar _ml->template fpfield<2>(_iml)
#define gabkbar_columnindex 2
#define ik _ml->template fpfield<3>(_iml)
#define ik_columnindex 3
#define gak _ml->template fpfield<4>(_iml)
#define gak_columnindex 4
#define gabk _ml->template fpfield<5>(_iml)
#define gabk_columnindex 5
#define ainf _ml->template fpfield<6>(_iml)
#define ainf_columnindex 6
#define atau _ml->template fpfield<7>(_iml)
#define atau_columnindex 7
#define abinf _ml->template fpfield<8>(_iml)
#define abinf_columnindex 8
#define abtau _ml->template fpfield<9>(_iml)
#define abtau_columnindex 9
#define acai _ml->template fpfield<10>(_iml)
#define acai_columnindex 10
#define ab _ml->template fpfield<11>(_iml)
#define ab_columnindex 11
#define a _ml->template fpfield<12>(_iml)
#define a_columnindex 12
#define ek _ml->template fpfield<13>(_iml)
#define ek_columnindex 13
#define cai _ml->template fpfield<14>(_iml)
#define cai_columnindex 14
#define ncai _ml->template fpfield<15>(_iml)
#define ncai_columnindex 15
#define Dab _ml->template fpfield<16>(_iml)
#define Dab_columnindex 16
#define Da _ml->template fpfield<17>(_iml)
#define Da_columnindex 17
#define v _ml->template fpfield<18>(_iml)
#define v_columnindex 18
#define _g _ml->template fpfield<19>(_iml)
#define _g_columnindex 19
#define _ion_cai *(_ml->dptr_field<0>(_iml))
#define _p_ion_cai static_cast<neuron::container::data_handle<double>>(_ppvar[0])
#define _ion_cao *(_ml->dptr_field<1>(_iml))
#define _p_ion_cao static_cast<neuron::container::data_handle<double>>(_ppvar[1])
#define _ion_ek *(_ml->dptr_field<2>(_iml))
#define _p_ion_ek static_cast<neuron::container::data_handle<double>>(_ppvar[2])
#define _ion_ik *(_ml->dptr_field<3>(_iml))
#define _p_ion_ik static_cast<neuron::container::data_handle<double>>(_ppvar[3])
#define _ion_dikdv *(_ml->dptr_field<4>(_iml))
#define _ion_ncai *(_ml->dptr_field<5>(_iml))
#define _p_ion_ncai static_cast<neuron::container::data_handle<double>>(_ppvar[5])
#define _ion_ncao *(_ml->dptr_field<6>(_iml))
#define _p_ion_ncao static_cast<neuron::container::data_handle<double>>(_ppvar[6])
 /* Thread safe. No static _ml, _iml or _ppvar. */
 static int hoc_nrnpointerindex =  -1;
 static _nrn_mechanism_std_vector<Datum> _extcall_thread;
 static Prop* _extcall_prop;
 /* _prop_id kind of shadows _extcall_prop to allow validity checking. */
 static _nrn_non_owning_id_without_container _prop_id{};
 /* external NEURON variables */
 /* declaration of user functions */
 static void _hoc_peakab(void);
 static void _hoc_peaka(void);
 static void _hoc_rates(void);
 static void _hoc_shiftab(void);
 static void _hoc_shifta(void);
 static void _hoc_taufunc(void);
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
 {"setdata_BK", _hoc_setdata},
 {"peakab_BK", _hoc_peakab},
 {"peaka_BK", _hoc_peaka},
 {"rates_BK", _hoc_rates},
 {"shiftab_BK", _hoc_shiftab},
 {"shifta_BK", _hoc_shifta},
 {"taufunc_BK", _hoc_taufunc},
 {0, 0}
};
 
/* Direct Python call wrappers to density mechanism functions.*/
 static double _npy_peakab(Prop*);
 static double _npy_peaka(Prop*);
 static double _npy_rates(Prop*);
 static double _npy_shiftab(Prop*);
 static double _npy_shifta(Prop*);
 static double _npy_taufunc(Prop*);
 
static NPyDirectMechFunc npy_direct_func_proc[] = {
 {"peakab", _npy_peakab},
 {"peaka", _npy_peaka},
 {"rates", _npy_rates},
 {"shiftab", _npy_shiftab},
 {"shifta", _npy_shifta},
 {"taufunc", _npy_taufunc},
 {0, 0}
};
#define peakab peakab_BK
#define peaka peaka_BK
#define shiftab shiftab_BK
#define shifta shifta_BK
#define taufunc taufunc_BK
 extern double peakab( _internalthreadargsprotocomma_ double );
 extern double peaka( _internalthreadargsprotocomma_ double );
 extern double shiftab( _internalthreadargsprotocomma_ double );
 extern double shifta( _internalthreadargsprotocomma_ double );
 extern double taufunc( _internalthreadargsprotocomma_ double );
 /* declare global and static user variables */
 #define gind 0
 #define _gth 0
#define base base_BK
 double base = 4;
 /* some parameters have upper and lower limits */
 static HocParmLimits _hoc_parm_limits[] = {
 {0, 0, 0}
};
 static HocParmUnits _hoc_parm_units[] = {
 {"base_BK", "mV"},
 {"diff_BK", "1"},
 {"gakbar_BK", "S/cm2"},
 {"gabkbar_BK", "S/cm2"},
 {"ik_BK", "mA/cm2"},
 {"gak_BK", "S/cm2"},
 {"gabk_BK", "S/cm2"},
 {"atau_BK", "ms"},
 {"abtau_BK", "ms"},
 {"acai_BK", "mM"},
 {0, 0}
};
 static double a0 = 0;
 static double ab0 = 0;
 static double delta_t = 0.01;
 /* connect global user variables to hoc */
 static DoubScal hoc_scdoub[] = {
 {"base_BK", &base_BK},
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
 
#define _cvode_ieq _ppvar[7].literal_value<int>()
 static void _ode_matsol_instance1(_internalthreadargsproto_);
 /* connect range variables in _p that hoc is supposed to know about */
 static const char *_mechanism[] = {
 "7.7.0",
"BK",
 "diff_BK",
 "gakbar_BK",
 "gabkbar_BK",
 0,
 "ik_BK",
 "gak_BK",
 "gabk_BK",
 "ainf_BK",
 "atau_BK",
 "abinf_BK",
 "abtau_BK",
 "acai_BK",
 0,
 "ab_BK",
 "a_BK",
 0,
 0};
 static Symbol* _ca_sym;
 static Symbol* _k_sym;
 static Symbol* _nca_sym;
 
 /* Used by NrnProperty */
 static _nrn_mechanism_std_vector<double> _parm_default{
     1, /* diff */
     0.01, /* gakbar */
     0.01, /* gabkbar */
 }; 
 
 
extern Prop* need_memb(Symbol*);
static void nrn_alloc(Prop* _prop) {
  Prop *prop_ion{};
  Datum *_ppvar{};
   _ppvar = nrn_prop_datum_alloc(_mechtype, 8, _prop);
    _nrn_mechanism_access_dparam(_prop) = _ppvar;
     _nrn_mechanism_cache_instance _ml_real{_prop};
    auto* const _ml = &_ml_real;
    size_t const _iml{};
    assert(_nrn_mechanism_get_num_vars(_prop) == 20);
 	/*initialize range parameters*/
 	diff = _parm_default[0]; /* 1 */
 	gakbar = _parm_default[1]; /* 0.01 */
 	gabkbar = _parm_default[2]; /* 0.01 */
 	 assert(_nrn_mechanism_get_num_vars(_prop) == 20);
 	_nrn_mechanism_access_dparam(_prop) = _ppvar;
 	/*connect ionic variables to this model*/
 prop_ion = need_memb(_ca_sym);
 nrn_promote(prop_ion, 1, 0);
 	_ppvar[0] = _nrn_mechanism_get_param_handle(prop_ion, 1); /* cai */
 	_ppvar[1] = _nrn_mechanism_get_param_handle(prop_ion, 2); /* cao */
 prop_ion = need_memb(_k_sym);
 nrn_promote(prop_ion, 0, 1);
 	_ppvar[2] = _nrn_mechanism_get_param_handle(prop_ion, 0); /* ek */
 	_ppvar[3] = _nrn_mechanism_get_param_handle(prop_ion, 3); /* ik */
 	_ppvar[4] = _nrn_mechanism_get_param_handle(prop_ion, 4); /* _ion_dikdv */
 prop_ion = need_memb(_nca_sym);
 nrn_promote(prop_ion, 1, 0);
 	_ppvar[5] = _nrn_mechanism_get_param_handle(prop_ion, 1); /* ncai */
 	_ppvar[6] = _nrn_mechanism_get_param_handle(prop_ion, 2); /* ncao */
 
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

 extern "C" void _BK_reg() {
	int _vectorized = 1;
  _initlists();
 	ion_reg("ca", -10000.);
 	ion_reg("k", -10000.);
 	ion_reg("nca", 0.0);
 	_ca_sym = hoc_lookup("ca_ion");
 	_k_sym = hoc_lookup("k_ion");
 	_nca_sym = hoc_lookup("nca_ion");
 	register_mech(_mechanism, nrn_alloc,nrn_cur, nrn_jacob, nrn_state, nrn_init, hoc_nrnpointerindex, 1);
 _mechtype = nrn_get_mechtype(_mechanism[1]);
 hoc_register_parm_default(_mechtype, &_parm_default);
         hoc_register_npy_direct(_mechtype, npy_direct_func_proc);
     _nrn_setdata_reg(_mechtype, _setdata);
 #if NMODL_TEXT
  register_nmodl_text_and_filename(_mechtype);
#endif
   _nrn_mechanism_register_data_fields(_mechtype,
                                       _nrn_mechanism_field<double>{"diff"} /* 0 */,
                                       _nrn_mechanism_field<double>{"gakbar"} /* 1 */,
                                       _nrn_mechanism_field<double>{"gabkbar"} /* 2 */,
                                       _nrn_mechanism_field<double>{"ik"} /* 3 */,
                                       _nrn_mechanism_field<double>{"gak"} /* 4 */,
                                       _nrn_mechanism_field<double>{"gabk"} /* 5 */,
                                       _nrn_mechanism_field<double>{"ainf"} /* 6 */,
                                       _nrn_mechanism_field<double>{"atau"} /* 7 */,
                                       _nrn_mechanism_field<double>{"abinf"} /* 8 */,
                                       _nrn_mechanism_field<double>{"abtau"} /* 9 */,
                                       _nrn_mechanism_field<double>{"acai"} /* 10 */,
                                       _nrn_mechanism_field<double>{"ab"} /* 11 */,
                                       _nrn_mechanism_field<double>{"a"} /* 12 */,
                                       _nrn_mechanism_field<double>{"ek"} /* 13 */,
                                       _nrn_mechanism_field<double>{"cai"} /* 14 */,
                                       _nrn_mechanism_field<double>{"ncai"} /* 15 */,
                                       _nrn_mechanism_field<double>{"Dab"} /* 16 */,
                                       _nrn_mechanism_field<double>{"Da"} /* 17 */,
                                       _nrn_mechanism_field<double>{"v"} /* 18 */,
                                       _nrn_mechanism_field<double>{"_g"} /* 19 */,
                                       _nrn_mechanism_field<double*>{"_ion_cai", "ca_ion"} /* 0 */,
                                       _nrn_mechanism_field<double*>{"_ion_cao", "ca_ion"} /* 1 */,
                                       _nrn_mechanism_field<double*>{"_ion_ek", "k_ion"} /* 2 */,
                                       _nrn_mechanism_field<double*>{"_ion_ik", "k_ion"} /* 3 */,
                                       _nrn_mechanism_field<double*>{"_ion_dikdv", "k_ion"} /* 4 */,
                                       _nrn_mechanism_field<double*>{"_ion_ncai", "nca_ion"} /* 5 */,
                                       _nrn_mechanism_field<double*>{"_ion_ncao", "nca_ion"} /* 6 */,
                                       _nrn_mechanism_field<int>{"_cvode_ieq", "cvodeieq"} /* 7 */);
  hoc_register_prop_size(_mechtype, 20, 8);
  hoc_register_dparam_semantics(_mechtype, 0, "ca_ion");
  hoc_register_dparam_semantics(_mechtype, 1, "ca_ion");
  hoc_register_dparam_semantics(_mechtype, 2, "k_ion");
  hoc_register_dparam_semantics(_mechtype, 3, "k_ion");
  hoc_register_dparam_semantics(_mechtype, 4, "k_ion");
  hoc_register_dparam_semantics(_mechtype, 5, "nca_ion");
  hoc_register_dparam_semantics(_mechtype, 6, "nca_ion");
  hoc_register_dparam_semantics(_mechtype, 7, "cvodeieq");
 	hoc_register_cvode(_mechtype, _ode_count, _ode_map, _ode_spec, _ode_matsol);
 	hoc_register_tolerance(_mechtype, _hoc_state_tol, &_atollist);
 
    hoc_register_var(hoc_scdoub, hoc_vdoub, hoc_intfunc);
 	ivoc_help("help ?1 BK /home/raven/PycharmProjects/Masters_model/Mod_Files_Beining_2017/BK.mod\n");
 hoc_register_limits(_mechtype, _hoc_parm_limits);
 hoc_register_units(_mechtype, _hoc_parm_units);
 }
static int _reset;
static const char *modelname = "";

static int error;
static int _ninits = 0;
static int _match_recurse=1;
static void _modl_cleanup(){ _match_recurse=1;}
static int rates(_internalthreadargsprotocomma_ double, double);
 
static int _ode_spec1(_internalthreadargsproto_);
/*static int _ode_matsol1(_internalthreadargsproto_);*/
 static neuron::container::field_index _slist1[2], _dlist1[2];
 static int state(_internalthreadargsproto_);
 
/*CVODE*/
 static int _ode_spec1 (_internalthreadargsproto_) {int _reset = 0; {
   acai = ( ncai ) / diff ;
   if ( acai < cai ) {
     acai = cai ;
     }
   rates ( _threadargscomma_ v , acai ) ;
   Da = ( ainf - a ) / atau ;
   Dab = ( abinf - ab ) / abtau ;
   }
 return _reset;
}
 static int _ode_matsol1 (_internalthreadargsproto_) {
 acai = ( ncai ) / diff ;
 if ( acai < cai ) {
   acai = cai ;
   }
 rates ( _threadargscomma_ v , acai ) ;
 Da = Da  / (1. - dt*( ( ( ( - 1.0 ) ) ) / atau )) ;
 Dab = Dab  / (1. - dt*( ( ( ( - 1.0 ) ) ) / abtau )) ;
  return 0;
}
 /*END CVODE*/
 static int state (_internalthreadargsproto_) { {
   acai = ( ncai ) / diff ;
   if ( acai < cai ) {
     acai = cai ;
     }
   rates ( _threadargscomma_ v , acai ) ;
    a = a + (1. - exp(dt*(( ( ( - 1.0 ) ) ) / atau)))*(- ( ( ( ainf ) ) / atau ) / ( ( ( ( - 1.0 ) ) ) / atau ) - a) ;
    ab = ab + (1. - exp(dt*(( ( ( - 1.0 ) ) ) / abtau)))*(- ( ( ( abinf ) ) / abtau ) / ( ( ( ( - 1.0 ) ) ) / abtau ) - ab) ;
   }
  return 0;
}
 
double shifta ( _internalthreadargsprotocomma_ double _lca ) {
   double _lshifta;
 _lshifta = 25.0 - 50.3 + ( 107.5 * exp ( - .12 * _lca * 1e3 ) ) ;
   
return _lshifta;
 }
 
static void _hoc_shifta(void) {
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
 _r =  shifta ( _threadargscomma_ *getarg(1) );
 hoc_retpushx(_r);
}
 
static double _npy_shifta(Prop* _prop) {
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
 _r =  shifta ( _threadargscomma_ *getarg(1) );
 return(_r);
}
 
double peaka ( _internalthreadargsprotocomma_ double _lca ) {
   double _lpeaka;
 _lpeaka = 2.9 + ( 6.3 * exp ( - .36 * _lca * 1e3 ) ) ;
   
return _lpeaka;
 }
 
static void _hoc_peaka(void) {
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
 _r =  peaka ( _threadargscomma_ *getarg(1) );
 hoc_retpushx(_r);
}
 
static double _npy_peaka(Prop* _prop) {
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
 _r =  peaka ( _threadargscomma_ *getarg(1) );
 return(_r);
}
 
double shiftab ( _internalthreadargsprotocomma_ double _lca ) {
   double _lshiftab;
 _lshiftab = 25.0 - 55.7 + 136.9 * exp ( - .28 * _lca * 1e3 ) ;
   
return _lshiftab;
 }
 
static void _hoc_shiftab(void) {
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
 _r =  shiftab ( _threadargscomma_ *getarg(1) );
 hoc_retpushx(_r);
}
 
static double _npy_shiftab(Prop* _prop) {
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
 _r =  shiftab ( _threadargscomma_ *getarg(1) );
 return(_r);
}
 
double peakab ( _internalthreadargsprotocomma_ double _lca ) {
   double _lpeakab;
 _lpeakab = 13.7 + 234.0 * exp ( - .72 * _lca * 1e3 ) ;
   
return _lpeakab;
 }
 
static void _hoc_peakab(void) {
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
 _r =  peakab ( _threadargscomma_ *getarg(1) );
 hoc_retpushx(_r);
}
 
static double _npy_peakab(Prop* _prop) {
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
 _r =  peakab ( _threadargscomma_ *getarg(1) );
 return(_r);
}
 
double taufunc ( _internalthreadargsprotocomma_ double _lv ) {
   double _ltaufunc;
 _ltaufunc = 1.0 / ( ( 10.0 * ( exp ( - _lv / 63.6 ) + exp ( - ( 150.0 - _lv ) / 63.6 ) ) ) - 5.2 ) ;
   if ( _ltaufunc <= 0.2 ) {
     _ltaufunc = 0.2 ;
     }
   
return _ltaufunc;
 }
 
static void _hoc_taufunc(void) {
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
 _r =  taufunc ( _threadargscomma_ *getarg(1) );
 hoc_retpushx(_r);
}
 
static double _npy_taufunc(Prop* _prop) {
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
 _r =  taufunc ( _threadargscomma_ *getarg(1) );
 return(_r);
}
 
static int  rates ( _internalthreadargsprotocomma_ double _lv , double _lc ) {
   double _lrange , _lvv , _lashift , _lbshift ;
 _lashift = - 32.0 + ( 59.2 * exp ( - .09 * _lc * 1e3 ) ) + ( 96.7 * exp ( - .47 * _lc * 1e3 ) ) ;
   ainf = 1.0 / ( 1.0 + exp ( ( _lashift - _lv ) / ( 25.0 / 1.6 ) ) ) ;
   _lvv = _lv + 100.0 - shifta ( _threadargscomma_ _lc ) ;
   atau = taufunc ( _threadargscomma_ _lvv ) ;
   _lrange = peaka ( _threadargscomma_ _lc ) - 1.0 ;
   atau = ( _lrange * ( ( atau - .2 ) / .8 ) ) + 1.0 ;
   _lbshift = - 56.449 + 104.52 * exp ( - .22964 * _lc * 1e3 ) + 295.68 * exp ( - 2.1571 * _lc * 1e3 ) ;
   abinf = 1.0 / ( 1.0 + exp ( ( _lbshift - _lv ) / ( 25.0 / 1.6 ) ) ) ;
   _lvv = _lv + 100.0 - shiftab ( _threadargscomma_ _lc ) ;
   abtau = taufunc ( _threadargscomma_ _lvv ) ;
   _lrange = peakab ( _threadargscomma_ _lc ) - base ;
   abtau = ( _lrange * ( ( abtau - .2 ) / .8 ) ) + base ;
    return 0; }
 
static void _hoc_rates(void) {
  double _r;
 Datum* _ppvar; Datum* _thread; NrnThread* _nt;
 
  if(!_prop_id) {
    hoc_execerror("No data for rates_BK. Requires prior call to setdata_BK and that the specified mechanism instance still be in existence.", NULL);
  }
  Prop* _local_prop = _extcall_prop;
  _nrn_mechanism_cache_instance _ml_real{_local_prop};
auto* const _ml = &_ml_real;
size_t const _iml{};
_ppvar = _local_prop ? _nrn_mechanism_access_dparam(_local_prop) : nullptr;
_thread = _extcall_thread.data();
double* _globals = nullptr;
if (gind != 0 && _thread != nullptr) { _globals = _thread[_gth].get<double*>(); }
_nt = nrn_threads;
 _r = 1.;
 rates ( _threadargscomma_ *getarg(1) , *getarg(2) );
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
 rates ( _threadargscomma_ *getarg(1) , *getarg(2) );
 return(_r);
}
 
static int _ode_count(int _type){ return 2;}
 
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
  ek = _ion_ek;
  ncai = _ion_ncai;
     _ode_spec1 (_threadargs_);
  }}
 
static void _ode_map(Prop* _prop, int _ieq, neuron::container::data_handle<double>* _pv, neuron::container::data_handle<double>* _pvdot, double* _atol, int _type) { 
  Datum* _ppvar;
  _ppvar = _nrn_mechanism_access_dparam(_prop);
  _cvode_ieq = _ieq;
  for (int _i=0; _i < 2; ++_i) {
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
  ek = _ion_ek;
  ncai = _ion_ncai;
 _ode_matsol_instance1(_threadargs_);
 }}

static void initmodel(_internalthreadargsproto_) {
  int _i; double _save;{
  ab = ab0;
  a = a0;
 {
   rates ( _threadargscomma_ v , cai ) ;
   a = ainf ;
   ab = abinf ;
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
  ek = _ion_ek;
  ncai = _ion_ncai;
 initmodel(_threadargs_);
 }
}

static double _nrn_current(_internalthreadargsprotocomma_ double _v) {
double _current=0.; v=_v;
{ {
   gak = gakbar * a ;
   gabk = gabkbar * ab ;
   ik = ( gabk + gak ) * ( v - ek ) ;
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
  cai = _ion_cai;
  ek = _ion_ek;
  ncai = _ion_ncai;
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
  ek = _ion_ek;
  ncai = _ion_ncai;
 {   state(_threadargs_);
  } }}

}

static void terminal(){}

static void _initlists(){
 int _i; static int _first = 1;
  if (!_first) return;
 _slist1[0] = {a_columnindex, 0};  _dlist1[0] = {Da_columnindex, 0};
 _slist1[1] = {ab_columnindex, 0};  _dlist1[1] = {Dab_columnindex, 0};
_first = 0;
}

#if NMODL_TEXT
static void register_nmodl_text_and_filename(int mech_type) {
    const char* nmodl_filename = "/home/raven/PycharmProjects/Masters_model/Mod_Files_Beining_2017/BK.mod";
    const char* nmodl_file_text = 
  ": Ca-dependent K channels (BK) - alphabeta4 and alpha\n"
  ": Bin Wang, Robert Brenner, and David Jaffe - Originally written May 27, 2010\n"
  ": \n"
  ": June 1, 2010 - added double exponential function for voltage-dependent activation \n"
  ":\n"
  ": July 3, 2010 - changed voltage-dependence for the two channels based on revised data\n"
  ":\n"
  ": April 2, 2011 - adjusted parameters based on updated Bin data\n"
  "\n"
  ": Mar 2016 - added instant N-type Calcium concentration (Beining et al (2016), \"A novel comprehensive and consistent electrophysiologcal model of dentate granule cells\")\n"
  "\n"
  "NEURON {\n"
  "	SUFFIX BK\n"
  "	USEION ca READ cai\n"
  "	USEION k READ ek WRITE ik\n"
  "	USEION nca READ ncai VALENCE 0\n"
  "	RANGE gakbar, gabkbar, gak, gabk, atau, ainf, a, ab, abinf, abtau,  ik, diff, acai\n"
  "	GLOBAL base\n"
  "}\n"
  "\n"
  "UNITS {\n"
  "	(molar) = (1/liter)\n"
  "	(mM) = (millimolar)\n"
  "	(mV) = (millivolt)\n"
  "	(mA) = (milliamp)\n"
  "	(S) = (siemens)\n"
  "}\n"
  "\n"
  "PARAMETER {\n"
  "	diff = 1 (1)\n"
  "	gakbar = .01	(S/cm2)	: maximum permeability - alpha\n"
  "	gabkbar = .01	(S/cm2)	: maximum permeability - alphabeta\n"
  "	base = 4  	(mV)	: alphabeta4 base time constant\n"
  "	\n"
  "}\n"
  "\n"
  "ASSIGNED {\n"
  "	v				(mV)\n"
  "	ek			(mV)\n"
  "	\n"
  "	ik				(mA/cm2)\n"
  "      gak			(S/cm2)\n"
  "      gabk		(S/cm2)\n"
  "      ainf	\n"
  "      atau		(ms)\n"
  "      abinf	\n"
  "      abtau		(ms)\n"
  "	  cai 			(mM)\n"
  "	  ncai		(mM)\n"
  "	  acai		(mM)\n"
  "\n"
  "}\n"
  "\n"
  "STATE {ab a} :a\n"
  "\n"
  "BREAKPOINT {\n"
  "	SOLVE state METHOD cnexp\n"
  "	gak = gakbar*a\n"
  "	gabk = gabkbar*ab\n"
  "	ik = (gabk+gak)*(v - ek)                               :+ gak\n"
  "}\n"
  "\n"
  "DERIVATIVE state {\n"
  "	: exact when v held constant; integrates over dt step\n"
  "	\n"
  "	acai = (ncai) / diff : instantaneous calcium concentration of N-type Ca channels for BK activation modified by diffusion factor for nanodomains\n"
  "\n"
  "	if (acai < cai)\n"
  "		{acai = cai}\n"
  "		\n"
  "	rates(v,acai)				      \n"
  "	a' = (ainf-a)/atau	\n"
  "	ab' = (abinf-ab)/abtau\n"
  "}\n"
  "\n"
  "INITIAL {\n"
  "	rates(v, cai)\n"
  "	a = ainf\n"
  "	ab = abinf\n"
  "}\n"
  "\n"
  ": alpha channel properties\n"
  "FUNCTION shifta(ca (mM))  {\n"
  "	shifta = 25 - 50.3 + (107.5*exp(-.12*ca*1e3))\n"
  "}\n"
  "\n"
  "\n"
  "FUNCTION peaka(ca (mM))  {\n"
  "	peaka = 2.9 + (6.3*exp(-.36*ca*1e3))\n"
  "}\n"
  "\n"
  ": alpha-beta4 channel properties\n"
  "\n"
  "\n"
  "FUNCTION shiftab(ca (mM))  {\n"
  "	shiftab = 25 - 55.7 + 136.9*exp(-.28*ca*1e3)\n"
  "}\n"
  "\n"
  "\n"
  "FUNCTION peakab(ca (mM))  {\n"
  "	peakab = 13.7 + 234*exp(-.72*ca*1e3)\n"
  "}\n"
  "\n"
  ": Double sigmoid function for tau voltage-dependence\n"
  "\n"
  "\n"
  "FUNCTION taufunc(v (mV)) {\n"
  "	 taufunc = 1 / (          (10*(exp(-v/63.6) + exp (-(150-v)/63.6)))  - 5.2                  )\n"
  "	 if (taufunc <= 0.2) {	  : stop the function between 0.2 and 1\n"
  "	    taufunc = 0.2\n"
  "	 }\n"
  "}\n"
  "\n"
  "PROCEDURE rates(v (mV), c (mM)) { : nc (mM), \n"
  "	  LOCAL range, vv,ashift, bshift\n"
  "\n"
  "	  : alpha model\n"
  "\n"
  "	  ashift =  -32 + (59.2*exp(-.09*c*1e3)) + (96.7*exp(-.47*c*1e3))\n"
  "	  ainf = 1/(1+exp((ashift-v)/(25/1.6)))\n"
  "\n"
  "	  vv = v + 100 - shifta(c)\n"
  "	  atau = taufunc(vv)\n"
  "	  range = peaka(c)-1\n"
  "	  atau = (range*((atau-.2)/.8)) + 1\n"
  "\n"
  "	  : alpha-beta4 model\n"
  "\n"
  "	  bshift = -56.449 + 104.52*exp(-.22964*c*1e3) + 295.68*exp(-2.1571*c*1e3)\n"
  "\n"
  "	  abinf = 1/(1+exp((bshift-v)/(25/1.6)))\n"
  "\n"
  "	  vv = v + 100 - shiftab(c)\n"
  "	  abtau = taufunc(vv)\n"
  "	  range = peakab(c)-base\n"
  "	  abtau = (range*((abtau-.2)/.8)) + base		\n"
  "\n"
  "}\n"
  ;
    hoc_reg_nmodl_filename(mech_type, nmodl_filename);
    hoc_reg_nmodl_text(mech_type, nmodl_file_text);
}
#endif
