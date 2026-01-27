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
static constexpr auto number_of_floating_point_variables = 36;
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
 
#define nrn_init _nrn_init__Kv42
#define _nrn_initial _nrn_initial__Kv42
#define nrn_cur _nrn_cur__Kv42
#define _nrn_current _nrn_current__Kv42
#define nrn_jacob _nrn_jacob__Kv42
#define nrn_state _nrn_state__Kv42
#define _net_receive _net_receive__Kv42 
#define kin kin__Kv42 
#define rate rate__Kv42 
 
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
#define DO _ml->template fpfield<16>(_iml)
#define DO_columnindex 16
#define DC0 _ml->template fpfield<17>(_iml)
#define DC0_columnindex 17
#define DC1 _ml->template fpfield<18>(_iml)
#define DC1_columnindex 18
#define DC2 _ml->template fpfield<19>(_iml)
#define DC2_columnindex 19
#define DC3 _ml->template fpfield<20>(_iml)
#define DC3_columnindex 20
#define DC4 _ml->template fpfield<21>(_iml)
#define DC4_columnindex 21
#define DI0 _ml->template fpfield<22>(_iml)
#define DI0_columnindex 22
#define DI1 _ml->template fpfield<23>(_iml)
#define DI1_columnindex 23
#define DI2 _ml->template fpfield<24>(_iml)
#define DI2_columnindex 24
#define DI3 _ml->template fpfield<25>(_iml)
#define DI3_columnindex 25
#define DI4 _ml->template fpfield<26>(_iml)
#define DI4_columnindex 26
#define DIO1 _ml->template fpfield<27>(_iml)
#define DIO1_columnindex 27
#define DIO2 _ml->template fpfield<28>(_iml)
#define DIO2_columnindex 28
#define ek _ml->template fpfield<29>(_iml)
#define ek_columnindex 29
#define alpha _ml->template fpfield<30>(_iml)
#define alpha_columnindex 30
#define beta _ml->template fpfield<31>(_iml)
#define beta_columnindex 31
#define kco _ml->template fpfield<32>(_iml)
#define kco_columnindex 32
#define koc _ml->template fpfield<33>(_iml)
#define koc_columnindex 33
#define v _ml->template fpfield<34>(_iml)
#define v_columnindex 34
#define _g _ml->template fpfield<35>(_iml)
#define _g_columnindex 35
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
 {"setdata_Kv42", _hoc_setdata},
 {"exponential_Kv42", _hoc_exponential},
 {"rate_Kv42", _hoc_rate},
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
#define exponential exponential_Kv42
 extern double exponential( _internalthreadargsprotocomma_ double , double , double , double );
 /* declare global and static user variables */
 #define gind 0
 #define _gth 0
#define a0 a0_Kv42
 double a0 = 0.175;
#define b0 b0_Kv42
 double b0 = 0.003598;
#define f f_Kv42
 double f = 0.3;
#define kio2 kio2_Kv42
 double kio2 = 0.01152;
#define koi2 koi2_Kv42
 double koi2 = 0.0308;
#define kio kio_Kv42
 double kio = 0.03686;
#define koi koi_Kv42
 double koi = 0.194;
#define kic kic_Kv42
 double kic = 3.7e-05;
#define kci kci_Kv42
 double kci = 0.02392;
#define koc0 koc0_Kv42
 double koc0 = 1.267;
#define kco0 kco0_Kv42
 double kco0 = 0.347;
#define vshift vshift_Kv42
 double vshift = 0;
#define zoc zoc_Kv42
 double zoc = -0.047;
#define zco zco_Kv42
 double zco = 0.185;
#define zb zb_Kv42
 double zb = -1.742;
