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
static constexpr auto number_of_floating_point_variables = 26;
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
 
#define nrn_init _nrn_init__ichan3
#define _nrn_initial _nrn_initial__ichan3
#define nrn_cur _nrn_cur__ichan3
#define _nrn_current _nrn_current__ichan3
#define nrn_jacob _nrn_jacob__ichan3
#define nrn_state _nrn_state__ichan3
#define _net_receive _net_receive__ichan3 
#define states states__ichan3 
 
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
#define gnabar _ml->template fpfield<0>(_iml)
#define gnabar_columnindex 0
#define gkfbar _ml->template fpfield<1>(_iml)
#define gkfbar_columnindex 1
#define gksbar _ml->template fpfield<2>(_iml)
#define gksbar_columnindex 2
#define gkabar _ml->template fpfield<3>(_iml)
#define gkabar_columnindex 3
#define ina _ml->template fpfield<4>(_iml)
#define ina_columnindex 4
#define ik _ml->template fpfield<5>(_iml)
#define ik_columnindex 5
#define gna _ml->template fpfield<6>(_iml)
#define gna_columnindex 6
#define gkf _ml->template fpfield<7>(_iml)
#define gkf_columnindex 7
#define gks _ml->template fpfield<8>(_iml)
#define gks_columnindex 8
#define gka _ml->template fpfield<9>(_iml)
#define gka_columnindex 9
#define m _ml->template fpfield<10>(_iml)
#define m_columnindex 10
#define n1 _ml->template fpfield<11>(_iml)
#define n1_columnindex 11
#define h _ml->template fpfield<12>(_iml)
#define h_columnindex 12
#define n2 _ml->template fpfield<13>(_iml)
#define n2_columnindex 13
#define k _ml->template fpfield<14>(_iml)
#define k_columnindex 14
#define l _ml->template fpfield<15>(_iml)
#define l_columnindex 15
#define ena _ml->template fpfield<16>(_iml)
#define ena_columnindex 16
#define ek _ml->template fpfield<17>(_iml)
#define ek_columnindex 17
#define Dm _ml->template fpfield<18>(_iml)
#define Dm_columnindex 18
#define Dn1 _ml->template fpfield<19>(_iml)
#define Dn1_columnindex 19
#define Dh _ml->template fpfield<20>(_iml)
#define Dh_columnindex 20
#define Dn2 _ml->template fpfield<21>(_iml)
#define Dn2_columnindex 21
#define Dk _ml->template fpfield<22>(_iml)
#define Dk_columnindex 22
#define Dl _ml->template fpfield<23>(_iml)
#define Dl_columnindex 23
#define v _ml->template fpfield<24>(_iml)
#define v_columnindex 24
#define _g _ml->template fpfield<25>(_iml)
#define _g_columnindex 25
#define _ion_ena *(_ml->dptr_field<0>(_iml))
#define _p_ion_ena static_cast<neuron::container::data_handle<double>>(_ppvar[0])
#define _ion_ina *(_ml->dptr_field<1>(_iml))
#define _p_ion_ina static_cast<neuron::container::data_handle<double>>(_ppvar[1])
#define _ion_dinadv *(_ml->dptr_field<2>(_iml))
#define _ion_ek *(_ml->dptr_field<3>(_iml))
#define _p_ion_ek static_cast<neuron::container::data_handle<double>>(_ppvar[3])
#define _ion_ik *(_ml->dptr_field<4>(_iml))
#define _p_ion_ik static_cast<neuron::container::data_handle<double>>(_ppvar[4])
#define _ion_dikdv *(_ml->dptr_field<5>(_iml))
 /* Thread safe. No static _ml, _iml or _ppvar. */
 static int hoc_nrnpointerindex =  -1;
 static _nrn_mechanism_std_vector<Datum> _extcall_thread;
 static Prop* _extcall_prop;
 /* _prop_id kind of shadows _extcall_prop to allow validity checking. */
 static _nrn_non_owning_id_without_container _prop_id{};
 /* external NEURON variables */
 /* declaration of user functions */
 static void _hoc_al(void);
 static void _hoc_ak(void);
 static void _hoc_an2(void);
 static void _hoc_an1(void);
 static void _hoc_ah(void);
 static void _hoc_am(void);
 static void _hoc_bl(void);
 static void _hoc_bk(void);
 static void _hoc_bn2(void);
 static void _hoc_bn1(void);
 static void _hoc_bh(void);
 static void _hoc_bm(void);
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
 {"setdata_ichan3", _hoc_setdata},
 {"al_ichan3", _hoc_al},
 {"ak_ichan3", _hoc_ak},
 {"an2_ichan3", _hoc_an2},
 {"an1_ichan3", _hoc_an1},
 {"ah_ichan3", _hoc_ah},
 {"am_ichan3", _hoc_am},
 {"bl_ichan3", _hoc_bl},
 {"bk_ichan3", _hoc_bk},
 {"bn2_ichan3", _hoc_bn2},
 {"bn1_ichan3", _hoc_bn1},
 {"bh_ichan3", _hoc_bh},
 {"bm_ichan3", _hoc_bm},
 {0, 0}
};
 
