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
static constexpr auto number_of_floating_point_variables = 33;
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
 
#define nrn_init _nrn_init__Cav32
#define _nrn_initial _nrn_initial__Cav32
#define nrn_cur _nrn_cur__Cav32
#define _nrn_current _nrn_current__Cav32
#define nrn_jacob _nrn_jacob__Cav32
#define nrn_state _nrn_state__Cav32
#define _net_receive _net_receive__Cav32 
#define kin kin__Cav32 
#define rates rates__Cav32 
 
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
#define kc1c2 _ml->template fpfield<2>(_iml)
#define kc1c2_columnindex 2
#define kc2c1 _ml->template fpfield<3>(_iml)
#define kc2c1_columnindex 3
#define kc2c3 _ml->template fpfield<4>(_iml)
#define kc2c3_columnindex 4
#define kc3c2 _ml->template fpfield<5>(_iml)
#define kc3c2_columnindex 5
#define kc3o _ml->template fpfield<6>(_iml)
#define kc3o_columnindex 6
#define koc3 _ml->template fpfield<7>(_iml)
#define koc3_columnindex 7
#define ki2i1 _ml->template fpfield<8>(_iml)
#define ki2i1_columnindex 8
#define ki3i2 _ml->template fpfield<9>(_iml)
#define ki3i2_columnindex 9
#define c1 _ml->template fpfield<10>(_iml)
#define c1_columnindex 10
#define c2 _ml->template fpfield<11>(_iml)
#define c2_columnindex 11
#define c3 _ml->template fpfield<12>(_iml)
#define c3_columnindex 12
#define i1 _ml->template fpfield<13>(_iml)
#define i1_columnindex 13
#define i2 _ml->template fpfield<14>(_iml)
#define i2_columnindex 14
#define i3 _ml->template fpfield<15>(_iml)
#define i3_columnindex 15
#define io _ml->template fpfield<16>(_iml)
#define io_columnindex 16
#define o _ml->template fpfield<17>(_iml)
#define o_columnindex 17
#define eca _ml->template fpfield<18>(_iml)
#define eca_columnindex 18
#define ica _ml->template fpfield<19>(_iml)
#define ica_columnindex 19
#define ki1i2 _ml->template fpfield<20>(_iml)
#define ki1i2_columnindex 20
#define ki2i3 _ml->template fpfield<21>(_iml)
#define ki2i3_columnindex 21
#define itca _ml->template fpfield<22>(_iml)
#define itca_columnindex 22
#define Dc1 _ml->template fpfield<23>(_iml)
#define Dc1_columnindex 23
#define Dc2 _ml->template fpfield<24>(_iml)
#define Dc2_columnindex 24
#define Dc3 _ml->template fpfield<25>(_iml)
#define Dc3_columnindex 25
#define Di1 _ml->template fpfield<26>(_iml)
#define Di1_columnindex 26
#define Di2 _ml->template fpfield<27>(_iml)
#define Di2_columnindex 27
#define Di3 _ml->template fpfield<28>(_iml)
#define Di3_columnindex 28
#define Dio _ml->template fpfield<29>(_iml)
#define Dio_columnindex 29
#define Do _ml->template fpfield<30>(_iml)
#define Do_columnindex 30
#define v _ml->template fpfield<31>(_iml)
#define v_columnindex 31
#define _g _ml->template fpfield<32>(_iml)
#define _g_columnindex 32
#define _ion_eca *(_ml->dptr_field<0>(_iml))
#define _p_ion_eca static_cast<neuron::container::data_handle<double>>(_ppvar[0])
#define _ion_ica *(_ml->dptr_field<1>(_iml))
#define _p_ion_ica static_cast<neuron::container::data_handle<double>>(_ppvar[1])
#define _ion_dicadv *(_ml->dptr_field<2>(_iml))
#define _ion_itca *(_ml->dptr_field<3>(_iml))
#define _p_ion_itca static_cast<neuron::container::data_handle<double>>(_ppvar[3])
#define _ion_ditcadv *(_ml->dptr_field<4>(_iml))
 /* Thread safe. No static _ml, _iml or _ppvar. */
 static int hoc_nrnpointerindex =  -1;
 static _nrn_mechanism_std_vector<Datum> _extcall_thread;
 static Prop* _extcall_prop;
 /* _prop_id kind of shadows _extcall_prop to allow validity checking. */
 static _nrn_non_owning_id_without_container _prop_id{};
 /* external NEURON variables */
 extern double celsius;
 /* declaration of user functions */
 static void _hoc_alpha(void);
 static void _hoc_beta(void);
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
 {"setdata_Cav32", _hoc_setdata},
 {"alpha_Cav32", _hoc_alpha},
 {"beta_Cav32", _hoc_beta},
 {"rates_Cav32", _hoc_rates},
 {0, 0}
};
 
/* Direct Python call wrappers to density mechanism functions.*/
 static double _npy_alpha(Prop*);
 static double _npy_beta(Prop*);
 static double _npy_rates(Prop*);
 
static NPyDirectMechFunc npy_direct_func_proc[] = {
 {"alpha", _npy_alpha},
 {"beta", _npy_beta},
 {"rates", _npy_rates},
 {0, 0}
};
#define alpha alpha_Cav32
#define beta beta_Cav32
 extern double alpha( _internalthreadargsprotocomma_ double , double , double , double );
 extern double beta( _internalthreadargsprotocomma_ double , double , double , double );
 /* declare global and static user variables */
 #define gind 0
 #define _gth 0