#define za za_Kv42
 double za = 2.7;
 /* some parameters have upper and lower limits */
 static HocParmLimits _hoc_parm_limits[] = {
 {0, 0, 0}
};
 static HocParmUnits _hoc_parm_units[] = {
 {"a0_Kv42", "/ms"},
 {"b0_Kv42", "/ms"},
 {"kco0_Kv42", "/ms"},
 {"koc0_Kv42", "/ms"},
 {"kci_Kv42", "/ms"},
 {"kic_Kv42", "/ms"},
 {"koi_Kv42", "/ms"},
 {"kio_Kv42", "/ms"},
 {"koi2_Kv42", "/ms"},
 {"kio2_Kv42", "/ms"},
 {"vshift_Kv42", "mV"},
 {"gkbar_Kv42", "S/cm2"},
 {"ik_Kv42", "mA/cm2"},
 {"gk_Kv42", "S/cm2"},
 {0, 0}
};
 static double C40 = 0;
 static double C30 = 0;
 static double C20 = 0;
 static double C10 = 0;
 static double C00 = 0;
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
 {"f_Kv42", &f_Kv42},
 {"a0_Kv42", &a0_Kv42},
 {"za_Kv42", &za_Kv42},
 {"b0_Kv42", &b0_Kv42},
 {"zb_Kv42", &zb_Kv42},
 {"kco0_Kv42", &kco0_Kv42},
 {"zco_Kv42", &zco_Kv42},
 {"koc0_Kv42", &koc0_Kv42},
 {"zoc_Kv42", &zoc_Kv42},
 {"kci_Kv42", &kci_Kv42},
 {"kic_Kv42", &kic_Kv42},
 {"koi_Kv42", &koi_Kv42},
 {"kio_Kv42", &kio_Kv42},
 {"koi2_Kv42", &koi2_Kv42},
 {"kio2_Kv42", &kio2_Kv42},
 {"vshift_Kv42", &vshift_Kv42},
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
"Kv42",
 "gkbar_Kv42",
 0,
 "ik_Kv42",
 "gk_Kv42",
 0,
 "O_Kv42",
 "C0_Kv42",
 "C1_Kv42",
 "C2_Kv42",
 "C3_Kv42",
 "C4_Kv42",
 "I0_Kv42",
 "I1_Kv42",
 "I2_Kv42",
 "I3_Kv42",
 "I4_Kv42",
 "IO1_Kv42",
 "IO2_Kv42",
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
    assert(_nrn_mechanism_get_num_vars(_prop) == 36);
 	/*initialize range parameters*/
 	gkbar = _parm_default[0]; /* 0.00015 */
 	 assert(_nrn_mechanism_get_num_vars(_prop) == 36);
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

 extern "C" void _Kv42_reg() {
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
                                       _nrn_mechanism_field<double>{"DO"} /* 16 */,
                                       _nrn_mechanism_field<double>{"DC0"} /* 17 */,
                                       _nrn_mechanism_field<double>{"DC1"} /* 18 */,
                                       _nrn_mechanism_field<double>{"DC2"} /* 19 */,
                                       _nrn_mechanism_field<double>{"DC3"} /* 20 */,
                                       _nrn_mechanism_field<double>{"DC4"} /* 21 */,
                                       _nrn_mechanism_field<double>{"DI0"} /* 22 */,
                                       _nrn_mechanism_field<double>{"DI1"} /* 23 */,
                                       _nrn_mechanism_field<double>{"DI2"} /* 24 */,
                                       _nrn_mechanism_field<double>{"DI3"} /* 25 */,
                                       _nrn_mechanism_field<double>{"DI4"} /* 26 */,
                                       _nrn_mechanism_field<double>{"DIO1"} /* 27 */,
                                       _nrn_mechanism_field<double>{"DIO2"} /* 28 */,
                                       _nrn_mechanism_field<double>{"ek"} /* 29 */,
                                       _nrn_mechanism_field<double>{"alpha"} /* 30 */,
                                       _nrn_mechanism_field<double>{"beta"} /* 31 */,
                                       _nrn_mechanism_field<double>{"kco"} /* 32 */,
                                       _nrn_mechanism_field<double>{"koc"} /* 33 */,
                                       _nrn_mechanism_field<double>{"v"} /* 34 */,
                                       _nrn_mechanism_field<double>{"_g"} /* 35 */,
                                       _nrn_mechanism_field<double*>{"_ion_ek", "k_ion"} /* 0 */,
                                       _nrn_mechanism_field<double*>{"_ion_ik", "k_ion"} /* 1 */,
                                       _nrn_mechanism_field<double*>{"_ion_dikdv", "k_ion"} /* 2 */,
                                       _nrn_mechanism_field<int>{"_cvode_ieq", "cvodeieq"} /* 3 */);
  hoc_register_prop_size(_mechtype, 36, 4);
  hoc_register_dparam_semantics(_mechtype, 0, "k_ion");
  hoc_register_dparam_semantics(_mechtype, 1, "k_ion");
  hoc_register_dparam_semantics(_mechtype, 2, "k_ion");
  hoc_register_dparam_semantics(_mechtype, 3, "cvodeieq");
 	hoc_register_cvode(_mechtype, _ode_count, _ode_map, _ode_spec, _ode_matsol);
 	hoc_register_tolerance(_mechtype, _hoc_state_tol, &_atollist);
 
    hoc_register_var(hoc_scdoub, hoc_vdoub, hoc_intfunc);
 	ivoc_help("help ?1 Kv42 /home/raven/PycharmProjects/Masters_model/Mod_Files_Beining_2017/Kv42.mod\n");
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
 static neuron::container::field_index _slist1[13], _dlist1[13]; static double *_temp1;
 static int kin (void* _so, double* _rhs, _internalthreadargsproto_);
 