/* Direct Python call wrappers to density mechanism functions.*/
 static double _npy_al(Prop*);
 static double _npy_ak(Prop*);
 static double _npy_an2(Prop*);
 static double _npy_an1(Prop*);
 static double _npy_ah(Prop*);
 static double _npy_am(Prop*);
 static double _npy_bl(Prop*);
 static double _npy_bk(Prop*);
 static double _npy_bn2(Prop*);
 static double _npy_bn1(Prop*);
 static double _npy_bh(Prop*);
 static double _npy_bm(Prop*);
 
static NPyDirectMechFunc npy_direct_func_proc[] = {
 {"al", _npy_al},
 {"ak", _npy_ak},
 {"an2", _npy_an2},
 {"an1", _npy_an1},
 {"ah", _npy_ah},
 {"am", _npy_am},
 {"bl", _npy_bl},
 {"bk", _npy_bk},
 {"bn2", _npy_bn2},
 {"bn1", _npy_bn1},
 {"bh", _npy_bh},
 {"bm", _npy_bm},
 {0, 0}
};
#define al al_ichan3
#define ak ak_ichan3
#define an2 an2_ichan3
#define an1 an1_ichan3
#define ah ah_ichan3
#define am am_ichan3
#define bl bl_ichan3
#define bk bk_ichan3
#define bn2 bn2_ichan3
#define bn1 bn1_ichan3
#define bh bh_ichan3
#define bm bm_ichan3
 extern double al( _internalthreadargsprotocomma_ double );
 extern double ak( _internalthreadargsprotocomma_ double );
 extern double an2( _internalthreadargsprotocomma_ double );
 extern double an1( _internalthreadargsprotocomma_ double );
 extern double ah( _internalthreadargsprotocomma_ double );
 extern double am( _internalthreadargsprotocomma_ double );
 extern double bl( _internalthreadargsprotocomma_ double );
 extern double bk( _internalthreadargsprotocomma_ double );
 extern double bn2( _internalthreadargsprotocomma_ double );
 extern double bn1( _internalthreadargsprotocomma_ double );
 extern double bh( _internalthreadargsprotocomma_ double );
 extern double bm( _internalthreadargsprotocomma_ double );
 /* declare global and static user variables */
 #define gind 0
 #define _gth 0
#define vshiftna vshiftna_ichan3
 double vshiftna = 0;
#define vshiftks vshiftks_ichan3
 double vshiftks = 0;
