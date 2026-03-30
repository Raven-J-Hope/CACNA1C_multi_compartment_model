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
static constexpr auto number_of_datum_variables = 5;
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
 
#define nrn_init _nrn_init__CadepK
#define _nrn_initial _nrn_initial__CadepK
#define nrn_cur _nrn_cur__CadepK
#define _nrn_current _nrn_current__CadepK
#define nrn_jacob _nrn_jacob__CadepK
#define nrn_state _nrn_state__CadepK
#define _net_receive _net_receive__CadepK 
#define state state__CadepK 
 
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
#define gbkbar _ml->template fpfield<0>(_iml)
#define gbkbar_columnindex 0
#define gskbar _ml->template fpfield<1>(_iml)
#define gskbar_columnindex 1
#define caim _ml->template fpfield<2>(_iml)
#define caim_columnindex 2
#define ca_i _ml->template fpfield<3>(_iml)
#define ca_i_columnindex 3
#define q _ml->template fpfield<4>(_iml)
#define q_columnindex 4
#define r _ml->template fpfield<5>(_iml)
#define r_columnindex 5
#define s _ml->template fpfield<6>(_iml)
#define s_columnindex 6
#define ek _ml->template fpfield<7>(_iml)
#define ek_columnindex 7
#define ik _ml->template fpfield<8>(_iml)
#define ik_columnindex 8
#define ica _ml->template fpfield<9>(_iml)
#define ica_columnindex 9
#define gbk _ml->template fpfield<10>(_iml)
#define gbk_columnindex 10
#define gsk _ml->template fpfield<11>(_iml)
#define gsk_columnindex 11
#define Dca_i _ml->template fpfield<12>(_iml)
#define Dca_i_columnindex 12
#define Dq _ml->template fpfield<13>(_iml)
#define Dq_columnindex 13
#define Dr _ml->template fpfield<14>(_iml)
#define Dr_columnindex 14
#define Ds _ml->template fpfield<15>(_iml)
#define Ds_columnindex 15
#define v _ml->template fpfield<16>(_iml)
#define v_columnindex 16
#define _g _ml->template fpfield<17>(_iml)
#define _g_columnindex 17
#define _ion_ica *(_ml->dptr_field<0>(_iml))
#define _p_ion_ica static_cast<neuron::container::data_handle<double>>(_ppvar[0])
#define _ion_ek *(_ml->dptr_field<1>(_iml))
#define _p_ion_ek static_cast<neuron::container::data_handle<double>>(_ppvar[1])
#define _ion_ik *(_ml->dptr_field<2>(_iml))
#define _p_ion_ik static_cast<neuron::container::data_handle<double>>(_ppvar[2])
#define _ion_dikdv *(_ml->dptr_field<3>(_iml))
#define area	(*(_ml->dptr_field<4>(_iml)))
 /* Thread safe. No static _ml, _iml or _ppvar. */
 static int hoc_nrnpointerindex =  -1;
 static _nrn_mechanism_std_vector<Datum> _extcall_thread;
 static Prop* _extcall_prop;
 /* _prop_id kind of shadows _extcall_prop to allow validity checking. */
 static _nrn_non_owning_id_without_container _prop_id{};
 /* external NEURON variables */
 /* declaration of user functions */
 static void _hoc_alphaq(void);
 static void _hoc_betar(void);
 static void _hoc_betaq(void);
 static void _hoc_exp1(void);
 static void _hoc_sinf(void);
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
 {"setdata_CadepK", _hoc_setdata},
 {"alphaq_CadepK", _hoc_alphaq},
 {"betar_CadepK", _hoc_betar},
 {"betaq_CadepK", _hoc_betaq},
 {"exp1_CadepK", _hoc_exp1},
 {"sinf_CadepK", _hoc_sinf},
 {0, 0}
};
 
/* Direct Python call wrappers to density mechanism functions.*/
 static double _npy_alphaq(Prop*);
 static double _npy_betar(Prop*);
 static double _npy_betaq(Prop*);
 static double _npy_exp1(Prop*);
 static double _npy_sinf(Prop*);
 