static int kin (void* _so, double* _rhs, _internalthreadargsproto_)
 {int _reset=0;
 {
   double b_flux, f_flux, _term; int _i;
 {int _i; double _dt1 = 1.0/dt;
for(_i=1;_i<13;_i++){
  	_RHS1(_i) = -_dt1*(_ml->data(_iml, _slist1[_i]) - _ml->data(_iml, _dlist1[_i]));
	_MATELM1(_i, _i) = _dt1;
      
} }
 rate ( _threadargscomma_ v ) ;
   /* ~ I0 <-> I1 ( 4.0 * alpha / f , beta * f )*/
 f_flux =  4.0 * alpha / f * I0 ;
 b_flux =  beta * f * I1 ;
 _RHS1( 11) -= (f_flux - b_flux);
 _RHS1( 10) += (f_flux - b_flux);
 
 _term =  4.0 * alpha / f ;
 _MATELM1( 11 ,11)  += _term;
 _MATELM1( 10 ,11)  -= _term;
 _term =  beta * f ;
 _MATELM1( 11 ,10)  -= _term;
 _MATELM1( 10 ,10)  += _term;
 /*REACTION*/
  /* ~ I1 <-> I2 ( 3.0 * alpha / f , 2.0 * beta * f )*/
 f_flux =  3.0 * alpha / f * I1 ;
 b_flux =  2.0 * beta * f * I2 ;
 _RHS1( 10) -= (f_flux - b_flux);
 _RHS1( 9) += (f_flux - b_flux);
 
 _term =  3.0 * alpha / f ;
 _MATELM1( 10 ,10)  += _term;
 _MATELM1( 9 ,10)  -= _term;
 _term =  2.0 * beta * f ;
 _MATELM1( 10 ,9)  -= _term;
 _MATELM1( 9 ,9)  += _term;
 /*REACTION*/
  /* ~ I2 <-> I3 ( 2.0 * alpha / f , 3.0 * beta * f )*/
 f_flux =  2.0 * alpha / f * I2 ;
 b_flux =  3.0 * beta * f * I3 ;
 _RHS1( 9) -= (f_flux - b_flux);
 _RHS1( 8) += (f_flux - b_flux);
 
 _term =  2.0 * alpha / f ;
 _MATELM1( 9 ,9)  += _term;
 _MATELM1( 8 ,9)  -= _term;
 _term =  3.0 * beta * f ;
 _MATELM1( 9 ,8)  -= _term;
 _MATELM1( 8 ,8)  += _term;
 /*REACTION*/
  /* ~ I3 <-> I4 ( alpha / f , 4.0 * beta * f )*/
 f_flux =  alpha / f * I3 ;
 b_flux =  4.0 * beta * f * I4 ;
 _RHS1( 8) -= (f_flux - b_flux);
 _RHS1( 7) += (f_flux - b_flux);
 
 _term =  alpha / f ;
 _MATELM1( 8 ,8)  += _term;
 _MATELM1( 7 ,8)  -= _term;
 _term =  4.0 * beta * f ;
 _MATELM1( 8 ,7)  -= _term;
 _MATELM1( 7 ,7)  += _term;
 /*REACTION*/
  /* ~ C0 <-> I0 ( kci * pow( f , 4.0 ) , kic / pow( f , 4.0 ) )*/
 f_flux =  kci * pow( f , 4.0 ) * C0 ;
 b_flux =  kic / pow( f , 4.0 ) * I0 ;
 _RHS1( 5) -= (f_flux - b_flux);
 _RHS1( 11) += (f_flux - b_flux);
 
 _term =  kci * pow( f , 4.0 ) ;
 _MATELM1( 5 ,5)  += _term;
 _MATELM1( 11 ,5)  -= _term;
 _term =  kic / pow( f , 4.0 ) ;
 _MATELM1( 5 ,11)  -= _term;
 _MATELM1( 11 ,11)  += _term;
 /*REACTION*/
  /* ~ C1 <-> I1 ( kci * pow( f , 3.0 ) , kic / pow( f , 3.0 ) )*/
 f_flux =  kci * pow( f , 3.0 ) * C1 ;
 b_flux =  kic / pow( f , 3.0 ) * I1 ;
 _RHS1( 4) -= (f_flux - b_flux);
 _RHS1( 10) += (f_flux - b_flux);
 
 _term =  kci * pow( f , 3.0 ) ;
 _MATELM1( 4 ,4)  += _term;
 _MATELM1( 10 ,4)  -= _term;
 _term =  kic / pow( f , 3.0 ) ;
 _MATELM1( 4 ,10)  -= _term;
 _MATELM1( 10 ,10)  += _term;
 /*REACTION*/
  /* ~ C2 <-> I2 ( kci * pow( f , 2.0 ) , kic / pow( f , 2.0 ) )*/
 f_flux =  kci * pow( f , 2.0 ) * C2 ;
 b_flux =  kic / pow( f , 2.0 ) * I2 ;
 _RHS1( 3) -= (f_flux - b_flux);
 _RHS1( 9) += (f_flux - b_flux);
 
 _term =  kci * pow( f , 2.0 ) ;
 _MATELM1( 3 ,3)  += _term;
 _MATELM1( 9 ,3)  -= _term;
 _term =  kic / pow( f , 2.0 ) ;
 _MATELM1( 3 ,9)  -= _term;
 _MATELM1( 9 ,9)  += _term;
 /*REACTION*/
  /* ~ C3 <-> I3 ( kci * f , kic / f )*/
 f_flux =  kci * f * C3 ;
 b_flux =  kic / f * I3 ;
 _RHS1( 2) -= (f_flux - b_flux);
 _RHS1( 8) += (f_flux - b_flux);
 
 _term =  kci * f ;
 _MATELM1( 2 ,2)  += _term;
 _MATELM1( 8 ,2)  -= _term;
 _term =  kic / f ;
 _MATELM1( 2 ,8)  -= _term;
 _MATELM1( 8 ,8)  += _term;
 /*REACTION*/
  /* ~ C4 <-> I4 ( kci , kic )*/
 f_flux =  kci * C4 ;
 b_flux =  kic * I4 ;
 _RHS1( 1) -= (f_flux - b_flux);
 _RHS1( 7) += (f_flux - b_flux);
 
 _term =  kci ;
 _MATELM1( 1 ,1)  += _term;
 _MATELM1( 7 ,1)  -= _term;
 _term =  kic ;
 _MATELM1( 1 ,7)  -= _term;
 _MATELM1( 7 ,7)  += _term;
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
  /* ~ O <-> IO1 ( koi , kio )*/
 f_flux =  koi * O ;
 b_flux =  kio * IO1 ;
 _RHS1( 12) -= (f_flux - b_flux);
 _RHS1( 6) += (f_flux - b_flux);
 
 _term =  koi ;
 _MATELM1( 12 ,12)  += _term;
 _MATELM1( 6 ,12)  -= _term;
 _term =  kio ;
 _MATELM1( 12 ,6)  -= _term;
 _MATELM1( 6 ,6)  += _term;
 /*REACTION*/
  /* ~ IO1 <-> IO2 ( koi2 , kio2 )*/
 f_flux =  koi2 * IO1 ;
 b_flux =  kio2 * IO2 ;
 _RHS1( 6) -= (f_flux - b_flux);
 
 _term =  koi2 ;
 _MATELM1( 6 ,6)  += _term;
 _term =  kio2 ;
 _MATELM1( 6 ,0)  -= _term;
 /*REACTION*/
  /* ~ C4 <-> O ( kco , koc )*/
 f_flux =  kco * C4 ;
 b_flux =  koc * O ;
 _RHS1( 1) -= (f_flux - b_flux);
 _RHS1( 12) += (f_flux - b_flux);
 
 _term =  kco ;
 _MATELM1( 1 ,1)  += _term;
 _MATELM1( 12 ,1)  -= _term;
 _term =  koc ;
 _MATELM1( 1 ,12)  -= _term;
 _MATELM1( 12 ,12)  += _term;
 /*REACTION*/
   /* O + C0 + C1 + C2 + C3 + C4 + I0 + I1 + I2 + I3 + I4 + IO1 + IO2 = 1.0 */
 _RHS1(0) =  1.0;
 _MATELM1(0, 0) = 1;
 _RHS1(0) -= IO2 ;
 _MATELM1(0, 6) = 1;
 _RHS1(0) -= IO1 ;
 _MATELM1(0, 7) = 1;
 _RHS1(0) -= I4 ;
 _MATELM1(0, 8) = 1;
 _RHS1(0) -= I3 ;
 _MATELM1(0, 9) = 1;
 _RHS1(0) -= I2 ;
 _MATELM1(0, 10) = 1;
 _RHS1(0) -= I1 ;
 _MATELM1(0, 11) = 1;
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
 _MATELM1(0, 12) = 1;
 _RHS1(0) -= O ;
 /*CONSERVATION*/
   } return _reset;
 }
 