#define vshiftak vshiftak_ichan3
 double vshiftak = 0;
 /* some parameters have upper and lower limits */
 static HocParmLimits _hoc_parm_limits[] = {
 {0, 0, 0}
};
 static HocParmUnits _hoc_parm_units[] = {
 {"vshiftak_ichan3", "mV"},
 {"vshiftks_ichan3", "mV"},
 {"vshiftna_ichan3", "mV"},
 {"gnabar_ichan3", "S/cm2"},
 {"gkfbar_ichan3", "S/cm2"},
 {"gksbar_ichan3", "S/cm2"},
 {"gkabar_ichan3", "S/cm2"},
 {"ina_ichan3", "mA/cm2"},
 {"ik_ichan3", "mA/cm2"},
 {"gna_ichan3", "S/cm2"},
 {"gkf_ichan3", "S/cm2"},
 {"gks_ichan3", "S/cm2"},
 {"gka_ichan3", "S/cm2"},
 {0, 0}
};
 static double delta_t = 0.01;
 static double h0 = 0;
 static double k0 = 0;
 static double l0 = 0;
 static double m0 = 0;
 static double n20 = 0;
 static double n10 = 0;
 /* connect global user variables to hoc */
 static DoubScal hoc_scdoub[] = {
 {"vshiftak_ichan3", &vshiftak_ichan3},
 {"vshiftks_ichan3", &vshiftks_ichan3},
 {"vshiftna_ichan3", &vshiftna_ichan3},
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
"ichan3",
 "gnabar_ichan3",
 "gkfbar_ichan3",
 "gksbar_ichan3",
 "gkabar_ichan3",
 0,
 "ina_ichan3",
 "ik_ichan3",
 "gna_ichan3",
 "gkf_ichan3",
 "gks_ichan3",
 "gka_ichan3",
 0,
 "m_ichan3",
 "n1_ichan3",
 "h_ichan3",
 "n2_ichan3",
 "k_ichan3",
 "l_ichan3",
 0,
 0};
 static Symbol* _na_sym;
 static Symbol* _k_sym;
 
 /* Used by NrnProperty */
 static _nrn_mechanism_std_vector<double> _parm_default{
     0, /* gnabar */
     0, /* gkfbar */
     0, /* gksbar */
     0, /* gkabar */
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
    assert(_nrn_mechanism_get_num_vars(_prop) == 26);
 	/*initialize range parameters*/
 	gnabar = _parm_default[0]; /* 0 */
 	gkfbar = _parm_default[1]; /* 0 */
 	gksbar = _parm_default[2]; /* 0 */
 	gkabar = _parm_default[3]; /* 0 */
 	 assert(_nrn_mechanism_get_num_vars(_prop) == 26);
 	_nrn_mechanism_access_dparam(_prop) = _ppvar;
 	/*connect ionic variables to this model*/
 prop_ion = need_memb(_na_sym);
 nrn_promote(prop_ion, 0, 1);
 	_ppvar[0] = _nrn_mechanism_get_param_handle(prop_ion, 0); /* ena */
 	_ppvar[1] = _nrn_mechanism_get_param_handle(prop_ion, 3); /* ina */
 	_ppvar[2] = _nrn_mechanism_get_param_handle(prop_ion, 4); /* _ion_dinadv */
 prop_ion = need_memb(_k_sym);
 nrn_promote(prop_ion, 0, 1);
 	_ppvar[3] = _nrn_mechanism_get_param_handle(prop_ion, 0); /* ek */
 	_ppvar[4] = _nrn_mechanism_get_param_handle(prop_ion, 3); /* ik */
 	_ppvar[5] = _nrn_mechanism_get_param_handle(prop_ion, 4); /* _ion_dikdv */
 
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

 extern "C" void _ichan3_reg() {
	int _vectorized = 1;
  _initlists();
 	ion_reg("na", -10000.);
 	ion_reg("k", -10000.);
 	_na_sym = hoc_lookup("na_ion");
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
                                       _nrn_mechanism_field<double>{"gnabar"} /* 0 */,
                                       _nrn_mechanism_field<double>{"gkfbar"} /* 1 */,
                                       _nrn_mechanism_field<double>{"gksbar"} /* 2 */,
                                       _nrn_mechanism_field<double>{"gkabar"} /* 3 */,
                                       _nrn_mechanism_field<double>{"ina"} /* 4 */,
                                       _nrn_mechanism_field<double>{"ik"} /* 5 */,
                                       _nrn_mechanism_field<double>{"gna"} /* 6 */,
                                       _nrn_mechanism_field<double>{"gkf"} /* 7 */,
                                       _nrn_mechanism_field<double>{"gks"} /* 8 */,
                                       _nrn_mechanism_field<double>{"gka"} /* 9 */,
                                       _nrn_mechanism_field<double>{"m"} /* 10 */,
                                       _nrn_mechanism_field<double>{"n1"} /* 11 */,
                                       _nrn_mechanism_field<double>{"h"} /* 12 */,
                                       _nrn_mechanism_field<double>{"n2"} /* 13 */,
                                       _nrn_mechanism_field<double>{"k"} /* 14 */,
                                       _nrn_mechanism_field<double>{"l"} /* 15 */,
                                       _nrn_mechanism_field<double>{"ena"} /* 16 */,
                                       _nrn_mechanism_field<double>{"ek"} /* 17 */,
                                       _nrn_mechanism_field<double>{"Dm"} /* 18 */,
                                       _nrn_mechanism_field<double>{"Dn1"} /* 19 */,
                                       _nrn_mechanism_field<double>{"Dh"} /* 20 */,
                                       _nrn_mechanism_field<double>{"Dn2"} /* 21 */,
                                       _nrn_mechanism_field<double>{"Dk"} /* 22 */,
                                       _nrn_mechanism_field<double>{"Dl"} /* 23 */,
                                       _nrn_mechanism_field<double>{"v"} /* 24 */,
                                       _nrn_mechanism_field<double>{"_g"} /* 25 */,
                                       _nrn_mechanism_field<double*>{"_ion_ena", "na_ion"} /* 0 */,
                                       _nrn_mechanism_field<double*>{"_ion_ina", "na_ion"} /* 1 */,
                                       _nrn_mechanism_field<double*>{"_ion_dinadv", "na_ion"} /* 2 */,
                                       _nrn_mechanism_field<double*>{"_ion_ek", "k_ion"} /* 3 */,
                                       _nrn_mechanism_field<double*>{"_ion_ik", "k_ion"} /* 4 */,
                                       _nrn_mechanism_field<double*>{"_ion_dikdv", "k_ion"} /* 5 */,
                                       _nrn_mechanism_field<int>{"_cvode_ieq", "cvodeieq"} /* 6 */);
  hoc_register_prop_size(_mechtype, 26, 7);
  hoc_register_dparam_semantics(_mechtype, 0, "na_ion");
  hoc_register_dparam_semantics(_mechtype, 1, "na_ion");
  hoc_register_dparam_semantics(_mechtype, 2, "na_ion");
  hoc_register_dparam_semantics(_mechtype, 3, "k_ion");
  hoc_register_dparam_semantics(_mechtype, 4, "k_ion");
  hoc_register_dparam_semantics(_mechtype, 5, "k_ion");
  hoc_register_dparam_semantics(_mechtype, 6, "cvodeieq");
 	hoc_register_cvode(_mechtype, _ode_count, _ode_map, _ode_spec, _ode_matsol);
 	hoc_register_tolerance(_mechtype, _hoc_state_tol, &_atollist);
 
    hoc_register_var(hoc_scdoub, hoc_vdoub, hoc_intfunc);
 	ivoc_help("help ?1 ichan3 /home/raven/PycharmProjects/Masters_model/Mod_Files_Beining_2017/ichan3.mod\n");
 hoc_register_limits(_mechtype, _hoc_parm_limits);
 hoc_register_units(_mechtype, _hoc_parm_units);
 }
static int _reset;
static const char *modelname = "";

static int error;
static int _ninits = 0;
static int _match_recurse=1;
static void _modl_cleanup(){ _match_recurse=1;}
 