#define kic3 kic3_Cav32
 double kic3 = 8e-05;
#define kci3 kci3_Cav32
 double kci3 = 0.058;
#define kic2 kic2_Cav32
 double kic2 = 0.0007;
#define kci2 kci2_Cav32
 double kci2 = 0.0034;
#define kic kic_Cav32
 double kic = 0.0002;
#define kci kci_Cav32
 double kci = 0.0006;
 /* some parameters have upper and lower limits */
 static HocParmLimits _hoc_parm_limits[] = {
 {0, 0, 0}
};
 static HocParmUnits _hoc_parm_units[] = {
 {"kci_Cav32", "/ms"},
 {"kic_Cav32", "/ms"},
 {"kci2_Cav32", "/ms"},
 {"kic2_Cav32", "/ms"},
 {"kci3_Cav32", "/ms"},
 {"kic3_Cav32", "/ms"},
 {"gbar_Cav32", "S/cm2"},
 {"g_Cav32", "S/cm2"},
 {"kc1c2_Cav32", "/ms"},
 {"kc2c1_Cav32", "/ms"},
 {"kc2c3_Cav32", "/ms"},
 {"kc3c2_Cav32", "/ms"},
 {"kc3o_Cav32", "/ms"},
 {"koc3_Cav32", "/ms"},
 {"ki2i1_Cav32", "/ms"},
 {"ki3i2_Cav32", "/ms"},
 {0, 0}
};
 static double c30 = 0;
 static double c20 = 0;
 static double c10 = 0;
 static double delta_t = 0.01;
 static double io0 = 0;
 static double i30 = 0;
 static double i20 = 0;
 static double i10 = 0;
 static double o0 = 0;
 /* connect global user variables to hoc */
 static DoubScal hoc_scdoub[] = {
 {"kci_Cav32", &kci_Cav32},
 {"kic_Cav32", &kic_Cav32},
 {"kci2_Cav32", &kci2_Cav32},
 {"kic2_Cav32", &kic2_Cav32},
 {"kci3_Cav32", &kci3_Cav32},
 {"kic3_Cav32", &kic3_Cav32},
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
"Cav32",
 "gbar_Cav32",
 0,
 "g_Cav32",
 "kc1c2_Cav32",
 "kc2c1_Cav32",
 "kc2c3_Cav32",
 "kc3c2_Cav32",
 "kc3o_Cav32",
 "koc3_Cav32",
 "ki2i1_Cav32",
 "ki3i2_Cav32",
 0,
 "c1_Cav32",
 "c2_Cav32",
 "c3_Cav32",
 "i1_Cav32",
 "i2_Cav32",
 "i3_Cav32",
 "io_Cav32",
 "o_Cav32",
 0,
 0};
 static Symbol* _ca_sym;
 static Symbol* _tca_sym;
 
 /* Used by NrnProperty */
 static _nrn_mechanism_std_vector<double> _parm_default{
     0, /* gbar */
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
    assert(_nrn_mechanism_get_num_vars(_prop) == 33);
 	/*initialize range parameters*/
 	gbar = _parm_default[0]; /* 0 */
 	 assert(_nrn_mechanism_get_num_vars(_prop) == 33);
 	_nrn_mechanism_access_dparam(_prop) = _ppvar;
 	/*connect ionic variables to this model*/
 prop_ion = need_memb(_ca_sym);
 nrn_promote(prop_ion, 0, 1);
 	_ppvar[0] = _nrn_mechanism_get_param_handle(prop_ion, 0); /* eca */
 	_ppvar[1] = _nrn_mechanism_get_param_handle(prop_ion, 3); /* ica */
 	_ppvar[2] = _nrn_mechanism_get_param_handle(prop_ion, 4); /* _ion_dicadv */
 prop_ion = need_memb(_tca_sym);
 	_ppvar[3] = _nrn_mechanism_get_param_handle(prop_ion, 3); /* itca */
 	_ppvar[4] = _nrn_mechanism_get_param_handle(prop_ion, 4); /* _ion_ditcadv */
 
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

 extern "C" void _Cav32_reg() {
	int _vectorized = 1;
  _initlists();
 	ion_reg("ca", -10000.);
 	ion_reg("tca", 0.0);
 	_ca_sym = hoc_lookup("ca_ion");
 	_tca_sym = hoc_lookup("tca_ion");
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
                                       _nrn_mechanism_field<double>{"gbar"} /* 0 */,
                                       _nrn_mechanism_field<double>{"g"} /* 1 */,
                                       _nrn_mechanism_field<double>{"kc1c2"} /* 2 */,
                                       _nrn_mechanism_field<double>{"kc2c1"} /* 3 */,
                                       _nrn_mechanism_field<double>{"kc2c3"} /* 4 */,
                                       _nrn_mechanism_field<double>{"kc3c2"} /* 5 */,
                                       _nrn_mechanism_field<double>{"kc3o"} /* 6 */,
                                       _nrn_mechanism_field<double>{"koc3"} /* 7 */,
                                       _nrn_mechanism_field<double>{"ki2i1"} /* 8 */,
                                       _nrn_mechanism_field<double>{"ki3i2"} /* 9 */,
                                       _nrn_mechanism_field<double>{"c1"} /* 10 */,
                                       _nrn_mechanism_field<double>{"c2"} /* 11 */,
                                       _nrn_mechanism_field<double>{"c3"} /* 12 */,
                                       _nrn_mechanism_field<double>{"i1"} /* 13 */,
                                       _nrn_mechanism_field<double>{"i2"} /* 14 */,
                                       _nrn_mechanism_field<double>{"i3"} /* 15 */,
                                       _nrn_mechanism_field<double>{"io"} /* 16 */,
                                       _nrn_mechanism_field<double>{"o"} /* 17 */,
                                       _nrn_mechanism_field<double>{"eca"} /* 18 */,
                                       _nrn_mechanism_field<double>{"ica"} /* 19 */,
                                       _nrn_mechanism_field<double>{"ki1i2"} /* 20 */,
                                       _nrn_mechanism_field<double>{"ki2i3"} /* 21 */,
                                       _nrn_mechanism_field<double>{"itca"} /* 22 */,
                                       _nrn_mechanism_field<double>{"Dc1"} /* 23 */,
                                       _nrn_mechanism_field<double>{"Dc2"} /* 24 */,
                                       _nrn_mechanism_field<double>{"Dc3"} /* 25 */,
                                       _nrn_mechanism_field<double>{"Di1"} /* 26 */,
                                       _nrn_mechanism_field<double>{"Di2"} /* 27 */,
                                       _nrn_mechanism_field<double>{"Di3"} /* 28 */,
                                       _nrn_mechanism_field<double>{"Dio"} /* 29 */,
                                       _nrn_mechanism_field<double>{"Do"} /* 30 */,
                                       _nrn_mechanism_field<double>{"v"} /* 31 */,
                                       _nrn_mechanism_field<double>{"_g"} /* 32 */,
                                       _nrn_mechanism_field<double*>{"_ion_eca", "ca_ion"} /* 0 */,
                                       _nrn_mechanism_field<double*>{"_ion_ica", "ca_ion"} /* 1 */,
                                       _nrn_mechanism_field<double*>{"_ion_dicadv", "ca_ion"} /* 2 */,
                                       _nrn_mechanism_field<double*>{"_ion_itca", "tca_ion"} /* 3 */,
                                       _nrn_mechanism_field<double*>{"_ion_ditcadv", "tca_ion"} /* 4 */,
                                       _nrn_mechanism_field<int>{"_cvode_ieq", "cvodeieq"} /* 5 */);
  hoc_register_prop_size(_mechtype, 33, 6);
  hoc_register_dparam_semantics(_mechtype, 0, "ca_ion");
  hoc_register_dparam_semantics(_mechtype, 1, "ca_ion");
  hoc_register_dparam_semantics(_mechtype, 2, "ca_ion");
  hoc_register_dparam_semantics(_mechtype, 3, "tca_ion");
  hoc_register_dparam_semantics(_mechtype, 4, "tca_ion");
  hoc_register_dparam_semantics(_mechtype, 5, "cvodeieq");
 	hoc_register_cvode(_mechtype, _ode_count, _ode_map, _ode_spec, _ode_matsol);
 	hoc_register_tolerance(_mechtype, _hoc_state_tol, &_atollist);
 
    hoc_register_var(hoc_scdoub, hoc_vdoub, hoc_intfunc);
 	ivoc_help("help ?1 Cav32 /home/raven/PycharmProjects/Masters_model/Mod_Files_Beining_2017/Cav32.mod\n");
 hoc_register_limits(_mechtype, _hoc_parm_limits);
 hoc_register_units(_mechtype, _hoc_parm_units);
 }
 static double FARADAY = 0x1.81f0fae775425p+6;
 static double R = 0x1.0a1013e8990bep+3;