static int  rate ( _internalthreadargsprotocomma_ double _lv ) {
   alpha = exponential ( _threadargscomma_ a0 , za , _lv , vshift ) ;
   beta = exponential ( _threadargscomma_ b0 , zb , _lv , vshift ) ;
   kco = exponential ( _threadargscomma_ kco0 , zco , _lv , vshift ) ;
   koc = exponential ( _threadargscomma_ koc0 , zoc , _lv , vshift ) ;
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
 {int _i; for(_i=0;_i<13;_i++) _ml->data(_iml, _dlist1[_i]) = 0.0;}
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
  /* ~ O <-> IO1 ( koi , kio )*/
 f_flux =  koi * O ;
 b_flux =  kio * IO1 ;
 DO -= (f_flux - b_flux);
 DIO1 += (f_flux - b_flux);
 
 /*REACTION*/
  /* ~ IO1 <-> IO2 ( koi2 , kio2 )*/
 f_flux =  koi2 * IO1 ;
 b_flux =  kio2 * IO2 ;
 DIO1 -= (f_flux - b_flux);
 DIO2 += (f_flux - b_flux);
 
 /*REACTION*/
  /* ~ C4 <-> O ( kco , koc )*/
 f_flux =  kco * C4 ;
 b_flux =  koc * O ;
 DC4 -= (f_flux - b_flux);
 DO += (f_flux - b_flux);
 
 /*REACTION*/
   /* O + C0 + C1 + C2 + C3 + C4 + I0 + I1 + I2 + I3 + I4 + IO1 + IO2 = 1.0 */
 /*CONSERVATION*/
   } return _reset;
 }
 