static int _ode_spec1(_internalthreadargsproto_);
/*static int _ode_matsol1(_internalthreadargsproto_);*/
 static neuron::container::field_index _slist1[6], _dlist1[6];
 static int states(_internalthreadargsproto_);
 
/*CVODE*/
 static int _ode_spec1 (_internalthreadargsproto_) {int _reset = 0; {
   Dm = ( 1.0 - m ) * am ( _threadargscomma_ v ) - m * bm ( _threadargscomma_ v ) ;
   Dh = ( 1.0 - h ) * ah ( _threadargscomma_ v ) - h * bh ( _threadargscomma_ v ) ;
   Dn1 = ( 1.0 - n1 ) * an1 ( _threadargscomma_ v ) - n1 * bn1 ( _threadargscomma_ v ) ;
   Dn2 = ( 1.0 - n2 ) * an2 ( _threadargscomma_ v ) - n2 * bn2 ( _threadargscomma_ v ) ;
   Dk = ( 1.0 - k ) * ak ( _threadargscomma_ v ) - k * bk ( _threadargscomma_ v ) ;
   Dl = ( 1.0 - l ) * al ( _threadargscomma_ v ) - l * bl ( _threadargscomma_ v ) ;
   }
 return _reset;
}
 static int _ode_matsol1 (_internalthreadargsproto_) {
 Dm = Dm  / (1. - dt*( ( ( ( - 1.0 ) ) )*( am ( _threadargscomma_ v ) ) - ( 1.0 )*( bm ( _threadargscomma_ v ) ) )) ;
 Dh = Dh  / (1. - dt*( ( ( ( - 1.0 ) ) )*( ah ( _threadargscomma_ v ) ) - ( 1.0 )*( bh ( _threadargscomma_ v ) ) )) ;
 Dn1 = Dn1  / (1. - dt*( ( ( ( - 1.0 ) ) )*( an1 ( _threadargscomma_ v ) ) - ( 1.0 )*( bn1 ( _threadargscomma_ v ) ) )) ;
 Dn2 = Dn2  / (1. - dt*( ( ( ( - 1.0 ) ) )*( an2 ( _threadargscomma_ v ) ) - ( 1.0 )*( bn2 ( _threadargscomma_ v ) ) )) ;
 Dk = Dk  / (1. - dt*( ( ( ( - 1.0 ) ) )*( ak ( _threadargscomma_ v ) ) - ( 1.0 )*( bk ( _threadargscomma_ v ) ) )) ;
 Dl = Dl  / (1. - dt*( ( ( ( - 1.0 ) ) )*( al ( _threadargscomma_ v ) ) - ( 1.0 )*( bl ( _threadargscomma_ v ) ) )) ;
  return 0;
}
 /*END CVODE*/
 static int states (_internalthreadargsproto_) { {
    m = m + (1. - exp(dt*(( ( ( - 1.0 ) ) )*( am ( _threadargscomma_ v ) ) - ( 1.0 )*( bm ( _threadargscomma_ v ) ))))*(- ( ( ( 1.0 ) )*( am ( _threadargscomma_ v ) ) ) / ( ( ( ( - 1.0 ) ) )*( am ( _threadargscomma_ v ) ) - ( 1.0 )*( bm ( _threadargscomma_ v ) ) ) - m) ;
    h = h + (1. - exp(dt*(( ( ( - 1.0 ) ) )*( ah ( _threadargscomma_ v ) ) - ( 1.0 )*( bh ( _threadargscomma_ v ) ))))*(- ( ( ( 1.0 ) )*( ah ( _threadargscomma_ v ) ) ) / ( ( ( ( - 1.0 ) ) )*( ah ( _threadargscomma_ v ) ) - ( 1.0 )*( bh ( _threadargscomma_ v ) ) ) - h) ;
    n1 = n1 + (1. - exp(dt*(( ( ( - 1.0 ) ) )*( an1 ( _threadargscomma_ v ) ) - ( 1.0 )*( bn1 ( _threadargscomma_ v ) ))))*(- ( ( ( 1.0 ) )*( an1 ( _threadargscomma_ v ) ) ) / ( ( ( ( - 1.0 ) ) )*( an1 ( _threadargscomma_ v ) ) - ( 1.0 )*( bn1 ( _threadargscomma_ v ) ) ) - n1) ;
    n2 = n2 + (1. - exp(dt*(( ( ( - 1.0 ) ) )*( an2 ( _threadargscomma_ v ) ) - ( 1.0 )*( bn2 ( _threadargscomma_ v ) ))))*(- ( ( ( 1.0 ) )*( an2 ( _threadargscomma_ v ) ) ) / ( ( ( ( - 1.0 ) ) )*( an2 ( _threadargscomma_ v ) ) - ( 1.0 )*( bn2 ( _threadargscomma_ v ) ) ) - n2) ;
    k = k + (1. - exp(dt*(( ( ( - 1.0 ) ) )*( ak ( _threadargscomma_ v ) ) - ( 1.0 )*( bk ( _threadargscomma_ v ) ))))*(- ( ( ( 1.0 ) )*( ak ( _threadargscomma_ v ) ) ) / ( ( ( ( - 1.0 ) ) )*( ak ( _threadargscomma_ v ) ) - ( 1.0 )*( bk ( _threadargscomma_ v ) ) ) - k) ;
    l = l + (1. - exp(dt*(( ( ( - 1.0 ) ) )*( al ( _threadargscomma_ v ) ) - ( 1.0 )*( bl ( _threadargscomma_ v ) ))))*(- ( ( ( 1.0 ) )*( al ( _threadargscomma_ v ) ) ) / ( ( ( ( - 1.0 ) ) )*( al ( _threadargscomma_ v ) ) - ( 1.0 )*( bl ( _threadargscomma_ v ) ) ) - l) ;
   }
  return 0;
}
 
