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
static constexpr auto number_of_floating_point_variables = 29;
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
 
#define nrn_init _nrn_init__Kv723
#define _nrn_initial _nrn_initial__Kv723
#define nrn_cur _nrn_cur__Kv723
#define _nrn_current _nrn_current__Kv723
#define nrn_jacob _nrn_jacob__Kv723
#define nrn_state _nrn_state__Kv723
#define _net_receive _net_receive__Kv723 
#define _f_rates _f_rates__Kv723 
#define rates rates__Kv723 
#define states states__Kv723 
 
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
#define i _ml->template fpfield<1>(_iml)
#define i_columnindex 1
#define ik _ml->template fpfield<2>(_iml)
#define ik_columnindex 2
#define gk _ml->template fpfield<3>(_iml)
#define gk_columnindex 3
#define minf _ml->template fpfield<4>(_iml)
#define minf_columnindex 4
#define tau1 _ml->template fpfield<5>(_iml)
#define tau1_columnindex 5
#define tau2 _ml->template fpfield<6>(_iml)
#define tau2_columnindex 6
#define tadjtau _ml->template fpfield<7>(_iml)
#define tadjtau_columnindex 7
#define m1 _ml->template fpfield<8>(_iml)
#define m1_columnindex 8
#define m2 _ml->template fpfield<9>(_iml)
#define m2_columnindex 9
#define ek _ml->template fpfield<10>(_iml)
#define ek_columnindex 10
#define Vhalf1 _ml->template fpfield<11>(_iml)
#define Vhalf1_columnindex 11
#define Dtau1 _ml->template fpfield<12>(_iml)
#define Dtau1_columnindex 12
#define z1 _ml->template fpfield<13>(_iml)
#define z1_columnindex 13
#define tau01 _ml->template fpfield<14>(_iml)
#define tau01_columnindex 14
#define Vhalf2 _ml->template fpfield<15>(_iml)
#define Vhalf2_columnindex 15
#define Dtau2 _ml->template fpfield<16>(_iml)
#define Dtau2_columnindex 16
#define z2 _ml->template fpfield<17>(_iml)
#define z2_columnindex 17
#define tau02 _ml->template fpfield<18>(_iml)
#define tau02_columnindex 18
#define alpha1 _ml->template fpfield<19>(_iml)
#define alpha1_columnindex 19
#define beta1 _ml->template fpfield<20>(_iml)
#define beta1_columnindex 20
#define alpha2 _ml->template fpfield<21>(_iml)
#define alpha2_columnindex 21
#define beta2 _ml->template fpfield<22>(_iml)
#define beta2_columnindex 22
#define v0 _ml->template fpfield<23>(_iml)
#define v0_columnindex 23
#define frt _ml->template fpfield<24>(_iml)
#define frt_columnindex 24
#define Dm1 _ml->template fpfield<25>(_iml)
#define Dm1_columnindex 25
#define Dm2 _ml->template fpfield<26>(_iml)
#define Dm2_columnindex 26
#define v _ml->template fpfield<27>(_iml)
#define v_columnindex 27
#define _g _ml->template fpfield<28>(_iml)
#define _g_columnindex 28
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
 extern double celsius;
 /* declaration of user functions */
 static void _hoc_gsat(void);
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
 {"setdata_Kv723", _hoc_setdata},
 {"gsat_Kv723", _hoc_gsat},
 {"rates_Kv723", _hoc_rates},
 {0, 0}
};
 
/* Direct Python call wrappers to density mechanism functions.*/
 static double _npy_gsat(Prop*);
 static double _npy_rates(Prop*);
 
static NPyDirectMechFunc npy_direct_func_proc[] = {
 {"gsat", _npy_gsat},
 {"rates", _npy_rates},
 {0, 0}
};
#define gsat gsat_Kv723
 extern double gsat( _internalthreadargsprotocomma_ double );
 /* declare global and static user variables */
 #define gind 0
 #define _gth 0