static NPyDirectMechFunc npy_direct_func_proc[] = {
 {"alphaq", _npy_alphaq},
 {"betar", _npy_betar},
 {"betaq", _npy_betaq},
 {"exp1", _npy_exp1},
 {"sinf", _npy_sinf},
 {0, 0}
};
#define alphaq alphaq_CadepK
#define betar betar_CadepK
#define betaq betaq_CadepK
#define exp1 exp1_CadepK
#define sinf sinf_CadepK
 extern double alphaq( _internalthreadargsprotocomma_ double );
 extern double betar( _internalthreadargsprotocomma_ double );
 extern double betaq( _internalthreadargsprotocomma_ double );
 extern double exp1( _internalthreadargsprotocomma_ double , double , double , double );
 extern double sinf( _internalthreadargsprotocomma_ double );
 /* declare global and static user variables */
 #define gind 0
 #define _gth 0
#define alphar alphar_CadepK
 double alphar = 7.5;
#define ca0 ca0_CadepK
 double ca0 = 7e-05;
#define stau stau_CadepK
 double stau = 10;
#define tau tau_CadepK
 double tau = 9;
 /* some parameters have upper and lower limits */
 static HocParmLimits _hoc_parm_limits[] = {
 {0, 0, 0}
};
 static HocParmUnits _hoc_parm_units[] = {
 {"ca0_CadepK", "mM"},
 {"tau_CadepK", "ms"},
 {"alphar_CadepK", "/ms"},
 {"stau_CadepK", "ms"},
 {"gbkbar_CadepK", "S/cm2"},
 {"gskbar_CadepK", "S/cm2"},
 {"ca_i_CadepK", "mM"},
 {"caim_CadepK", "mM"},
 {0, 0}
};
 static double ca_i0 = 0;
 static double delta_t = 0.01;
 static double q0 = 0;
 static double r0 = 0;
 static double s0 = 0;
 /* connect global user variables to hoc */
 static DoubScal hoc_scdoub[] = {
 {"ca0_CadepK", &ca0_CadepK},
 {"tau_CadepK", &tau_CadepK},
 {"alphar_CadepK", &alphar_CadepK},
 {"stau_CadepK", &stau_CadepK},
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
 
#define _cvode_ieq _ppvar[5].literal_value<int>()
 static void _ode_matsol_instance1(_internalthreadargsproto_);
 /* connect range variables in _p that hoc is supposed to know about */
 static const char *_mechanism[] = {
 "7.7.0",
"CadepK",
 "gbkbar_CadepK",
 "gskbar_CadepK",
 0,
 "caim_CadepK",
 0,
 "ca_i_CadepK",
 "q_CadepK",
 "r_CadepK",
 "s_CadepK",
 0,
 0};
 extern Node* nrn_alloc_node_;
 static Symbol* _ca_sym;
 static Symbol* _k_sym;
 
 /* Used by NrnProperty */
 static _nrn_mechanism_std_vector<double> _parm_default{
     0.01, /* gbkbar */
     0.01, /* gskbar */
 }; 
 
 
extern Prop* need_memb(Symbol*);
static void nrn_alloc(Prop* _prop) {
  Prop *prop_ion{};
  Datum *_ppvar{};
   _ppvar = nrn_prop_datum_alloc(_mechtype, 6, _prop);
    _nrn_mechanism_access_dparam(_prop) = _ppvar;
     _nrn_mechanism_cache_instance _ml_real{_prop};
    auto* const _ml = &_ml_real;
    size_t const _iml{};
    assert(_nrn_mechanism_get_num_vars(_prop) == 18);
 	/*initialize range parameters*/
 	gbkbar = _parm_default[0]; /* 0.01 */
 	gskbar = _parm_default[1]; /* 0.01 */
 	 assert(_nrn_mechanism_get_num_vars(_prop) == 18);
 	_nrn_mechanism_access_dparam(_prop) = _ppvar;
 	/*connect ionic variables to this model*/
 	_ppvar[4] = _nrn_mechanism_get_area_handle(nrn_alloc_node_);
 prop_ion = need_memb(_ca_sym);
 	_ppvar[0] = _nrn_mechanism_get_param_handle(prop_ion, 3); /* ica */
 prop_ion = need_memb(_k_sym);
 nrn_promote(prop_ion, 0, 1);
 	_ppvar[1] = _nrn_mechanism_get_param_handle(prop_ion, 0); /* ek */
 	_ppvar[2] = _nrn_mechanism_get_param_handle(prop_ion, 3); /* ik */
 	_ppvar[3] = _nrn_mechanism_get_param_handle(prop_ion, 4); /* _ion_dikdv */
 
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

 extern "C" void _CadepK_reg() {
	int _vectorized = 1;
  _initlists();
 	ion_reg("ca", -10000.);
 	ion_reg("k", -10000.);
 	_ca_sym = hoc_lookup("ca_ion");
 	_k_sym = hoc_lookup("k_ion");
 	register_mech(_mechanism, nrn_alloc,nrn_cur, nrn_jacob, nrn_state, nrn_init, hoc_nrnpointerindex, 1);
 _mechtype = nrn_get_mechtype(_mechanism[1]);
 hoc_register_parm_default(_mechtype, &_parm_default);
         hoc_register_npy_direct(_mechtype, npy_direct_func_proc);
     _nrn_setdata_reg(_mechtype, _setdata);
 #if NMODL_TEXT
  register_nmodl_text_and_filename(_mechtype);
#endif
   _nrn_mechanism_register_data_fields(_mechtype,
                                       _nrn_mechanism_field<double>{"gbkbar"} /* 0 */,
                                       _nrn_mechanism_field<double>{"gskbar"} /* 1 */,
                                       _nrn_mechanism_field<double>{"caim"} /* 2 */,
                                       _nrn_mechanism_field<double>{"ca_i"} /* 3 */,
                                       _nrn_mechanism_field<double>{"q"} /* 4 */,
                                       _nrn_mechanism_field<double>{"r"} /* 5 */,
                                       _nrn_mechanism_field<double>{"s"} /* 6 */,
                                       _nrn_mechanism_field<double>{"ek"} /* 7 */,
                                       _nrn_mechanism_field<double>{"ik"} /* 8 */,
                                       _nrn_mechanism_field<double>{"ica"} /* 9 */,
                                       _nrn_mechanism_field<double>{"gbk"} /* 10 */,
                                       _nrn_mechanism_field<double>{"gsk"} /* 11 */,
                                       _nrn_mechanism_field<double>{"Dca_i"} /* 12 */,
                                       _nrn_mechanism_field<double>{"Dq"} /* 13 */,
                                       _nrn_mechanism_field<double>{"Dr"} /* 14 */,
                                       _nrn_mechanism_field<double>{"Ds"} /* 15 */,
                                       _nrn_mechanism_field<double>{"v"} /* 16 */,
                                       _nrn_mechanism_field<double>{"_g"} /* 17 */,
                                       _nrn_mechanism_field<double*>{"_ion_ica", "ca_ion"} /* 0 */,
                                       _nrn_mechanism_field<double*>{"_ion_ek", "k_ion"} /* 1 */,
                                       _nrn_mechanism_field<double*>{"_ion_ik", "k_ion"} /* 2 */,
                                       _nrn_mechanism_field<double*>{"_ion_dikdv", "k_ion"} /* 3 */,
                                       _nrn_mechanism_field<double*>{"area", "area"} /* 4 */,
                                       _nrn_mechanism_field<int>{"_cvode_ieq", "cvodeieq"} /* 5 */);
  hoc_register_prop_size(_mechtype, 18, 6);
  hoc_register_dparam_semantics(_mechtype, 0, "ca_ion");
  hoc_register_dparam_semantics(_mechtype, 1, "k_ion");
  hoc_register_dparam_semantics(_mechtype, 2, "k_ion");
  hoc_register_dparam_semantics(_mechtype, 3, "k_ion");
  hoc_register_dparam_semantics(_mechtype, 5, "cvodeieq");
  hoc_register_dparam_semantics(_mechtype, 4, "area");
 	hoc_register_cvode(_mechtype, _ode_count, _ode_map, _ode_spec, _ode_matsol);
 	hoc_register_tolerance(_mechtype, _hoc_state_tol, &_atollist);
 
    hoc_register_var(hoc_scdoub, hoc_vdoub, hoc_intfunc);
 	ivoc_help("help ?1 CadepK /home/raven/PycharmProjects/Masters_model/Mod_Files_Beining_2017/CadepK.mod\n");
 hoc_register_limits(_mechtype, _hoc_parm_limits);
 hoc_register_units(_mechtype, _hoc_parm_units);
 }
 static double B = .26;
static int _reset;
static const char *modelname = "";

static int error;
static int _ninits = 0;
static int _match_recurse=1;
static void _modl_cleanup(){ _match_recurse=1;}
 
static int _ode_spec1(_internalthreadargsproto_);
/*static int _ode_matsol1(_internalthreadargsproto_);*/
 static neuron::container::field_index _slist1[4], _dlist1[4];
 static int state(_internalthreadargsproto_);
 
/*CVODE*/
 static int _ode_spec1 (_internalthreadargsproto_) {int _reset = 0; {
   Dca_i = - B * ica - ( ca_i - ca0 ) / tau ;
   Dq = alphaq ( _threadargscomma_ ca_i ) * ( 1.0 - q ) - betaq ( _threadargscomma_ ca_i ) * q ;
   Dr = alphar * ( 1.0 - r ) - betar ( _threadargscomma_ v ) * r ;
   Ds = ( sinf ( _threadargscomma_ ca_i ) - s ) / stau ;
   }
 return _reset;
}
 static int _ode_matsol1 (_internalthreadargsproto_) {
 Dca_i = Dca_i  / (1. - dt*( ( - ( ( 1.0 ) ) / tau ) )) ;
 Dq = Dq  / (1. - dt*( ( alphaq ( _threadargscomma_ ca_i ) )*( ( ( - 1.0 ) ) ) - ( betaq ( _threadargscomma_ ca_i ) )*( 1.0 ) )) ;
 Dr = Dr  / (1. - dt*( ( alphar )*( ( ( - 1.0 ) ) ) - ( betar ( _threadargscomma_ v ) )*( 1.0 ) )) ;
 Ds = Ds  / (1. - dt*( ( ( ( - 1.0 ) ) ) / stau )) ;
  return 0;
}
 /*END CVODE*/
 static int state (_internalthreadargsproto_) { {
    ca_i = ca_i + (1. - exp(dt*(( - ( ( 1.0 ) ) / tau ))))*(- ( ( - B )*( ica ) - ( ( ( - ca0 ) ) ) / tau ) / ( ( - ( ( 1.0 ) ) / tau ) ) - ca_i) ;
    q = q + (1. - exp(dt*(( alphaq ( _threadargscomma_ ca_i ) )*( ( ( - 1.0 ) ) ) - ( betaq ( _threadargscomma_ ca_i ) )*( 1.0 ))))*(- ( ( alphaq ( _threadargscomma_ ca_i ) )*( ( 1.0 ) ) ) / ( ( alphaq ( _threadargscomma_ ca_i ) )*( ( ( - 1.0 ) ) ) - ( betaq ( _threadargscomma_ ca_i ) )*( 1.0 ) ) - q) ;
    r = r + (1. - exp(dt*(( alphar )*( ( ( - 1.0 ) ) ) - ( betar ( _threadargscomma_ v ) )*( 1.0 ))))*(- ( ( alphar )*( ( 1.0 ) ) ) / ( ( alphar )*( ( ( - 1.0 ) ) ) - ( betar ( _threadargscomma_ v ) )*( 1.0 ) ) - r) ;
    s = s + (1. - exp(dt*(( ( ( - 1.0 ) ) ) / stau)))*(- ( ( ( sinf ( _threadargscomma_ ca_i ) ) ) / stau ) / ( ( ( ( - 1.0 ) ) ) / stau ) - s) ;
   }
  return 0;
}
 
double exp1 ( _internalthreadargsprotocomma_ double _lA , double _ld , double _lk , double _lx ) {
   double _lexp1;
  _lexp1 = _lA / exp ( ( 12.0 * log10 ( _lx ) + _ld ) / _lk ) ;
    
return _lexp1;
 }
 
static void _hoc_exp1(void) {
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
 _r =  exp1 ( _threadargscomma_ *getarg(1) , *getarg(2) , *getarg(3) , *getarg(4) );
 hoc_retpushx(_r);
}
 
static double _npy_exp1(Prop* _prop) {
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
 _r =  exp1 ( _threadargscomma_ *getarg(1) , *getarg(2) , *getarg(3) , *getarg(4) );
 return(_r);
}
 
double alphaq ( _internalthreadargsprotocomma_ double _lx ) {
   double _lalphaq;
 _lalphaq = exp1 ( _threadargscomma_ 0.00246 , 28.48 , - 4.5 , _lx ) ;
   
return _lalphaq;
 }
 
static void _hoc_alphaq(void) {
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
 _r =  alphaq ( _threadargscomma_ *getarg(1) );
 hoc_retpushx(_r);
}
 
static double _npy_alphaq(Prop* _prop) {
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
 _r =  alphaq ( _threadargscomma_ *getarg(1) );
 return(_r);
}
 
double betaq ( _internalthreadargsprotocomma_ double _lx ) {
   double _lbetaq;
 _lbetaq = exp1 ( _threadargscomma_ 0.006 , 60.4 , 35.0 , _lx ) ;
   
return _lbetaq;
 }
 
static void _hoc_betaq(void) {
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
 _r =  betaq ( _threadargscomma_ *getarg(1) );
 hoc_retpushx(_r);
}
 
static double _npy_betaq(Prop* _prop) {
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
 _r =  betaq ( _threadargscomma_ *getarg(1) );
 return(_r);
}
 
double betar ( _internalthreadargsprotocomma_ double _lv ) {
   double _lbetar;
  _lbetar = 0.11 / exp ( ( _lv - 35.0 ) / 14.9 ) ;
    
return _lbetar;
 }
 
static void _hoc_betar(void) {
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
 _r =  betar ( _threadargscomma_ *getarg(1) );
 hoc_retpushx(_r);
}
 
static double _npy_betar(Prop* _prop) {
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
 _r =  betar ( _threadargscomma_ *getarg(1) );
 return(_r);
}
 
double sinf ( _internalthreadargsprotocomma_ double _lx ) {
   double _lsinf;
  _lsinf = 1.0 / ( 1.0 + 4.0 / ( 1000.0 * _lx ) ) ;
    
return _lsinf;
 }
 
static void _hoc_sinf(void) {
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
 _r =  sinf ( _threadargscomma_ *getarg(1) );
 hoc_retpushx(_r);
}
 
static double _npy_sinf(Prop* _prop) {
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
 _r =  sinf ( _threadargscomma_ *getarg(1) );
 return(_r);
}
 
static int _ode_count(int _type){ return 4;}
 
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
  ica = _ion_ica;
  ek = _ion_ek;
     _ode_spec1 (_threadargs_);
  }}
 