static int _reset;
static const char *modelname = "";

static int error;
static int _ninits = 0;
static int _match_recurse=1;
static void _modl_cleanup(){ _match_recurse=1;}
static int rates(_internalthreadargsprotocomma_ double);
 
#define _MATELM1(_row,_col) *(_nrn_thread_getelm(static_cast<SparseObj*>(_so), _row + 1, _col + 1))
 
#define _RHS1(_arg) _rhs[_arg+1]
  
#define _linmat1  1
 static int _spth1 = 1;
 static int _cvspth1 = 0;
 
static int _ode_spec1(_internalthreadargsproto_);
/*static int _ode_matsol1(_internalthreadargsproto_);*/
 static neuron::container::field_index _slist1[8], _dlist1[8]; static double *_temp1;
 static int kin (void* _so, double* _rhs, _internalthreadargsproto_);
 
static int kin (void* _so, double* _rhs, _internalthreadargsproto_)
 {int _reset=0;
 {
   double b_flux, f_flux, _term; int _i;
 {int _i; double _dt1 = 1.0/dt;
for(_i=1;_i<8;_i++){
  	_RHS1(_i) = -_dt1*(_ml->data(_iml, _slist1[_i]) - _ml->data(_iml, _dlist1[_i]));
	_MATELM1(_i, _i) = _dt1;
      
} }
 rates ( _threadargscomma_ v ) ;
   /* ~ c1 <-> c2 ( kc1c2 , kc2c1 )*/
 f_flux =  kc1c2 * c1 ;
 b_flux =  kc2c1 * c2 ;
 _RHS1( 3) -= (f_flux - b_flux);
 _RHS1( 2) += (f_flux - b_flux);
 
 _term =  kc1c2 ;
 _MATELM1( 3 ,3)  += _term;
 _MATELM1( 2 ,3)  -= _term;
 _term =  kc2c1 ;
 _MATELM1( 3 ,2)  -= _term;
 _MATELM1( 2 ,2)  += _term;
 /*REACTION*/
  /* ~ c2 <-> c3 ( kc2c3 , kc3c2 )*/
 f_flux =  kc2c3 * c2 ;
 b_flux =  kc3c2 * c3 ;
 _RHS1( 2) -= (f_flux - b_flux);
 _RHS1( 1) += (f_flux - b_flux);
 
 _term =  kc2c3 ;
 _MATELM1( 2 ,2)  += _term;
 _MATELM1( 1 ,2)  -= _term;
 _term =  kc3c2 ;
 _MATELM1( 2 ,1)  -= _term;
 _MATELM1( 1 ,1)  += _term;
 /*REACTION*/
  /* ~ c3 <-> o ( kc3o , koc3 )*/
 f_flux =  kc3o * c3 ;
 b_flux =  koc3 * o ;
 _RHS1( 1) -= (f_flux - b_flux);
 
 _term =  kc3o ;
 _MATELM1( 1 ,1)  += _term;
 _term =  koc3 ;
 _MATELM1( 1 ,0)  -= _term;
 /*REACTION*/
  /* ~ i1 <-> i2 ( ki1i2 , ki2i1 )*/
 f_flux =  ki1i2 * i1 ;
 b_flux =  ki2i1 * i2 ;
 _RHS1( 7) -= (f_flux - b_flux);
 _RHS1( 6) += (f_flux - b_flux);
 
 _term =  ki1i2 ;
 _MATELM1( 7 ,7)  += _term;
 _MATELM1( 6 ,7)  -= _term;
 _term =  ki2i1 ;
 _MATELM1( 7 ,6)  -= _term;
 _MATELM1( 6 ,6)  += _term;
 /*REACTION*/
  /* ~ i2 <-> i3 ( ki2i3 , ki3i2 )*/
 f_flux =  ki2i3 * i2 ;
 b_flux =  ki3i2 * i3 ;
 _RHS1( 6) -= (f_flux - b_flux);
 _RHS1( 5) += (f_flux - b_flux);
 
 _term =  ki2i3 ;
 _MATELM1( 6 ,6)  += _term;
 _MATELM1( 5 ,6)  -= _term;
 _term =  ki3i2 ;
 _MATELM1( 6 ,5)  -= _term;
 _MATELM1( 5 ,5)  += _term;
 /*REACTION*/
  /* ~ i3 <-> io ( kc3o , koc3 )*/
 f_flux =  kc3o * i3 ;
 b_flux =  koc3 * io ;
 _RHS1( 5) -= (f_flux - b_flux);
 _RHS1( 4) += (f_flux - b_flux);
 
 _term =  kc3o ;
 _MATELM1( 5 ,5)  += _term;
 _MATELM1( 4 ,5)  -= _term;
 _term =  koc3 ;
 _MATELM1( 5 ,4)  -= _term;
 _MATELM1( 4 ,4)  += _term;
 /*REACTION*/
  /* ~ i1 <-> c1 ( kic , kci )*/
 f_flux =  kic * i1 ;
 b_flux =  kci * c1 ;
 _RHS1( 7) -= (f_flux - b_flux);
 _RHS1( 3) += (f_flux - b_flux);
 
 _term =  kic ;
 _MATELM1( 7 ,7)  += _term;
 _MATELM1( 3 ,7)  -= _term;
 _term =  kci ;
 _MATELM1( 7 ,3)  -= _term;
 _MATELM1( 3 ,3)  += _term;
 /*REACTION*/
  /* ~ i2 <-> c2 ( kic2 , kci2 )*/
 f_flux =  kic2 * i2 ;
 b_flux =  kci2 * c2 ;
 _RHS1( 6) -= (f_flux - b_flux);
 _RHS1( 2) += (f_flux - b_flux);
 
 _term =  kic2 ;
 _MATELM1( 6 ,6)  += _term;
 _MATELM1( 2 ,6)  -= _term;
 _term =  kci2 ;
 _MATELM1( 6 ,2)  -= _term;
 _MATELM1( 2 ,2)  += _term;
 /*REACTION*/
  /* ~ i3 <-> c3 ( kic3 , kci3 )*/
 f_flux =  kic3 * i3 ;
 b_flux =  kci3 * c3 ;
 _RHS1( 5) -= (f_flux - b_flux);
 _RHS1( 1) += (f_flux - b_flux);
 
 _term =  kic3 ;
 _MATELM1( 5 ,5)  += _term;
 _MATELM1( 1 ,5)  -= _term;
 _term =  kci3 ;
 _MATELM1( 5 ,1)  -= _term;
 _MATELM1( 1 ,1)  += _term;
 /*REACTION*/
  /* ~ io <-> o ( kic3 , kci3 )*/
 f_flux =  kic3 * io ;
 b_flux =  kci3 * o ;
 _RHS1( 4) -= (f_flux - b_flux);
 
 _term =  kic3 ;
 _MATELM1( 4 ,4)  += _term;
 _term =  kci3 ;
 _MATELM1( 4 ,0)  -= _term;
 /*REACTION*/
   /* c1 + c2 + c3 + i1 + i2 + i3 + io + o = 1.0 */
 _RHS1(0) =  1.0;
 _MATELM1(0, 0) = 1;
 _RHS1(0) -= o ;
 _MATELM1(0, 4) = 1;
 _RHS1(0) -= io ;
 _MATELM1(0, 5) = 1;
 _RHS1(0) -= i3 ;
 _MATELM1(0, 6) = 1;
 _RHS1(0) -= i2 ;
 _MATELM1(0, 7) = 1;
 _RHS1(0) -= i1 ;
 _MATELM1(0, 1) = 1;
 _RHS1(0) -= c3 ;
 _MATELM1(0, 2) = 1;
 _RHS1(0) -= c2 ;
 _MATELM1(0, 3) = 1;
 _RHS1(0) -= c1 ;
 /*CONSERVATION*/
   } return _reset;
 }
 
