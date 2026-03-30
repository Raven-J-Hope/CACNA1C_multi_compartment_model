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
static constexpr auto number_of_datum_variables = 6;
static constexpr auto number_of_floating_point_variables = 27;
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
 
#define nrn_init _nrn_init__Cav2_1
#define _nrn_initial _nrn_initial__Cav2_1
#define nrn_cur _nrn_cur__Cav2_1
#define _nrn_current _nrn_current__Cav2_1
#define nrn_jacob _nrn_jacob__Cav2_1
#define nrn_state _nrn_state__Cav2_1
#define _net_receive _net_receive__Cav2_1 
#define rates rates__Cav2_1 
#define states states__Cav2_1 
 
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
#define vhalfm _ml->template fpfield<0>(_iml)
#define vhalfm_columnindex 0
#define cvm _ml->template fpfield<1>(_iml)
#define cvm_columnindex 1
#define vhalfh _ml->template fpfield<2>(_iml)
#define vhalfh_columnindex 2
#define cvh _ml->template fpfield<3>(_iml)
#define cvh_columnindex 3
#define vshift _ml->template fpfield<4>(_iml)
#define vshift_columnindex 4
#define tauh_base _ml->template fpfield<5>(_iml)
#define tauh_base_columnindex 5
#define tauh_amp _ml->template fpfield<6>(_iml)
#define tauh_amp_columnindex 6
#define pcabar _ml->template fpfield<7>(_iml)
#define pcabar_columnindex 7
#define ica _ml->template fpfield<8>(_iml)
#define ica_columnindex 8
#define ipca _ml->template fpfield<9>(_iml)
#define ipca_columnindex 9
#define minf _ml->template fpfield<10>(_iml)
#define minf_columnindex 10
#define hinf _ml->template fpfield<11>(_iml)
#define hinf_columnindex 11
#define taum _ml->template fpfield<12>(_iml)
#define taum_columnindex 12
#define tauh _ml->template fpfield<13>(_iml)
#define tauh_columnindex 13
#define gk _ml->template fpfield<14>(_iml)
#define gk_columnindex 14
#define m _ml->template fpfield<15>(_iml)
#define m_columnindex 15
#define h _ml->template fpfield<16>(_iml)
#define h_columnindex 16
#define cai _ml->template fpfield<17>(_iml)
#define cai_columnindex 17
#define cao _ml->template fpfield<18>(_iml)
#define cao_columnindex 18
#define qt _ml->template fpfield<19>(_iml)
#define qt_columnindex 19
#define T _ml->template fpfield<20>(_iml)
#define T_columnindex 20
#define E _ml->template fpfield<21>(_iml)
#define E_columnindex 21
#define zeta _ml->template fpfield<22>(_iml)
#define zeta_columnindex 22
#define Dm _ml->template fpfield<23>(_iml)
#define Dm_columnindex 23
#define Dh _ml->template fpfield<24>(_iml)
#define Dh_columnindex 24
#define v _ml->template fpfield<25>(_iml)
#define v_columnindex 25
#define _g _ml->template fpfield<26>(_iml)
#define _g_columnindex 26
#define _ion_cai *(_ml->dptr_field<0>(_iml))
#define _p_ion_cai static_cast<neuron::container::data_handle<double>>(_ppvar[0])
#define _ion_cao *(_ml->dptr_field<1>(_iml))
#define _p_ion_cao static_cast<neuron::container::data_handle<double>>(_ppvar[1])
#define _ion_ica *(_ml->dptr_field<2>(_iml))
#define _p_ion_ica static_cast<neuron::container::data_handle<double>>(_ppvar[2])
#define _ion_dicadv *(_ml->dptr_field<3>(_iml))
#define _ion_ipca *(_ml->dptr_field<4>(_iml))
#define _p_ion_ipca static_cast<neuron::container::data_handle<double>>(_ppvar[4])
#define _ion_dipcadv *(_ml->dptr_field<5>(_iml))
 /* Thread safe. No static _ml, _iml or _ppvar. */
 static int hoc_nrnpointerindex =  -1;
 static _nrn_mechanism_std_vector<Datum> _extcall_thread;
 static Prop* _extcall_prop;
 /* _prop_id kind of shadows _extcall_prop to allow validity checking. */
 static _nrn_non_owning_id_without_container _prop_id{};
 /* external NEURON variables */
 extern double celsius;
 /* declaration of user functions */
 static void _hoc_ghk(void);
 static void _hoc_kelvinfkt(void);
 static void _hoc_rates(void);
 static void _hoc_taumfkt(void);
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
 {"setdata_Cav2_1", _hoc_setdata},
 {"ghk_Cav2_1", _hoc_ghk},
 {"kelvinfkt_Cav2_1", _hoc_kelvinfkt},
 {"rates_Cav2_1", _hoc_rates},
 {"taumfkt_Cav2_1", _hoc_taumfkt},
 {0, 0}
};
 