double am ( _internalthreadargsprotocomma_ double _lVm ) {
   double _lam;
 double _lx ;
  _lx = 0.2 * ( _lVm - ( - 45.0 + vshiftna ) ) ;
   if ( fabs ( _lx ) > 1e-6 ) {
     _lam = 1.5 * _lx / ( 1.0 - exp ( - _lx ) ) ;
     }
   else {
     _lam = 1.5 / ( 1.0 + 0.5 * _lx ) ;
     }
    
return _lam;
 }
 
static void _hoc_am(void) {
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
 _r =  am ( _threadargscomma_ *getarg(1) );
 hoc_retpushx(_r);
}
 
static double _npy_am(Prop* _prop) {
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
 _r =  am ( _threadargscomma_ *getarg(1) );
 return(_r);
}
 
double bm ( _internalthreadargsprotocomma_ double _lVm ) {
   double _lbm;
 double _lx ;
  _lx = - 0.2 * ( _lVm - ( - 17.0 + vshiftna ) ) ;
   if ( fabs ( _lx ) > 1e-6 ) {
     _lbm = 1.5 * _lx / ( 1.0 - exp ( - _lx ) ) ;
     }
   else {
     _lbm = 1.5 / ( 1.0 + 0.5 * _lx ) ;
     }
    
return _lbm;
 }
 
static void _hoc_bm(void) {
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
 _r =  bm ( _threadargscomma_ *getarg(1) );
 hoc_retpushx(_r);
}
 
static double _npy_bm(Prop* _prop) {
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
 _r =  bm ( _threadargscomma_ *getarg(1) );
 return(_r);
}
 
double ah ( _internalthreadargsprotocomma_ double _lVm ) {
   double _lah;
  _lah = 0.23 * exp ( - 0.05 * ( _lVm - ( - 67.0 + vshiftna ) ) ) ;
    
return _lah;
 }
 
static void _hoc_ah(void) {
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
 _r =  ah ( _threadargscomma_ *getarg(1) );
 hoc_retpushx(_r);
}
 
static double _npy_ah(Prop* _prop) {
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
 _r =  ah ( _threadargscomma_ *getarg(1) );
 return(_r);
}
 
double bh ( _internalthreadargsprotocomma_ double _lVm ) {
   double _lbh;
 double _lx ;
  _lx = - ( - 0.1 ) * ( ( - 14.5 + vshiftna ) - _lVm ) ;
   _lbh = 3.33 / ( 1.0 + exp ( _lx ) ) ;
    
return _lbh;
 }
 
static void _hoc_bh(void) {
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
 _r =  bh ( _threadargscomma_ *getarg(1) );
 hoc_retpushx(_r);
}
 
static double _npy_bh(Prop* _prop) {
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
 _r =  bh ( _threadargscomma_ *getarg(1) );
 return(_r);
}
 
double an1 ( _internalthreadargsprotocomma_ double _lVm ) {
   double _lan1;
 double _lx ;
  _lx = 0.16667 * ( _lVm + 23.0 ) ;
   if ( fabs ( _lx ) > 1e-6 ) {
     _lan1 = 0.42 * _lx / ( 1.0 - exp ( - _lx ) ) ;
     }
   else {
     _lan1 = 0.42 / ( 1.0 + 0.5 * _lx ) ;
     }
    
return _lan1;
 }
 
static void _hoc_an1(void) {
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
 _r =  an1 ( _threadargscomma_ *getarg(1) );
 hoc_retpushx(_r);
}
 
static double _npy_an1(Prop* _prop) {
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
 _r =  an1 ( _threadargscomma_ *getarg(1) );
 return(_r);
}
 
double bn1 ( _internalthreadargsprotocomma_ double _lVm ) {
   double _lbn1;
  _lbn1 = 0.264 * exp ( - 0.025 * ( _lVm + 48.0 ) ) ;
    
return _lbn1;
 }
 
static void _hoc_bn1(void) {
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
 _r =  bn1 ( _threadargscomma_ *getarg(1) );
 hoc_retpushx(_r);
}
 
static double _npy_bn1(Prop* _prop) {
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
 _r =  bn1 ( _threadargscomma_ *getarg(1) );
 return(_r);
}
 
double an2 ( _internalthreadargsprotocomma_ double _lVm ) {
   double _lan2;
 double _lx ;
  _lx = 0.16667 * ( _lVm - ( - 35.0 + vshiftks ) ) ;
   if ( fabs ( _lx ) > 1e-6 ) {
     _lan2 = 0.168 * _lx / ( 1.0 - exp ( - _lx ) ) ;
     }
   else {
     _lan2 = 0.42 / ( 1.0 + 0.5 * _lx ) ;
     }
    
return _lan2;
 }
 