static int  rates ( _internalthreadargsprotocomma_ double _lv ) {
   kc1c2 = alpha ( _threadargscomma_ 1.6 , 0.72 , 1.82 , _lv ) ;
   kc2c1 = beta ( _threadargscomma_ 0.032 , 0.72 , 1.82 , _lv ) ;
   kc2c3 = alpha ( _threadargscomma_ 41.0 , 0.31 , 4.5 , _lv ) ;
   kc3c2 = beta ( _threadargscomma_ 0.027 , 0.31 , 4.5 , _lv ) ;
   kc3o = 0.42 ;
   koc3 = beta ( _threadargscomma_ 0.015 , 0.0 , 0.7 , _lv ) ;
   ki1i2 = alpha ( _threadargscomma_ 0.032 , 0.72 , 1.82 , _lv ) ;
   ki2i1 = beta ( _threadargscomma_ 0.0004 , 0.72 , 1.82 , _lv ) ;
   ki2i3 = alpha ( _threadargscomma_ 0.25 , 0.31 , 4.5 , _lv ) ;
   ki3i2 = beta ( _threadargscomma_ 0.0000011 , 0.31 , 4.5 , _lv ) ;
    return 0; }
 
static void _hoc_rates(void) {
  double _r;
 Datum* _ppvar; Datum* _thread; NrnThread* _nt;
 
  if(!_prop_id) {
    hoc_execerror("No data for rates_Cav32. Requires prior call to setdata_Cav32 and that the specified mechanism instance still be in existence.", NULL);
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
 
double alpha ( _internalthreadargsprotocomma_ double _lA , double _ld , double _lq , double _lv ) {
   double _lalpha;
 _lalpha = _lA * exp ( _ld * _lq * _lv * FARADAY / ( R * ( celsius + 273.15 ) ) ) ;
   
return _lalpha;
 }
 
static void _hoc_alpha(void) {
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
 _r =  alpha ( _threadargscomma_ *getarg(1) , *getarg(2) , *getarg(3) , *getarg(4) );
 hoc_retpushx(_r);
}
 
static double _npy_alpha(Prop* _prop) {
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
 _r =  alpha ( _threadargscomma_ *getarg(1) , *getarg(2) , *getarg(3) , *getarg(4) );
 return(_r);
}
 
double beta ( _internalthreadargsprotocomma_ double _lA , double _ld , double _lq , double _lv ) {
   double _lbeta;
 _lbeta = _lA * exp ( - ( 1.0 - _ld ) * _lq * _lv * FARADAY / ( R * ( celsius + 273.15 ) ) ) ;
   
return _lbeta;
 }
 
static void _hoc_beta(void) {
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
 _r =  beta ( _threadargscomma_ *getarg(1) , *getarg(2) , *getarg(3) , *getarg(4) );
 hoc_retpushx(_r);
}
 
static double _npy_beta(Prop* _prop) {
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
 _r =  beta ( _threadargscomma_ *getarg(1) , *getarg(2) , *getarg(3) , *getarg(4) );
 return(_r);
}
 
/*CVODE ode begin*/
 static int _ode_spec1(_internalthreadargsproto_) {
  int _reset=0;
  {
 double b_flux, f_flux, _term; int _i;
 {int _i; for(_i=0;_i<8;_i++) _ml->data(_iml, _dlist1[_i]) = 0.0;}
 rates ( _threadargscomma_ v ) ;
 /* ~ c1 <-> c2 ( kc1c2 , kc2c1 )*/
 f_flux =  kc1c2 * c1 ;
 b_flux =  kc2c1 * c2 ;
 Dc1 -= (f_flux - b_flux);
 Dc2 += (f_flux - b_flux);
 
 /*REACTION*/
  /* ~ c2 <-> c3 ( kc2c3 , kc3c2 )*/
 f_flux =  kc2c3 * c2 ;
 b_flux =  kc3c2 * c3 ;
 Dc2 -= (f_flux - b_flux);
 Dc3 += (f_flux - b_flux);
 
 /*REACTION*/
  /* ~ c3 <-> o ( kc3o , koc3 )*/
 f_flux =  kc3o * c3 ;
 b_flux =  koc3 * o ;
 Dc3 -= (f_flux - b_flux);
 Do += (f_flux - b_flux);
 
 /*REACTION*/
  /* ~ i1 <-> i2 ( ki1i2 , ki2i1 )*/
 f_flux =  ki1i2 * i1 ;
 b_flux =  ki2i1 * i2 ;
 Di1 -= (f_flux - b_flux);
 Di2 += (f_flux - b_flux);
 
 /*REACTION*/
  /* ~ i2 <-> i3 ( ki2i3 , ki3i2 )*/
 f_flux =  ki2i3 * i2 ;
 b_flux =  ki3i2 * i3 ;
 Di2 -= (f_flux - b_flux);
 Di3 += (f_flux - b_flux);
 
 /*REACTION*/
  /* ~ i3 <-> io ( kc3o , koc3 )*/
 f_flux =  kc3o * i3 ;
 b_flux =  koc3 * io ;
 Di3 -= (f_flux - b_flux);
 Dio += (f_flux - b_flux);
 
 /*REACTION*/
  /* ~ i1 <-> c1 ( kic , kci )*/
 f_flux =  kic * i1 ;
 b_flux =  kci * c1 ;
 Di1 -= (f_flux - b_flux);
 Dc1 += (f_flux - b_flux);
 
 /*REACTION*/
  /* ~ i2 <-> c2 ( kic2 , kci2 )*/
 f_flux =  kic2 * i2 ;
 b_flux =  kci2 * c2 ;
 Di2 -= (f_flux - b_flux);
 Dc2 += (f_flux - b_flux);
 
 /*REACTION*/
  /* ~ i3 <-> c3 ( kic3 , kci3 )*/
 f_flux =  kic3 * i3 ;
 b_flux =  kci3 * c3 ;
 Di3 -= (f_flux - b_flux);
 Dc3 += (f_flux - b_flux);
 
 /*REACTION*/
  /* ~ io <-> o ( kic3 , kci3 )*/
 f_flux =  kic3 * io ;
 b_flux =  kci3 * o ;
 Dio -= (f_flux - b_flux);
 Do += (f_flux - b_flux);
 
 /*REACTION*/
   /* c1 + c2 + c3 + i1 + i2 + i3 + io + o = 1.0 */
 /*CONSERVATION*/
   } return _reset;
 }
 
/*CVODE matsol*/
 static int _ode_matsol1(void* _so, double* _rhs, _internalthreadargsproto_) {int _reset=0;{
 double b_flux, f_flux, _term; int _i;
   b_flux = f_flux = 0.;
 {int _i; double _dt1 = 1.0/dt;
for(_i=0;_i<8;_i++){
  	_RHS1(_i) = _dt1*(_ml->data(_iml, _dlist1[_i]));
	_MATELM1(_i, _i) = _dt1;
      
} }
 rates ( _threadargscomma_ v ) ;
 /* ~ c1 <-> c2 ( kc1c2 , kc2c1 )*/
 _term =  kc1c2 ;
 _MATELM1( 3 ,3)  += _term;
 _MATELM1( 2 ,3)  -= _term;
 _term =  kc2c1 ;
 _MATELM1( 3 ,2)  -= _term;
 _MATELM1( 2 ,2)  += _term;
 /*REACTION*/
  /* ~ c2 <-> c3 ( kc2c3 , kc3c2 )*/
 _term =  kc2c3 ;
 _MATELM1( 2 ,2)  += _term;
 _MATELM1( 1 ,2)  -= _term;
 _term =  kc3c2 ;
 _MATELM1( 2 ,1)  -= _term;
 _MATELM1( 1 ,1)  += _term;
 /*REACTION*/
  /* ~ c3 <-> o ( kc3o , koc3 )*/
 _term =  kc3o ;
 _MATELM1( 1 ,1)  += _term;
 _MATELM1( 0 ,1)  -= _term;
 _term =  koc3 ;
 _MATELM1( 1 ,0)  -= _term;
 _MATELM1( 0 ,0)  += _term;
 /*REACTION*/
  /* ~ i1 <-> i2 ( ki1i2 , ki2i1 )*/
 _term =  ki1i2 ;
 _MATELM1( 7 ,7)  += _term;
 _MATELM1( 6 ,7)  -= _term;
 _term =  ki2i1 ;
 _MATELM1( 7 ,6)  -= _term;
 _MATELM1( 6 ,6)  += _term;
 /*REACTION*/
  /* ~ i2 <-> i3 ( ki2i3 , ki3i2 )*/
 _term =  ki2i3 ;
 _MATELM1( 6 ,6)  += _term;
 _MATELM1( 5 ,6)  -= _term;
 _term =  ki3i2 ;
 _MATELM1( 6 ,5)  -= _term;
 _MATELM1( 5 ,5)  += _term;
 /*REACTION*/
  /* ~ i3 <-> io ( kc3o , koc3 )*/
 _term =  kc3o ;
 _MATELM1( 5 ,5)  += _term;
 _MATELM1( 4 ,5)  -= _term;
 _term =  koc3 ;
 _MATELM1( 5 ,4)  -= _term;
 _MATELM1( 4 ,4)  += _term;
 /*REACTION*/
  /* ~ i1 <-> c1 ( kic , kci )*/
 _term =  kic ;
 _MATELM1( 7 ,7)  += _term;
 _MATELM1( 3 ,7)  -= _term;
 _term =  kci ;
 _MATELM1( 7 ,3)  -= _term;
 _MATELM1( 3 ,3)  += _term;
 /*REACTION*/
  /* ~ i2 <-> c2 ( kic2 , kci2 )*/
 _term =  kic2 ;
 _MATELM1( 6 ,6)  += _term;
 _MATELM1( 2 ,6)  -= _term;
 _term =  kci2 ;
 _MATELM1( 6 ,2)  -= _term;
 _MATELM1( 2 ,2)  += _term;
 /*REACTION*/
  /* ~ i3 <-> c3 ( kic3 , kci3 )*/
 _term =  kic3 ;
 _MATELM1( 5 ,5)  += _term;
 _MATELM1( 1 ,5)  -= _term;
 _term =  kci3 ;
 _MATELM1( 5 ,1)  -= _term;
 _MATELM1( 1 ,1)  += _term;
 /*REACTION*/
  /* ~ io <-> o ( kic3 , kci3 )*/
 _term =  kic3 ;
 _MATELM1( 4 ,4)  += _term;
 _MATELM1( 0 ,4)  -= _term;
 _term =  kci3 ;
 _MATELM1( 4 ,0)  -= _term;
 _MATELM1( 0 ,0)  += _term;
 /*REACTION*/
   /* c1 + c2 + c3 + i1 + i2 + i3 + io + o = 1.0 */
 /*CONSERVATION*/
   } return _reset;
 }
 
/*CVODE end*/
 
static int _ode_count(int _type){ return 8;}
 
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
  eca = _ion_eca;
     _ode_spec1 (_threadargs_);
   }}
 
