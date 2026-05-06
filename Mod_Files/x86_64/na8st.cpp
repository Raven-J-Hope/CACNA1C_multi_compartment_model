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
static constexpr auto number_of_floating_point_variables = 53;
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
 
#define nrn_init _nrn_init__na8st
#define _nrn_initial _nrn_initial__na8st
#define nrn_cur _nrn_cur__na8st
#define _nrn_current _nrn_current__na8st
#define nrn_jacob _nrn_jacob__na8st
#define nrn_state _nrn_state__na8st
#define _net_receive _net_receive__na8st 
#define kin kin__na8st 
#define rates rates__na8st 
 
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
#define a1_0 _ml->template fpfield<1>(_iml)
#define a1_0_columnindex 1
#define a1_1 _ml->template fpfield<2>(_iml)
#define a1_1_columnindex 2
#define b1_0 _ml->template fpfield<3>(_iml)
#define b1_0_columnindex 3
#define b1_1 _ml->template fpfield<4>(_iml)
#define b1_1_columnindex 4
#define a2_0 _ml->template fpfield<5>(_iml)
#define a2_0_columnindex 5
#define a2_1 _ml->template fpfield<6>(_iml)
#define a2_1_columnindex 6
#define b2_0 _ml->template fpfield<7>(_iml)
#define b2_0_columnindex 7
#define b2_1 _ml->template fpfield<8>(_iml)
#define b2_1_columnindex 8
#define a3_0 _ml->template fpfield<9>(_iml)
#define a3_0_columnindex 9
#define a3_1 _ml->template fpfield<10>(_iml)
#define a3_1_columnindex 10
#define b3_0 _ml->template fpfield<11>(_iml)
#define b3_0_columnindex 11
#define b3_1 _ml->template fpfield<12>(_iml)
#define b3_1_columnindex 12
#define bh_0 _ml->template fpfield<13>(_iml)
#define bh_0_columnindex 13
#define bh_1 _ml->template fpfield<14>(_iml)
#define bh_1_columnindex 14
#define bh_2 _ml->template fpfield<15>(_iml)
#define bh_2_columnindex 15
#define ah_0 _ml->template fpfield<16>(_iml)
#define ah_0_columnindex 16
#define ah_1 _ml->template fpfield<17>(_iml)
#define ah_1_columnindex 17
#define ah_2 _ml->template fpfield<18>(_iml)
#define ah_2_columnindex 18
#define vShift_inact_local _ml->template fpfield<19>(_iml)
#define vShift_inact_local_columnindex 19
#define g _ml->template fpfield<20>(_iml)
#define g_columnindex 20
#define c1 _ml->template fpfield<21>(_iml)
#define c1_columnindex 21
#define c2 _ml->template fpfield<22>(_iml)
#define c2_columnindex 22
#define c3 _ml->template fpfield<23>(_iml)
#define c3_columnindex 23
#define i1 _ml->template fpfield<24>(_iml)
#define i1_columnindex 24
#define i2 _ml->template fpfield<25>(_iml)
#define i2_columnindex 25
#define i3 _ml->template fpfield<26>(_iml)
#define i3_columnindex 26
#define i4 _ml->template fpfield<27>(_iml)
#define i4_columnindex 27
#define i5 _ml->template fpfield<28>(_iml)
#define i5_columnindex 28
#define i6 _ml->template fpfield<29>(_iml)
#define i6_columnindex 29
#define o _ml->template fpfield<30>(_iml)
#define o_columnindex 30
#define ena _ml->template fpfield<31>(_iml)
#define ena_columnindex 31
#define ina _ml->template fpfield<32>(_iml)
#define ina_columnindex 32
#define a1 _ml->template fpfield<33>(_iml)
#define a1_columnindex 33
#define b1 _ml->template fpfield<34>(_iml)
#define b1_columnindex 34
#define a2 _ml->template fpfield<35>(_iml)
#define a2_columnindex 35
#define b2 _ml->template fpfield<36>(_iml)
#define b2_columnindex 36
#define a3 _ml->template fpfield<37>(_iml)
#define a3_columnindex 37
#define b3 _ml->template fpfield<38>(_iml)
#define b3_columnindex 38
#define ah _ml->template fpfield<39>(_iml)
#define ah_columnindex 39
#define bh _ml->template fpfield<40>(_iml)
#define bh_columnindex 40
#define Dc1 _ml->template fpfield<41>(_iml)
#define Dc1_columnindex 41
#define Dc2 _ml->template fpfield<42>(_iml)
#define Dc2_columnindex 42
#define Dc3 _ml->template fpfield<43>(_iml)
#define Dc3_columnindex 43
#define Di1 _ml->template fpfield<44>(_iml)
#define Di1_columnindex 44
#define Di2 _ml->template fpfield<45>(_iml)
#define Di2_columnindex 45
#define Di3 _ml->template fpfield<46>(_iml)
#define Di3_columnindex 46
#define Di4 _ml->template fpfield<47>(_iml)
#define Di4_columnindex 47
#define Di5 _ml->template fpfield<48>(_iml)
#define Di5_columnindex 48
#define Di6 _ml->template fpfield<49>(_iml)
#define Di6_columnindex 49
#define Do _ml->template fpfield<50>(_iml)
#define Do_columnindex 50
#define v _ml->template fpfield<51>(_iml)
#define v_columnindex 51
#define _g _ml->template fpfield<52>(_iml)
#define _g_columnindex 52
#define _ion_ena *(_ml->dptr_field<0>(_iml))
#define _p_ion_ena static_cast<neuron::container::data_handle<double>>(_ppvar[0])
#define _ion_ina *(_ml->dptr_field<1>(_iml))
#define _p_ion_ina static_cast<neuron::container::data_handle<double>>(_ppvar[1])
#define _ion_dinadv *(_ml->dptr_field<2>(_iml))
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
 {"setdata_na8st", _hoc_setdata},
 {"rates_na8st", _hoc_rates},
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
#define maxrate maxrate_na8st
 double maxrate = 8000;
