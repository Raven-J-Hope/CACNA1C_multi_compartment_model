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
static constexpr auto number_of_datum_variables = 2;
static constexpr auto number_of_floating_point_variables = 23;
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
 
#define nrn_init _nrn_init__Caold
#define _nrn_initial _nrn_initial__Caold
#define nrn_cur _nrn_cur__Caold
#define _nrn_current _nrn_current__Caold
#define nrn_jacob _nrn_jacob__Caold
#define nrn_state _nrn_state__Caold
#define _net_receive _net_receive__Caold 
#define state state__Caold 
 
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
#define gtcabar _ml->template fpfield<0>(_iml)
#define gtcabar_columnindex 0
#define gncabar _ml->template fpfield<1>(_iml)
#define gncabar_columnindex 1
#define glcabar _ml->template fpfield<2>(_iml)
#define glcabar_columnindex 2
#define e_ca _ml->template fpfield<3>(_iml)
#define e_ca_columnindex 3
#define gtca _ml->template fpfield<4>(_iml)
#define gtca_columnindex 4
#define gnca _ml->template fpfield<5>(_iml)
#define gnca_columnindex 5
#define glca _ml->template fpfield<6>(_iml)
#define glca_columnindex 6
#define caim _ml->template fpfield<7>(_iml)
#define caim_columnindex 7
#define ca_i _ml->template fpfield<8>(_iml)
#define ca_i_columnindex 8
#define a _ml->template fpfield<9>(_iml)
#define a_columnindex 9
#define b _ml->template fpfield<10>(_iml)
#define b_columnindex 10
#define c _ml->template fpfield<11>(_iml)
#define c_columnindex 11
#define d _ml->template fpfield<12>(_iml)
#define d_columnindex 12
#define e _ml->template fpfield<13>(_iml)
#define e_columnindex 13
#define ica _ml->template fpfield<14>(_iml)
#define ica_columnindex 14
#define Dca_i _ml->template fpfield<15>(_iml)
#define Dca_i_columnindex 15
#define Da _ml->template fpfield<16>(_iml)
#define Da_columnindex 16
#define Db _ml->template fpfield<17>(_iml)
#define Db_columnindex 17
#define Dc _ml->template fpfield<18>(_iml)
#define Dc_columnindex 18
#define Dd _ml->template fpfield<19>(_iml)
#define Dd_columnindex 19
#define De _ml->template fpfield<20>(_iml)
#define De_columnindex 20
#define v _ml->template fpfield<21>(_iml)
#define v_columnindex 21
#define _g _ml->template fpfield<22>(_iml)
#define _g_columnindex 22
#define _ion_ica *(_ml->dptr_field<0>(_iml))
#define _p_ion_ica static_cast<neuron::container::data_handle<double>>(_ppvar[0])
#define _ion_dicadv *(_ml->dptr_field<1>(_iml))
 /* Thread safe. No static _ml, _iml or _ppvar. */
 static int hoc_nrnpointerindex =  -1;
 static _nrn_mechanism_std_vector<Datum> _extcall_thread;
 static Prop* _extcall_prop;
 /* _prop_id kind of shadows _extcall_prop to allow validity checking. */
 static _nrn_non_owning_id_without_container _prop_id{};
 /* external NEURON variables */
 /* declaration of user functions */
 static void _hoc_alphae(void);
 static void _hoc_alphad(void);
 static void _hoc_alphac(void);
 static void _hoc_alphab(void);
 static void _hoc_alphaa(void);
 static void _hoc_betae(void);
 static void _hoc_betad(void);
 static void _hoc_betac(void);
 static void _hoc_betab(void);
 static void _hoc_betaa(void);
 static void _hoc_exponential(void);
 static void _hoc_f(void);
 static void _hoc_logistic(void);
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
 {"setdata_Caold", _hoc_setdata},
 {"alphae_Caold", _hoc_alphae},
 {"alphad_Caold", _hoc_alphad},
 {"alphac_Caold", _hoc_alphac},
 {"alphab_Caold", _hoc_alphab},
 {"alphaa_Caold", _hoc_alphaa},
 {"betae_Caold", _hoc_betae},
 {"betad_Caold", _hoc_betad},
 {"betac_Caold", _hoc_betac},
 {"betab_Caold", _hoc_betab},
 {"betaa_Caold", _hoc_betaa},
 {"exponential_Caold", _hoc_exponential},
 {"f_Caold", _hoc_f},
 {"logistic_Caold", _hoc_logistic},
 {0, 0}
};
 