static void _ode_map(Prop* _prop, int _ieq, neuron::container::data_handle<double>* _pv, neuron::container::data_handle<double>* _pvdot, double* _atol, int _type) { 
  Datum* _ppvar;
  _ppvar = _nrn_mechanism_access_dparam(_prop);
  _cvode_ieq = _ieq;
  for (int _i=0; _i < 8; ++_i) {
    _pv[_i] = _nrn_mechanism_get_param_handle(_prop, _slist1[_i]);
    _pvdot[_i] = _nrn_mechanism_get_param_handle(_prop, _dlist1[_i]);
    _cvode_abstol(_atollist, _atol, _i);
  }
 }
 
static void _ode_matsol_instance1(_internalthreadargsproto_) {
 _cvode_sparse_thread(&(_thread[_cvspth1].literal_value<void*>()), 8, _dlist1, neuron::scopmath::row_view{_ml, _iml}, _ode_matsol1, _threadargs_);
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
  eca = _ion_eca;
 _ode_matsol_instance1(_threadargs_);
 }}
 
static void _thread_cleanup(Datum* _thread) {
   _nrn_destroy_sparseobj_thread(static_cast<SparseObj*>(_thread[_cvspth1].get<void*>()));
   _nrn_destroy_sparseobj_thread(static_cast<SparseObj*>(_thread[_spth1].get<void*>()));
 }