#define slow slow_na8st
 double slow = 1;
#define vShift_inact vShift_inact_na8st
 double vShift_inact = 10;
#define vShift vShift_na8st
 double vShift = 12;
 /* some parameters have upper and lower limits */
 static HocParmLimits _hoc_parm_limits[] = {
 {0, 0, 0}
};
 static HocParmUnits _hoc_parm_units[] = {
 {"vShift_na8st", "mV"},
 {"vShift_inact_na8st", "mV"},
 {"maxrate_na8st", "/ms"},
 {"gbar_na8st", "S/cm2"},
 {"a1_0_na8st", "/ms"},
 {"a1_1_na8st", "/mV"},
 {"b1_0_na8st", "/ms"},
 {"b1_1_na8st", "/mV"},
 {"a2_0_na8st", "/ms"},
 {"a2_1_na8st", "/mV"},
 {"b2_0_na8st", "/ms"},
 {"b2_1_na8st", "/mV"},
 {"a3_0_na8st", "/ms"},
 {"a3_1_na8st", "/mV"},
 {"b3_0_na8st", "/ms"},
 {"b3_1_na8st", "/mV"},
 {"bh_0_na8st", "/ms"},
 {"bh_2_na8st", "/mV"},
 {"ah_0_na8st", "/ms"},
 {"ah_2_na8st", "/mV"},
 {"vShift_inact_local_na8st", "mV"},
 {"g_na8st", "S/cm2"},
 {0, 0}
};
 static double c30 = 0;
 static double c20 = 0;
 static double c10 = 0;
 static double delta_t = 0.01;
 static double i60 = 0;
 static double i50 = 0;
 static double i40 = 0;
 static double i30 = 0;
 static double i20 = 0;
 static double i10 = 0;
 static double o0 = 0;
 /* connect global user variables to hoc */
 static DoubScal hoc_scdoub[] = {
 {"slow_na8st", &slow_na8st},
 {"vShift_na8st", &vShift_na8st},
 {"vShift_inact_na8st", &vShift_inact_na8st},
 {"maxrate_na8st", &maxrate_na8st},
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
"na8st",
 "gbar_na8st",
 "a1_0_na8st",
 "a1_1_na8st",
 "b1_0_na8st",
 "b1_1_na8st",
 "a2_0_na8st",
 "a2_1_na8st",
 "b2_0_na8st",
 "b2_1_na8st",
 "a3_0_na8st",
 "a3_1_na8st",
 "b3_0_na8st",
 "b3_1_na8st",
 "bh_0_na8st",
 "bh_1_na8st",
 "bh_2_na8st",
 "ah_0_na8st",
 "ah_1_na8st",
 "ah_2_na8st",
 "vShift_inact_local_na8st",
 0,
 "g_na8st",
 0,
 "c1_na8st",
 "c2_na8st",
 "c3_na8st",
 "i1_na8st",
 "i2_na8st",
 "i3_na8st",
 "i4_na8st",
 "i5_na8st",
 "i6_na8st",
 "o_na8st",
 0,
 0};
 static Symbol* _na_sym;
 
 /* Used by NrnProperty */
 static _nrn_mechanism_std_vector<double> _parm_default{
     0, /* gbar */
     0, /* a1_0 */
     0, /* a1_1 */
     0, /* b1_0 */
     0, /* b1_1 */
     0, /* a2_0 */
     0, /* a2_1 */
     0, /* b2_0 */
     0, /* b2_1 */
     0, /* a3_0 */
     0, /* a3_1 */
     0, /* b3_0 */
     0, /* b3_1 */
     0, /* bh_0 */
     0, /* bh_1 */
     0, /* bh_2 */
     0, /* ah_0 */
     0, /* ah_1 */
     0, /* ah_2 */
     0, /* vShift_inact_local */
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
    assert(_nrn_mechanism_get_num_vars(_prop) == 53);
 	/*initialize range parameters*/
 	gbar = _parm_default[0]; /* 0 */
 	a1_0 = _parm_default[1]; /* 0 */
 	a1_1 = _parm_default[2]; /* 0 */
 	b1_0 = _parm_default[3]; /* 0 */
 	b1_1 = _parm_default[4]; /* 0 */
 	a2_0 = _parm_default[5]; /* 0 */
 	a2_1 = _parm_default[6]; /* 0 */
 	b2_0 = _parm_default[7]; /* 0 */
 	b2_1 = _parm_default[8]; /* 0 */
 	a3_0 = _parm_default[9]; /* 0 */
 	a3_1 = _parm_default[10]; /* 0 */
 	b3_0 = _parm_default[11]; /* 0 */
 	b3_1 = _parm_default[12]; /* 0 */
 	bh_0 = _parm_default[13]; /* 0 */
 	bh_1 = _parm_default[14]; /* 0 */
 	bh_2 = _parm_default[15]; /* 0 */
 	ah_0 = _parm_default[16]; /* 0 */
 	ah_1 = _parm_default[17]; /* 0 */
 	ah_2 = _parm_default[18]; /* 0 */
 	vShift_inact_local = _parm_default[19]; /* 0 */
 	 assert(_nrn_mechanism_get_num_vars(_prop) == 53);
 	_nrn_mechanism_access_dparam(_prop) = _ppvar;
 	/*connect ionic variables to this model*/
 prop_ion = need_memb(_na_sym);
 nrn_promote(prop_ion, 0, 1);
 	_ppvar[0] = _nrn_mechanism_get_param_handle(prop_ion, 0); /* ena */
 	_ppvar[1] = _nrn_mechanism_get_param_handle(prop_ion, 3); /* ina */
 	_ppvar[2] = _nrn_mechanism_get_param_handle(prop_ion, 4); /* _ion_dinadv */
 
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

 extern "C" void _na8st_reg() {
	int _vectorized = 1;
  _initlists();
 	ion_reg("na", -10000.);
 	_na_sym = hoc_lookup("na_ion");
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
                                       _nrn_mechanism_field<double>{"a1_0"} /* 1 */,
                                       _nrn_mechanism_field<double>{"a1_1"} /* 2 */,
                                       _nrn_mechanism_field<double>{"b1_0"} /* 3 */,
                                       _nrn_mechanism_field<double>{"b1_1"} /* 4 */,
                                       _nrn_mechanism_field<double>{"a2_0"} /* 5 */,
                                       _nrn_mechanism_field<double>{"a2_1"} /* 6 */,
                                       _nrn_mechanism_field<double>{"b2_0"} /* 7 */,
                                       _nrn_mechanism_field<double>{"b2_1"} /* 8 */,
                                       _nrn_mechanism_field<double>{"a3_0"} /* 9 */,
                                       _nrn_mechanism_field<double>{"a3_1"} /* 10 */,
                                       _nrn_mechanism_field<double>{"b3_0"} /* 11 */,
                                       _nrn_mechanism_field<double>{"b3_1"} /* 12 */,
                                       _nrn_mechanism_field<double>{"bh_0"} /* 13 */,
                                       _nrn_mechanism_field<double>{"bh_1"} /* 14 */,
                                       _nrn_mechanism_field<double>{"bh_2"} /* 15 */,
                                       _nrn_mechanism_field<double>{"ah_0"} /* 16 */,
                                       _nrn_mechanism_field<double>{"ah_1"} /* 17 */,
                                       _nrn_mechanism_field<double>{"ah_2"} /* 18 */,
                                       _nrn_mechanism_field<double>{"vShift_inact_local"} /* 19 */,
                                       _nrn_mechanism_field<double>{"g"} /* 20 */,
                                       _nrn_mechanism_field<double>{"c1"} /* 21 */,
                                       _nrn_mechanism_field<double>{"c2"} /* 22 */,
                                       _nrn_mechanism_field<double>{"c3"} /* 23 */,
                                       _nrn_mechanism_field<double>{"i1"} /* 24 */,
                                       _nrn_mechanism_field<double>{"i2"} /* 25 */,
                                       _nrn_mechanism_field<double>{"i3"} /* 26 */,
                                       _nrn_mechanism_field<double>{"i4"} /* 27 */,
                                       _nrn_mechanism_field<double>{"i5"} /* 28 */,
                                       _nrn_mechanism_field<double>{"i6"} /* 29 */,
                                       _nrn_mechanism_field<double>{"o"} /* 30 */,
                                       _nrn_mechanism_field<double>{"ena"} /* 31 */,
                                       _nrn_mechanism_field<double>{"ina"} /* 32 */,
                                       _nrn_mechanism_field<double>{"a1"} /* 33 */,
                                       _nrn_mechanism_field<double>{"b1"} /* 34 */,
                                       _nrn_mechanism_field<double>{"a2"} /* 35 */,
                                       _nrn_mechanism_field<double>{"b2"} /* 36 */,
                                       _nrn_mechanism_field<double>{"a3"} /* 37 */,
                                       _nrn_mechanism_field<double>{"b3"} /* 38 */,
                                       _nrn_mechanism_field<double>{"ah"} /* 39 */,
                                       _nrn_mechanism_field<double>{"bh"} /* 40 */,
                                       _nrn_mechanism_field<double>{"Dc1"} /* 41 */,
                                       _nrn_mechanism_field<double>{"Dc2"} /* 42 */,
                                       _nrn_mechanism_field<double>{"Dc3"} /* 43 */,
                                       _nrn_mechanism_field<double>{"Di1"} /* 44 */,
                                       _nrn_mechanism_field<double>{"Di2"} /* 45 */,
                                       _nrn_mechanism_field<double>{"Di3"} /* 46 */,
                                       _nrn_mechanism_field<double>{"Di4"} /* 47 */,
                                       _nrn_mechanism_field<double>{"Di5"} /* 48 */,
                                       _nrn_mechanism_field<double>{"Di6"} /* 49 */,
                                       _nrn_mechanism_field<double>{"Do"} /* 50 */,
                                       _nrn_mechanism_field<double>{"v"} /* 51 */,
                                       _nrn_mechanism_field<double>{"_g"} /* 52 */,
                                       _nrn_mechanism_field<double*>{"_ion_ena", "na_ion"} /* 0 */,
                                       _nrn_mechanism_field<double*>{"_ion_ina", "na_ion"} /* 1 */,
                                       _nrn_mechanism_field<double*>{"_ion_dinadv", "na_ion"} /* 2 */,
                                       _nrn_mechanism_field<int>{"_cvode_ieq", "cvodeieq"} /* 3 */);
  hoc_register_prop_size(_mechtype, 53, 4);
  hoc_register_dparam_semantics(_mechtype, 0, "na_ion");
  hoc_register_dparam_semantics(_mechtype, 1, "na_ion");
  hoc_register_dparam_semantics(_mechtype, 2, "na_ion");
  hoc_register_dparam_semantics(_mechtype, 3, "cvodeieq");
 	hoc_register_cvode(_mechtype, _ode_count, _ode_map, _ode_spec, _ode_matsol);
 	hoc_register_tolerance(_mechtype, _hoc_state_tol, &_atollist);
 
    hoc_register_var(hoc_scdoub, hoc_vdoub, hoc_intfunc);
 	ivoc_help("help ?1 na8st /home/raven/PycharmProjects/Masters/Mod_Files/na8st.mod\n");
 hoc_register_limits(_mechtype, _hoc_parm_limits);
 hoc_register_units(_mechtype, _hoc_parm_units);
 }
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
 static neuron::container::field_index _slist1[10], _dlist1[10]; static double *_temp1;
 static int kin (void* _so, double* _rhs, _internalthreadargsproto_);
 