/* Direct Python call wrappers to density mechanism functions.*/
 static double _npy_alphae(Prop*);
 static double _npy_alphad(Prop*);
 static double _npy_alphac(Prop*);
 static double _npy_alphab(Prop*);
 static double _npy_alphaa(Prop*);
 static double _npy_betae(Prop*);
 static double _npy_betad(Prop*);
 static double _npy_betac(Prop*);
 static double _npy_betab(Prop*);
 static double _npy_betaa(Prop*);
 static double _npy_exponential(Prop*);
 static double _npy_f(Prop*);
 static double _npy_logistic(Prop*);
 
static NPyDirectMechFunc npy_direct_func_proc[] = {
 {"alphae", _npy_alphae},
 {"alphad", _npy_alphad},
 {"alphac", _npy_alphac},
 {"alphab", _npy_alphab},
 {"alphaa", _npy_alphaa},
 {"betae", _npy_betae},
 {"betad", _npy_betad},
 {"betac", _npy_betac},
 {"betab", _npy_betab},
 {"betaa", _npy_betaa},
 {"exponential", _npy_exponential},
 {"f", _npy_f},
 {"logistic", _npy_logistic},
 {0, 0}
};
#define alphae alphae_Caold
#define alphad alphad_Caold
#define alphac alphac_Caold
#define alphab alphab_Caold
#define alphaa alphaa_Caold
#define betae betae_Caold
#define betad betad_Caold
#define betac betac_Caold
#define betab betab_Caold
#define betaa betaa_Caold
#define exponential exponential_Caold
#define f f_Caold
#define logistic logistic_Caold
 extern double alphae( _internalthreadargsprotocomma_ double );
 extern double alphad( _internalthreadargsprotocomma_ double );
 extern double alphac( _internalthreadargsprotocomma_ double );
 extern double alphab( _internalthreadargsprotocomma_ double );
 extern double alphaa( _internalthreadargsprotocomma_ double );
 extern double betae( _internalthreadargsprotocomma_ double );
 extern double betad( _internalthreadargsprotocomma_ double );
 extern double betac( _internalthreadargsprotocomma_ double );
 extern double betab( _internalthreadargsprotocomma_ double );
 extern double betaa( _internalthreadargsprotocomma_ double );
 extern double exponential( _internalthreadargsprotocomma_ double , double , double , double );
 extern double f( _internalthreadargsprotocomma_ double , double , double , double );
 extern double logistic( _internalthreadargsprotocomma_ double , double , double , double );
 /* declare global and static user variables */
 #define gind 0
 #define _gth 0
#define cao cao_Caold
 double cao = 2;
#define ca0 ca0_Caold
 double ca0 = 7e-05;