#define Dtaumult2 Dtaumult2_Kv723
 double Dtaumult2 = 6;
#define Dtaumult1 Dtaumult1_Kv723
 double Dtaumult1 = 6;
#define FoverR FoverR_Kv723
 double FoverR = 11.6045;
#define Vshift Vshift_Kv723
 double Vshift = 0;
#define Vhalf Vhalf_Kv723
 double Vhalf = -50;
#define gamma gamma_Kv723
 double gamma = 0.5;
#define kV kV_Kv723
 double kV = 40;
#define k k_Kv723
 double k = 9;
#define q10tau q10tau_Kv723
 double q10tau = 5;
#define temp0 temp0_Kv723
 double temp0 = 273;
#define temptau temptau_Kv723
 double temptau = 22;
#define taudiv taudiv_Kv723
 double taudiv = 1;
#define tau0mult tau0mult_Kv723
 double tau0mult = 0.2;
#define usetable usetable_Kv723
 double usetable = 1;
#define vmax vmax_Kv723
 double vmax = 100;
#define vmin vmin_Kv723
 double vmin = -100;
#define v0erev v0erev_Kv723
 double v0erev = 65;
 
static void _check_rates(_internalthreadargsproto_); 
static void _check_table_thread(_threadargsprotocomma_ int _type, _nrn_model_sorted_token const& _sorted_token) {
  if (gind != 0 && _thread != nullptr) { _globals = _thread[_gth].get<double*>(); } 
  _nrn_mechanism_cache_range _lmr{_sorted_token, *_nt, *_ml, _type};
  {
    auto* const _ml = &_lmr;
   _check_rates(_threadargs_);
   }
}
 /* some parameters have upper and lower limits */
 static HocParmLimits _hoc_parm_limits[] = {
 {"usetable_Kv723", 0, 1},
 {0, 0, 0}
};
 static HocParmUnits _hoc_parm_units[] = {
 {"k_Kv723", "mV"},
 {"Vhalf_Kv723", "mV"},
 {"Vshift_Kv723", "mV"},
 {"v0erev_Kv723", "mV"},
 {"kV_Kv723", "mV"},
 {"temptau_Kv723", "degC"},
 {"vmin_Kv723", "mV"},
 {"vmax_Kv723", "mV"},
 {"temp0_Kv723", "degC"},
 {"FoverR_Kv723", "degC/mV"},
 {"gkbar_Kv723", "S/cm2"},
 {"i_Kv723", "mA/cm2"},
 {"ik_Kv723", "mA/cm2"},
 {"gk_Kv723", "S/cm2"},
 {"tau1_Kv723", "ms"},
 {"tau2_Kv723", "ms"},
 {0, 0}
};
 static double delta_t = 0.01;
 static double m20 = 0;
 static double m10 = 0;
 /* connect global user variables to hoc */
 static DoubScal hoc_scdoub[] = {
 {"k_Kv723", &k_Kv723},
 {"Vhalf_Kv723", &Vhalf_Kv723},
 {"Vshift_Kv723", &Vshift_Kv723},
 {"v0erev_Kv723", &v0erev_Kv723},
 {"kV_Kv723", &kV_Kv723},
 {"gamma_Kv723", &gamma_Kv723},
 {"temptau_Kv723", &temptau_Kv723},
 {"q10tau_Kv723", &q10tau_Kv723},
 {"taudiv_Kv723", &taudiv_Kv723},
 {"Dtaumult1_Kv723", &Dtaumult1_Kv723},
 {"Dtaumult2_Kv723", &Dtaumult2_Kv723},
 {"tau0mult_Kv723", &tau0mult_Kv723},
 {"vmin_Kv723", &vmin_Kv723},
 {"vmax_Kv723", &vmax_Kv723},
 {"temp0_Kv723", &temp0_Kv723},
 {"FoverR_Kv723", &FoverR_Kv723},
 {"usetable_Kv723", &usetable_Kv723},
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
"Kv723",
 "gkbar_Kv723",
 0,
 "i_Kv723",
 "ik_Kv723",
 "gk_Kv723",
 "minf_Kv723",
 "tau1_Kv723",
 "tau2_Kv723",
 "tadjtau_Kv723",
 0,
 "m1_Kv723",
 "m2_Kv723",
 0,
 0};
 static Symbol* _k_sym;
 
 /* Used by NrnProperty */
 static _nrn_mechanism_std_vector<double> _parm_default{
     0.002, /* gkbar */
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
    assert(_nrn_mechanism_get_num_vars(_prop) == 29);
 	/*initialize range parameters*/
 	gkbar = _parm_default[0]; /* 0.002 */
 	 assert(_nrn_mechanism_get_num_vars(_prop) == 29);
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
 extern Symbol* hoc_lookup(const char*);
extern void _nrn_thread_reg(int, int, void(*)(Datum*));
void _nrn_thread_table_reg(int, nrn_thread_table_check_t);
extern void hoc_register_tolerance(int, HocStateTolerance*, Symbol***);
extern void _cvode_abstol( Symbol**, double*, int);

 extern "C" void _Kv723_reg() {
	int _vectorized = 1;
  _initlists();
 	ion_reg("k", -10000.);
 	_k_sym = hoc_lookup("k_ion");
 	register_mech(_mechanism, nrn_alloc,nrn_cur, nrn_jacob, nrn_state, nrn_init, hoc_nrnpointerindex, 1);
 _mechtype = nrn_get_mechtype(_mechanism[1]);
 hoc_register_parm_default(_mechtype, &_parm_default);
         hoc_register_npy_direct(_mechtype, npy_direct_func_proc);
     _nrn_setdata_reg(_mechtype, _setdata);
     _nrn_thread_table_reg(_mechtype, _check_table_thread);
 #if NMODL_TEXT
  register_nmodl_text_and_filename(_mechtype);
#endif
   _nrn_mechanism_register_data_fields(_mechtype,
                                       _nrn_mechanism_field<double>{"gkbar"} /* 0 */,
                                       _nrn_mechanism_field<double>{"i"} /* 1 */,
                                       _nrn_mechanism_field<double>{"ik"} /* 2 */,
                                       _nrn_mechanism_field<double>{"gk"} /* 3 */,
                                       _nrn_mechanism_field<double>{"minf"} /* 4 */,
                                       _nrn_mechanism_field<double>{"tau1"} /* 5 */,
                                       _nrn_mechanism_field<double>{"tau2"} /* 6 */,
                                       _nrn_mechanism_field<double>{"tadjtau"} /* 7 */,
                                       _nrn_mechanism_field<double>{"m1"} /* 8 */,
                                       _nrn_mechanism_field<double>{"m2"} /* 9 */,
                                       _nrn_mechanism_field<double>{"ek"} /* 10 */,
                                       _nrn_mechanism_field<double>{"Vhalf1"} /* 11 */,
                                       _nrn_mechanism_field<double>{"Dtau1"} /* 12 */,
                                       _nrn_mechanism_field<double>{"z1"} /* 13 */,
                                       _nrn_mechanism_field<double>{"tau01"} /* 14 */,
                                       _nrn_mechanism_field<double>{"Vhalf2"} /* 15 */,
                                       _nrn_mechanism_field<double>{"Dtau2"} /* 16 */,
                                       _nrn_mechanism_field<double>{"z2"} /* 17 */,
                                       _nrn_mechanism_field<double>{"tau02"} /* 18 */,
                                       _nrn_mechanism_field<double>{"alpha1"} /* 19 */,
                                       _nrn_mechanism_field<double>{"beta1"} /* 20 */,
                                       _nrn_mechanism_field<double>{"alpha2"} /* 21 */,
                                       _nrn_mechanism_field<double>{"beta2"} /* 22 */,
                                       _nrn_mechanism_field<double>{"v0"} /* 23 */,
                                       _nrn_mechanism_field<double>{"frt"} /* 24 */,
                                       _nrn_mechanism_field<double>{"Dm1"} /* 25 */,
                                       _nrn_mechanism_field<double>{"Dm2"} /* 26 */,
                                       _nrn_mechanism_field<double>{"v"} /* 27 */,
                                       _nrn_mechanism_field<double>{"_g"} /* 28 */,
                                       _nrn_mechanism_field<double*>{"_ion_ek", "k_ion"} /* 0 */,
                                       _nrn_mechanism_field<double*>{"_ion_ik", "k_ion"} /* 1 */,
                                       _nrn_mechanism_field<double*>{"_ion_dikdv", "k_ion"} /* 2 */,
                                       _nrn_mechanism_field<int>{"_cvode_ieq", "cvodeieq"} /* 3 */);
  hoc_register_prop_size(_mechtype, 29, 4);
  hoc_register_dparam_semantics(_mechtype, 0, "k_ion");
  hoc_register_dparam_semantics(_mechtype, 1, "k_ion");
  hoc_register_dparam_semantics(_mechtype, 2, "k_ion");
  hoc_register_dparam_semantics(_mechtype, 3, "cvodeieq");
 	hoc_register_cvode(_mechtype, _ode_count, _ode_map, _ode_spec, _ode_matsol);
 	hoc_register_tolerance(_mechtype, _hoc_state_tol, &_atollist);
 
    hoc_register_var(hoc_scdoub, hoc_vdoub, hoc_intfunc);
 	ivoc_help("help ?1 Kv723 /home/raven/PycharmProjects/Masters_model/Mod_Files_Beining_2017/Kv723.mod\n");
 hoc_register_limits(_mechtype, _hoc_parm_limits);
 hoc_register_units(_mechtype, _hoc_parm_units);
 }
 static double *_t_minf;
 static double *_t_tau1;
 static double *_t_tau2;
static int _reset;
static const char *modelname = "";

static int error;
static int _ninits = 0;
static int _match_recurse=1;
static void _modl_cleanup(){ _match_recurse=1;}
static int _f_rates(_internalthreadargsprotocomma_ double);
static int rates(_internalthreadargsprotocomma_ double);
 
static int _ode_spec1(_internalthreadargsproto_);
/*static int _ode_matsol1(_internalthreadargsproto_);*/
 static void _n_rates(_internalthreadargsprotocomma_ double _lv);
 static neuron::container::field_index _slist1[2], _dlist1[2];
 static int states(_internalthreadargsproto_);
 
/*CVODE*/
 static int _ode_spec1 (_internalthreadargsproto_) {int _reset = 0; {
   rates ( _threadargscomma_ v ) ;
   Dm1 = ( minf - m1 ) / tau1 ;
   Dm2 = ( minf - m2 ) / tau2 ;
   }
 return _reset;
}
 static int _ode_matsol1 (_internalthreadargsproto_) {
 rates ( _threadargscomma_ v ) ;
 Dm1 = Dm1  / (1. - dt*( ( ( ( - 1.0 ) ) ) / tau1 )) ;
 Dm2 = Dm2  / (1. - dt*( ( ( ( - 1.0 ) ) ) / tau2 )) ;
  return 0;
}
 /*END CVODE*/
 static int states (_internalthreadargsproto_) { {
   rates ( _threadargscomma_ v ) ;
    m1 = m1 + (1. - exp(dt*(( ( ( - 1.0 ) ) ) / tau1)))*(- ( ( ( minf ) ) / tau1 ) / ( ( ( ( - 1.0 ) ) ) / tau1 ) - m1) ;
    m2 = m2 + (1. - exp(dt*(( ( ( - 1.0 ) ) ) / tau2)))*(- ( ( ( minf ) ) / tau2 ) / ( ( ( ( - 1.0 ) ) ) / tau2 ) - m2) ;
   }
  return 0;
}
 static double _mfac_rates, _tmin_rates;
  static void _check_rates(_internalthreadargsproto_) {
  static int _maktable=1; int _i, _j, _ix = 0;
  double _xi, _tmax;
  static double _sav_celsius;
  static double _sav_gamma;
  static double _sav_k;
  static double _sav_Vhalf;
  static double _sav_Vshift;
  static double _sav_taudiv;
  static double _sav_Dtaumult1;
  static double _sav_Dtaumult2;
  static double _sav_tau0mult;
  if (!usetable) {return;}
  if (_sav_celsius != celsius) { _maktable = 1;}
  if (_sav_gamma != gamma) { _maktable = 1;}
  if (_sav_k != k) { _maktable = 1;}
  if (_sav_Vhalf != Vhalf) { _maktable = 1;}
  if (_sav_Vshift != Vshift) { _maktable = 1;}
  if (_sav_taudiv != taudiv) { _maktable = 1;}
  if (_sav_Dtaumult1 != Dtaumult1) { _maktable = 1;}
  if (_sav_Dtaumult2 != Dtaumult2) { _maktable = 1;}
  if (_sav_tau0mult != tau0mult) { _maktable = 1;}
  if (_maktable) { double _x, _dx; _maktable=0;
   _tmin_rates =  vmin ;
   _tmax =  vmax ;
   _dx = (_tmax - _tmin_rates)/199.; _mfac_rates = 1./_dx;
   for (_i=0, _x=_tmin_rates; _i < 200; _x += _dx, _i++) {
    _f_rates(_threadargscomma_ _x);
    _t_minf[_i] = minf;
    _t_tau1[_i] = tau1;
    _t_tau2[_i] = tau2;
   }
   _sav_celsius = celsius;
   _sav_gamma = gamma;
   _sav_k = k;
   _sav_Vhalf = Vhalf;
   _sav_Vshift = Vshift;
   _sav_taudiv = taudiv;
   _sav_Dtaumult1 = Dtaumult1;
   _sav_Dtaumult2 = Dtaumult2;
   _sav_tau0mult = tau0mult;
  }
 }

 static int rates(_internalthreadargsprotocomma_ double _lv) { 
#if 0
_check_rates(_threadargs_);
#endif
 _n_rates(_threadargscomma_ _lv);
 return 0;
 }

 static void _n_rates(_internalthreadargsprotocomma_ double _lv){ int _i, _j;
 double _xi, _theta;
 if (!usetable) {
 _f_rates(_threadargscomma_ _lv); return; 
}
 _xi = _mfac_rates * (_lv - _tmin_rates);
 if (std::isnan(_xi)) {
  minf = _xi;
  tau1 = _xi;
  tau2 = _xi;
  return;
 }
 if (_xi <= 0.) {
 minf = _t_minf[0];
 tau1 = _t_tau1[0];
 tau2 = _t_tau2[0];
 return; }
 if (_xi >= 199.) {
 minf = _t_minf[199];
 tau1 = _t_tau1[199];
 tau2 = _t_tau2[199];
 return; }
 _i = (int) _xi;
 _theta = _xi - (double)_i;
 minf = _t_minf[_i] + _theta*(_t_minf[_i+1] - _t_minf[_i]);
 tau1 = _t_tau1[_i] + _theta*(_t_tau1[_i+1] - _t_tau1[_i]);
 tau2 = _t_tau2[_i] + _theta*(_t_tau2[_i+1] - _t_tau2[_i]);
 }

 
static int  _f_rates ( _internalthreadargsprotocomma_ double _lv ) {
   if ( gamma  == 0.5 ) {
     z1 = 2.8 ;
     Vhalf1 = - 49.8 + Vshift ;
     tau01 = 20.7 * tau0mult ;
     Dtau1 = 176.1 * Dtaumult1 ;
     z2 = 8.9 ;
     Vhalf2 = - 55.5 + Vshift ;
     tau02 = 149.0 * tau0mult ;
     Dtau2 = 1473.0 * Dtaumult2 ;
     }
   if ( gamma  == 1.0 ) {
     z1 = 3.6 ;
     Vhalf1 = - 25.3 + Vshift ;
     tau01 = 29.2 * tau0mult ;
     Dtau1 = 74.6 * Dtaumult1 ;
     z2 = 9.8 ;
     Vhalf2 = - 44.7 + Vshift ;
     tau02 = 155.0 * tau0mult ;
     Dtau2 = 549.0 * Dtaumult2 ;
     }
   tadjtau = pow( q10tau , ( ( celsius - temptau ) / 10.0 ) ) ;
   frt = FoverR / ( temp0 + celsius ) ;
   alpha1 = exp ( z1 * gamma * frt * ( _lv - Vhalf1 ) ) ;
   beta1 = exp ( - z1 * ( 1.0 - gamma ) * frt * ( _lv - Vhalf1 ) ) ;
   tau1 = ( Dtau1 / ( alpha1 + beta1 ) + tau01 ) / ( tadjtau * taudiv ) ;
   alpha2 = exp ( z2 * gamma * frt * ( _lv - Vhalf2 ) ) ;
   beta2 = exp ( - z2 * ( 1.0 - gamma ) * frt * ( _lv - Vhalf2 ) ) ;
   tau2 = ( Dtau2 / ( alpha2 + beta2 ) + tau02 ) / ( tadjtau * taudiv ) ;
   minf = 1.0 / ( 1.0 + exp ( - ( _lv - Vhalf - Vshift ) / k ) ) ;
    return 0; }
 
static void _hoc_rates(void) {
  double _r;
 Datum* _ppvar; Datum* _thread; NrnThread* _nt;
 
  if(!_prop_id) {
    hoc_execerror("No data for rates_Kv723. Requires prior call to setdata_Kv723 and that the specified mechanism instance still be in existence.", NULL);
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
 
#if 1
 _check_rates(_threadargs_);
#endif
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
 
#if 1
 _check_rates(_threadargs_);
#endif
 _r = 1.;
 rates ( _threadargscomma_ *getarg(1) );
 return(_r);
}
 
double gsat ( _internalthreadargsprotocomma_ double _lv ) {
   double _lgsat;
 _lgsat = 1.0 ;
   v0 = v0erev + ek ;
   if ( _lv > v0 ) {
     _lgsat = 1.0 + ( v0 - _lv + kV * ( 1.0 - exp ( - ( _lv - v0 ) / kV ) ) ) / ( _lv - ek ) ;
     }
   
return _lgsat;
 }
 
static void _hoc_gsat(void) {
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
 _r =  gsat ( _threadargscomma_ *getarg(1) );
 hoc_retpushx(_r);
}
 
static double _npy_gsat(Prop* _prop) {
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
 _r =  gsat ( _threadargscomma_ *getarg(1) );
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
  ek = _ion_ek;
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
  ek = _ion_ek;
 _ode_matsol_instance1(_threadargs_);
 }}

static void initmodel(_internalthreadargsproto_) {
  int _i; double _save;{
  m2 = m20;
  m1 = m10;
 {
   rates ( _threadargscomma_ v ) ;
   m1 = minf ;
   m2 = minf ;
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

#if 0
 _check_rates(_threadargs_);
#endif
   _v = _vec_v[_ni[_iml]];
 v = _v;
  ek = _ion_ek;
 initmodel(_threadargs_);
 }
}

static double _nrn_current(_internalthreadargsprotocomma_ double _v) {
double _current=0.; v=_v;
{ {
   gk = gkbar * gsat ( _threadargscomma_ v ) * ( pow( m1 , 2.0 ) ) * m2 ;
   ik = gk * ( v - ek ) ;
   i = ik ;
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
 {   states(_threadargs_);
  } }}

}

static void terminal(){}

static void _initlists(){
 int _i; static int _first = 1;
  if (!_first) return;
 _slist1[0] = {m1_columnindex, 0};  _dlist1[0] = {Dm1_columnindex, 0};
 _slist1[1] = {m2_columnindex, 0};  _dlist1[1] = {Dm2_columnindex, 0};
   _t_minf = makevector(200*sizeof(double));
   _t_tau1 = makevector(200*sizeof(double));
   _t_tau2 = makevector(200*sizeof(double));
_first = 0;
}

#if NMODL_TEXT
static void register_nmodl_text_and_filename(int mech_type) {
    const char* nmodl_filename = "/home/raven/PycharmProjects/Masters_model/Mod_Files_Beining_2017/Kv723.mod";
    const char* nmodl_file_text = 
  ": M conductance\n"
  ": from Mateos-Aparicio et al (2014)\n"
  "\n"
  "NEURON {\n"
  "	SUFFIX Kv723\n"
  "	USEION k READ ek WRITE ik\n"
  "	RANGE gkbar, ik, minf, tau1, tau2, i, gk, m1, m2, tadjtau\n"
  "	GLOBAL Vhalf, Vshift, k, v0erev, kV, gamma\n"
  "	GLOBAL Dtaumult1, Dtaumult2, tau0mult, taudiv, q10tau\n"
  "}\n"
  "\n"
  "UNITS {\n"
  "	(mA) = (milliamp)\n"
  "	(mV) = (millivolt)\n"
  "	(S) = (siemens)\n"
  "	(um) = (micron)\n"
  "} \n"
  "\n"
  "PARAMETER {\n"
  "	gkbar = 0.002  	    				(S/cm2)\n"
  "	k = 9           				(mV)\n"
  "	Vhalf = -50             (mV)  :for minf(V)\n"
  "	Vshift = 0              (mV)	:for g(V) and minf(V)     \n"
  "	v0erev = 65             (mV)     :50-80\n"
  "	kV = 40                 (mV)     \n"
  "	gamma = 0.5                      :0.5,1\n"
  "\n"
  "	temptau = 22	          (degC) :tau reference temperature 	\n"
  "	q10tau  = 5\n"
  "	taudiv = 1\n"
  "	Dtaumult1 = 6  \n"
  "	Dtaumult2 = 6 \n"
  "	tau0mult = 0.2\n"
  "\n"
  "	vmin = -100	            (mV)\n"
  "	vmax = 100	            (mV)\n"
  "	temp0 = 273		          (degC)\n"
  "	FoverR = 11.6045039552	(degC/mV)\n"
  "} \n"
  " \n"
  "ASSIGNED {\n"
  "	ek		(mV)\n"
  "	v 	     	(mV)\n"
  "	celsius		(degC)\n"
  "	Vhalf1    (mV) \n"
  "	Dtau1     (ms)\n"
  "	z1               \n"
  "	tau01   	(ms)	 \n"
  "	Vhalf2  	(mV)	  \n"
  "	Dtau2   	(ms)  \n"
  "	z2               \n"
  "	tau02   	(ms)	  \n"
  "	alpha1				  \n"
  "	beta1	  		  \n"
  "	alpha2		\n"
  "	beta2	\n"
  "	i 	    	(mA/cm2)\n"
  "	ik 	     	(mA/cm2)\n"
  "	gk		      (S/cm2)\n"
  "	minf\n"
  "	v0        (mV)      \n"
  "	tau1			(ms)\n"
  "	tau2			(ms)\n"
  "	tadjtau\n"
  "	frt		    (/mV)\n"
  "}\n"
  " \n"
  "STATE { m1 m2 }\n"
  "\n"
  "INITIAL { \n"
  "	rates(v)\n"
  "	m1 = minf\n"
  "	m2 = minf\n"
  "}\n"
  "\n"
  "BREAKPOINT {\n"
  "  SOLVE states METHOD cnexp\n"
  "	gk = gkbar*gsat(v)*(m1^2)*m2\n"
  "	ik = gk*(v - ek)\n"
  "	i = ik\n"
  "} \n"
  "\n"
  "DERIVATIVE states {\n"
  "	rates(v)\n"
  "	m1' = (minf - m1)/tau1\n"
  "	m2' = (minf - m2)/tau2\n"
  "}\n"
  "\n"
  "PROCEDURE rates(v (mV)) {\n"
  "  TABLE minf, tau1, tau2\n"
  "	DEPEND celsius, gamma, k, Vhalf, Vshift, taudiv, Dtaumult1, Dtaumult2, tau0mult\n"
  "	FROM vmin TO vmax WITH 199\n"
  "	\n"
  "  IF (gamma == 0.5) {\n"
  "  	z1 = 2.8\n"
  "		Vhalf1 = -49.8+Vshift 	:(mV)  shifted - 20 mV (when Vshift = 0)\n"
  "		tau01 = 20.7 (ms) *tau0mult	  :(ms)\n"
  "		Dtau1 = 176.1 (ms) *Dtaumult1	:(ms)\n"
  "		z2 = 8.9	              \n"
  "		Vhalf2 = -55.5+Vshift 					:(mV)  shifted - 20 mV\n"
  "		tau02 = 149 (ms) *tau0mult   					:(ms)\n"
  "		Dtau2 = 1473 (ms) *Dtaumult2 	  			:(ms)\n"
  "	}	\n"
  "	IF (gamma == 1) {\n"
  "  	z1 = 3.6\n"
  "		Vhalf1 = -25.3+Vshift		:(mV)  shifted - 20 mV\n"
  "		tau01 = 29.2 (ms) *tau0mult	  :(ms)\n"
  "		Dtau1 = 74.6 (ms) *Dtaumult1	:(ms)\n"
  "		z2 = 9.8	\n"
  "		Vhalf2 = -44.7+Vshift 					:(mV)  shifted - 20 mV\n"
  "		tau02 = 155 (ms) *tau0mult   					:(ms)\n"
  "		Dtau2 = 549 (ms) *Dtaumult2  	  			:(ms)\n"
  "	}\n"
  "  tadjtau = q10tau^((celsius - temptau)/10 (degC))\n"
  "	frt = FoverR/(temp0 + celsius)\n"
  "\n"
  "  alpha1 = exp(z1*gamma*frt*(v - Vhalf1))\n"
  "  beta1 = exp(-z1*(1-gamma)*frt*(v - Vhalf1))\n"
  "  tau1 = (Dtau1/(alpha1 + beta1) + tau01)/(tadjtau*taudiv)\n"
  "  \n"
  "  alpha2 = exp(z2*gamma*frt*(v - Vhalf2))\n"
  "  beta2 = exp(-z2*(1-gamma)*frt*(v - Vhalf2))\n"
  "  tau2 = (Dtau2/(alpha2 + beta2) + tau02)/(tadjtau*taudiv)\n"
  "\n"
  "  minf = 1/(1 + exp(-(v - Vhalf - Vshift)/k))\n"
  "}\n"
  "\n"
  "FUNCTION gsat (v (mV)) {\n"
  "	gsat = 1\n"
  "	v0 = v0erev + ek  \n"
  "	IF (v > v0) {\n"
  "		gsat = 1+(v0-v+kV*(1-exp(-(v-v0)/kV)))/(v-ek)\n"
  "	}\n"
  "}\n"
  "\n"
  "\n"
  ;
    hoc_reg_nmodl_filename(mech_type, nmodl_filename);
    hoc_reg_nmodl_text(mech_type, nmodl_file_text);
}
#endif