static int kin (void* _so, double* _rhs, _internalthreadargsproto_)
 {int _reset=0;
 {
   double b_flux, f_flux, _term; int _i;
 {int _i; double _dt1 = 1.0/dt;
for(_i=1;_i<10;_i++){
  	_RHS1(_i) = -_dt1*(_ml->data(_iml, _slist1[_i]) - _ml->data(_iml, _dlist1[_i]));
	_MATELM1(_i, _i) = _dt1;
      
} }
 rates ( _threadargscomma_ v ) ;
   /* ~ c1 <-> c2 ( a1 , b1 )*/
 f_flux =  a1 * c1 ;
 b_flux =  b1 * c2 ;
 _RHS1( 3) -= (f_flux - b_flux);
 _RHS1( 2) += (f_flux - b_flux);
 
 _term =  a1 ;
 _MATELM1( 3 ,3)  += _term;
 _MATELM1( 2 ,3)  -= _term;
 _term =  b1 ;
 _MATELM1( 3 ,2)  -= _term;
 _MATELM1( 2 ,2)  += _term;
 /*REACTION*/
  /* ~ c2 <-> c3 ( a2 , b2 )*/
 f_flux =  a2 * c2 ;
 b_flux =  b2 * c3 ;
 _RHS1( 2) -= (f_flux - b_flux);
 _RHS1( 1) += (f_flux - b_flux);
 
 _term =  a2 ;
 _MATELM1( 2 ,2)  += _term;
 _MATELM1( 1 ,2)  -= _term;
 _term =  b2 ;
 _MATELM1( 2 ,1)  -= _term;
 _MATELM1( 1 ,1)  += _term;
 /*REACTION*/
  /* ~ c3 <-> o ( a3 , b3 )*/
 f_flux =  a3 * c3 ;
 b_flux =  b3 * o ;
 _RHS1( 1) -= (f_flux - b_flux);
 
 _term =  a3 ;
 _MATELM1( 1 ,1)  += _term;
 _term =  b3 ;
 _MATELM1( 1 ,0)  -= _term;
 /*REACTION*/
  /* ~ i1 <-> i2 ( a1 , b1 )*/
 f_flux =  a1 * i1 ;
 b_flux =  b1 * i2 ;
 _RHS1( 9) -= (f_flux - b_flux);
 _RHS1( 8) += (f_flux - b_flux);
 
 _term =  a1 ;
 _MATELM1( 9 ,9)  += _term;
 _MATELM1( 8 ,9)  -= _term;
 _term =  b1 ;
 _MATELM1( 9 ,8)  -= _term;
 _MATELM1( 8 ,8)  += _term;
 /*REACTION*/
  /* ~ i2 <-> i3 ( a2 , b2 )*/
 f_flux =  a2 * i2 ;
 b_flux =  b2 * i3 ;
 _RHS1( 8) -= (f_flux - b_flux);
 _RHS1( 7) += (f_flux - b_flux);
 
 _term =  a2 ;
 _MATELM1( 8 ,8)  += _term;
 _MATELM1( 7 ,8)  -= _term;
 _term =  b2 ;
 _MATELM1( 8 ,7)  -= _term;
 _MATELM1( 7 ,7)  += _term;
 /*REACTION*/
  /* ~ i3 <-> i4 ( a3 , b3 )*/
 f_flux =  a3 * i3 ;
 b_flux =  b3 * i4 ;
 _RHS1( 7) -= (f_flux - b_flux);
 _RHS1( 6) += (f_flux - b_flux);
 
 _term =  a3 ;
 _MATELM1( 7 ,7)  += _term;
 _MATELM1( 6 ,7)  -= _term;
 _term =  b3 ;
 _MATELM1( 7 ,6)  -= _term;
 _MATELM1( 6 ,6)  += _term;
 /*REACTION*/
  /* ~ i1 <-> c1 ( ah , bh )*/
 f_flux =  ah * i1 ;
 b_flux =  bh * c1 ;
 _RHS1( 9) -= (f_flux - b_flux);
 _RHS1( 3) += (f_flux - b_flux);
 
 _term =  ah ;
 _MATELM1( 9 ,9)  += _term;
 _MATELM1( 3 ,9)  -= _term;
 _term =  bh ;
 _MATELM1( 9 ,3)  -= _term;
 _MATELM1( 3 ,3)  += _term;
 /*REACTION*/
  /* ~ i2 <-> c2 ( ah , bh )*/
 f_flux =  ah * i2 ;
 b_flux =  bh * c2 ;
 _RHS1( 8) -= (f_flux - b_flux);
 _RHS1( 2) += (f_flux - b_flux);
 
 _term =  ah ;
 _MATELM1( 8 ,8)  += _term;
 _MATELM1( 2 ,8)  -= _term;
 _term =  bh ;
 _MATELM1( 8 ,2)  -= _term;
 _MATELM1( 2 ,2)  += _term;
 /*REACTION*/
  /* ~ i3 <-> c3 ( ah , bh )*/
 f_flux =  ah * i3 ;
 b_flux =  bh * c3 ;
 _RHS1( 7) -= (f_flux - b_flux);
 _RHS1( 1) += (f_flux - b_flux);
 
 _term =  ah ;
 _MATELM1( 7 ,7)  += _term;
 _MATELM1( 1 ,7)  -= _term;
 _term =  bh ;
 _MATELM1( 7 ,1)  -= _term;
 _MATELM1( 1 ,1)  += _term;
 /*REACTION*/
  /* ~ i4 <-> o ( ah , bh )*/
 f_flux =  ah * i4 ;
 b_flux =  bh * o ;
 _RHS1( 6) -= (f_flux - b_flux);
 
 _term =  ah ;
 _MATELM1( 6 ,6)  += _term;
 _term =  bh ;
 _MATELM1( 6 ,0)  -= _term;
 /*REACTION*/
  /* ~ i5 <-> c3 ( ah / 10.0 , slow * bh / 10.0 )*/
 f_flux =  ah / 10.0 * i5 ;
 b_flux =  slow * bh / 10.0 * c3 ;
 _RHS1( 5) -= (f_flux - b_flux);
 _RHS1( 1) += (f_flux - b_flux);
 
 _term =  ah / 10.0 ;
 _MATELM1( 5 ,5)  += _term;
 _MATELM1( 1 ,5)  -= _term;
 _term =  slow * bh / 10.0 ;
 _MATELM1( 5 ,1)  -= _term;
 _MATELM1( 1 ,1)  += _term;
 /*REACTION*/
  /* ~ i6 <-> o ( ah / 10.0 , slow * bh / 10.0 )*/
 f_flux =  ah / 10.0 * i6 ;
 b_flux =  slow * bh / 10.0 * o ;
 _RHS1( 4) -= (f_flux - b_flux);
 
 _term =  ah / 10.0 ;
 _MATELM1( 4 ,4)  += _term;
 _term =  slow * bh / 10.0 ;
 _MATELM1( 4 ,0)  -= _term;
 /*REACTION*/
   /* c1 + c2 + c3 + i1 + i2 + i3 + i4 + i5 + i6 + o = 1.0 */
 _RHS1(0) =  1.0;
 _MATELM1(0, 0) = 1;
 _RHS1(0) -= o ;
 _MATELM1(0, 4) = 1;
 _RHS1(0) -= i6 ;
 _MATELM1(0, 5) = 1;
 _RHS1(0) -= i5 ;
 _MATELM1(0, 6) = 1;
 _RHS1(0) -= i4 ;
 _MATELM1(0, 7) = 1;
 _RHS1(0) -= i3 ;
 _MATELM1(0, 8) = 1;
 _RHS1(0) -= i2 ;
 _MATELM1(0, 9) = 1;
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
   double _lvS ;
 _lvS = _lv - vShift ;
   a1 = a1_0 * exp ( a1_1 * _lvS ) ;
   b1 = b1_0 * exp ( - b1_1 * _lvS ) ;
   a2 = a2_0 * exp ( a2_1 * _lvS ) ;
   b2 = b2_0 * exp ( - b2_1 * _lvS ) ;
   a3 = a3_0 * exp ( a3_1 * _lvS ) ;
   b3 = b3_0 * exp ( - b3_1 * _lvS ) ;
   bh = bh_0 / ( 1.0 + bh_1 * exp ( - bh_2 * ( _lvS - vShift_inact - vShift_inact_local ) ) ) ;
   ah = ah_0 / ( 1.0 + ah_1 * exp ( ah_2 * ( _lvS - vShift_inact - vShift_inact_local ) ) ) ;
   a1 = a1 * maxrate / ( a1 + maxrate ) ;
   b1 = b1 * maxrate / ( b1 + maxrate ) ;
   a2 = a2 * maxrate / ( a2 + maxrate ) ;
   b2 = b2 * maxrate / ( b2 + maxrate ) ;
   a3 = a3 * maxrate / ( a3 + maxrate ) ;
   b3 = b3 * maxrate / ( b3 + maxrate ) ;
   bh = bh * maxrate / ( bh + maxrate ) ;
   ah = ah * maxrate / ( ah + maxrate ) ;
    return 0; }
 
static void _hoc_rates(void) {
  double _r;
 Datum* _ppvar; Datum* _thread; NrnThread* _nt;
 
  if(!_prop_id) {
    hoc_execerror("No data for rates_na8st. Requires prior call to setdata_na8st and that the specified mechanism instance still be in existence.", NULL);
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
 
/*CVODE ode begin*/
 static int _ode_spec1(_internalthreadargsproto_) {
  int _reset=0;
  {
 double b_flux, f_flux, _term; int _i;
 {int _i; for(_i=0;_i<10;_i++) _ml->data(_iml, _dlist1[_i]) = 0.0;}
 rates ( _threadargscomma_ v ) ;
 /* ~ c1 <-> c2 ( a1 , b1 )*/
 f_flux =  a1 * c1 ;
 b_flux =  b1 * c2 ;
 Dc1 -= (f_flux - b_flux);
 Dc2 += (f_flux - b_flux);
 
 /*REACTION*/
  /* ~ c2 <-> c3 ( a2 , b2 )*/
 f_flux =  a2 * c2 ;
 b_flux =  b2 * c3 ;
 Dc2 -= (f_flux - b_flux);
 Dc3 += (f_flux - b_flux);
 
 /*REACTION*/
  /* ~ c3 <-> o ( a3 , b3 )*/
 f_flux =  a3 * c3 ;
 b_flux =  b3 * o ;
 Dc3 -= (f_flux - b_flux);
 Do += (f_flux - b_flux);
 
 /*REACTION*/
  /* ~ i1 <-> i2 ( a1 , b1 )*/
 f_flux =  a1 * i1 ;
 b_flux =  b1 * i2 ;
 Di1 -= (f_flux - b_flux);
 Di2 += (f_flux - b_flux);
 
 /*REACTION*/
  /* ~ i2 <-> i3 ( a2 , b2 )*/
 f_flux =  a2 * i2 ;
 b_flux =  b2 * i3 ;
 Di2 -= (f_flux - b_flux);
 Di3 += (f_flux - b_flux);
 
 /*REACTION*/
  /* ~ i3 <-> i4 ( a3 , b3 )*/
 f_flux =  a3 * i3 ;
 b_flux =  b3 * i4 ;
 Di3 -= (f_flux - b_flux);
 Di4 += (f_flux - b_flux);
 
 /*REACTION*/
  /* ~ i1 <-> c1 ( ah , bh )*/
 f_flux =  ah * i1 ;
 b_flux =  bh * c1 ;
 Di1 -= (f_flux - b_flux);
 Dc1 += (f_flux - b_flux);
 
 /*REACTION*/
  /* ~ i2 <-> c2 ( ah , bh )*/
 f_flux =  ah * i2 ;
 b_flux =  bh * c2 ;
 Di2 -= (f_flux - b_flux);
 Dc2 += (f_flux - b_flux);
 
 /*REACTION*/
  /* ~ i3 <-> c3 ( ah , bh )*/
 f_flux =  ah * i3 ;
 b_flux =  bh * c3 ;
 Di3 -= (f_flux - b_flux);
 Dc3 += (f_flux - b_flux);
 
 /*REACTION*/
  /* ~ i4 <-> o ( ah , bh )*/
 f_flux =  ah * i4 ;
 b_flux =  bh * o ;
 Di4 -= (f_flux - b_flux);
 Do += (f_flux - b_flux);
 
 /*REACTION*/
  /* ~ i5 <-> c3 ( ah / 10.0 , slow * bh / 10.0 )*/
 f_flux =  ah / 10.0 * i5 ;
 b_flux =  slow * bh / 10.0 * c3 ;
 Di5 -= (f_flux - b_flux);
 Dc3 += (f_flux - b_flux);
 
 /*REACTION*/
  /* ~ i6 <-> o ( ah / 10.0 , slow * bh / 10.0 )*/
 f_flux =  ah / 10.0 * i6 ;
 b_flux =  slow * bh / 10.0 * o ;
 Di6 -= (f_flux - b_flux);
 Do += (f_flux - b_flux);
 
 /*REACTION*/
   /* c1 + c2 + c3 + i1 + i2 + i3 + i4 + i5 + i6 + o = 1.0 */
 /*CONSERVATION*/
   } return _reset;
 }
 
/*CVODE matsol*/
 static int _ode_matsol1(void* _so, double* _rhs, _internalthreadargsproto_) {int _reset=0;{
 double b_flux, f_flux, _term; int _i;
   b_flux = f_flux = 0.;
 {int _i; double _dt1 = 1.0/dt;
for(_i=0;_i<10;_i++){
  	_RHS1(_i) = _dt1*(_ml->data(_iml, _dlist1[_i]));
	_MATELM1(_i, _i) = _dt1;
      
} }
 rates ( _threadargscomma_ v ) ;
 /* ~ c1 <-> c2 ( a1 , b1 )*/
 _term =  a1 ;
 _MATELM1( 3 ,3)  += _term;
 _MATELM1( 2 ,3)  -= _term;
 _term =  b1 ;
 _MATELM1( 3 ,2)  -= _term;
 _MATELM1( 2 ,2)  += _term;
 /*REACTION*/
  /* ~ c2 <-> c3 ( a2 , b2 )*/
 _term =  a2 ;
 _MATELM1( 2 ,2)  += _term;
 _MATELM1( 1 ,2)  -= _term;
 _term =  b2 ;
 _MATELM1( 2 ,1)  -= _term;
 _MATELM1( 1 ,1)  += _term;
 /*REACTION*/
  /* ~ c3 <-> o ( a3 , b3 )*/
 _term =  a3 ;
 _MATELM1( 1 ,1)  += _term;
 _MATELM1( 0 ,1)  -= _term;
 _term =  b3 ;
 _MATELM1( 1 ,0)  -= _term;
 _MATELM1( 0 ,0)  += _term;
 /*REACTION*/
  /* ~ i1 <-> i2 ( a1 , b1 )*/
 _term =  a1 ;
 _MATELM1( 9 ,9)  += _term;
 _MATELM1( 8 ,9)  -= _term;
 _term =  b1 ;
 _MATELM1( 9 ,8)  -= _term;
 _MATELM1( 8 ,8)  += _term;
 /*REACTION*/
  /* ~ i2 <-> i3 ( a2 , b2 )*/
 _term =  a2 ;
 _MATELM1( 8 ,8)  += _term;
 _MATELM1( 7 ,8)  -= _term;
 _term =  b2 ;
 _MATELM1( 8 ,7)  -= _term;
 _MATELM1( 7 ,7)  += _term;
 /*REACTION*/
  /* ~ i3 <-> i4 ( a3 , b3 )*/
 _term =  a3 ;
 _MATELM1( 7 ,7)  += _term;
 _MATELM1( 6 ,7)  -= _term;
 _term =  b3 ;
 _MATELM1( 7 ,6)  -= _term;
 _MATELM1( 6 ,6)  += _term;
 /*REACTION*/
  /* ~ i1 <-> c1 ( ah , bh )*/
 _term =  ah ;
 _MATELM1( 9 ,9)  += _term;
 _MATELM1( 3 ,9)  -= _term;
 _term =  bh ;
 _MATELM1( 9 ,3)  -= _term;
 _MATELM1( 3 ,3)  += _term;
 /*REACTION*/
  /* ~ i2 <-> c2 ( ah , bh )*/
 _term =  ah ;
 _MATELM1( 8 ,8)  += _term;
 _MATELM1( 2 ,8)  -= _term;
 _term =  bh ;
 _MATELM1( 8 ,2)  -= _term;
 _MATELM1( 2 ,2)  += _term;
 /*REACTION*/
  /* ~ i3 <-> c3 ( ah , bh )*/
 _term =  ah ;
 _MATELM1( 7 ,7)  += _term;
 _MATELM1( 1 ,7)  -= _term;
 _term =  bh ;
 _MATELM1( 7 ,1)  -= _term;
 _MATELM1( 1 ,1)  += _term;
 /*REACTION*/
  /* ~ i4 <-> o ( ah , bh )*/
 _term =  ah ;
 _MATELM1( 6 ,6)  += _term;
 _MATELM1( 0 ,6)  -= _term;
 _term =  bh ;
 _MATELM1( 6 ,0)  -= _term;
 _MATELM1( 0 ,0)  += _term;
 /*REACTION*/
  /* ~ i5 <-> c3 ( ah / 10.0 , slow * bh / 10.0 )*/
 _term =  ah / 10.0 ;
 _MATELM1( 5 ,5)  += _term;
 _MATELM1( 1 ,5)  -= _term;
 _term =  slow * bh / 10.0 ;
 _MATELM1( 5 ,1)  -= _term;
 _MATELM1( 1 ,1)  += _term;
 /*REACTION*/
  /* ~ i6 <-> o ( ah / 10.0 , slow * bh / 10.0 )*/
 _term =  ah / 10.0 ;
 _MATELM1( 4 ,4)  += _term;
 _MATELM1( 0 ,4)  -= _term;
 _term =  slow * bh / 10.0 ;
 _MATELM1( 4 ,0)  -= _term;
 _MATELM1( 0 ,0)  += _term;
 /*REACTION*/
   /* c1 + c2 + c3 + i1 + i2 + i3 + i4 + i5 + i6 + o = 1.0 */
 /*CONSERVATION*/
   } return _reset;
 }
 
/*CVODE end*/
 
static int _ode_count(int _type){ return 10;}
 
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
  ena = _ion_ena;
     _ode_spec1 (_threadargs_);
  }}
 
static void _ode_map(Prop* _prop, int _ieq, neuron::container::data_handle<double>* _pv, neuron::container::data_handle<double>* _pvdot, double* _atol, int _type) { 
  Datum* _ppvar;
  _ppvar = _nrn_mechanism_access_dparam(_prop);
  _cvode_ieq = _ieq;
  for (int _i=0; _i < 10; ++_i) {
    _pv[_i] = _nrn_mechanism_get_param_handle(_prop, _slist1[_i]);
    _pvdot[_i] = _nrn_mechanism_get_param_handle(_prop, _dlist1[_i]);
    _cvode_abstol(_atollist, _atol, _i);
  }
 }
 
static void _ode_matsol_instance1(_internalthreadargsproto_) {
 _cvode_sparse_thread(&(_thread[_cvspth1].literal_value<void*>()), 10, _dlist1, neuron::scopmath::row_view{_ml, _iml}, _ode_matsol1, _threadargs_);
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
  ena = _ion_ena;
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
  i6 = i60;
  i5 = i50;
  i4 = i40;
  i3 = i30;
  i2 = i20;
  i1 = i10;
  o = o0;
 {
    _ss_sparse_thread(&(_thread[_spth1].literal_value<void*>()), 10, _slist1, _dlist1, neuron::scopmath::row_view{_ml, _iml}, &t, dt, kin, _linmat1, _threadargs_);
     if (secondorder) {
    int _i;
    for (_i = 0; _i < 10; ++_i) {
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
  ena = _ion_ena;
 initmodel(_threadargs_);
 }
}

static double _nrn_current(_internalthreadargsprotocomma_ double _v) {
double _current=0.; v=_v;
{ {
   g = gbar * o ;
   ina = ( g ) * ( v - ena ) ;
   }
 _current += ina;

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
  ena = _ion_ena;
 auto const _g_local = _nrn_current(_threadargscomma_ _v + .001);
 	{ double _dina;
  _dina = ina;
 _rhs = _nrn_current(_threadargscomma_ _v);
  _ion_dinadv += (_dina - ina)/.001 ;
 	}
 _g = (_g_local - _rhs)/.001;
  _ion_ina += ina ;
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
  ena = _ion_ena;
 {  sparse_thread(&(_thread[_spth1].literal_value<void*>()), 10, _slist1, _dlist1, neuron::scopmath::row_view{_ml, _iml}, &t, dt, kin, _linmat1, _threadargs_);
     if (secondorder) {
    int _i;
    for (_i = 0; _i < 10; ++_i) {
      _ml->data(_iml, _slist1[_i]) += dt*_ml->data(_iml, _dlist1[_i]);
    }}
 } }}
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
 _slist1[4] = {i6_columnindex, 0};  _dlist1[4] = {Di6_columnindex, 0};
 _slist1[5] = {i5_columnindex, 0};  _dlist1[5] = {Di5_columnindex, 0};
 _slist1[6] = {i4_columnindex, 0};  _dlist1[6] = {Di4_columnindex, 0};
 _slist1[7] = {i3_columnindex, 0};  _dlist1[7] = {Di3_columnindex, 0};
 _slist1[8] = {i2_columnindex, 0};  _dlist1[8] = {Di2_columnindex, 0};
 _slist1[9] = {i1_columnindex, 0};  _dlist1[9] = {Di1_columnindex, 0};
_first = 0;
}

#if NMODL_TEXT
static void register_nmodl_text_and_filename(int mech_type) {
    const char* nmodl_filename = "/home/raven/PycharmProjects/Masters/Mod_Files/na8st.mod";
    const char* nmodl_file_text = 
  ": Eight state kinetic sodium channel gating scheme\n"
  ": Modified from k3st.mod, chapter 9.9 (example 9.7)\n"
  ": of the NEURON book\n"
  ": 12 August 2008, Christoph Schmidt-Hieber\n"
  ":\n"
  ": accompanies the publication:\n"
  ": Schmidt-Hieber C, Bischofberger J. (2010)\n"
  ": Fast sodium channel gating supports localized and efficient \n"
  ": axonal action potential initiation.\n"
  ": J Neurosci 30:10233-42\n"
  ": added possibility to implement slow inactivation (Beining et al (2016), \"A novel comprehensive and consistent electrophysiological model of dentate granule cells\")\n"
  "\n"
  "\n"
  "\n"
  "NEURON {\n"
  "    SUFFIX na8st\n"
  "    USEION na READ ena WRITE ina\n"
  "    GLOBAL vShift, vShift_inact, slow\n"
  "    RANGE vShift_inact_local\n"
  "    RANGE g, gbar\n"
  "    RANGE a1_0, a1_1, b1_0, b1_1, a2_0, a2_1\n"
  "    RANGE b2_0, b2_1, a3_0, a3_1, b3_0, b3_1\n"
  "    RANGE bh_0, bh_1, bh_2, ah_0, ah_1, ah_2\n"
  "}\n"
  "\n"
  "UNITS { (mV) = (millivolt) \n"
  "(S) = (siemens)\n"
  "}\n"
  "\n"
  ": initialize parameters\n"
  "\n"
  "PARAMETER {\n"
  "    gbar = 0     (S/cm2)\n"
  "	slow = 1\n"
  "    a1_0 =0 (/ms) : 5.142954478051616e+01 (/ms)\n"
  "    a1_1 = 0 (/mV) : 7.674641248142576e-03 (/mV) \n"
  "    \n"
  "    b1_0 = 0 (/ms) :9.132202467321037e-03 (/ms)\n"
  "    b1_1 = 0 (/mV) :9.342823457307300e-02 (/mV)\n"
  "\n"
  "    a2_0 = 0 (/ms) :7.488753944786941e+01 (/ms)\n"
  "    a2_1 = 0 (/mV) :2.014613733367395e-02 (/mV) \n"
  "    \n"
  "    b2_0 = 0 (/ms) :6.387047323688771e-03 (/ms)\n"
  "    b2_1 = 0 (/mV) :1.501806374396736e-01 (/mV)\n"
  "\n"
  "    a3_0 = 0 (/ms) :3.838866325780059e+01 (/ms)\n"
  "    a3_1 = 0 (/mV) :1.253027842782742e-02 (/mV) \n"
  "    \n"
  "    b3_0 = 0 (/ms) :3.989222258297797e-01 (/ms)\n"
  "    b3_1 = 0 (/mV) :9.001475021228642e-02 (/mV)\n"
  "\n"
  "    bh_0 = 0 (/ms) :1.687524670388565e+00 (/ms)\n"
  "    bh_1 = 0:1.210600094822588e-01 \n"
  "    bh_2 = 0 (/mV) :6.827857751079400e-02 (/mV)\n"
  "\n"
  "    ah_0 = 0 (/ms) :3.800097357917129e+00 (/ms)\n"
  "    ah_1 = 0:4.445911330118979e+03  \n"
  "    ah_2 =0 (/mV) :4.059075804728014e-02 (/mV)\n"
  "\n"
  "    vShift = 12            (mV)  : shift to the right to account for Donnan potentials\n"
  "                                 : 12 mV for cclamp, 0 for oo-patch vclamp simulations\n"
  "    vShift_inact = 10      (mV)  : global additional shift to the right for inactivation\n"
  "                                 : 10 mV for cclamp, 0 for oo-patch vclamp simulations\n"
  "    vShift_inact_local = 0 (mV)  : additional shift to the right for inactivation, used as local range variable\n"
  "    maxrate = 8.00e+03     (/ms) : limiting value for reaction rates\n"
  "                                 : See Patlak, 1991\n"
  "}\n"
  "\n"
  "ASSIGNED {\n"
  "    v    (mV)\n"
  "    ena  (mV)\n"
  "    g    (S/cm2)\n"
  "    ina  (milliamp/cm2)\n"
  "    a1   (/ms)\n"
  "    b1   (/ms)\n"
  "    a2   (/ms)\n"
  "    b2   (/ms)\n"
  "    a3   (/ms)\n"
  "    b3   (/ms)\n"
  "    ah   (/ms)\n"
  "    bh   (/ms)\n"
  "}\n"
  "\n"
  "STATE { c1 c2 c3 i1 i2 i3 i4 i5 i6 o }:i11 i22 i33 i44 }\n"
  "\n"
  "BREAKPOINT {\n"
  "    SOLVE kin METHOD sparse\n"
  "    g = gbar*o\n"
  "    ina = (g)*(v - ena)\n"
  "}\n"
  "\n"
  "INITIAL { SOLVE kin STEADYSTATE sparse }\n"
  "\n"
  "KINETIC kin {\n"
  "    rates(v)\n"
  "    ~ c1 <-> c2 (a1, b1)\n"
  "    ~ c2 <-> c3 (a2, b2)\n"
  "    ~ c3 <-> o (a3, b3)\n"
  "    ~ i1 <-> i2 (a1, b1)\n"
  "    ~ i2 <-> i3 (a2, b2)\n"
  "    ~ i3 <-> i4 (a3, b3)\n"
  "    ~ i1 <-> c1 (ah, bh)\n"
  "    ~ i2 <-> c2 (ah, bh)\n"
  "    ~ i3 <-> c3 (ah, bh)\n"
  "    ~ i4 <-> o  (ah, bh)\n"
  "	~ i5 <-> c3 (ah/10, slow*bh/10)\n"
  "    ~ i6 <-> o  (ah/10, slow*bh/10)\n"
  "    CONSERVE c1 + c2 + c3 + i1 + i2 + i3 + i4 + i5 + i6 + o = 1 \n"
  "}\n"
  "\n"
  "\n"
  "PROCEDURE rates(v(millivolt)) {\n"
  "    LOCAL vS\n"
  "    vS = v-vShift\n"
  "	\n"
  "    a1 = a1_0*exp( a1_1*vS)\n"
  "    b1 = b1_0*exp(-b1_1*vS)\n"
  "\n"
  "    \n"
  "    a2 = a2_0*exp( a2_1*vS)\n"
  "    b2 = b2_0*exp(-b2_1*vS)\n"
  "\n"
  "    \n"
  "    a3 = a3_0*exp( a3_1*vS)\n"
  "    b3 = b3_0*exp(-b3_1*vS)\n"
  "\n"
  "    \n"
  "    bh = bh_0/(1+bh_1*exp(-bh_2*(vS-vShift_inact-vShift_inact_local)))\n"
  "\n"
  "    ah = ah_0/(1+ah_1*exp( ah_2*(vS-vShift_inact-vShift_inact_local)))\n"
  "	\n"
  "\n"
  "		a1 = a1*maxrate / (a1+maxrate)\n"
  "		b1 = b1*maxrate / (b1+maxrate)\n"
  "		a2 = a2*maxrate / (a2+maxrate)\n"
  "		b2 = b2*maxrate / (b2+maxrate)\n"
  "		a3 = a3*maxrate / (a3+maxrate)\n"
  "		b3 = b3*maxrate / (b3+maxrate)\n"
  "		bh = bh*maxrate / (bh+maxrate)\n"
  "		ah = ah*maxrate / (ah+maxrate)\n"
  "}\n"
  ;
    hoc_reg_nmodl_filename(mech_type, nmodl_filename);
    hoc_reg_nmodl_text(mech_type, nmodl_file_text);
}
#endif