#define tau tau_Caold
 double tau = 9;
 /* some parameters have upper and lower limits */
 static HocParmLimits _hoc_parm_limits[] = {
 {0, 0, 0}
};
 static HocParmUnits _hoc_parm_units[] = {
 {"ca0_Caold", "mM"},
 {"cao_Caold", "mM"},
 {"tau_Caold", "ms"},
 {"gtcabar_Caold", "S/cm2"},
 {"gncabar_Caold", "S/cm2"},
 {"glcabar_Caold", "S/cm2"},
 {"ca_i_Caold", "mM"},
 {"e_ca_Caold", "mV"},
 {"gtca_Caold", "S/cm2"},
 {"gnca_Caold", "S/cm2"},
 {"glca_Caold", "S/cm2"},
 {"caim_Caold", "mM"},
 {0, 0}
};
 static double a0 = 0;
 static double b0 = 0;
 static double c0 = 0;
 static double ca_i0 = 0;
 static double delta_t = 0.01;
 static double d0 = 0;
 static double e0 = 0;
 /* connect global user variables to hoc */
 static DoubScal hoc_scdoub[] = {
 {"ca0_Caold", &ca0_Caold},
 {"cao_Caold", &cao_Caold},
 {"tau_Caold", &tau_Caold},
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
 
#define _cvode_ieq _ppvar[2].literal_value<int>()
 static void _ode_matsol_instance1(_internalthreadargsproto_);
 /* connect range variables in _p that hoc is supposed to know about */
 static const char *_mechanism[] = {
 "7.7.0",
"Caold",
 "gtcabar_Caold",
 "gncabar_Caold",
 "glcabar_Caold",
 0,
 "e_ca_Caold",
 "gtca_Caold",
 "gnca_Caold",
 "glca_Caold",
 "caim_Caold",
 0,
 "ca_i_Caold",
 "a_Caold",
 "b_Caold",
 "c_Caold",
 "d_Caold",
 "e_Caold",
 0,
 0};
 static Symbol* _ca_sym;
 
 /* Used by NrnProperty */
 static _nrn_mechanism_std_vector<double> _parm_default{
     0.01, /* gtcabar */
     0.01, /* gncabar */
     0.01, /* glcabar */
 }; 
 
 
extern Prop* need_memb(Symbol*);
static void nrn_alloc(Prop* _prop) {
  Prop *prop_ion{};
  Datum *_ppvar{};
   _ppvar = nrn_prop_datum_alloc(_mechtype, 3, _prop);
    _nrn_mechanism_access_dparam(_prop) = _ppvar;
     _nrn_mechanism_cache_instance _ml_real{_prop};
    auto* const _ml = &_ml_real;
    size_t const _iml{};
    assert(_nrn_mechanism_get_num_vars(_prop) == 23);
 	/*initialize range parameters*/
 	gtcabar = _parm_default[0]; /* 0.01 */
 	gncabar = _parm_default[1]; /* 0.01 */
 	glcabar = _parm_default[2]; /* 0.01 */
 	 assert(_nrn_mechanism_get_num_vars(_prop) == 23);
 	_nrn_mechanism_access_dparam(_prop) = _ppvar;
 	/*connect ionic variables to this model*/
 prop_ion = need_memb(_ca_sym);
 	_ppvar[0] = _nrn_mechanism_get_param_handle(prop_ion, 3); /* ica */
 	_ppvar[1] = _nrn_mechanism_get_param_handle(prop_ion, 4); /* _ion_dicadv */
 
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

 extern "C" void _Caold_reg() {
	int _vectorized = 1;
  _initlists();
 	ion_reg("ca", -10000.);
 	_ca_sym = hoc_lookup("ca_ion");
 	register_mech(_mechanism, nrn_alloc,nrn_cur, nrn_jacob, nrn_state, nrn_init, hoc_nrnpointerindex, 1);
 _mechtype = nrn_get_mechtype(_mechanism[1]);
 hoc_register_parm_default(_mechtype, &_parm_default);
         hoc_register_npy_direct(_mechtype, npy_direct_func_proc);
     _nrn_setdata_reg(_mechtype, _setdata);
 #if NMODL_TEXT
  register_nmodl_text_and_filename(_mechtype);
#endif
   _nrn_mechanism_register_data_fields(_mechtype,
                                       _nrn_mechanism_field<double>{"gtcabar"} /* 0 */,
                                       _nrn_mechanism_field<double>{"gncabar"} /* 1 */,
                                       _nrn_mechanism_field<double>{"glcabar"} /* 2 */,
                                       _nrn_mechanism_field<double>{"e_ca"} /* 3 */,
                                       _nrn_mechanism_field<double>{"gtca"} /* 4 */,
                                       _nrn_mechanism_field<double>{"gnca"} /* 5 */,
                                       _nrn_mechanism_field<double>{"glca"} /* 6 */,
                                       _nrn_mechanism_field<double>{"caim"} /* 7 */,
                                       _nrn_mechanism_field<double>{"ca_i"} /* 8 */,
                                       _nrn_mechanism_field<double>{"a"} /* 9 */,
                                       _nrn_mechanism_field<double>{"b"} /* 10 */,
                                       _nrn_mechanism_field<double>{"c"} /* 11 */,
                                       _nrn_mechanism_field<double>{"d"} /* 12 */,
                                       _nrn_mechanism_field<double>{"e"} /* 13 */,
                                       _nrn_mechanism_field<double>{"ica"} /* 14 */,
                                       _nrn_mechanism_field<double>{"Dca_i"} /* 15 */,
                                       _nrn_mechanism_field<double>{"Da"} /* 16 */,
                                       _nrn_mechanism_field<double>{"Db"} /* 17 */,
                                       _nrn_mechanism_field<double>{"Dc"} /* 18 */,
                                       _nrn_mechanism_field<double>{"Dd"} /* 19 */,
                                       _nrn_mechanism_field<double>{"De"} /* 20 */,
                                       _nrn_mechanism_field<double>{"v"} /* 21 */,
                                       _nrn_mechanism_field<double>{"_g"} /* 22 */,
                                       _nrn_mechanism_field<double*>{"_ion_ica", "ca_ion"} /* 0 */,
                                       _nrn_mechanism_field<double*>{"_ion_dicadv", "ca_ion"} /* 1 */,
                                       _nrn_mechanism_field<int>{"_cvode_ieq", "cvodeieq"} /* 2 */);
  hoc_register_prop_size(_mechtype, 23, 3);
  hoc_register_dparam_semantics(_mechtype, 0, "ca_ion");
  hoc_register_dparam_semantics(_mechtype, 1, "ca_ion");
  hoc_register_dparam_semantics(_mechtype, 2, "cvodeieq");
 	hoc_register_cvode(_mechtype, _ode_count, _ode_map, _ode_spec, _ode_matsol);
 	hoc_register_tolerance(_mechtype, _hoc_state_tol, &_atollist);
 
    hoc_register_var(hoc_scdoub, hoc_vdoub, hoc_intfunc);
 	ivoc_help("help ?1 Caold /home/raven/PycharmProjects/Masters_model/Mod_Files_Beining_2017/Caold.mod\n");
 hoc_register_limits(_mechtype, _hoc_parm_limits);
 hoc_register_units(_mechtype, _hoc_parm_units);
 }
 static double B = .26;
 static double F = 0x1.78e555060882cp+16;
 static double R = 0x1.0a1013e8990bep+3;
 static double TEMP = 25.0;
static int _reset;
static const char *modelname = "";

static int error;
static int _ninits = 0;
static int _match_recurse=1;
static void _modl_cleanup(){ _match_recurse=1;}
 
static int _ode_spec1(_internalthreadargsproto_);
/*static int _ode_matsol1(_internalthreadargsproto_);*/
 static neuron::container::field_index _slist1[6], _dlist1[6];
 static int state(_internalthreadargsproto_);
 
/*CVODE*/
 static int _ode_spec1 (_internalthreadargsproto_) {int _reset = 0; {
   Dca_i = - B * ica - ( ca_i - ca0 ) / tau ;
   Da = alphaa ( _threadargscomma_ v ) * ( 1.0 - a ) - betaa ( _threadargscomma_ v ) * a ;
   Db = alphab ( _threadargscomma_ v ) * ( 1.0 - b ) - betab ( _threadargscomma_ v ) * b ;
   Dc = alphac ( _threadargscomma_ v ) * ( 1.0 - c ) - betac ( _threadargscomma_ v ) * c ;
   Dd = alphad ( _threadargscomma_ v ) * ( 1.0 - d ) - betad ( _threadargscomma_ v ) * d ;
   De = alphae ( _threadargscomma_ v ) * ( 1.0 - e ) - betae ( _threadargscomma_ v ) * e ;
   }
 return _reset;
}
 static int _ode_matsol1 (_internalthreadargsproto_) {
 Dca_i = Dca_i  / (1. - dt*( ( - ( ( 1.0 ) ) / tau ) )) ;
 Da = Da  / (1. - dt*( ( alphaa ( _threadargscomma_ v ) )*( ( ( - 1.0 ) ) ) - ( betaa ( _threadargscomma_ v ) )*( 1.0 ) )) ;
 Db = Db  / (1. - dt*( ( alphab ( _threadargscomma_ v ) )*( ( ( - 1.0 ) ) ) - ( betab ( _threadargscomma_ v ) )*( 1.0 ) )) ;
 Dc = Dc  / (1. - dt*( ( alphac ( _threadargscomma_ v ) )*( ( ( - 1.0 ) ) ) - ( betac ( _threadargscomma_ v ) )*( 1.0 ) )) ;
 Dd = Dd  / (1. - dt*( ( alphad ( _threadargscomma_ v ) )*( ( ( - 1.0 ) ) ) - ( betad ( _threadargscomma_ v ) )*( 1.0 ) )) ;
 De = De  / (1. - dt*( ( alphae ( _threadargscomma_ v ) )*( ( ( - 1.0 ) ) ) - ( betae ( _threadargscomma_ v ) )*( 1.0 ) )) ;
  return 0;
}
 /*END CVODE*/
 static int state (_internalthreadargsproto_) { {
    ca_i = ca_i + (1. - exp(dt*(( - ( ( 1.0 ) ) / tau ))))*(- ( ( - B )*( ica ) - ( ( ( - ca0 ) ) ) / tau ) / ( ( - ( ( 1.0 ) ) / tau ) ) - ca_i) ;
    a = a + (1. - exp(dt*(( alphaa ( _threadargscomma_ v ) )*( ( ( - 1.0 ) ) ) - ( betaa ( _threadargscomma_ v ) )*( 1.0 ))))*(- ( ( alphaa ( _threadargscomma_ v ) )*( ( 1.0 ) ) ) / ( ( alphaa ( _threadargscomma_ v ) )*( ( ( - 1.0 ) ) ) - ( betaa ( _threadargscomma_ v ) )*( 1.0 ) ) - a) ;
    b = b + (1. - exp(dt*(( alphab ( _threadargscomma_ v ) )*( ( ( - 1.0 ) ) ) - ( betab ( _threadargscomma_ v ) )*( 1.0 ))))*(- ( ( alphab ( _threadargscomma_ v ) )*( ( 1.0 ) ) ) / ( ( alphab ( _threadargscomma_ v ) )*( ( ( - 1.0 ) ) ) - ( betab ( _threadargscomma_ v ) )*( 1.0 ) ) - b) ;
    c = c + (1. - exp(dt*(( alphac ( _threadargscomma_ v ) )*( ( ( - 1.0 ) ) ) - ( betac ( _threadargscomma_ v ) )*( 1.0 ))))*(- ( ( alphac ( _threadargscomma_ v ) )*( ( 1.0 ) ) ) / ( ( alphac ( _threadargscomma_ v ) )*( ( ( - 1.0 ) ) ) - ( betac ( _threadargscomma_ v ) )*( 1.0 ) ) - c) ;
    d = d + (1. - exp(dt*(( alphad ( _threadargscomma_ v ) )*( ( ( - 1.0 ) ) ) - ( betad ( _threadargscomma_ v ) )*( 1.0 ))))*(- ( ( alphad ( _threadargscomma_ v ) )*( ( 1.0 ) ) ) / ( ( alphad ( _threadargscomma_ v ) )*( ( ( - 1.0 ) ) ) - ( betad ( _threadargscomma_ v ) )*( 1.0 ) ) - d) ;
    e = e + (1. - exp(dt*(( alphae ( _threadargscomma_ v ) )*( ( ( - 1.0 ) ) ) - ( betae ( _threadargscomma_ v ) )*( 1.0 ))))*(- ( ( alphae ( _threadargscomma_ v ) )*( ( 1.0 ) ) ) / ( ( alphae ( _threadargscomma_ v ) )*( ( ( - 1.0 ) ) ) - ( betae ( _threadargscomma_ v ) )*( 1.0 ) ) - e) ;
   }
  return 0;
}
 
double alphaa ( _internalthreadargsprotocomma_ double _lv ) {
   double _lalphaa;
 _lalphaa = f ( _threadargscomma_ 2.0 , 0.1 , _lv , 19.26 ) ;
   
return _lalphaa;
 }
 
static void _hoc_alphaa(void) {
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
 _r =  alphaa ( _threadargscomma_ *getarg(1) );
 hoc_retpushx(_r);
}
 
static double _npy_alphaa(Prop* _prop) {
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
 _r =  alphaa ( _threadargscomma_ *getarg(1) );
 return(_r);
}
 
double betaa ( _internalthreadargsprotocomma_ double _lv ) {
   double _lbetaa;
 _lbetaa = exponential ( _threadargscomma_ 0.009 , - 0.045393 , _lv , 0.0 ) ;
   
return _lbetaa;
 }
 
static void _hoc_betaa(void) {
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
 _r =  betaa ( _threadargscomma_ *getarg(1) );
 hoc_retpushx(_r);
}
 
static double _npy_betaa(Prop* _prop) {
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
 _r =  betaa ( _threadargscomma_ *getarg(1) );
 return(_r);
}
 
double alphab ( _internalthreadargsprotocomma_ double _lv ) {
   double _lalphab;
 _lalphab = exponential ( _threadargscomma_ 1e-6 , - 0.061501 , _lv , 0.0 ) ;
   
return _lalphab;
 }
 
static void _hoc_alphab(void) {
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
 _r =  alphab ( _threadargscomma_ *getarg(1) );
 hoc_retpushx(_r);
}
 
static double _npy_alphab(Prop* _prop) {
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
 _r =  alphab ( _threadargscomma_ *getarg(1) );
 return(_r);
}
 
double betab ( _internalthreadargsprotocomma_ double _lv ) {
   double _lbetab;
 _lbetab = logistic ( _threadargscomma_ 1.0 , - 0.1 , _lv , 29.79 ) ;
   
return _lbetab;
 }
 
static void _hoc_betab(void) {
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
 _r =  betab ( _threadargscomma_ *getarg(1) );
 hoc_retpushx(_r);
}
 
static double _npy_betab(Prop* _prop) {
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
 _r =  betab ( _threadargscomma_ *getarg(1) );
 return(_r);
}
 
double alphac ( _internalthreadargsprotocomma_ double _lv ) {
   double _lalphac;
 _lalphac = f ( _threadargscomma_ 1.9 , 0.1 , _lv , 19.88 ) ;
   
return _lalphac;
 }
 
static void _hoc_alphac(void) {
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
 _r =  alphac ( _threadargscomma_ *getarg(1) );
 hoc_retpushx(_r);
}
 
static double _npy_alphac(Prop* _prop) {
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
 _r =  alphac ( _threadargscomma_ *getarg(1) );
 return(_r);
}
 
double betac ( _internalthreadargsprotocomma_ double _lv ) {
   double _lbetac;
 _lbetac = exponential ( _threadargscomma_ 0.046 , - 0.048239 , _lv , 0.0 ) ;
   
return _lbetac;
 }
 
static void _hoc_betac(void) {
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
 _r =  betac ( _threadargscomma_ *getarg(1) );
 hoc_retpushx(_r);
}
 
static double _npy_betac(Prop* _prop) {
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
 _r =  betac ( _threadargscomma_ *getarg(1) );
 return(_r);
}
 
double alphad ( _internalthreadargsprotocomma_ double _lv ) {
   double _lalphad;
 _lalphad = exponential ( _threadargscomma_ 1.6e-4 , - 0.020661 , _lv , 0.0 ) ;
   
return _lalphad;
 }
 
static void _hoc_alphad(void) {
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
 _r =  alphad ( _threadargscomma_ *getarg(1) );
 hoc_retpushx(_r);
}
 
static double _npy_alphad(Prop* _prop) {
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
 _r =  alphad ( _threadargscomma_ *getarg(1) );
 return(_r);
}
 
double betad ( _internalthreadargsprotocomma_ double _lv ) {
   double _lbetad;
 _lbetad = logistic ( _threadargscomma_ 1.0 , - 0.1 , _lv , 39.0 ) ;
   
return _lbetad;
 }
 
static void _hoc_betad(void) {
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
 _r =  betad ( _threadargscomma_ *getarg(1) );
 hoc_retpushx(_r);
}
 
static double _npy_betad(Prop* _prop) {
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
 _r =  betad ( _threadargscomma_ *getarg(1) );
 return(_r);
}
 
double alphae ( _internalthreadargsprotocomma_ double _lv ) {
   double _lalphae;
 _lalphae = f ( _threadargscomma_ 156.9 , 0.1 , _lv , 81.5 ) ;
   
return _lalphae;
 }
 
static void _hoc_alphae(void) {
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
 _r =  alphae ( _threadargscomma_ *getarg(1) );
 hoc_retpushx(_r);
}
 
static double _npy_alphae(Prop* _prop) {
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
 _r =  alphae ( _threadargscomma_ *getarg(1) );
 return(_r);
}
 
double betae ( _internalthreadargsprotocomma_ double _lv ) {
   double _lbetae;
 _lbetae = exponential ( _threadargscomma_ 0.29 , - 0.092081 , _lv , 0.0 ) ;
   
return _lbetae;
 }
 
static void _hoc_betae(void) {
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
 _r =  betae ( _threadargscomma_ *getarg(1) );
 hoc_retpushx(_r);
}
 
static double _npy_betae(Prop* _prop) {
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
 _r =  betae ( _threadargscomma_ *getarg(1) );
 return(_r);
}
 
double f ( _internalthreadargsprotocomma_ double _lA , double _lk , double _lv , double _lD ) {
   double _lf;
 double _lx ;
  _lx = _lk * ( _lv - _lD ) ;
   if ( fabs ( _lx ) > 1e-6 ) {
     _lf = _lA * _lx / ( 1.0 - exp ( - _lx ) ) ;
     }
   else {
     _lf = _lA / ( 1.0 - 0.5 * _lx ) ;
     }
    
return _lf;
 }
 
static void _hoc_f(void) {
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
 _r =  f ( _threadargscomma_ *getarg(1) , *getarg(2) , *getarg(3) , *getarg(4) );
 hoc_retpushx(_r);
}
 
static double _npy_f(Prop* _prop) {
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
 _r =  f ( _threadargscomma_ *getarg(1) , *getarg(2) , *getarg(3) , *getarg(4) );
 return(_r);
}
 
double logistic ( _internalthreadargsprotocomma_ double _lA , double _lk , double _lv , double _lD ) {
   double _llogistic;
  _llogistic = _lA / ( 1.0 + exp ( _lk * ( _lv - _lD ) ) ) ;
    
return _llogistic;
 }
 
static void _hoc_logistic(void) {
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
 _r =  logistic ( _threadargscomma_ *getarg(1) , *getarg(2) , *getarg(3) , *getarg(4) );
 hoc_retpushx(_r);
}
 
static double _npy_logistic(Prop* _prop) {
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
 _r =  logistic ( _threadargscomma_ *getarg(1) , *getarg(2) , *getarg(3) , *getarg(4) );
 return(_r);
}
 
double exponential ( _internalthreadargsprotocomma_ double _lA , double _lk , double _lv , double _lD ) {
   double _lexponential;
  _lexponential = _lA * exp ( _lk * ( _lv - _lD ) ) ;
    
return _lexponential;
 }
 
static void _hoc_exponential(void) {
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
 _r =  exponential ( _threadargscomma_ *getarg(1) , *getarg(2) , *getarg(3) , *getarg(4) );
 hoc_retpushx(_r);
}
 
static double _npy_exponential(Prop* _prop) {
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
 _r =  exponential ( _threadargscomma_ *getarg(1) , *getarg(2) , *getarg(3) , *getarg(4) );
 return(_r);
}
 
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
 _ode_matsol_instance1(_threadargs_);
 }}