/*CVODE matsol*/
 static int _ode_matsol1(void* _so, double* _rhs, _internalthreadargsproto_) {int _reset=0;{
 double b_flux, f_flux, _term; int _i;
   b_flux = f_flux = 0.;
 {int _i; double _dt1 = 1.0/dt;
for(_i=0;_i<13;_i++){
  	_RHS1(_i) = _dt1*(_ml->data(_iml, _dlist1[_i]));
	_MATELM1(_i, _i) = _dt1;
      
} }
 rate ( _threadargscomma_ v ) ;
 /* ~ I0 <-> I1 ( 4.0 * alpha / f , beta * f )*/
 _term =  4.0 * alpha / f ;
 _MATELM1( 11 ,11)  += _term;
 _MATELM1( 10 ,11)  -= _term;
 _term =  beta * f ;
 _MATELM1( 11 ,10)  -= _term;
 _MATELM1( 10 ,10)  += _term;
 /*REACTION*/
  /* ~ I1 <-> I2 ( 3.0 * alpha / f , 2.0 * beta * f )*/
 _term =  3.0 * alpha / f ;
 _MATELM1( 10 ,10)  += _term;
 _MATELM1( 9 ,10)  -= _term;
 _term =  2.0 * beta * f ;
 _MATELM1( 10 ,9)  -= _term;
 _MATELM1( 9 ,9)  += _term;
 /*REACTION*/
  /* ~ I2 <-> I3 ( 2.0 * alpha / f , 3.0 * beta * f )*/
 _term =  2.0 * alpha / f ;
 _MATELM1( 9 ,9)  += _term;
 _MATELM1( 8 ,9)  -= _term;
 _term =  3.0 * beta * f ;
 _MATELM1( 9 ,8)  -= _term;
 _MATELM1( 8 ,8)  += _term;
 /*REACTION*/
  /* ~ I3 <-> I4 ( alpha / f , 4.0 * beta * f )*/
 _term =  alpha / f ;
 _MATELM1( 8 ,8)  += _term;
 _MATELM1( 7 ,8)  -= _term;
 _term =  4.0 * beta * f ;
 _MATELM1( 8 ,7)  -= _term;
 _MATELM1( 7 ,7)  += _term;
 /*REACTION*/
  /* ~ C0 <-> I0 ( kci * pow( f , 4.0 ) , kic / pow( f , 4.0 ) )*/
 _term =  kci * pow( f , 4.0 ) ;
 _MATELM1( 5 ,5)  += _term;
 _MATELM1( 11 ,5)  -= _term;
 _term =  kic / pow( f , 4.0 ) ;
 _MATELM1( 5 ,11)  -= _term;
 _MATELM1( 11 ,11)  += _term;
 /*REACTION*/
  /* ~ C1 <-> I1 ( kci * pow( f , 3.0 ) , kic / pow( f , 3.0 ) )*/
 _term =  kci * pow( f , 3.0 ) ;
 _MATELM1( 4 ,4)  += _term;
 _MATELM1( 10 ,4)  -= _term;
 _term =  kic / pow( f , 3.0 ) ;
 _MATELM1( 4 ,10)  -= _term;
 _MATELM1( 10 ,10)  += _term;
 /*REACTION*/
  /* ~ C2 <-> I2 ( kci * pow( f , 2.0 ) , kic / pow( f , 2.0 ) )*/
 _term =  kci * pow( f , 2.0 ) ;
 _MATELM1( 3 ,3)  += _term;
 _MATELM1( 9 ,3)  -= _term;
 _term =  kic / pow( f , 2.0 ) ;
 _MATELM1( 3 ,9)  -= _term;
 _MATELM1( 9 ,9)  += _term;
 /*REACTION*/
  /* ~ C3 <-> I3 ( kci * f , kic / f )*/
 _term =  kci * f ;
 _MATELM1( 2 ,2)  += _term;
 _MATELM1( 8 ,2)  -= _term;
 _term =  kic / f ;
 _MATELM1( 2 ,8)  -= _term;
 _MATELM1( 8 ,8)  += _term;
 /*REACTION*/
  /* ~ C4 <-> I4 ( kci , kic )*/
 _term =  kci ;
 _MATELM1( 1 ,1)  += _term;
 _MATELM1( 7 ,1)  -= _term;
 _term =  kic ;
 _MATELM1( 1 ,7)  -= _term;
 _MATELM1( 7 ,7)  += _term;
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
  /* ~ O <-> IO1 ( koi , kio )*/
 _term =  koi ;
 _MATELM1( 12 ,12)  += _term;
 _MATELM1( 6 ,12)  -= _term;
 _term =  kio ;
 _MATELM1( 12 ,6)  -= _term;
 _MATELM1( 6 ,6)  += _term;
 /*REACTION*/
  /* ~ IO1 <-> IO2 ( koi2 , kio2 )*/
 _term =  koi2 ;
 _MATELM1( 6 ,6)  += _term;
 _MATELM1( 0 ,6)  -= _term;
 _term =  kio2 ;
 _MATELM1( 6 ,0)  -= _term;
 _MATELM1( 0 ,0)  += _term;
 /*REACTION*/
  /* ~ C4 <-> O ( kco , koc )*/
 _term =  kco ;
 _MATELM1( 1 ,1)  += _term;
 _MATELM1( 12 ,1)  -= _term;
 _term =  koc ;
 _MATELM1( 1 ,12)  -= _term;
 _MATELM1( 12 ,12)  += _term;
 /*REACTION*/
   /* O + C0 + C1 + C2 + C3 + C4 + I0 + I1 + I2 + I3 + I4 + IO1 + IO2 = 1.0 */
 /*CONSERVATION*/
   } return _reset;
 }
 
