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
static constexpr auto number_of_floating_point_variables = 42;
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
 
#define nrn_init _nrn_init__Kv42b
#define _nrn_initial _nrn_initial__Kv42b
#define nrn_cur _nrn_cur__Kv42b
#define _nrn_current _nrn_current__Kv42b
#define nrn_jacob _nrn_jacob__Kv42b
#define nrn_state _nrn_state__Kv42b
#define _net_receive _net_receive__Kv42b 
#define kin kin__Kv42b 
#define rate rate__Kv42b 
 
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
#define C0 _ml->template fpfield<4>(_iml)
#define C0_columnindex 4
#define C1 _ml->template fpfield<5>(_iml)
#define C1_columnindex 5
#define C2 _ml->template fpfield<6>(_iml)
#define C2_columnindex 6
#define C3 _ml->template fpfield<7>(_iml)
#define C3_columnindex 7
#define C4 _ml->template fpfield<8>(_iml)
#define C4_columnindex 8
#define I0 _ml->template fpfield<9>(_iml)
#define I0_columnindex 9
#define I1 _ml->template fpfield<10>(_iml)
#define I1_columnindex 10
#define I2 _ml->template fpfield<11>(_iml)
#define I2_columnindex 11
#define I3 _ml->template fpfield<12>(_iml)
#define I3_columnindex 12
#define I4 _ml->template fpfield<13>(_iml)
#define I4_columnindex 13
#define IO1 _ml->template fpfield<14>(_iml)
#define IO1_columnindex 14
#define IO2 _ml->template fpfield<15>(_iml)
#define IO2_columnindex 15
#define I5 _ml->template fpfield<16>(_iml)
#define I5_columnindex 16
#define C5 _ml->template fpfield<17>(_iml)
#define C5_columnindex 17
#define DO _ml->template fpfield<18>(_iml)
#define DO_columnindex 18
#define DC0 _ml->template fpfield<19>(_iml)
#define DC0_columnindex 19
#define DC1 _ml->template fpfield<20>(_iml)
#define DC1_columnindex 20
#define DC2 _ml->template fpfield<21>(_iml)
#define DC2_columnindex 21
#define DC3 _ml->template fpfield<22>(_iml)
#define DC3_columnindex 22
#define DC4 _ml->template fpfield<23>(_iml)
#define DC4_columnindex 23
#define DI0 _ml->template fpfield<24>(_iml)
#define DI0_columnindex 24
#define DI1 _ml->template fpfield<25>(_iml)
#define DI1_columnindex 25
#define DI2 _ml->template fpfield<26>(_iml)
#define DI2_columnindex 26
#define DI3 _ml->template fpfield<27>(_iml)
#define DI3_columnindex 27
#define DI4 _ml->template fpfield<28>(_iml)
#define DI4_columnindex 28
#define DIO1 _ml->template fpfield<29>(_iml)
#define DIO1_columnindex 29
#define DIO2 _ml->template fpfield<30>(_iml)
#define DIO2_columnindex 30
#define DI5 _ml->template fpfield<31>(_iml)
#define DI5_columnindex 31
#define DC5 _ml->template fpfield<32>(_iml)
#define DC5_columnindex 32
#define ek _ml->template fpfield<33>(_iml)
#define ek_columnindex 33
#define alpha _ml->template fpfield<34>(_iml)
#define alpha_columnindex 34
#define beta _ml->template fpfield<35>(_iml)
#define beta_columnindex 35
#define gamma _ml->template fpfield<36>(_iml)
#define gamma_columnindex 36
#define delta _ml->template fpfield<37>(_iml)
#define delta_columnindex 37
#define epsilon _ml->template fpfield<38>(_iml)
#define epsilon_columnindex 38
#define phi _ml->template fpfield<39>(_iml)
#define phi_columnindex 39
#define v _ml->template fpfield<40>(_iml)
#define v_columnindex 40
#define _g _ml->template fpfield<41>(_iml)
#define _g_columnindex 41
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
 static void _hoc_exponential(void);
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
 {"setdata_Kv42b", _hoc_setdata},
 {"exponential_Kv42b", _hoc_exponential},
 {"rate_Kv42b", _hoc_rate},
 {0, 0}
};
 
/* Direct Python call wrappers to density mechanism functions.*/
 static double _npy_exponential(Prop*);
 static double _npy_rate(Prop*);
 
static NPyDirectMechFunc npy_direct_func_proc[] = {
 {"exponential", _npy_exponential},
 {"rate", _npy_rate},
 {0, 0}
};
#define exponential exponential_Kv42b
 extern double exponential( _internalthreadargsprotocomma_ double , double , double , double );
 /* declare global and static user variables */
 #define gind 0
 #define _gth 0
#define a0 a0_Kv42b
 double a0 = 1.589;
#define b0 b0_Kv42b
 double b0 = 0.0184;
#define c0 c0_Kv42b
 double c0 = 6.668;
#define d0 d0_Kv42b
 double d0 = 2.381;
#define e0 e0_Kv42b
 double e0 = 0.503;
#define f0 f0_Kv42b
 double f0 = 0.174;
#define f f_Kv42b
 double f = 0.045;
#define g g_Kv42b
 double g = 1.03;
#define kappa2 kappa2_Kv42b
 double kappa2 = 0.0487;
#define kappa1 kappa1_Kv42b
 double kappa1 = 0.229;
#define kic kic_Kv42b
 double kic = 3e-05;
#define kci kci_Kv42b
 double kci = 0.047;
#define lambda2 lambda2_Kv42b
 double lambda2 = 0.0065;
#define lambda1 lambda1_Kv42b
 double lambda1 = 0.151;
#define vshift vshift_Kv42b
 double vshift = 0;
#define zf zf_Kv42b
 double zf = -0.25;
#define ze ze_Kv42b
 double ze = 0.07;
#define zd zd_Kv42b
 double zd = -1.21;
#define zc zc_Kv42b
 double zc = 0.15;
#define zb zb_Kv42b
 double zb = -1.31;