static void initmodel(_internalthreadargsproto_) {
  int _i; double _save;{
  a = a0;
  b = b0;
  c = c0;
  ca_i = ca_i0;
  d = d0;
  e = e0;
 {
   ca_i = ca0 ;
   a = alphaa ( _threadargscomma_ v ) / ( alphaa ( _threadargscomma_ v ) + betaa ( _threadargscomma_ v ) ) ;
   b = alphab ( _threadargscomma_ v ) / ( alphab ( _threadargscomma_ v ) + betab ( _threadargscomma_ v ) ) ;
   c = alphac ( _threadargscomma_ v ) / ( alphac ( _threadargscomma_ v ) + betac ( _threadargscomma_ v ) ) ;
   d = alphad ( _threadargscomma_ v ) / ( alphad ( _threadargscomma_ v ) + betad ( _threadargscomma_ v ) ) ;
   e = alphae ( _threadargscomma_ v ) / ( alphae ( _threadargscomma_ v ) + betae ( _threadargscomma_ v ) ) ;
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
 initmodel(_threadargs_);
 }
}

static double _nrn_current(_internalthreadargsprotocomma_ double _v) {
double _current=0.; v=_v;
{ {
   e_ca = ( 1000.0 ) * ( TEMP + 273.15 ) * R / ( 2.0 * F ) * log ( cao / ca_i ) ;
   gtca = gtcabar * a * a * b ;
   gnca = gncabar * c * c * d ;
   glca = glcabar * e * e ;
   ica = ( gtca + gnca + glca ) * ( v - e_ca ) ;
   caim = ca_i ;
   }
 _current += ica;

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
 auto const _g_local = _nrn_current(_threadargscomma_ _v + .001);
 	{ double _dica;
  _dica = ica;
 _rhs = _nrn_current(_threadargscomma_ _v);
  _ion_dicadv += (_dica - ica)/.001 ;
 	}
 _g = (_g_local - _rhs)/.001;
  _ion_ica += ica ;
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
 {   state(_threadargs_);
  } }}

}