/*CVODE end*/
 
static int _ode_count(int _type){ return 13;}
 
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
  for (int _i=0; _i < 13; ++_i) {
    _pv[_i] = _nrn_mechanism_get_param_handle(_prop, _slist1[_i]);
    _pvdot[_i] = _nrn_mechanism_get_param_handle(_prop, _dlist1[_i]);
    _cvode_abstol(_atollist, _atol, _i);
  }
 }
 
static void _ode_matsol_instance1(_internalthreadargsproto_) {
 _cvode_sparse_thread(&(_thread[_cvspth1].literal_value<void*>()), 13, _dlist1, neuron::scopmath::row_view{_ml, _iml}, _ode_matsol1, _threadargs_);
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
  C4 = C40;
  C3 = C30;
  C2 = C20;
  C1 = C10;
  C0 = C00;
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
    _ss_sparse_thread(&(_thread[_spth1].literal_value<void*>()), 13, _slist1, _dlist1, neuron::scopmath::row_view{_ml, _iml}, &t, dt, kin, _linmat1, _threadargs_);
     if (secondorder) {
    int _i;
    for (_i = 0; _i < 13; ++_i) {
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
 {  sparse_thread(&(_thread[_spth1].literal_value<void*>()), 13, _slist1, _dlist1, neuron::scopmath::row_view{_ml, _iml}, &t, dt, kin, _linmat1, _threadargs_);
     if (secondorder) {
    int _i;
    for (_i = 0; _i < 13; ++_i) {
      _ml->data(_iml, _slist1[_i]) += dt*_ml->data(_iml, _dlist1[_i]);
    }}
 } }}
 dt = _dtsav;
}