#define za za_Kv42b
 double za = 0.64;
 /* some parameters have upper and lower limits */
 static HocParmLimits _hoc_parm_limits[] = {
 {0, 0, 0}
};
 static HocParmUnits _hoc_parm_units[] = {
 {"a0_Kv42b", "/ms"},
 {"b0_Kv42b", "/ms"},
 {"c0_Kv42b", "/ms"},
 {"d0_Kv42b", "/ms"},
 {"e0_Kv42b", "/ms"},
 {"f0_Kv42b", "/ms"},
 {"kci_Kv42b", "/ms"},
 {"kic_Kv42b", "/ms"},
 {"kappa1_Kv42b", "/ms"},
 {"lambda1_Kv42b", "/ms"},
 {"kappa2_Kv42b", "/ms"},
 {"lambda2_Kv42b", "/ms"},
 {"vshift_Kv42b", "mV"},
 {"gkbar_Kv42b", "S/cm2"},
 {"ik_Kv42b", "mA/cm2"},
 {"gk_Kv42b", "S/cm2"},
 {0, 0}
};
 static double C50 = 0;
 static double C40 = 0;
 static double C30 = 0;
 static double C20 = 0;
 static double C10 = 0;
 static double C00 = 0;
 static double I50 = 0;
 static double IO20 = 0;
 static double IO10 = 0;
 static double I40 = 0;
 static double I30 = 0;
 static double I20 = 0;
 static double I10 = 0;
 static double I00 = 0;
 static double O0 = 0;
 static double delta_t = 0.01;
 /* connect global user variables to hoc */
 static DoubScal hoc_scdoub[] = {
 {"f_Kv42b", &f_Kv42b},
 {"g_Kv42b", &g_Kv42b},
 {"a0_Kv42b", &a0_Kv42b},
 {"za_Kv42b", &za_Kv42b},
 {"b0_Kv42b", &b0_Kv42b},
 {"zb_Kv42b", &zb_Kv42b},
 {"c0_Kv42b", &c0_Kv42b},
 {"zc_Kv42b", &zc_Kv42b},
 {"d0_Kv42b", &d0_Kv42b},
 {"zd_Kv42b", &zd_Kv42b},
 {"e0_Kv42b", &e0_Kv42b},
 {"ze_Kv42b", &ze_Kv42b},
 {"f0_Kv42b", &f0_Kv42b},
 {"zf_Kv42b", &zf_Kv42b},
 {"kci_Kv42b", &kci_Kv42b},
 {"kic_Kv42b", &kic_Kv42b},
 {"kappa1_Kv42b", &kappa1_Kv42b},
 {"lambda1_Kv42b", &lambda1_Kv42b},
 {"kappa2_Kv42b", &kappa2_Kv42b},
 {"lambda2_Kv42b", &lambda2_Kv42b},
 {"vshift_Kv42b", &vshift_Kv42b},
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
"Kv42b",
 "gkbar_Kv42b",
 0,
 "ik_Kv42b",
 "gk_Kv42b",
 0,
 "O_Kv42b",
 "C0_Kv42b",
 "C1_Kv42b",
 "C2_Kv42b",
 "C3_Kv42b",
 "C4_Kv42b",
 "I0_Kv42b",
 "I1_Kv42b",
 "I2_Kv42b",
 "I3_Kv42b",
 "I4_Kv42b",
 "IO1_Kv42b",
 "IO2_Kv42b",
 "I5_Kv42b",
 "C5_Kv42b",
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
    assert(_nrn_mechanism_get_num_vars(_prop) == 42);
 	/*initialize range parameters*/
 	gkbar = _parm_default[0]; /* 0.00015 */
 	 assert(_nrn_mechanism_get_num_vars(_prop) == 42);
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

 extern "C" void _Kv42b_reg() {
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
                                       _nrn_mechanism_field<double>{"C0"} /* 4 */,
                                       _nrn_mechanism_field<double>{"C1"} /* 5 */,
                                       _nrn_mechanism_field<double>{"C2"} /* 6 */,
                                       _nrn_mechanism_field<double>{"C3"} /* 7 */,
                                       _nrn_mechanism_field<double>{"C4"} /* 8 */,
                                       _nrn_mechanism_field<double>{"I0"} /* 9 */,
                                       _nrn_mechanism_field<double>{"I1"} /* 10 */,
                                       _nrn_mechanism_field<double>{"I2"} /* 11 */,
                                       _nrn_mechanism_field<double>{"I3"} /* 12 */,
                                       _nrn_mechanism_field<double>{"I4"} /* 13 */,
                                       _nrn_mechanism_field<double>{"IO1"} /* 14 */,
                                       _nrn_mechanism_field<double>{"IO2"} /* 15 */,
                                       _nrn_mechanism_field<double>{"I5"} /* 16 */,
                                       _nrn_mechanism_field<double>{"C5"} /* 17 */,
                                       _nrn_mechanism_field<double>{"DO"} /* 18 */,
                                       _nrn_mechanism_field<double>{"DC0"} /* 19 */,
                                       _nrn_mechanism_field<double>{"DC1"} /* 20 */,
                                       _nrn_mechanism_field<double>{"DC2"} /* 21 */,
                                       _nrn_mechanism_field<double>{"DC3"} /* 22 */,
                                       _nrn_mechanism_field<double>{"DC4"} /* 23 */,
                                       _nrn_mechanism_field<double>{"DI0"} /* 24 */,
                                       _nrn_mechanism_field<double>{"DI1"} /* 25 */,
                                       _nrn_mechanism_field<double>{"DI2"} /* 26 */,
                                       _nrn_mechanism_field<double>{"DI3"} /* 27 */,
                                       _nrn_mechanism_field<double>{"DI4"} /* 28 */,
                                       _nrn_mechanism_field<double>{"DIO1"} /* 29 */,
                                       _nrn_mechanism_field<double>{"DIO2"} /* 30 */,
                                       _nrn_mechanism_field<double>{"DI5"} /* 31 */,
                                       _nrn_mechanism_field<double>{"DC5"} /* 32 */,
                                       _nrn_mechanism_field<double>{"ek"} /* 33 */,
                                       _nrn_mechanism_field<double>{"alpha"} /* 34 */,
                                       _nrn_mechanism_field<double>{"beta"} /* 35 */,
                                       _nrn_mechanism_field<double>{"gamma"} /* 36 */,
                                       _nrn_mechanism_field<double>{"delta"} /* 37 */,
                                       _nrn_mechanism_field<double>{"epsilon"} /* 38 */,
                                       _nrn_mechanism_field<double>{"phi"} /* 39 */,
                                       _nrn_mechanism_field<double>{"v"} /* 40 */,
                                       _nrn_mechanism_field<double>{"_g"} /* 41 */,
                                       _nrn_mechanism_field<double*>{"_ion_ek", "k_ion"} /* 0 */,
                                       _nrn_mechanism_field<double*>{"_ion_ik", "k_ion"} /* 1 */,
                                       _nrn_mechanism_field<double*>{"_ion_dikdv", "k_ion"} /* 2 */,
                                       _nrn_mechanism_field<int>{"_cvode_ieq", "cvodeieq"} /* 3 */);
  hoc_register_prop_size(_mechtype, 42, 4);
  hoc_register_dparam_semantics(_mechtype, 0, "k_ion");
  hoc_register_dparam_semantics(_mechtype, 1, "k_ion");
  hoc_register_dparam_semantics(_mechtype, 2, "k_ion");
  hoc_register_dparam_semantics(_mechtype, 3, "cvodeieq");
 	hoc_register_cvode(_mechtype, _ode_count, _ode_map, _ode_spec, _ode_matsol);
 	hoc_register_tolerance(_mechtype, _hoc_state_tol, &_atollist);
 
    hoc_register_var(hoc_scdoub, hoc_vdoub, hoc_intfunc);
 	ivoc_help("help ?1 Kv42b /home/raven/PycharmProjects/Masters_model/Mod_Files_Beining_2017/Kv42b.mod\n");
 hoc_register_limits(_mechtype, _hoc_parm_limits);
 hoc_register_units(_mechtype, _hoc_parm_units);
 }
 static double FARADAY = 0x1.81f0fae775425p+6;
 static double R = 0x1.0a1013e8990bep+3;