static void _ode_map(Prop* _prop, int _ieq, neuron::container::data_handle<double>* _pv, neuron::container::data_handle<double>* _pvdot, double* _atol, int _type) { 
  Datum* _ppvar;
  _ppvar = _nrn_mechanism_access_dparam(_prop);
  _cvode_ieq = _ieq;
  for (int _i=0; _i < 4; ++_i) {
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
  ica = _ion_ica;
  ek = _ion_ek;
 _ode_matsol_instance1(_threadargs_);
 }}

static void initmodel(_internalthreadargsproto_) {
  int _i; double _save;{
  ca_i = ca_i0;
  q = q0;
  r = r0;
  s = s0;
 {
   ca_i = ca0 ;
   q = alphaq ( _threadargscomma_ ca_i ) / ( alphaq ( _threadargscomma_ ca_i ) + betaq ( _threadargscomma_ ca_i ) ) ;
   r = alphar / ( alphar + betar ( _threadargscomma_ v ) ) ;
   s = sinf ( _threadargscomma_ ca_i ) ;
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
  ica = _ion_ica;
  ek = _ion_ek;
 initmodel(_threadargs_);
 }
}

static double _nrn_current(_internalthreadargsprotocomma_ double _v) {
double _current=0.; v=_v;
{ {
   gbk = gbkbar * r * s * s ;
   gsk = gskbar * q * q ;
   ik = ( gbk + gsk ) * ( v - ek ) ;
   caim = ca_i ;
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
  ica = _ion_ica;
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
  ica = _ion_ica;
  ek = _ion_ek;
 {   state(_threadargs_);
  } }}

}

static void terminal(){}

static void _initlists(){
 int _i; static int _first = 1;
  if (!_first) return;
 _slist1[0] = {ca_i_columnindex, 0};  _dlist1[0] = {Dca_i_columnindex, 0};
 _slist1[1] = {q_columnindex, 0};  _dlist1[1] = {Dq_columnindex, 0};
 _slist1[2] = {r_columnindex, 0};  _dlist1[2] = {Dr_columnindex, 0};
 _slist1[3] = {s_columnindex, 0};  _dlist1[3] = {Ds_columnindex, 0};
_first = 0;
}

#if NMODL_TEXT
static void register_nmodl_text_and_filename(int mech_type) {
    const char* nmodl_filename = "/home/raven/PycharmProjects/Masters_model/Mod_Files_Beining_2017/CadepK.mod";
    const char* nmodl_file_text = 
  ": Ca-dependent K channels (BK and SK)\n"
  "\n"
  "\n"
  "NEURON {\n"
  "	SUFFIX CadepK\n"
  "	USEION ca READ ica\n"
  "	USEION k READ ek WRITE ik\n"
  "	RANGE gbkbar, gskbar, caim\n"
  "	GLOBAL ca0, tau, stau\n"
  "}\n"
  "\n"
  "UNITS {\n"
  "	(molar) = (1/liter)\n"
  "	(mM) = (millimolar)\n"
  "	(mV) = (millivolt)\n"
  "	(mA) = (milliamp)\n"
  "	(S) = (siemens)\n"
  "	B = .26 (mM-cm2/mA-ms)\n"
  "}\n"
  "\n"
  "PARAMETER {\n"
  "	gbkbar = .01	(S/cm2)	: maximum permeability\n"
  "	gskbar = .01	(S/cm2)	: maximum permeability\n"
  "	ca0 = .00007	(mM)\n"
  "	tau = 9		(ms)\n"
  "	alphar = 7.5	(/ms)\n"
  "	stau = 10		(ms)\n"
  "}\n"
  "\n"
  "ASSIGNED {\n"
  "	v		(mV)\n"
  "	ek		(mV)\n"
  "	ik		(mA/cm2)\n"
  "	ica		(mA/cm2)\n"
  "	area		(microm2)\n"
  "      gbk		(S/cm2)\n"
  "      gsk		(S/cm2)\n"
  "	  caim  (mM)\n"
  "}\n"
  "\n"
  "STATE { ca_i (mM) q r s }\n"
  "\n"
  "BREAKPOINT {\n"
  "	SOLVE state METHOD cnexp\n"
  "	gbk = gbkbar*r*s*s\n"
  "	gsk = gskbar*q*q\n"
  "	ik = (gbk+gsk)*(v - ek)\n"
  "	caim = ca_i\n"
  "}\n"
  "\n"
  "DERIVATIVE state {	: exact when v held constant; integrates over dt step\n"
  "	ca_i' = -B*ica-(ca_i-ca0)/tau\n"
  "	q' = alphaq(ca_i)*(1-q)-betaq(ca_i)*q\n"
  "	r' = alphar*(1-r)-betar(v)*r\n"
  "	s' = (sinf(ca_i)-s)/stau\n"
  "}\n"
  "\n"
  "INITIAL {\n"
  "	ca_i = ca0\n"
  "	q = alphaq(ca_i)/(alphaq(ca_i)+betaq(ca_i))\n"
  "	r = alphar/(alphar+betar(v))\n"
  "      s = sinf(ca_i)\n"
  "}\n"
  "\n"
  "FUNCTION exp1(A (/ms), d, k, x (mM)) (/ms) {\n"
  "	UNITSOFF\n"
  "	exp1 = A/exp((12*log10(x)+d)/k)\n"
  "	UNITSON\n"
  "}\n"
  "\n"
  "FUNCTION alphaq(x (mM)) (/ms) {\n"
  "	alphaq = exp1(0.00246,28.48,-4.5,x)\n"
  "}\n"
  "\n"
  "FUNCTION betaq(x (mM)) (/ms) {\n"
  "	betaq = exp1(0.006,60.4,35,x)\n"
  "}\n"
  "\n"
  "FUNCTION betar(v (mV)) (/ms) {\n"
  "	UNITSOFF\n"
  "	betar = 0.11/exp((v-35)/14.9)\n"
  "	UNITSON\n"
  "}\n"
  "\n"
  "FUNCTION sinf(x (mM)) {\n"
  "	UNITSOFF\n"
  "	sinf = 1/(1+4/(1000*x))\n"
  "	UNITSON\n"
  "}\n"
  ;
    hoc_reg_nmodl_filename(mech_type, nmodl_filename);
    hoc_reg_nmodl_text(mech_type, nmodl_file_text);
}
#endif