/* Direct Python call wrappers to density mechanism functions.*/
 static double _npy_ghk(Prop*);
 static double _npy_kelvinfkt(Prop*);
 static double _npy_rates(Prop*);
 static double _npy_taumfkt(Prop*);
 
static NPyDirectMechFunc npy_direct_func_proc[] = {
 {"ghk", _npy_ghk},
 {"kelvinfkt", _npy_kelvinfkt},
 {"rates", _npy_rates},
 {"taumfkt", _npy_taumfkt},
 {0, 0}
};
#define ghk ghk_Cav2_1
#define kelvinfkt kelvinfkt_Cav2_1
#define taumfkt taumfkt_Cav2_1
 extern double ghk( _internalthreadargsprotocomma_ double , double , double , double );
 extern double kelvinfkt( _internalthreadargsprotocomma_ double );
 extern double taumfkt( _internalthreadargsprotocomma_ double );
 /* declare global and static user variables */
 #define gind 0
 #define _gth 0
 /* some parameters have upper and lower limits */
 static HocParmLimits _hoc_parm_limits[] = {
 {0, 0, 0}
};
 static HocParmUnits _hoc_parm_units[] = {
 {"vhalfm_Cav2_1", "mV"},
 {"cvm_Cav2_1", "mV"},
 {"vhalfh_Cav2_1", "mV"},
 {"cvh_Cav2_1", "mV"},
 {"vshift_Cav2_1", "mV"},
 {"tauh_base_Cav2_1", "ms"},
 {"tauh_amp_Cav2_1", "ms"},
 {"pcabar_Cav2_1", "cm/s"},
 {"ica_Cav2_1", "mA/cm2"},
 {"ipca_Cav2_1", "mA/cm2"},
 {"taum_Cav2_1", "ms"},
 {"tauh_Cav2_1", "ms"},
 {"gk_Cav2_1", "coulombs/cm3"},
 {0, 0}
};
 static double delta_t = 0.01;
 static double h0 = 0;
 static double m0 = 0;
 /* connect global user variables to hoc */
 static DoubScal hoc_scdoub[] = {
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
 
#define _cvode_ieq _ppvar[6].literal_value<int>()
 static void _ode_matsol_instance1(_internalthreadargsproto_);
 /* connect range variables in _p that hoc is supposed to know about */
 static const char *_mechanism[] = {
 "7.7.0",
"Cav2_1",
 "vhalfm_Cav2_1",
 "cvm_Cav2_1",
 "vhalfh_Cav2_1",
 "cvh_Cav2_1",
 "vshift_Cav2_1",
 "tauh_base_Cav2_1",
 "tauh_amp_Cav2_1",
 "pcabar_Cav2_1",
 0,
 "ica_Cav2_1",
 "ipca_Cav2_1",
 "minf_Cav2_1",
 "hinf_Cav2_1",
 "taum_Cav2_1",
 "tauh_Cav2_1",
 "gk_Cav2_1",
 0,
 "m_Cav2_1",
 "h_Cav2_1",
 0,
 0};
 static Symbol* _ca_sym;
 static Symbol* _pca_sym;
 
 /* Used by NrnProperty */
 static _nrn_mechanism_std_vector<double> _parm_default{
     -29.458, /* vhalfm */
     8.429, /* cvm */
     -45, /* vhalfh */
     10, /* cvh */
     0, /* vshift */
     50, /* tauh_base */
     30, /* tauh_amp */
     0.00022, /* pcabar */
 }; 
 
 
extern Prop* need_memb(Symbol*);
static void nrn_alloc(Prop* _prop) {
  Prop *prop_ion{};
  Datum *_ppvar{};
   _ppvar = nrn_prop_datum_alloc(_mechtype, 7, _prop);
    _nrn_mechanism_access_dparam(_prop) = _ppvar;
     _nrn_mechanism_cache_instance _ml_real{_prop};
    auto* const _ml = &_ml_real;
    size_t const _iml{};
    assert(_nrn_mechanism_get_num_vars(_prop) == 27);
 	/*initialize range parameters*/
 	vhalfm = _parm_default[0]; /* -29.458 */
 	cvm = _parm_default[1]; /* 8.429 */
 	vhalfh = _parm_default[2]; /* -45 */
 	cvh = _parm_default[3]; /* 10 */
 	vshift = _parm_default[4]; /* 0 */
 	tauh_base = _parm_default[5]; /* 50 */
 	tauh_amp = _parm_default[6]; /* 30 */
 	pcabar = _parm_default[7]; /* 0.00022 */
 	 assert(_nrn_mechanism_get_num_vars(_prop) == 27);
 	_nrn_mechanism_access_dparam(_prop) = _ppvar;
 	/*connect ionic variables to this model*/
 prop_ion = need_memb(_ca_sym);
 nrn_promote(prop_ion, 1, 0);
 	_ppvar[0] = _nrn_mechanism_get_param_handle(prop_ion, 1); /* cai */
 	_ppvar[1] = _nrn_mechanism_get_param_handle(prop_ion, 2); /* cao */
 	_ppvar[2] = _nrn_mechanism_get_param_handle(prop_ion, 3); /* ica */
 	_ppvar[3] = _nrn_mechanism_get_param_handle(prop_ion, 4); /* _ion_dicadv */
 prop_ion = need_memb(_pca_sym);
 	_ppvar[4] = _nrn_mechanism_get_param_handle(prop_ion, 3); /* ipca */
 	_ppvar[5] = _nrn_mechanism_get_param_handle(prop_ion, 4); /* _ion_dipcadv */
 
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

 extern "C" void _Cav2_1_reg() {
	int _vectorized = 1;
  _initlists();
 	ion_reg("ca", -10000.);
 	ion_reg("pca", 0.0);
 	_ca_sym = hoc_lookup("ca_ion");
 	_pca_sym = hoc_lookup("pca_ion");
 	register_mech(_mechanism, nrn_alloc,nrn_cur, nrn_jacob, nrn_state, nrn_init, hoc_nrnpointerindex, 1);
 _mechtype = nrn_get_mechtype(_mechanism[1]);
 hoc_register_parm_default(_mechtype, &_parm_default);
         hoc_register_npy_direct(_mechtype, npy_direct_func_proc);
     _nrn_setdata_reg(_mechtype, _setdata);
 #if NMODL_TEXT
  register_nmodl_text_and_filename(_mechtype);
#endif
   _nrn_mechanism_register_data_fields(_mechtype,
                                       _nrn_mechanism_field<double>{"vhalfm"} /* 0 */,
                                       _nrn_mechanism_field<double>{"cvm"} /* 1 */,
                                       _nrn_mechanism_field<double>{"vhalfh"} /* 2 */,
                                       _nrn_mechanism_field<double>{"cvh"} /* 3 */,
                                       _nrn_mechanism_field<double>{"vshift"} /* 4 */,
                                       _nrn_mechanism_field<double>{"tauh_base"} /* 5 */,
                                       _nrn_mechanism_field<double>{"tauh_amp"} /* 6 */,
                                       _nrn_mechanism_field<double>{"pcabar"} /* 7 */,
                                       _nrn_mechanism_field<double>{"ica"} /* 8 */,
                                       _nrn_mechanism_field<double>{"ipca"} /* 9 */,
                                       _nrn_mechanism_field<double>{"minf"} /* 10 */,
                                       _nrn_mechanism_field<double>{"hinf"} /* 11 */,
                                       _nrn_mechanism_field<double>{"taum"} /* 12 */,
                                       _nrn_mechanism_field<double>{"tauh"} /* 13 */,
                                       _nrn_mechanism_field<double>{"gk"} /* 14 */,
                                       _nrn_mechanism_field<double>{"m"} /* 15 */,
                                       _nrn_mechanism_field<double>{"h"} /* 16 */,
                                       _nrn_mechanism_field<double>{"cai"} /* 17 */,
                                       _nrn_mechanism_field<double>{"cao"} /* 18 */,
                                       _nrn_mechanism_field<double>{"qt"} /* 19 */,
                                       _nrn_mechanism_field<double>{"T"} /* 20 */,
                                       _nrn_mechanism_field<double>{"E"} /* 21 */,
                                       _nrn_mechanism_field<double>{"zeta"} /* 22 */,
                                       _nrn_mechanism_field<double>{"Dm"} /* 23 */,
                                       _nrn_mechanism_field<double>{"Dh"} /* 24 */,
                                       _nrn_mechanism_field<double>{"v"} /* 25 */,
                                       _nrn_mechanism_field<double>{"_g"} /* 26 */,
                                       _nrn_mechanism_field<double*>{"_ion_cai", "ca_ion"} /* 0 */,
                                       _nrn_mechanism_field<double*>{"_ion_cao", "ca_ion"} /* 1 */,
                                       _nrn_mechanism_field<double*>{"_ion_ica", "ca_ion"} /* 2 */,
                                       _nrn_mechanism_field<double*>{"_ion_dicadv", "ca_ion"} /* 3 */,
                                       _nrn_mechanism_field<double*>{"_ion_ipca", "pca_ion"} /* 4 */,
                                       _nrn_mechanism_field<double*>{"_ion_dipcadv", "pca_ion"} /* 5 */,
                                       _nrn_mechanism_field<int>{"_cvode_ieq", "cvodeieq"} /* 6 */);
  hoc_register_prop_size(_mechtype, 27, 7);
  hoc_register_dparam_semantics(_mechtype, 0, "ca_ion");
  hoc_register_dparam_semantics(_mechtype, 1, "ca_ion");
  hoc_register_dparam_semantics(_mechtype, 2, "ca_ion");
  hoc_register_dparam_semantics(_mechtype, 3, "ca_ion");
  hoc_register_dparam_semantics(_mechtype, 4, "pca_ion");
  hoc_register_dparam_semantics(_mechtype, 5, "pca_ion");
  hoc_register_dparam_semantics(_mechtype, 6, "cvodeieq");
 	hoc_register_cvode(_mechtype, _ode_count, _ode_map, _ode_spec, _ode_matsol);
 	hoc_register_tolerance(_mechtype, _hoc_state_tol, &_atollist);
 
    hoc_register_var(hoc_scdoub, hoc_vdoub, hoc_intfunc);
 	ivoc_help("help ?1 Cav2_1 /home/raven/PycharmProjects/Masters/Mod_Files/Cav2_1.mod\n");
 hoc_register_limits(_mechtype, _hoc_parm_limits);
 hoc_register_units(_mechtype, _hoc_parm_units);
 }
 static double q10 = 3;
 static double F = 9.6485e4;
 static double R = 8.3145;
static int _reset;
static const char *modelname = "P-type calcium channel";

static int error;
static int _ninits = 0;
static int _match_recurse=1;
static void _modl_cleanup(){ _match_recurse=1;}
static int rates(_internalthreadargsprotocomma_ double);
 
static int _ode_spec1(_internalthreadargsproto_);
/*static int _ode_matsol1(_internalthreadargsproto_);*/
 static neuron::container::field_index _slist1[2], _dlist1[2];
 static int states(_internalthreadargsproto_);
 
/*CVODE*/
 static int _ode_spec1 (_internalthreadargsproto_) {int _reset = 0; {
   rates ( _threadargscomma_ v ) ;
   Dm = ( minf - m ) / taum ;
   Dh = ( hinf - h ) / tauh ;
   }
 return _reset;
}
 static int _ode_matsol1 (_internalthreadargsproto_) {
 rates ( _threadargscomma_ v ) ;
 Dm = Dm  / (1. - dt*( ( ( ( - 1.0 ) ) ) / taum )) ;
 Dh = Dh  / (1. - dt*( ( ( ( - 1.0 ) ) ) / tauh )) ;
  return 0;
}
 /*END CVODE*/
 static int states (_internalthreadargsproto_) { {
   rates ( _threadargscomma_ v ) ;
    m = m + (1. - exp(dt*(( ( ( - 1.0 ) ) ) / taum)))*(- ( ( ( minf ) ) / taum ) / ( ( ( ( - 1.0 ) ) ) / taum ) - m) ;
    h = h + (1. - exp(dt*(( ( ( - 1.0 ) ) ) / tauh)))*(- ( ( ( hinf ) ) / tauh ) / ( ( ( ( - 1.0 ) ) ) / tauh ) - h) ;
   }
  return 0;
}
 
double ghk ( _internalthreadargsprotocomma_ double _lv , double _lci , double _lco , double _lz ) {
   double _lghk;
 E = ( 1e-3 ) * _lv ;
   zeta = ( _lz * F * E ) / ( R * T ) ;
   if ( fabs ( 1.0 - exp ( - zeta ) ) < 1e-6 ) {
     _lghk = ( 1e-6 ) * ( _lz * F ) * ( _lci - _lco * exp ( - zeta ) ) * ( 1.0 + zeta / 2.0 ) ;
     }
   else {
     _lghk = ( 1e-6 ) * ( _lz * zeta * F ) * ( _lci - _lco * exp ( - zeta ) ) / ( 1.0 - exp ( - zeta ) ) ;
     }
   
return _lghk;
 }
 
static void _hoc_ghk(void) {
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
 _r =  ghk ( _threadargscomma_ *getarg(1) , *getarg(2) , *getarg(3) , *getarg(4) );
 hoc_retpushx(_r);
}
 
static double _npy_ghk(Prop* _prop) {
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
 _r =  ghk ( _threadargscomma_ *getarg(1) , *getarg(2) , *getarg(3) , *getarg(4) );
 return(_r);
}
 
static int  rates ( _internalthreadargsprotocomma_ double _lv ) {
   minf = 1.0 / ( 1.0 + exp ( - ( _lv - vhalfm - vshift ) / cvm ) ) ;
   taum = taumfkt ( _threadargscomma_ _lv - vshift ) / qt ;
   hinf = 1.0 / ( 1.0 + exp ( ( _lv - vhalfh - vshift ) / cvh ) ) ;
   tauh = ( tauh_base + tauh_amp / ( 1.0 + exp ( ( _lv + 20.0 ) / 8.0 ) ) ) / qt ;
   gk = ghk ( _threadargscomma_ _lv - vshift , cai , cao , 2.0 ) ;
    return 0; }
 
static void _hoc_rates(void) {
  double _r;
 Datum* _ppvar; Datum* _thread; NrnThread* _nt;
 
  if(!_prop_id) {
    hoc_execerror("No data for rates_Cav2_1. Requires prior call to setdata_Cav2_1 and that the specified mechanism instance still be in existence.", NULL);
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
 rates ( _threadargscomma_ *getarg(1) );
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
 rates ( _threadargscomma_ *getarg(1) );
 return(_r);
}
 
double kelvinfkt ( _internalthreadargsprotocomma_ double _lt ) {
   double _lkelvinfkt;
  _lkelvinfkt = 273.19 + _lt ;
    
return _lkelvinfkt;
 }
 
static void _hoc_kelvinfkt(void) {
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
 _r =  kelvinfkt ( _threadargscomma_ *getarg(1) );
 hoc_retpushx(_r);
}
 
static double _npy_kelvinfkt(Prop* _prop) {
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
 _r =  kelvinfkt ( _threadargscomma_ *getarg(1) );
 return(_r);
}
 
double taumfkt ( _internalthreadargsprotocomma_ double _lv ) {
   double _ltaumfkt;
  if ( _lv >= - 40.0 ) {
     _ltaumfkt = 0.2702 + 1.1622 * exp ( - ( _lv + 26.798 ) * ( _lv + 26.798 ) / 164.19 ) ;
     }
   else {
     _ltaumfkt = 0.6923 * exp ( _lv / 1089.372 ) ;
     }
    
return _ltaumfkt;
 }
 
static void _hoc_taumfkt(void) {
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
 _r =  taumfkt ( _threadargscomma_ *getarg(1) );
 hoc_retpushx(_r);
}
 
static double _npy_taumfkt(Prop* _prop) {
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
 _r =  taumfkt ( _threadargscomma_ *getarg(1) );
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
  cao = _ion_cao;
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
  cao = _ion_cao;
 _ode_matsol_instance1(_threadargs_);
 }}

static void initmodel(_internalthreadargsproto_) {
  int _i; double _save;{
  h = h0;
  m = m0;
 {
   qt = pow( q10 , ( ( celsius - 23.0 ) / 10.0 ) ) ;
   T = kelvinfkt ( _threadargscomma_ celsius ) ;
   rates ( _threadargscomma_ v ) ;
   m = minf ;
   h = hinf ;
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
  cao = _ion_cao;
 initmodel(_threadargs_);
  }
}

static double _nrn_current(_internalthreadargsprotocomma_ double _v) {
double _current=0.; v=_v;
{ {
   ica = ( 1e3 ) * pcabar * m * m * m * h * gk ;
   ipca = ica ;
   }
 _current += ica;
 _current += ipca;

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
  cao = _ion_cao;
 auto const _g_local = _nrn_current(_threadargscomma_ _v + .001);
 	{ double _dipca;
 double _dica;
  _dica = ica;
  _dipca = ipca;
 _rhs = _nrn_current(_threadargscomma_ _v);
  _ion_dicadv += (_dica - ica)/.001 ;
  _ion_dipcadv += (_dipca - ipca)/.001 ;
 	}
 _g = (_g_local - _rhs)/.001;
  _ion_ica += ica ;
  _ion_ipca += ipca ;
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
  cao = _ion_cao;
 {   states(_threadargs_);
  }  }}

}

static void terminal(){}

static void _initlists(){
 int _i; static int _first = 1;
  if (!_first) return;
 _slist1[0] = {m_columnindex, 0};  _dlist1[0] = {Dm_columnindex, 0};
 _slist1[1] = {h_columnindex, 0};  _dlist1[1] = {Dh_columnindex, 0};
_first = 0;
}

#if NMODL_TEXT
static void register_nmodl_text_and_filename(int mech_type) {
    const char* nmodl_filename = "/home/raven/PycharmProjects/Masters/Mod_Files/Cav2_1.mod";
    const char* nmodl_file_text = 
  "TITLE P-type calcium channel\n"
  "\n"
  "COMMENT\n"
  "\n"
  "Constructed from the recording data provided by Bruce Bean.\n"
  "Reference: Swensen AM and Bean BP (2005) Robustness of burst firing in dissociated purkinje neurons with acute or long-term reductions in sodium conductance. J Neurosci 25:3509-20\n"
  "\n"
  "Current Model Reference: Anwar H, Hong S, De Schutter E (2010) Controlling Ca2+-activated K+ channels with models of Ca2+ buffering in Purkinje cell. Cerebellum*\n"
  "\n"
  "*Article available as Open Access\n"
  "\n"
  "PubMed link: http://www.ncbi.nlm.nih.gov/pubmed/20981513\n"
  "\n"
  "\n"
  "Written by Sungho Hong, Computational Neuroscience Unit, Okinawa Institute of Science and Technology, 2009.\n"
  "Contact: Sungho Hong (shhong@oist.jp)\n"
  "\n"
  "Suffix from newCaP to Cav2_1\n"
  "\n"
  "\n"
  "ENDCOMMENT\n"
  "\n"
  "INDEPENDENT {t FROM 0 TO 1 WITH 1 (ms)}\n"
  "\n"
  "NEURON {\n"
  "    SUFFIX Cav2_1\n"
  "    USEION ca READ cai, cao WRITE ica\n"
  "    USEION pca WRITE ipca VALENCE 0\n"
  "    RANGE pcabar, ica, ipca, gk, vhalfm, cvm, vhalfh, cvh, vshift, taum, tauh, minf, hinf, tauh_base, tauh_amp\n"
  "}\n"
  "\n"
  "UNITS {\n"
  "    (mV) = (millivolt)\n"
  "    (mA) = (milliamp)\n"
  "    (nA) = (nanoamp)\n"
  "    (pA) = (picoamp)\n"
  "    (S)  = (siemens)\n"
  "    (nS) = (nanosiemens)\n"
  "    (pS) = (picosiemens)\n"
  "    (um) = (micron)\n"
  "    (molar) = (1/liter)\n"
  "    (mM) = (millimolar)     \n"
  "}\n"
  "\n"
  "CONSTANT {\n"
  "    q10 = 3\n"
  "    F = 9.6485e4 (coulombs)\n"
  "    R = 8.3145 (joule/kelvin)\n"
  "}\n"
  "\n"
  "PARAMETER {\n"
  "    v (mV)\n"
  "    celsius (degC)\n"
  "\n"
  "    cai (mM)\n"
  "    cao (mM)\n"
  "\n"
  "    vhalfm = -29.458 (mV)\n"
  "    cvm = 8.429(mV)\n"
  "    vhalfh = -45 (mV)\n"
  "    cvh = 10 (mV)\n"
  "    vshift = 0 (mV)\n"
  "\n"
  "    tauh_base = 50 (ms)\n"
  "    tauh_amp = 30 (ms)\n"
  "\n"
  "    pcabar = 2.2e-4 (cm/s)\n"
  "}\n"
  "\n"
  "ASSIGNED {\n"
  "    qt\n"
  "    ica (mA/cm2)\n"
  "    ipca (mA/cm2)\n"
  "    minf\n"
  "    hinf\n"
  "    taum (ms)\n"
  "    tauh (ms)\n"
  "    gk (coulombs/cm3)\n"
  "    T (kelvin)\n"
  "    E (volt)\n"
  "    zeta\n"
  "}\n"
  "\n"
  "STATE { m h }\n"
  "\n"
  "INITIAL {\n"
  "    qt = q10^((celsius-23 (degC))/10 (degC))\n"
  "    T = kelvinfkt( celsius )\n"
  "    rates(v)\n"
  "    m = minf\n"
  "    h = hinf\n"
  "}\n"
  "\n"
  "BREAKPOINT {\n"
  "    SOLVE states METHOD cnexp\n"
  "\n"
  "    ica = (1e3) * pcabar * m * m * m * h * gk\n"
  "    ipca = ica\n"
  "}\n"
  "\n"
  "DERIVATIVE states {\n"
  "    rates(v)\n"
  "    m' = (minf-m)/taum\n"
  "    h' = (hinf-h)/tauh\n"
  "}\n"
  "\n"
  "FUNCTION ghk( v (mV), ci (mM), co (mM), z )  (coulombs/cm3) { \n"
  "    E = (1e-3) * v\n"
  "      zeta = (z*F*E)/(R*T)  \n"
  "    \n"
  "    if ( fabs(1-exp(-zeta)) < 1e-6 ) {\n"
  "        ghk = (1e-6) * (z*F) * (ci - co*exp(-zeta)) * (1 + zeta/2)\n"
  "    } else {\n"
  "        ghk = (1e-6) * (z*zeta*F) * (ci - co*exp(-zeta)) / (1-exp(-zeta))\n"
  "    }\n"
  "}\n"
  "\n"
  "PROCEDURE rates( v (mV) ) {\n"
  "\n"
  "    minf = 1 / ( 1 + exp(-(v-vhalfm-vshift)/cvm) )\n"
  "    taum = taumfkt(v-vshift)/qt\n"
  "\n"
  "    hinf = 1 / ( 1 + exp((v-vhalfh-vshift)/cvh) )\n"
  "    tauh = (tauh_base + tauh_amp / (1 + exp((v + 20)/8))) / qt\n"
  "\n"
  "    gk = ghk(v-vshift, cai, cao, 2)\n"
  "}\n"
  "\n"
  "\n"
  "FUNCTION kelvinfkt( t (degC) )  (kelvin) {\n"
  "    UNITSOFF\n"
  "    kelvinfkt = 273.19 + t\n"
  "    UNITSON\n"
  "}\n"
  "\n"
  "FUNCTION taumfkt( v (mV) ) (ms) {\n"
  "    UNITSOFF\n"
  "    if (v>=-40) {\n"
  "        taumfkt = 0.2702 + 1.1622 * exp(-(v+26.798)*(v+26.798)/164.19)\n"
  "    } else {\n"
  "        taumfkt = 0.6923 * exp(v/1089.372)\n"
  "    }\n"
  "    UNITSON\n"
  "}\n"
  "\n"
  ;
    hoc_reg_nmodl_filename(mech_type, nmodl_filename);
    hoc_reg_nmodl_text(mech_type, nmodl_file_text);
}
#endif