static void terminal(){}

static void _initlists(){
 int _i; static int _first = 1;
  if (!_first) return;
 _slist1[0] = {IO2_columnindex, 0};  _dlist1[0] = {DIO2_columnindex, 0};
 _slist1[1] = {C4_columnindex, 0};  _dlist1[1] = {DC4_columnindex, 0};
 _slist1[2] = {C3_columnindex, 0};  _dlist1[2] = {DC3_columnindex, 0};
 _slist1[3] = {C2_columnindex, 0};  _dlist1[3] = {DC2_columnindex, 0};
 _slist1[4] = {C1_columnindex, 0};  _dlist1[4] = {DC1_columnindex, 0};
 _slist1[5] = {C0_columnindex, 0};  _dlist1[5] = {DC0_columnindex, 0};
 _slist1[6] = {IO1_columnindex, 0};  _dlist1[6] = {DIO1_columnindex, 0};
 _slist1[7] = {I4_columnindex, 0};  _dlist1[7] = {DI4_columnindex, 0};
 _slist1[8] = {I3_columnindex, 0};  _dlist1[8] = {DI3_columnindex, 0};
 _slist1[9] = {I2_columnindex, 0};  _dlist1[9] = {DI2_columnindex, 0};
 _slist1[10] = {I1_columnindex, 0};  _dlist1[10] = {DI1_columnindex, 0};
 _slist1[11] = {I0_columnindex, 0};  _dlist1[11] = {DI0_columnindex, 0};
 _slist1[12] = {O_columnindex, 0};  _dlist1[12] = {DO_columnindex, 0};
_first = 0;
}