static int _reset;
static const char *modelname = "Kv4.2 with auxilliary subunits";

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
 static neuron::container::field_index _slist1[15], _dlist1[15]; static double *_temp1;
 static int kin (void* _so, double* _rhs, _internalthreadargsproto_);
 
static int kin (void* _so, double* _rhs, _internalthreadargsproto_)
 {int _reset=0;
 {
   double b_flux, f_flux, _term; int _i;
 {int _i; double _dt1 = 1.0/dt;
for(_i=1;_i<15;_i++){
  	_RHS1(_i) = -_dt1*(_ml->data(_iml, _slist1[_i]) - _ml->data(_iml, _dlist1[_i]));
	_MATELM1(_i, _i) = _dt1;
      
} }
 rate ( _threadargscomma_ v ) ;
   /* ~ I0 <-> I1 ( 4.0 * alpha / f , beta * f )*/
 f_flux =  4.0 * alpha / f * I0 ;
 b_flux =  beta * f * I1 ;
 _RHS1( 13) -= (f_flux - b_flux);
 _RHS1( 12) += (f_flux - b_flux);
 
 _term =  4.0 * alpha / f ;
 _MATELM1( 13 ,13)  += _term;
 _MATELM1( 12 ,13)  -= _term;
 _term =  beta * f ;
 _MATELM1( 13 ,12)  -= _term;
 _MATELM1( 12 ,12)  += _term;
 /*REACTION*/
  /* ~ I1 <-> I2 ( 3.0 * alpha / f , 2.0 * beta * f )*/
 f_flux =  3.0 * alpha / f * I1 ;
 b_flux =  2.0 * beta * f * I2 ;
 _RHS1( 12) -= (f_flux - b_flux);
 _RHS1( 11) += (f_flux - b_flux);
 
 _term =  3.0 * alpha / f ;
 _MATELM1( 12 ,12)  += _term;
 _MATELM1( 11 ,12)  -= _term;
 _term =  2.0 * beta * f ;
 _MATELM1( 12 ,11)  -= _term;
 _MATELM1( 11 ,11)  += _term;
 /*REACTION*/
  /* ~ I2 <-> I3 ( 2.0 * alpha / f , 3.0 * beta * f )*/
 f_flux =  2.0 * alpha / f * I2 ;
 b_flux =  3.0 * beta * f * I3 ;
 _RHS1( 11) -= (f_flux - b_flux);
 _RHS1( 10) += (f_flux - b_flux);
 
 _term =  2.0 * alpha / f ;
 _MATELM1( 11 ,11)  += _term;
 _MATELM1( 10 ,11)  -= _term;
 _term =  3.0 * beta * f ;
 _MATELM1( 11 ,10)  -= _term;
 _MATELM1( 10 ,10)  += _term;
 /*REACTION*/
  /* ~ I3 <-> I4 ( alpha / f , 4.0 * beta * f )*/
 f_flux =  alpha / f * I3 ;
 b_flux =  4.0 * beta * f * I4 ;
 _RHS1( 10) -= (f_flux - b_flux);
 _RHS1( 9) += (f_flux - b_flux);
 
 _term =  alpha / f ;
 _MATELM1( 10 ,10)  += _term;
 _MATELM1( 9 ,10)  -= _term;
 _term =  4.0 * beta * f ;
 _MATELM1( 10 ,9)  -= _term;
 _MATELM1( 9 ,9)  += _term;
 /*REACTION*/
  /* ~ I4 <-> I5 ( gamma * g , delta / g )*/
 f_flux =  gamma * g * I4 ;
 b_flux =  delta / g * I5 ;
 _RHS1( 9) -= (f_flux - b_flux);
 _RHS1( 6) += (f_flux - b_flux);
 
 _term =  gamma * g ;
 _MATELM1( 9 ,9)  += _term;
 _MATELM1( 6 ,9)  -= _term;
 _term =  delta / g ;
 _MATELM1( 9 ,6)  -= _term;
 _MATELM1( 6 ,6)  += _term;
 /*REACTION*/
  /* ~ C0 <-> I0 ( kci * pow( f , 4.0 ) , kic / pow( f , 4.0 ) )*/
 f_flux =  kci * pow( f , 4.0 ) * C0 ;
 b_flux =  kic / pow( f , 4.0 ) * I0 ;
 _RHS1( 5) -= (f_flux - b_flux);
 _RHS1( 13) += (f_flux - b_flux);
 
 _term =  kci * pow( f , 4.0 ) ;
 _MATELM1( 5 ,5)  += _term;
 _MATELM1( 13 ,5)  -= _term;
 _term =  kic / pow( f , 4.0 ) ;
 _MATELM1( 5 ,13)  -= _term;
 _MATELM1( 13 ,13)  += _term;
 /*REACTION*/
  /* ~ C1 <-> I1 ( kci * pow( f , 3.0 ) , kic / pow( f , 3.0 ) )*/
 f_flux =  kci * pow( f , 3.0 ) * C1 ;
 b_flux =  kic / pow( f , 3.0 ) * I1 ;
 _RHS1( 4) -= (f_flux - b_flux);
 _RHS1( 12) += (f_flux - b_flux);
 
 _term =  kci * pow( f , 3.0 ) ;
 _MATELM1( 4 ,4)  += _term;
 _MATELM1( 12 ,4)  -= _term;
 _term =  kic / pow( f , 3.0 ) ;
 _MATELM1( 4 ,12)  -= _term;
 _MATELM1( 12 ,12)  += _term;
 /*REACTION*/
  /* ~ C2 <-> I2 ( kci * pow( f , 2.0 ) , kic / pow( f , 2.0 ) )*/
 f_flux =  kci * pow( f , 2.0 ) * C2 ;
 b_flux =  kic / pow( f , 2.0 ) * I2 ;
 _RHS1( 3) -= (f_flux - b_flux);
 _RHS1( 11) += (f_flux - b_flux);
 
 _term =  kci * pow( f , 2.0 ) ;
 _MATELM1( 3 ,3)  += _term;
 _MATELM1( 11 ,3)  -= _term;
 _term =  kic / pow( f , 2.0 ) ;
 _MATELM1( 3 ,11)  -= _term;
 _MATELM1( 11 ,11)  += _term;
 /*REACTION*/
  /* ~ C3 <-> I3 ( kci * f , kic / f )*/
 f_flux =  kci * f * C3 ;
 b_flux =  kic / f * I3 ;
 _RHS1( 2) -= (f_flux - b_flux);
 _RHS1( 10) += (f_flux - b_flux);
 
 _term =  kci * f ;
 _MATELM1( 2 ,2)  += _term;
 _MATELM1( 10 ,2)  -= _term;
 _term =  kic / f ;
 _MATELM1( 2 ,10)  -= _term;
 _MATELM1( 10 ,10)  += _term;
 /*REACTION*/
  /* ~ C4 <-> I4 ( kci , kic )*/
 f_flux =  kci * C4 ;
 b_flux =  kic * I4 ;
 _RHS1( 1) -= (f_flux - b_flux);
 _RHS1( 9) += (f_flux - b_flux);
 
 _term =  kci ;
 _MATELM1( 1 ,1)  += _term;
 _MATELM1( 9 ,1)  -= _term;
 _term =  kic ;
 _MATELM1( 1 ,9)  -= _term;
 _MATELM1( 9 ,9)  += _term;
 /*REACTION*/
  /* ~ C5 <-> I5 ( kci * g , kic / g )*/
 f_flux =  kci * g * C5 ;
 b_flux =  kic / g * I5 ;
 _RHS1( 6) += (f_flux - b_flux);
 
 _term =  kci * g ;
 _MATELM1( 6 ,0)  -= _term;
 _term =  kic / g ;
 _MATELM1( 6 ,6)  += _term;
 /*REACTION*/
  /* ~ C0 <-> C1 ( 4.0 * alpha , beta )*/
 f_flux =  4.0 * alpha * C0 ;
 b_flux =  beta * C1 ;
 _RHS1( 5) -= (f_flux - b_flux);
 _RHS1( 4) += (f_flux - b_flux);
 
 _term =  4.0 * alpha ;
 _MATELM1( 5 ,5)  += _term;
 _MATELM1( 4 ,5)  -= _term;
 _term =  beta ;
 _MATELM1( 5 ,4)  -= _term;
 _MATELM1( 4 ,4)  += _term;
 /*REACTION*/
  /* ~ C1 <-> C2 ( 3.0 * alpha , 2.0 * beta )*/
 f_flux =  3.0 * alpha * C1 ;
 b_flux =  2.0 * beta * C2 ;
 _RHS1( 4) -= (f_flux - b_flux);
 _RHS1( 3) += (f_flux - b_flux);
 
 _term =  3.0 * alpha ;
 _MATELM1( 4 ,4)  += _term;
 _MATELM1( 3 ,4)  -= _term;
 _term =  2.0 * beta ;
 _MATELM1( 4 ,3)  -= _term;
 _MATELM1( 3 ,3)  += _term;
 /*REACTION*/
  /* ~ C2 <-> C3 ( 2.0 * alpha , 3.0 * beta )*/
 f_flux =  2.0 * alpha * C2 ;
 b_flux =  3.0 * beta * C3 ;
 _RHS1( 3) -= (f_flux - b_flux);
 _RHS1( 2) += (f_flux - b_flux);
 
 _term =  2.0 * alpha ;
 _MATELM1( 3 ,3)  += _term;
 _MATELM1( 2 ,3)  -= _term;
 _term =  3.0 * beta ;
 _MATELM1( 3 ,2)  -= _term;
 _MATELM1( 2 ,2)  += _term;
 /*REACTION*/
  /* ~ C3 <-> C4 ( alpha , 4.0 * beta )*/
 f_flux =  alpha * C3 ;
 b_flux =  4.0 * beta * C4 ;
 _RHS1( 2) -= (f_flux - b_flux);
 _RHS1( 1) += (f_flux - b_flux);
 
 _term =  alpha ;
 _MATELM1( 2 ,2)  += _term;
 _MATELM1( 1 ,2)  -= _term;
 _term =  4.0 * beta ;
 _MATELM1( 2 ,1)  -= _term;
 _MATELM1( 1 ,1)  += _term;
 /*REACTION*/
  /* ~ C4 <-> C5 ( gamma , delta )*/
 f_flux =  gamma * C4 ;
 b_flux =  delta * C5 ;
 _RHS1( 1) -= (f_flux - b_flux);
 
 _term =  gamma ;
 _MATELM1( 1 ,1)  += _term;
 _term =  delta ;
 _MATELM1( 1 ,0)  -= _term;
 /*REACTION*/
  /* ~ C5 <-> O ( epsilon , phi )*/
 f_flux =  epsilon * C5 ;
 b_flux =  phi * O ;
 _RHS1( 14) += (f_flux - b_flux);
 
 _term =  epsilon ;
 _MATELM1( 14 ,0)  -= _term;
 _term =  phi ;
 _MATELM1( 14 ,14)  += _term;
 /*REACTION*/
  /* ~ O <-> IO1 ( kappa1 , lambda1 )*/
 f_flux =  kappa1 * O ;
 b_flux =  lambda1 * IO1 ;
 _RHS1( 14) -= (f_flux - b_flux);
 _RHS1( 8) += (f_flux - b_flux);
 
 _term =  kappa1 ;
 _MATELM1( 14 ,14)  += _term;
 _MATELM1( 8 ,14)  -= _term;
 _term =  lambda1 ;
 _MATELM1( 14 ,8)  -= _term;
 _MATELM1( 8 ,8)  += _term;
 /*REACTION*/
  /* ~ IO1 <-> IO2 ( kappa2 , lambda2 )*/
 f_flux =  kappa2 * IO1 ;
 b_flux =  lambda2 * IO2 ;
 _RHS1( 8) -= (f_flux - b_flux);
 _RHS1( 7) += (f_flux - b_flux);
 
 _term =  kappa2 ;
 _MATELM1( 8 ,8)  += _term;
 _MATELM1( 7 ,8)  -= _term;
 _term =  lambda2 ;
 _MATELM1( 8 ,7)  -= _term;
 _MATELM1( 7 ,7)  += _term;
 /*REACTION*/
   /* O + C0 + C1 + C2 + C3 + C4 + I0 + I1 + I2 + I3 + I4 + IO1 + IO2 + I5 + C5 = 1.0 */
 _RHS1(0) =  1.0;
 _MATELM1(0, 0) = 1;
 _RHS1(0) -= C5 ;
 _MATELM1(0, 6) = 1;
 _RHS1(0) -= I5 ;
 _MATELM1(0, 7) = 1;
 _RHS1(0) -= IO2 ;
 _MATELM1(0, 8) = 1;
 _RHS1(0) -= IO1 ;
 _MATELM1(0, 9) = 1;
 _RHS1(0) -= I4 ;
 _MATELM1(0, 10) = 1;
 _RHS1(0) -= I3 ;
 _MATELM1(0, 11) = 1;
 _RHS1(0) -= I2 ;
 _MATELM1(0, 12) = 1;
 _RHS1(0) -= I1 ;
 _MATELM1(0, 13) = 1;
 _RHS1(0) -= I0 ;
 _MATELM1(0, 1) = 1;
 _RHS1(0) -= C4 ;
 _MATELM1(0, 2) = 1;
 _RHS1(0) -= C3 ;
 _MATELM1(0, 3) = 1;
 _RHS1(0) -= C2 ;
 _MATELM1(0, 4) = 1;
 _RHS1(0) -= C1 ;
 _MATELM1(0, 5) = 1;
 _RHS1(0) -= C0 ;
 _MATELM1(0, 14) = 1;
 _RHS1(0) -= O ;
 /*CONSERVATION*/
   } return _reset;
 }
 