static void _hoc_an2(void) {
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
 _r =  an2 ( _threadargscomma_ *getarg(1) );
 hoc_retpushx(_r);
}
 
static double _npy_an2(Prop* _prop) {
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
 _r =  an2 ( _threadargscomma_ *getarg(1) );
 return(_r);
}
 
double bn2 ( _internalthreadargsprotocomma_ double _lVm ) {
   double _lbn2;
  _lbn2 = 0.1056 * exp ( - 0.025 * ( _lVm - ( - 60.0 + vshiftks ) ) ) ;
    
return _lbn2;
 }
 
static void _hoc_bn2(void) {
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
 _r =  bn2 ( _threadargscomma_ *getarg(1) );
 hoc_retpushx(_r);
}
 
static double _npy_bn2(Prop* _prop) {
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
 _r =  bn2 ( _threadargscomma_ *getarg(1) );
 return(_r);
}
 
double ak ( _internalthreadargsprotocomma_ double _lVm ) {
   double _lak;
 double _lx ;
  _lx = 0.066667 * ( _lVm - ( - 25.0 + vshiftak ) ) ;
   if ( fabs ( _lx ) > 1e-6 ) {
     _lak = 0.75 * _lx / ( 1.0 - exp ( - _lx ) ) ;
     }
   else {
     _lak = 0.75 / ( 1.0 + 0.5 * _lx ) ;
     }
    
return _lak;
 }
 
static void _hoc_ak(void) {
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
 _r =  ak ( _threadargscomma_ *getarg(1) );
 hoc_retpushx(_r);
}
 
static double _npy_ak(Prop* _prop) {
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
 _r =  ak ( _threadargscomma_ *getarg(1) );
 return(_r);
}
 
double bk ( _internalthreadargsprotocomma_ double _lVm ) {
   double _lbk;
 double _lx ;
  _lx = - 0.125 * ( _lVm - ( - 15.0 + vshiftak ) ) ;
   if ( fabs ( _lx ) > 1e-6 ) {
     _lbk = 0.8 * _lx / ( 1.0 - exp ( - _lx ) ) ;
     }
   else {
     _lbk = 0.8 / ( 1.0 + 0.5 * _lx ) ;
     }
    
return _lbk;
 }
 
static void _hoc_bk(void) {
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
 _r =  bk ( _threadargscomma_ *getarg(1) );
 hoc_retpushx(_r);
}
 
static double _npy_bk(Prop* _prop) {
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
 _r =  bk ( _threadargscomma_ *getarg(1) );
 return(_r);
}
 
double al ( _internalthreadargsprotocomma_ double _lVm ) {
   double _lal;
  _lal = 0.00015 * exp ( - 0.066667 * ( _lVm - ( - 13.0 + vshiftak ) ) ) ;
    
return _lal;
 }
 
static void _hoc_al(void) {
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
 _r =  al ( _threadargscomma_ *getarg(1) );
 hoc_retpushx(_r);
}
 
static double _npy_al(Prop* _prop) {
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
 _r =  al ( _threadargscomma_ *getarg(1) );
 return(_r);
}
 
double bl ( _internalthreadargsprotocomma_ double _lVm ) {
   double _lbl;
  _lbl = 0.06 / ( 1.0 + exp ( - ( - 0.083333 ) * ( ( - 68.0 + vshiftak ) - _lVm ) ) ) ;
    
return _lbl;
 }
 
static void _hoc_bl(void) {
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
 _r =  bl ( _threadargscomma_ *getarg(1) );
 hoc_retpushx(_r);
}
 
static double _npy_bl(Prop* _prop) {
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
 _r =  bl ( _threadargscomma_ *getarg(1) );
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
  ena = _ion_ena;
  ek = _ion_ek;
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
  ena = _ion_ena;
  ek = _ion_ek;
 _ode_matsol_instance1(_threadargs_);
 }}

static void initmodel(_internalthreadargsproto_) {
  int _i; double _save;{
  h = h0;
  k = k0;
  l = l0;
  m = m0;
  n2 = n20;
  n1 = n10;
 {
   m = am ( _threadargscomma_ v ) / ( am ( _threadargscomma_ v ) + bm ( _threadargscomma_ v ) ) ;
   h = ah ( _threadargscomma_ v ) / ( ah ( _threadargscomma_ v ) + bh ( _threadargscomma_ v ) ) ;
   n1 = an1 ( _threadargscomma_ v ) / ( an1 ( _threadargscomma_ v ) + bn1 ( _threadargscomma_ v ) ) ;
   n2 = an2 ( _threadargscomma_ v ) / ( an2 ( _threadargscomma_ v ) + bn2 ( _threadargscomma_ v ) ) ;
   k = ak ( _threadargscomma_ v ) / ( ak ( _threadargscomma_ v ) + bk ( _threadargscomma_ v ) ) ;
   l = al ( _threadargscomma_ v ) / ( al ( _threadargscomma_ v ) + bl ( _threadargscomma_ v ) ) ;
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
  ek = _ion_ek;
 initmodel(_threadargs_);
  }
}

static double _nrn_current(_internalthreadargsprotocomma_ double _v) {
double _current=0.; v=_v;
{ {
   gna = gnabar * pow( m , 3.0 ) * h ;
   gkf = gkfbar * pow( n1 , 4.0 ) ;
   gks = gksbar * pow( n2 , 4.0 ) ;
   gka = gkabar * k * l ;
   ina = gna * ( v - ena ) ;
   ik = ( gks + gka ) * ( v - ek ) ;
   }
 _current += ina;
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
  ena = _ion_ena;
  ek = _ion_ek;
 auto const _g_local = _nrn_current(_threadargscomma_ _v + .001);
 	{ double _dik;
 double _dina;
  _dina = ina;
  _dik = ik;
 _rhs = _nrn_current(_threadargscomma_ _v);
  _ion_dinadv += (_dina - ina)/.001 ;
  _ion_dikdv += (_dik - ik)/.001 ;
 	}
 _g = (_g_local - _rhs)/.001;
  _ion_ina += ina ;
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
  ena = _ion_ena;
  ek = _ion_ek;
 {   states(_threadargs_);
  }  }}

}