#if NMODL_TEXT
static void register_nmodl_text_and_filename(int mech_type) {
    const char* nmodl_filename = "/home/raven/PycharmProjects/Masters_model/Mod_Files_Beining_2017/Kv42.mod";
    const char* nmodl_file_text = 
  "TITLE Kv4.2 with auxilliary subunits\n"
  "\n"
  "COMMENT\n"
  "This is the model of Barghaan et al. 2008: Role of N-terminal domain and accessory subunits in controlling deactivation-inactivation coupling of Kv4.2 channels.. Biophysical Journal\n"
  ": implemented by M.Beining; Beining et al (2016), \"A novel comprehensive and consistent electrophysiologcal model of dentate granule cells\"\n"
  "\n"
  "ENDCOMMENT\n"
  "\n"
  "NEURON {\n"
  "	SUFFIX Kv42\n"
  "	USEION k READ ek WRITE ik\n"
  "    RANGE  ik, gk, gkbar\n"
  "	GLOBAL 	f,	a0,za,b0,zb,kco0,zco,koc0,zoc,kci,kic,koi,kio,koi2,kio2 , vshift\n"
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
  "	f = 0.3  : The allosteric factor f defines the voltage-dependent coupling between activation and inactivation pathways for closed channels. deactivation\n"
  "	a0 = 0.175				(/ms)\n"
  "	za = 2.7				\n"
  "	b0 = 0.003598		(/ms)\n"
  "	zb = -1.742		\n"
  "	kco0 = 0.347			(/ms)\n"
  "	zco = 0.185\n"
  "	koc0 = 1.267			(/ms)\n"
  "	zoc = -0.047		\n"
  "	kci = 0.02392			(/ms)\n"
  "	kic = 0.000037			(/ms)\n"
  "	koi = 0.194				(/ms)\n"
  "	kio = 0.03686			(/ms)\n"
  "	koi2 = 0.0308			(/ms): is k56\n"
  "	kio2 = 0.01152		(/ms): is k65\n"
  "	vshift = 0						(mV)\n"
  "}\n"
  "\n"
  "STATE {\n"
  "        O C0 C1 C2 C3 C4 I0 I1 I2 I3 I4  IO1 IO2 :I5 C5\n"
  "}\n"
  "\n"
  "ASSIGNED {\n"
  "        ik                             (mA/cm2)\n"
  "        gk                            (S/cm2)\n"
  "        ek                            (mV)\n"
  "		alpha   					(/ms)\n"
  "		beta						(/ms)\n"
  "		kco   					(/ms)\n"
  "		koc						(/ms)\n"
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
  "rate(v)\n"
  "\n"
  "~ I0 <-> I1 (4*alpha/f,beta*f)\n"
  "~ I1 <-> I2 (3*alpha/f,2*beta*f)\n"
  "~ I2 <-> I3 (2*alpha/f,3*beta*f)\n"
  "~ I3 <-> I4 (alpha/f,4*beta*f)\n"
  "\n"
  "~ C0 <-> I0 (kci*f^4,kic/f^4)\n"
  "~ C1 <-> I1 (kci*f^3,kic/f^3)\n"
  "~ C2 <-> I2 (kci*f^2,kic/f^2)\n"
  "~ C3 <-> I3 (kci*f,kic/f)\n"
  "~ C4 <-> I4 (kci,kic)\n"
  "\n"
  "~ C0 <-> C1 (4*alpha,beta)\n"
  "~ C1 <-> C2 (3*alpha,2*beta)\n"
  "~ C2 <-> C3 (2*alpha,3*beta)\n"
  "~ C3 <-> C4 (alpha,4*beta)\n"
  "\n"
  "~ O <-> IO1 (koi,kio) :kappa1,lambda1\n"
  "~ IO1 <-> IO2 (koi2,kio2) :kappa2,lambda2  : is k56 and k65\n"
  "\n"
  "~ C4 <-> O (kco,koc)\n"
  "\n"
  "CONSERVE O + C0 + C1 + C2 + C3 + C4  + I0 + I1 + I2 + I3 + I4  + IO1 + IO2  = 1  :+ I5 + C5\n"
  "}\n"
  "\n"
  "\n"
  "PROCEDURE rate(v (mV)) { :callable from hoc\n"
  "	alpha = exponential(a0,za,v,vshift)\n"
  "	beta = exponential(b0,zb,v,vshift)\n"
  "	kco = exponential(kco0,zco,v,vshift)\n"
  "	koc = exponential(koc0,zoc,v,vshift)\n"
  "}\n"
  "\n"
  "\n"
  "FUNCTION exponential(A(/ms), z , v (mV), D (mV)) (/ms) {\n"
  "	exponential = A* exp(z*(v-D)*FARADAY/(R*(celsius+273.15))) : V / (RT/F) = V * F / RT\n"
  "}\n"
  ;
    hoc_reg_nmodl_filename(mech_type, nmodl_filename);
    hoc_reg_nmodl_text(mech_type, nmodl_file_text);
}
#endif