static int  rate ( _internalthreadargsprotocomma_ double _lv ) {
   alpha = exponential ( _threadargscomma_ a0 , za , _lv , vshift ) ;
   beta = exponential ( _threadargscomma_ b0 , zb , _lv , vshift ) ;
   gamma = exponential ( _threadargscomma_ c0 , zc , _lv , vshift ) ;
   delta = exponential ( _threadargscomma_ d0 , zd , _lv , vshift ) ;
   epsilon = exponential ( _threadargscomma_ e0 , ze , _lv , vshift ) ;
   phi = exponential ( _threadargscomma_ f0 , zf , _lv , vshift ) ;
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
 
double exponential ( _internalthreadargsprotocomma_ double _lA , double _lz , double _lv , double _lD ) {
   double _lexponential;
 _lexponential = _lA * exp ( _lz * ( _lv - _lD ) * FARADAY / ( R * ( celsius + 273.15 ) ) ) ;
   
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
 
/*CVODE ode begin*/
 static int _ode_spec1(_internalthreadargsproto_) {
  int _reset=0;
  {
 double b_flux, f_flux, _term; int _i;
 {int _i; for(_i=0;_i<15;_i++) _ml->data(_iml, _dlist1[_i]) = 0.0;}
 rate ( _threadargscomma_ v ) ;
 /* ~ I0 <-> I1 ( 4.0 * alpha / f , beta * f )*/
 f_flux =  4.0 * alpha / f * I0 ;
 b_flux =  beta * f * I1 ;
 DI0 -= (f_flux - b_flux);
 DI1 += (f_flux - b_flux);
 
 /*REACTION*/
  /* ~ I1 <-> I2 ( 3.0 * alpha / f , 2.0 * beta * f )*/
 f_flux =  3.0 * alpha / f * I1 ;
 b_flux =  2.0 * beta * f * I2 ;
 DI1 -= (f_flux - b_flux);
 DI2 += (f_flux - b_flux);
 
 /*REACTION*/
  /* ~ I2 <-> I3 ( 2.0 * alpha / f , 3.0 * beta * f )*/
 f_flux =  2.0 * alpha / f * I2 ;
 b_flux =  3.0 * beta * f * I3 ;
 DI2 -= (f_flux - b_flux);
 DI3 += (f_flux - b_flux);
 
 /*REACTION*/
  /* ~ I3 <-> I4 ( alpha / f , 4.0 * beta * f )*/
 f_flux =  alpha / f * I3 ;
 b_flux =  4.0 * beta * f * I4 ;
 DI3 -= (f_flux - b_flux);
 DI4 += (f_flux - b_flux);
 
 /*REACTION*/
  /* ~ I4 <-> I5 ( gamma * g , delta / g )*/
 f_flux =  gamma * g * I4 ;
 b_flux =  delta / g * I5 ;
 DI4 -= (f_flux - b_flux);
 DI5 += (f_flux - b_flux);
 
 /*REACTION*/
  /* ~ C0 <-> I0 ( kci * pow( f , 4.0 ) , kic / pow( f , 4.0 ) )*/
 f_flux =  kci * pow( f , 4.0 ) * C0 ;
 b_flux =  kic / pow( f , 4.0 ) * I0 ;
 DC0 -= (f_flux - b_flux);
 DI0 += (f_flux - b_flux);
 
 /*REACTION*/
  /* ~ C1 <-> I1 ( kci * pow( f , 3.0 ) , kic / pow( f , 3.0 ) )*/
 f_flux =  kci * pow( f , 3.0 ) * C1 ;
 b_flux =  kic / pow( f , 3.0 ) * I1 ;
 DC1 -= (f_flux - b_flux);
 DI1 += (f_flux - b_flux);
 
 /*REACTION*/
  /* ~ C2 <-> I2 ( kci * pow( f , 2.0 ) , kic / pow( f , 2.0 ) )*/
 f_flux =  kci * pow( f , 2.0 ) * C2 ;
 b_flux =  kic / pow( f , 2.0 ) * I2 ;
 DC2 -= (f_flux - b_flux);
 DI2 += (f_flux - b_flux);
 
 /*REACTION*/
  /* ~ C3 <-> I3 ( kci * f , kic / f )*/
 f_flux =  kci * f * C3 ;
 b_flux =  kic / f * I3 ;
 DC3 -= (f_flux - b_flux);
 DI3 += (f_flux - b_flux);
 
 /*REACTION*/
  /* ~ C4 <-> I4 ( kci , kic )*/
 f_flux =  kci * C4 ;
 b_flux =  kic * I4 ;
 DC4 -= (f_flux - b_flux);
 DI4 += (f_flux - b_flux);
 
 /*REACTION*/
  /* ~ C5 <-> I5 ( kci * g , kic / g )*/
 f_flux =  kci * g * C5 ;
 b_flux =  kic / g * I5 ;
 DC5 -= (f_flux - b_flux);
 DI5 += (f_flux - b_flux);
 
 /*REACTION*/
  /* ~ C0 <-> C1 ( 4.0 * alpha , beta )*/
 f_flux =  4.0 * alpha * C0 ;
 b_flux =  beta * C1 ;
 DC0 -= (f_flux - b_flux);
 DC1 += (f_flux - b_flux);
 
 /*REACTION*/
  /* ~ C1 <-> C2 ( 3.0 * alpha , 2.0 * beta )*/
 f_flux =  3.0 * alpha * C1 ;
 b_flux =  2.0 * beta * C2 ;
 DC1 -= (f_flux - b_flux);
 DC2 += (f_flux - b_flux);
 
 /*REACTION*/
  /* ~ C2 <-> C3 ( 2.0 * alpha , 3.0 * beta )*/
 f_flux =  2.0 * alpha * C2 ;
 b_flux =  3.0 * beta * C3 ;
 DC2 -= (f_flux - b_flux);
 DC3 += (f_flux - b_flux);
 
 /*REACTION*/
  /* ~ C3 <-> C4 ( alpha , 4.0 * beta )*/
 f_flux =  alpha * C3 ;
 b_flux =  4.0 * beta * C4 ;
 DC3 -= (f_flux - b_flux);
 DC4 += (f_flux - b_flux);
 
 /*REACTION*/
  /* ~ C4 <-> C5 ( gamma , delta )*/
 f_flux =  gamma * C4 ;
 b_flux =  delta * C5 ;
 DC4 -= (f_flux - b_flux);
 DC5 += (f_flux - b_flux);
 
 /*REACTION*/
  /* ~ C5 <-> O ( epsilon , phi )*/
 f_flux =  epsilon * C5 ;
 b_flux =  phi * O ;
 DC5 -= (f_flux - b_flux);
 DO += (f_flux - b_flux);
 
 /*REACTION*/
  /* ~ O <-> IO1 ( kappa1 , lambda1 )*/
 f_flux =  kappa1 * O ;
 b_flux =  lambda1 * IO1 ;
 DO -= (f_flux - b_flux);
 DIO1 += (f_flux - b_flux);
 
 /*REACTION*/
  /* ~ IO1 <-> IO2 ( kappa2 , lambda2 )*/
 f_flux =  kappa2 * IO1 ;
 b_flux =  lambda2 * IO2 ;
 DIO1 -= (f_flux - b_flux);
 DIO2 += (f_flux - b_flux);
 
 /*REACTION*/
   /* O + C0 + C1 + C2 + C3 + C4 + I0 + I1 + I2 + I3 + I4 + IO1 + IO2 + I5 + C5 = 1.0 */
 /*CONSERVATION*/
   } return _reset;
 }
 
/*CVODE matsol*/
 static int _ode_matsol1(void* _so, double* _rhs, _internalthreadargsproto_) {int _reset=0;{
 double b_flux, f_flux, _term; int _i;
   b_flux = f_flux = 0.;
 {int _i; double _dt1 = 1.0/dt;
for(_i=0;_i<15;_i++){
  	_RHS1(_i) = _dt1*(_ml->data(_iml, _dlist1[_i]));
	_MATELM1(_i, _i) = _dt1;
      
} }
 rate ( _threadargscomma_ v ) ;
 /* ~ I0 <-> I1 ( 4.0 * alpha / f , beta * f )*/
 _term =  4.0 * alpha / f ;
 _MATELM1( 13 ,13)  += _term;
 _MATELM1( 12 ,13)  -= _term;
 _term =  beta * f ;
 _MATELM1( 13 ,12)  -= _term;
 _MATELM1( 12 ,12)  += _term;
 /*REACTION*/
  /* ~ I1 <-> I2 ( 3.0 * alpha / f , 2.0 * beta * f )*/
 _term =  3.0 * alpha / f ;
 _MATELM1( 12 ,12)  += _term;
 _MATELM1( 11 ,12)  -= _term;
 _term =  2.0 * beta * f ;
 _MATELM1( 12 ,11)  -= _term;
 _MATELM1( 11 ,11)  += _term;
 /*REACTION*/
  /* ~ I2 <-> I3 ( 2.0 * alpha / f , 3.0 * beta * f )*/
 _term =  2.0 * alpha / f ;
 _MATELM1( 11 ,11)  += _term;
 _MATELM1( 10 ,11)  -= _term;
 _term =  3.0 * beta * f ;
 _MATELM1( 11 ,10)  -= _term;
 _MATELM1( 10 ,10)  += _term;
 /*REACTION*/
  /* ~ I3 <-> I4 ( alpha / f , 4.0 * beta * f )*/
 _term =  alpha / f ;
 _MATELM1( 10 ,10)  += _term;
 _MATELM1( 9 ,10)  -= _term;
 _term =  4.0 * beta * f ;
 _MATELM1( 10 ,9)  -= _term;
 _MATELM1( 9 ,9)  += _term;
 /*REACTION*/
  /* ~ I4 <-> I5 ( gamma * g , delta / g )*/
 _term =  gamma * g ;
 _MATELM1( 9 ,9)  += _term;
 _MATELM1( 6 ,9)  -= _term;
 _term =  delta / g ;
 _MATELM1( 9 ,6)  -= _term;
 _MATELM1( 6 ,6)  += _term;
 /*REACTION*/
  /* ~ C0 <-> I0 ( kci * pow( f , 4.0 ) , kic / pow( f , 4.0 ) )*/
 _term =  kci * pow( f , 4.0 ) ;
 _MATELM1( 5 ,5)  += _term;
 _MATELM1( 13 ,5)  -= _term;
 _term =  kic / pow( f , 4.0 ) ;
 _MATELM1( 5 ,13)  -= _term;
 _MATELM1( 13 ,13)  += _term;
 /*REACTION*/
  /* ~ C1 <-> I1 ( kci * pow( f , 3.0 ) , kic / pow( f , 3.0 ) )*/
 _term =  kci * pow( f , 3.0 ) ;
 _MATELM1( 4 ,4)  += _term;
 _MATELM1( 12 ,4)  -= _term;
 _term =  kic / pow( f , 3.0 ) ;
 _MATELM1( 4 ,12)  -= _term;
 _MATELM1( 12 ,12)  += _term;
 /*REACTION*/
  /* ~ C2 <-> I2 ( kci * pow( f , 2.0 ) , kic / pow( f , 2.0 ) )*/
 _term =  kci * pow( f , 2.0 ) ;
 _MATELM1( 3 ,3)  += _term;
 _MATELM1( 11 ,3)  -= _term;
 _term =  kic / pow( f , 2.0 ) ;
 _MATELM1( 3 ,11)  -= _term;
 _MATELM1( 11 ,11)  += _term;
 /*REACTION*/
  /* ~ C3 <-> I3 ( kci * f , kic / f )*/
 _term =  kci * f ;
 _MATELM1( 2 ,2)  += _term;
 _MATELM1( 10 ,2)  -= _term;
 _term =  kic / f ;
 _MATELM1( 2 ,10)  -= _term;
 _MATELM1( 10 ,10)  += _term;
 /*REACTION*/
  /* ~ C4 <-> I4 ( kci , kic )*/
 _term =  kci ;
 _MATELM1( 1 ,1)  += _term;
 _MATELM1( 9 ,1)  -= _term;
 _term =  kic ;
 _MATELM1( 1 ,9)  -= _term;
 _MATELM1( 9 ,9)  += _term;
 /*REACTION*/
  /* ~ C5 <-> I5 ( kci * g , kic / g )*/
 _term =  kci * g ;
 _MATELM1( 0 ,0)  += _term;
 _MATELM1( 6 ,0)  -= _term;
 _term =  kic / g ;
 _MATELM1( 0 ,6)  -= _term;
 _MATELM1( 6 ,6)  += _term;
 /*REACTION*/
  /* ~ C0 <-> C1 ( 4.0 * alpha , beta )*/
 _term =  4.0 * alpha ;
 _MATELM1( 5 ,5)  += _term;
 _MATELM1( 4 ,5)  -= _term;
 _term =  beta ;
 _MATELM1( 5 ,4)  -= _term;
 _MATELM1( 4 ,4)  += _term;
 /*REACTION*/
  /* ~ C1 <-> C2 ( 3.0 * alpha , 2.0 * beta )*/
 _term =  3.0 * alpha ;
 _MATELM1( 4 ,4)  += _term;
 _MATELM1( 3 ,4)  -= _term;
 _term =  2.0 * beta ;
 _MATELM1( 4 ,3)  -= _term;
 _MATELM1( 3 ,3)  += _term;
 /*REACTION*/
  /* ~ C2 <-> C3 ( 2.0 * alpha , 3.0 * beta )*/
 _term =  2.0 * alpha ;
 _MATELM1( 3 ,3)  += _term;
 _MATELM1( 2 ,3)  -= _term;
 _term =  3.0 * beta ;
 _MATELM1( 3 ,2)  -= _term;
 _MATELM1( 2 ,2)  += _term;
 /*REACTION*/
  /* ~ C3 <-> C4 ( alpha , 4.0 * beta )*/
 _term =  alpha ;
 _MATELM1( 2 ,2)  += _term;
 _MATELM1( 1 ,2)  -= _term;
 _term =  4.0 * beta ;
 _MATELM1( 2 ,1)  -= _term;
 _MATELM1( 1 ,1)  += _term;
 /*REACTION*/
  /* ~ C4 <-> C5 ( gamma , delta )*/
 _term =  gamma ;
 _MATELM1( 1 ,1)  += _term;
 _MATELM1( 0 ,1)  -= _term;
 _term =  delta ;
 _MATELM1( 1 ,0)  -= _term;
 _MATELM1( 0 ,0)  += _term;
 /*REACTION*/
  /* ~ C5 <-> O ( epsilon , phi )*/
 _term =  epsilon ;
 _MATELM1( 0 ,0)  += _term;
 _MATELM1( 14 ,0)  -= _term;
 _term =  phi ;
 _MATELM1( 0 ,14)  -= _term;
 _MATELM1( 14 ,14)  += _term;
 /*REACTION*/
  /* ~ O <-> IO1 ( kappa1 , lambda1 )*/
 _term =  kappa1 ;
 _MATELM1( 14 ,14)  += _term;
 _MATELM1( 8 ,14)  -= _term;
 _term =  lambda1 ;
 _MATELM1( 14 ,8)  -= _term;
 _MATELM1( 8 ,8)  += _term;
 /*REACTION*/
  /* ~ IO1 <-> IO2 ( kappa2 , lambda2 )*/
 _term =  kappa2 ;
 _MATELM1( 8 ,8)  += _term;
 _MATELM1( 7 ,8)  -= _term;
 _term =  lambda2 ;
 _MATELM1( 8 ,7)  -= _term;
 _MATELM1( 7 ,7)  += _term;
 /*REACTION*/
   /* O + C0 + C1 + C2 + C3 + C4 + I0 + I1 + I2 + I3 + I4 + IO1 + IO2 + I5 + C5 = 1.0 */
 /*CONSERVATION*/
   } return _reset;
 }
 
/*CVODE end*/
 
static int _ode_count(int _type){ return 15;}
 
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
  for (int _i=0; _i < 15; ++_i) {
    _pv[_i] = _nrn_mechanism_get_param_handle(_prop, _slist1[_i]);
    _pvdot[_i] = _nrn_mechanism_get_param_handle(_prop, _dlist1[_i]);
    _cvode_abstol(_atollist, _atol, _i);
  }
 }
 
static void _ode_matsol_instance1(_internalthreadargsproto_) {
 _cvode_sparse_thread(&(_thread[_cvspth1].literal_value<void*>()), 15, _dlist1, neuron::scopmath::row_view{_ml, _iml}, _ode_matsol1, _threadargs_);
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
  C5 = C50;
  C4 = C40;
  C3 = C30;
  C2 = C20;
  C1 = C10;
  C0 = C00;
  I5 = I50;
  IO2 = IO20;
  IO1 = IO10;
  I4 = I40;
  I3 = I30;
  I2 = I20;
  I1 = I10;
  I0 = I00;
  O = O0;
 {
   rate ( _threadargscomma_ v ) ;
    _ss_sparse_thread(&(_thread[_spth1].literal_value<void*>()), 15, _slist1, _dlist1, neuron::scopmath::row_view{_ml, _iml}, &t, dt, kin, _linmat1, _threadargs_);
     if (secondorder) {
    int _i;
    for (_i = 0; _i < 15; ++_i) {
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
   gk = gkbar * O ;
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
 {  sparse_thread(&(_thread[_spth1].literal_value<void*>()), 15, _slist1, _dlist1, neuron::scopmath::row_view{_ml, _iml}, &t, dt, kin, _linmat1, _threadargs_);
     if (secondorder) {
    int _i;
    for (_i = 0; _i < 15; ++_i) {
      _ml->data(_iml, _slist1[_i]) += dt*_ml->data(_iml, _dlist1[_i]);
    }}
 } }}
 dt = _dtsav;
}

static void terminal(){}

static void _initlists(){
 int _i; static int _first = 1;
  if (!_first) return;
 _slist1[0] = {C5_columnindex, 0};  _dlist1[0] = {DC5_columnindex, 0};
 _slist1[1] = {C4_columnindex, 0};  _dlist1[1] = {DC4_columnindex, 0};
 _slist1[2] = {C3_columnindex, 0};  _dlist1[2] = {DC3_columnindex, 0};
 _slist1[3] = {C2_columnindex, 0};  _dlist1[3] = {DC2_columnindex, 0};
 _slist1[4] = {C1_columnindex, 0};  _dlist1[4] = {DC1_columnindex, 0};
 _slist1[5] = {C0_columnindex, 0};  _dlist1[5] = {DC0_columnindex, 0};
 _slist1[6] = {I5_columnindex, 0};  _dlist1[6] = {DI5_columnindex, 0};
 _slist1[7] = {IO2_columnindex, 0};  _dlist1[7] = {DIO2_columnindex, 0};
 _slist1[8] = {IO1_columnindex, 0};  _dlist1[8] = {DIO1_columnindex, 0};
 _slist1[9] = {I4_columnindex, 0};  _dlist1[9] = {DI4_columnindex, 0};
 _slist1[10] = {I3_columnindex, 0};  _dlist1[10] = {DI3_columnindex, 0};
 _slist1[11] = {I2_columnindex, 0};  _dlist1[11] = {DI2_columnindex, 0};
 _slist1[12] = {I1_columnindex, 0};  _dlist1[12] = {DI1_columnindex, 0};
 _slist1[13] = {I0_columnindex, 0};  _dlist1[13] = {DI0_columnindex, 0};
 _slist1[14] = {O_columnindex, 0};  _dlist1[14] = {DO_columnindex, 0};
_first = 0;
}

#if NMODL_TEXT
static void register_nmodl_text_and_filename(int mech_type) {
    const char* nmodl_filename = "/home/raven/PycharmProjects/Masters_model/Mod_Files_Beining_2017/Kv42b.mod";
    const char* nmodl_file_text = 
  "TITLE Kv4.2 with auxilliary subunits\n"
  "\n"
  "COMMENT\n"
  "This is the Kv4.2 model of Amarillo et al 2008: Ternary Kv4.2 channels recapitulate voltage-dependent inactivation kinetics of A-type K+ channels in cerebellar granule neurons. The Journal of Physiology\n"
  ": implemented by M.Beining; Beining et al (2016), \"A novel comprehensive and consistent electrophysiologcal model of dentate granule cells\"\n"
  "\n"
  "\n"
  "ENDCOMMENT\n"
  "\n"
  "NEURON {\n"
  "	SUFFIX Kv42b\n"
  "	USEION k READ ek WRITE ik\n"
  "    RANGE  ik, gk, gkbar\n"
  "	GLOBAL 	f,	g,a0,za,b0,zb, c0, zc, d0, zd, e0, ze, f0, zf ,kci,kic, kappa1, lambda1, kappa2, lambda2, vshift\n"
  "}\n"
  "\n"
  "UNITS {\n"
  "	(mA) = (milliamp)\n"
  "	(mV) = (millivolt)\n"
  "    (S)  = (siemens)\n"
  "	\n"
  "	(molar) = (1/liter)\n"
  "	(mM) = (millimolar)\n"
  "	(uM) = (micromolar)\n"
  "	FARADAY = (faraday) (kilocoulombs)\n"
  "	R = (k-mole) (joule/degC)\n"
  "}\n"
  "\n"
  "PARAMETER {\n"
  "	v 		(mV)\n"
  "	gkbar  = 0.00015                (S/cm2) : to be fitted     	\n"
  "	f = 0.045  : The allosteric factor f defines the voltage-dependent coupling between activation and inactivation pathways for closed channels. deactivation\n"
  "	g = 1.03\n"
  "	a0 = 1.589				(/ms)\n"
  "	za = 0.64 				\n"
  "	b0 = 0.0184		(/ms)\n"
  "	zb = -1.31		\n"
  "	c0 = 6.668			(/ms)\n"
  "	zc = 0.15\n"
  "	d0 = 2.381			(/ms)\n"
  "	zd = -1.21\n"
  "	e0 = 0.503			(/ms)\n"
  "	ze = 0.07		\n"
  "	f0 = 0.174			(/ms)\n"
  "	zf = -0.25	\n"
  "	kci = 0.047			(/ms)\n"
  "	kic = 0.00003			(/ms)\n"
  "	kappa1 = 0.229				(/ms)\n"
  "	lambda1 = 0.151			(/ms)\n"
  "	kappa2 = 0.0487			(/ms)\n"
  "	lambda2 = 0.0065		(/ms)\n"
  "	vshift = 0 					(mV)\n"
  "}\n"
  "\n"
  "STATE {\n"
  "        O C0 C1 C2 C3 C4 I0 I1 I2 I3 I4  IO1 IO2 I5 C5\n"
  "}\n"
  "\n"
  "ASSIGNED {\n"
  "        ik                             (mA/cm2)\n"
  "        gk                            (S/cm2)\n"
  "        ek                            (mV)\n"
  "		alpha   					(/ms)\n"
  "		beta						(/ms)\n"
  "		gamma						(/ms)\n"
  "		delta   					(/ms)\n"
  "		epsilon 					(/ms)\n"
  "		phi							(/ms)\n"
  "		celsius (degC)\n"
  "}\n"
  "\n"
  "INITIAL {\n"
  "	rate(v)\n"
  "	SOLVE kin STEADYSTATE sparse\n"
  "}\n"
  "\n"
  "BREAKPOINT {\n"
  "	SOLVE kin METHOD sparse\n"
  "	gk = gkbar * O\n"
  "    ik = gk * ( v - ek )\n"
  "}\n"
  "\n"
  "\n"
  "KINETIC kin {\n"
  ":LOCAL alpha2, alpha3, beta2, beta3\n"
  "rate(v)\n"
  "\n"
  "~ I0 <-> I1 (4*alpha/f,beta*f)\n"
  "~ I1 <-> I2 (3*alpha/f,2*beta*f)\n"
  "~ I2 <-> I3 (2*alpha/f,3*beta*f)\n"
  "~ I3 <-> I4 (alpha/f,4*beta*f)\n"
  "~ I4 <-> I5 (gamma*g,delta/g)\n"
  "\n"
  "~ C0 <-> I0 (kci*f^4,kic/f^4)\n"
  "~ C1 <-> I1 (kci*f^3,kic/f^3)\n"
  "~ C2 <-> I2 (kci*f^2,kic/f^2)\n"
  "~ C3 <-> I3 (kci*f,kic/f)\n"
  "~ C4 <-> I4 (kci,kic)\n"
  "~ C5 <-> I5 (kci*g,kic/g)\n"
  "\n"
  "~ C0 <-> C1 (4*alpha,beta)\n"
  "~ C1 <-> C2 (3*alpha,2*beta)\n"
  "~ C2 <-> C3 (2*alpha,3*beta)\n"
  "~ C3 <-> C4 (alpha,4*beta)\n"
  "~ C4 <-> C5 (gamma,delta)\n"
  "~ C5 <-> O (epsilon,phi)\n"
  "~ O <-> IO1 (kappa1,lambda1)\n"
  "~ IO1 <-> IO2 (kappa2,lambda2)\n"
  "\n"
  "\n"
  "CONSERVE O + C0 + C1 + C2 + C3 + C4  + I0 + I1 + I2 + I3 + I4  + IO1 + IO2  + I5 + C5 = 1 \n"
  "}\n"
  "\n"
  "\n"
  "PROCEDURE rate(v (mV)) { :callable from hoc\n"
  "	alpha = exponential(a0,za,v,vshift)\n"
  "	beta = exponential(b0,zb,v,vshift)\n"
  "	gamma = exponential(c0,zc,v,vshift)\n"
  "	delta = exponential(d0,zd,v,vshift)\n"
  "	epsilon  = exponential(e0,ze,v,vshift)\n"
  "	phi  = exponential(f0,zf,v,vshift)\n"
  "}\n"
  "\n"
  "\n"
  "FUNCTION exponential(A(/ms), z , v (mV), D (mV)) (/ms) {\n"
  "	exponential = A* exp(z*(v-D)*FARADAY/(R*(celsius+273.15)))\n"
  "}\n"
  ;
    hoc_reg_nmodl_filename(mech_type, nmodl_filename);
    hoc_reg_nmodl_text(mech_type, nmodl_file_text);
}
#endif