static void terminal(){}

static void _initlists(){
 int _i; static int _first = 1;
  if (!_first) return;
 _slist1[0] = {m_columnindex, 0};  _dlist1[0] = {Dm_columnindex, 0};
 _slist1[1] = {h_columnindex, 0};  _dlist1[1] = {Dh_columnindex, 0};
 _slist1[2] = {n1_columnindex, 0};  _dlist1[2] = {Dn1_columnindex, 0};
 _slist1[3] = {n2_columnindex, 0};  _dlist1[3] = {Dn2_columnindex, 0};
 _slist1[4] = {k_columnindex, 0};  _dlist1[4] = {Dk_columnindex, 0};
 _slist1[5] = {l_columnindex, 0};  _dlist1[5] = {Dl_columnindex, 0};
_first = 0;
}

#if NMODL_TEXT
static void register_nmodl_text_and_filename(int mech_type) {
    const char* nmodl_filename = "/home/raven/PycharmProjects/Masters_model/Mod_Files_Beining_2017/ichan3.mod";
    const char* nmodl_file_text = 
  ": Na-channels from Aradi and Holmes 1999 as available in ModelDB (transformed into mod file)\n"
  ": Note: \"The Na and KDR parameters in the paper were chosen relative to a\n"
  ": resting potential of 0 mV; we have adjusted them to correspond to a\n"
  ": resting potential of -70 mV.\" \n"
  "\n"
  "NEURON {\n"
  "	SUFFIX ichan3\n"
  "	USEION na READ ena WRITE ina\n"
  "	USEION k READ ek WRITE ik\n"
  "	RANGE ik, ina, gnabar, gna,  gksbar, gks, gkabar, gka, gka,gks, gkfbar, gkf\n"
  "	GLOBAL vshiftak,vshiftks,vshiftna\n"
  "}\n"
  "\n"
  "UNITS {\n"
  "	(S) = (siemens)\n"
  "	(mV) = (millivolt)\n"
  "	(mA) = (milliamp)\n"
  "}\n"
  "\n"
  "PARAMETER {\n"
  "	gnabar (S/cm2) \n"
  "	gkfbar (S/cm2) \n"
  "	gksbar (S/cm2) \n"
  "	gkabar (S/cm2) \n"
  "	vshiftak = 0 (mV)\n"
  "	vshiftks = 0 (mV)\n"
  "	vshiftna = 0 (mV)\n"
  "}\n"
  "\n"
  "ASSIGNED {\n"
  "	v (mV)\n"
  "	ena (mV)\n"
  "	ek (mV)\n"
  "	ina (mA/cm2)\n"
  "	ik (mA/cm2)\n"
  "	gna (S/cm2)\n"
  "	gkf (S/cm2)\n"
  "	gks (S/cm2)\n"
  "	gka (S/cm2)\n"
  "}\n"
  "\n"
  "STATE { m n1 h n2 k l }\n"
  "\n"
  "BREAKPOINT {\n"
  "SOLVE states METHOD cnexp\n"
  "gna = gnabar * m^3*h\n"
  "gkf = gkfbar * n1^4\n"
  "gks = gksbar * n2^4\n"
  "gka = gkabar * k * l\n"
  "ina = gna * (v - ena)\n"
  "ik = (gks+gka) * (v - ek) :gkf\n"
  "}\n"
  "\n"
  "INITIAL {\n"
  ": Assume v has been constant for a long time\n"
  "	m = am(v)/(am(v) + bm(v))\n"
  "	h  = ah(v)/(ah(v) + bh(v))\n"
  "	n1 = an1(v)/(an1(v) + bn1(v))\n"
  "	n2 = an2(v)/(an2(v) + bn2(v))\n"
  "	k = ak(v)/(ak(v) + bk(v))\n"
  "	l = al(v)/(al(v) + bl(v))\n"
  "}\n"
  "\n"
  "DERIVATIVE states {\n"
  "	: Computes state variables at present v & t\n"
  "	m' = (1-m)*am(v) - m*bm(v)\n"
  "	h' = (1-h)*ah(v) - h*bh(v)\n"
  "	n1' = (1-n1)*an1(v) - n1*bn1(v)\n"
  "	n2' = (1-n2)*an2(v) - n2*bn2(v)\n"
  "	k' = (1-k)*ak(v) - k*bk(v)\n"
  "	l' = (1-l)*al(v) - l*bl(v)\n"
  "}\n"
  "\n"
  "FUNCTION am(Vm (mV)) (/ms) {\n"
  "	LOCAL x\n"
  "	UNITSOFF\n"
  "	x = 0.2*(Vm - (-45+vshiftna)) \n"
  "	if(fabs(x) > 1e-6) {\n"
  "		am = 1.5*x/(1-exp(-x))\n"
  "	}else{\n"
  "		am = 1.5/(1+0.5*x)\n"
  "	}\n"
  "	UNITSON\n"
  "}\n"
  "\n"
  "FUNCTION bm(Vm (mV)) (/ms) {\n"
  "	LOCAL x\n"
  "	UNITSOFF\n"
  "	x = -0.2*(Vm - (-17+vshiftna))\n"
  "	if(fabs(x) > 1e-6) {\n"
  "		bm = 1.5*x/(1-exp(-x))\n"
  "	}else{\n"
  "		bm = 1.5/(1+0.5*x)\n"
  "	}\n"
  "	UNITSON\n"
  "}\n"
  "\n"
  "FUNCTION ah(Vm (mV)) (/ms) {\n"
  "	UNITSOFF\n"
  "	ah = 0.23*exp(-0.05*(Vm - (-67+vshiftna)))\n"
  "	UNITSON\n"
  "}\n"
  "\n"
  "FUNCTION bh(Vm (mV)) (/ms) {\n"
  "	LOCAL x\n"
  "	UNITSOFF\n"
  "	x= -(-0.1)*((-14.5+vshiftna)-Vm)	\n"
  "	bh = 3.33/(1+exp(x))\n"
  "	UNITSON\n"
  "}\n"
  "\n"
  "FUNCTION an1(Vm (mV)) (/ms) {\n"
  "	LOCAL x\n"
  "	UNITSOFF\n"
  "	x = 0.16667*(Vm+23)\n"
  "	if(fabs(x) > 1e-6) {\n"
  "		an1 = 0.42*x/(1 - exp(-x))\n"
  "	}else{\n"
  "		an1 = 0.42/(1 +0.5*x)\n"
  "	}\n"
  "	UNITSON\n"
  "}\n"
  "\n"
  "FUNCTION bn1(Vm (mV)) (/ms) {\n"
  "	UNITSOFF\n"
  "	bn1 = 0.264*exp(-0.025*(Vm+48)) \n"
  "	UNITSON\n"
  "}\n"
  "\n"
  "FUNCTION an2(Vm (mV)) (/ms) {\n"
  "	LOCAL x\n"
  "	UNITSOFF\n"
  "	x = 0.16667*(Vm-(-35+vshiftks))\n"
  "	if(fabs(x) > 1e-6) {\n"
  "		an2 = 0.168*x/(1 - exp(-x))\n"
  "	}else{\n"
  "		an2 = 0.42/(1 +0.5*x)\n"
  "	}\n"
  "	UNITSON\n"
  "}\n"
  "\n"
  "FUNCTION bn2(Vm (mV)) (/ms) {\n"
  "	UNITSOFF\n"
  "	bn2 = 0.1056*exp(-0.025*(Vm-(-60+vshiftks)))\n"
  "	UNITSON\n"
  "}\n"
  "\n"
  "FUNCTION ak(Vm (mV)) (/ms) {\n"
  "	LOCAL x\n"
  "	UNITSOFF\n"
  "	x = 0.066667*(Vm-(-25+vshiftak))\n"
  "	if(fabs(x) > 1e-6) {\n"
  "		ak = 0.75*x/(1-exp(-x))\n"
  "	}else{\n"
  "		ak = 0.75/(1+0.5*x)\n"
  "	}\n"
  "	UNITSON\n"
  "}\n"
  "\n"
  "FUNCTION bk(Vm (mV)) (/ms) {\n"
  "	LOCAL x\n"
  "	UNITSOFF\n"
  "	x = -0.125*(Vm-(-15 +vshiftak)) \n"
  "	if(fabs(x) > 1e-6) {\n"
  "		bk = 0.8*x/(1-exp(-x))\n"
  "	}else{\n"
  "		bk = 0.8/(1+0.5*x)\n"
  "	}\n"
  "	UNITSON\n"
  "}\n"
  "\n"
  "FUNCTION al(Vm (mV)) (/ms) {\n"
  "	UNITSOFF\n"
  "	al = 0.00015*exp(-0.066667*(Vm -(-13+vshiftak)))\n"
  "	UNITSON\n"
  "}\n"
  "\n"
  "FUNCTION bl(Vm (mV)) (/ms) {\n"
  "	UNITSOFF\n"
  "	bl = 0.06/(1+exp(-(-0.083333)*((-68+vshiftak)-Vm)))\n"
  "	UNITSON\n"
  "}\n"
  ;
    hoc_reg_nmodl_filename(mech_type, nmodl_filename);
    hoc_reg_nmodl_text(mech_type, nmodl_file_text);
}
#endif