static void terminal(){}

static void _initlists(){
 int _i; static int _first = 1;
  if (!_first) return;
 _slist1[0] = {ca_i_columnindex, 0};  _dlist1[0] = {Dca_i_columnindex, 0};
 _slist1[1] = {a_columnindex, 0};  _dlist1[1] = {Da_columnindex, 0};
 _slist1[2] = {b_columnindex, 0};  _dlist1[2] = {Db_columnindex, 0};
 _slist1[3] = {c_columnindex, 0};  _dlist1[3] = {Dc_columnindex, 0};
 _slist1[4] = {d_columnindex, 0};  _dlist1[4] = {Dd_columnindex, 0};
 _slist1[5] = {e_columnindex, 0};  _dlist1[5] = {De_columnindex, 0};
_first = 0;
}

#if NMODL_TEXT
static void register_nmodl_text_and_filename(int mech_type) {
    const char* nmodl_filename = "/home/raven/PycharmProjects/Masters_model/Mod_Files_Beining_2017/Caold.mod";
    const char* nmodl_file_text = 
  ": Ca channels (T,N,L-type)\n"
  "\n"
  "\n"
  "NEURON {\n"
  "	SUFFIX Caold\n"
  "	USEION ca WRITE ica\n"
  "	RANGE gtcabar, gncabar, glcabar, gtca, gnca, glca, e_ca, caim\n"
  "	GLOBAL ca0, cao\n"
  "}\n"
  "\n"
  "UNITS {\n"
  "	(molar) = (1/liter)\n"
  "	(mM) = (millimolar)\n"
  "	(mV) = (millivolt)\n"
  "	(mA) = (milliamp)\n"
  "	(S) = (siemens)\n"
  "	B = .26 (mM-cm2/mA-ms)\n"
  "	F = (faraday) (coulomb)\n"
  "	R = (k-mole) (joule/degC)\n"
  "	TEMP = 25 (degC)\n"
  "}\n"
  "\n"
  "PARAMETER {\n"
  "	ca0 = .00007(mM)		: initial calcium concentration inside\n"
  "	cao = 2		(mM)		: calcium concentration outside\n"
  "	tau = 9		(ms)\n"
  "	gtcabar = .01	(S/cm2)	: maximum permeability\n"
  "	gncabar = .01	(S/cm2)\n"
  "	glcabar = .01	(S/cm2)\n"
  "}\n"
  "\n"
  "ASSIGNED {\n"
  "	v		(mV)\n"
  "	e_ca		(mV)\n"
  "	ica		(mA/cm2)\n"
  "	gtca		(S/cm2)\n"
  "	gnca		(S/cm2)\n"
  "	glca		(S/cm2)\n"
  "	caim (mM)\n"
  "}\n"
  "\n"
  "STATE { ca_i (mM) a b c d e}\n"
  "\n"
  "BREAKPOINT {\n"
  "	SOLVE state METHOD cnexp\n"
  "	e_ca = (1000)*(TEMP+273.15)*R/(2*F)*log(cao/ca_i)\n"
  "	gtca = gtcabar*a*a*b\n"
  "	gnca = gncabar*c*c*d\n"
  "	glca = glcabar*e*e\n"
  "	ica = (gtca+gnca+glca)*(v - e_ca)\n"
  "	caim = ca_i\n"
  "}\n"
  "\n"
  "DERIVATIVE state {	: exact when v held constant; integrates over dt step\n"
  "	ca_i' = -B*ica-(ca_i-ca0)/tau\n"
  "	a' = alphaa(v)*(1-a)-betaa(v)*a\n"
  "	b' = alphab(v)*(1-b)-betab(v)*b\n"
  "	c' = alphac(v)*(1-c)-betac(v)*c\n"
  "	d' = alphad(v)*(1-d)-betad(v)*d\n"
  "	e' = alphae(v)*(1-e)-betae(v)*e\n"
  "}\n"
  "\n"
  "INITIAL {\n"
  "	ca_i = ca0\n"
  "	a = alphaa(v)/(alphaa(v)+betaa(v))\n"
  "	b = alphab(v)/(alphab(v)+betab(v))\n"
  "	c = alphac(v)/(alphac(v)+betac(v))\n"
  "	d = alphad(v)/(alphad(v)+betad(v))\n"
  "	e = alphae(v)/(alphae(v)+betae(v))\n"
  "}\n"
  "\n"
  "FUNCTION alphaa(v (mV)) (/ms) {\n"
  "	alphaa = f(2,0.1,v,19.26)\n"
  "}\n"
  "\n"
  "FUNCTION betaa(v (mV)) (/ms) {\n"
  "	betaa = exponential(0.009,-0.045393,v,0)\n"
  "}\n"
  "\n"
  "FUNCTION alphab(v (mV)) (/ms) {\n"
  "	alphab = exponential(1e-6,-0.061501,v,0)\n"
  "}\n"
  "\n"
  "FUNCTION betab(v (mV)) (/ms) {\n"
  "	betab = logistic(1,-0.1,v,29.79)\n"
  "}\n"
  "\n"
  "FUNCTION alphac(v (mV)) (/ms) {\n"
  "	alphac = f(1.9,0.1,v,19.88)\n"
  "}\n"
  "\n"
  "FUNCTION betac(v (mV)) (/ms) {\n"
  "	betac = exponential(0.046,-0.048239,v,0)\n"
  "}\n"
  "\n"
  "FUNCTION alphad(v (mV)) (/ms) {\n"
  "	alphad = exponential(1.6e-4,-0.020661,v,0)\n"
  "}\n"
  "\n"
  "FUNCTION betad(v (mV)) (/ms) {\n"
  "	betad = logistic(1,-0.1,v,39)\n"
  "}\n"
  "\n"
  "FUNCTION alphae(v (mV)) (/ms) {\n"
  "	alphae = f(156.9,0.1,v,81.5)\n"
  "}\n"
  "\n"
  "FUNCTION betae(v (mV)) (/ms) {\n"
  "	betae = exponential(0.29,-0.092081,v,0)\n"
  "}\n"
  "\n"
  "FUNCTION f(A, k, v (mV), D) (/ms) {\n"
  "	LOCAL x\n"
  "	UNITSOFF\n"
  "	x = k*(v-D)\n"
  "	if (fabs(x) > 1e-6) {\n"
  "		f = A*x/(1-exp(-x))\n"
  "	}else{\n"
  "		f = A/(1-0.5*x)\n"
  "	}\n"
  "	UNITSON\n"
  "}\n"
  "\n"
  "FUNCTION logistic(A, k, v (mV), D) (/ms) {\n"
  "	UNITSOFF\n"
  "	logistic = A/(1+exp(k*(v-D)))\n"
  "	UNITSON\n"
  "}\n"
  "\n"
  "FUNCTION exponential(A, k, v (mV), D) (/ms) {\n"
  "	UNITSOFF\n"
  "	exponential = A*exp(k*(v-D))\n"
  "	UNITSON\n"
  "}\n"
  ;
    hoc_reg_nmodl_filename(mech_type, nmodl_filename);
    hoc_reg_nmodl_text(mech_type, nmodl_file_text);
}
#endif