static void initmodel(_internalthreadargsproto_) {
  int _i; double _save;{
  c3 = c30;
  c2 = c20;
  c1 = c10;
  io = io0;
  i3 = i30;
  i2 = i20;
  i1 = i10;
  o = o0;
 {
    _ss_sparse_thread(&(_thread[_spth1].literal_value<void*>()), 8, _slist1, _dlist1, neuron::scopmath::row_view{_ml, _iml}, &t, dt, kin, _linmat1, _threadargs_);
     if (secondorder) {
    int _i;
    for (_i = 0; _i < 8; ++_i) {
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
  eca = _ion_eca;
 initmodel(_threadargs_);
  }
}

static double _nrn_current(_internalthreadargsprotocomma_ double _v) {
double _current=0.; v=_v;
{ {
   g = gbar * o ;
   ica = g * ( v - eca ) ;
   itca = ica ;
   }
 _current += ica;
 _current += itca;

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
  eca = _ion_eca;
 auto const _g_local = _nrn_current(_threadargscomma_ _v + .001);
 	{ double _ditca;
 double _dica;
  _dica = ica;
  _ditca = itca;
 _rhs = _nrn_current(_threadargscomma_ _v);
  _ion_dicadv += (_dica - ica)/.001 ;
  _ion_ditcadv += (_ditca - itca)/.001 ;
 	}
 _g = (_g_local - _rhs)/.001;
  _ion_ica += ica ;
  _ion_itca += itca ;
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
  eca = _ion_eca;
 {  sparse_thread(&(_thread[_spth1].literal_value<void*>()), 8, _slist1, _dlist1, neuron::scopmath::row_view{_ml, _iml}, &t, dt, kin, _linmat1, _threadargs_);
     if (secondorder) {
    int _i;
    for (_i = 0; _i < 8; ++_i) {
      _ml->data(_iml, _slist1[_i]) += dt*_ml->data(_iml, _dlist1[_i]);
    }}
 }  }}
 dt = _dtsav;
}

static void terminal(){}

static void _initlists(){
 int _i; static int _first = 1;
  if (!_first) return;
 _slist1[0] = {o_columnindex, 0};  _dlist1[0] = {Do_columnindex, 0};
 _slist1[1] = {c3_columnindex, 0};  _dlist1[1] = {Dc3_columnindex, 0};
 _slist1[2] = {c2_columnindex, 0};  _dlist1[2] = {Dc2_columnindex, 0};
 _slist1[3] = {c1_columnindex, 0};  _dlist1[3] = {Dc1_columnindex, 0};
 _slist1[4] = {io_columnindex, 0};  _dlist1[4] = {Dio_columnindex, 0};
 _slist1[5] = {i3_columnindex, 0};  _dlist1[5] = {Di3_columnindex, 0};
 _slist1[6] = {i2_columnindex, 0};  _dlist1[6] = {Di2_columnindex, 0};
 _slist1[7] = {i1_columnindex, 0};  _dlist1[7] = {Di1_columnindex, 0};
_first = 0;
}

#if NMODL_TEXT
static void register_nmodl_text_and_filename(int mech_type) {
    const char* nmodl_filename = "/home/raven/PycharmProjects/Masters_model/Mod_Files_Beining_2017/Cav32.mod";
    const char* nmodl_file_text = 
  ":Model from Burgess et al., 2002\n"
  "\n"
  "NEURON {\n"
  "    SUFFIX Cav32\n"
  "    USEION ca READ eca WRITE ica\n"
  "	USEION tca WRITE itca VALENCE 0\n"
  "    RANGE g, gbar\n"
  "	RANGE     kc1c2, kc2c1 ,  kc2c3  ,  kc3c2,  kc3o, koc3 ,k1i2,ki2i1,ki3i2\n"
  "}\n"
  "\n"
  "UNITS { \n"
  "	(mV) = (millivolt) \n"
  "	(S) = (siemens)\n"
  "	FARADAY = (faraday) (kilocoulombs)\n"
  "	R = (k-mole) (joule/degC)\n"
  "}\n"
  "\n"
  ": initialize parameters\n"
  "\n"
  "PARAMETER {\n"
  "    gbar = 0     (S/cm2)\n"
  "\n"
  "    kci = 0.0006  (/ms)\n"
  "	kic = 0.0002  (/ms)\n"
  "	\n"
  "    kci2 = 0.0034  (/ms)\n"
  "    kic2 = 0.0007  (/ms)\n"
  "	\n"
  "	kci3 = 0.058  (/ms)\n"
  "	kic3 = 0.00008  (/ms)\n"
  "    \n"
  "}\n"
  "\n"
  "ASSIGNED {\n"
  "    v    (mV)\n"
  "    eca  (mV)\n"
  "    g    (S/cm2)\n"
  "    ica  (milliamp/cm2)\n"
  "    kc1c2   (/ms)\n"
  "    kc2c1   (/ms)\n"
  "    kc2c3   (/ms)\n"
  "    kc3c2   (/ms)\n"
  "    kc3o   (/ms)\n"
  "    koc3   (/ms)\n"
  "	ki1i2    (/ms)\n"
  "	ki2i1   (/ms)\n"
  "	ki2i3    (/ms)\n"
  "	ki3i2   (/ms)\n"
  "	celsius (degC)\n"
  "	itca (milliamp/cm2)\n"
  "}\n"
  "\n"
  "STATE { c1 c2 c3 i1 i2 i3 io o }\n"
  "\n"
  "BREAKPOINT {\n"
  "    SOLVE kin METHOD sparse\n"
  "    g = gbar*o\n"
  "    ica = g*(v - eca)\n"
  "	itca = ica\n"
  "}\n"
  "\n"
  "INITIAL { SOLVE kin STEADYSTATE sparse }\n"
  "\n"
  "KINETIC kin {\n"
  "    rates(v)\n"
  "    ~ c1 <-> c2 (kc1c2, kc2c1)\n"
  "    ~ c2 <-> c3 (kc2c3, kc3c2)\n"
  "    ~ c3 <-> o (kc3o, koc3)\n"
  "    ~ i1 <-> i2 (ki1i2, ki2i1)\n"
  "    ~ i2 <-> i3 (ki2i3, ki3i2)\n"
  "    ~ i3 <-> io (kc3o, koc3)\n"
  "    ~ i1 <-> c1 (kic, kci)\n"
  "    ~ i2 <-> c2 (kic2, kci2)\n"
  "    ~ i3 <-> c3 (kic3, kci3)\n"
  "    ~ io <-> o  (kic3, kci3)\n"
  "    CONSERVE c1 + c2 + c3 + i1 + i2 + i3 + io + o = 1\n"
  "}\n"
  "\n"
  "PROCEDURE rates(v(millivolt)) {\n"
  "\n"
  "    kc1c2 = alpha(1.6,0.72,1.82,v)\n"
  "    kc2c1 = beta(0.032,0.72,1.82,v)\n"
  "    \n"
  "    kc2c3 = alpha(41,0.31,4.5,v)\n"
  "    kc3c2 = beta(0.027,0.31,4.5,v)\n"
  "    \n"
  "    kc3o = 0.42\n"
  "    koc3 = beta(0.015,0,0.7,v)  \n"
  "    \n"
  "	ki1i2 = alpha(0.032,0.72,1.82,v)\n"
  "	ki2i1 = beta(0.0004,0.72,1.82,v)\n"
  "	\n"
  "	ki2i3 = alpha(0.25,0.31,4.5,v)\n"
  "	ki3i2 = beta(0.0000011,0.31,4.5,v)\n"
  "}\n"
  "\n"
  "FUNCTION alpha(A (/ms), d, q ,  v (mV)) (/ms) {\n"
  "	\n"
  "	alpha = A*exp(d*q*v*FARADAY/(R*(celsius+273.15)))\n"
  "}\n"
  "\n"
  "FUNCTION beta(A (/ms), d, q ,  v (mV)) (/ms) {\n"
  "	beta = A*exp(-(1-d)*q*v*FARADAY/(R*(celsius+273.15)))\n"
  "}\n"
  ;
    hoc_reg_nmodl_filename(mech_type, nmodl_filename);
    hoc_reg_nmodl_text(mech_type, nmodl_file_text);
}
#endif
