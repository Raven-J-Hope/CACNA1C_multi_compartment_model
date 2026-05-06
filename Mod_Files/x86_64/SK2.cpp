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
static constexpr auto number_of_floating_point_variables = 35;
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
 
#define nrn_init _nrn_init__SK2
#define _nrn_initial _nrn_initial__SK2
#define nrn_cur _nrn_cur__SK2
#define _nrn_current _nrn_current__SK2
#define nrn_jacob _nrn_jacob__SK2
#define nrn_state _nrn_state__SK2
#define _net_receive _net_receive__SK2 
#define kin kin__SK2 
#define rates rates__SK2 
#define rate rate__SK2 
 
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
#define gk _ml->template fpfield<1>(_iml)
#define gk_columnindex 1
#define ik _ml->template fpfield<2>(_iml)
#define ik_columnindex 2
#define acai _ml->template fpfield<3>(_iml)
#define acai_columnindex 3
#define c1 _ml->template fpfield<4>(_iml)
#define c1_columnindex 4
#define c2 _ml->template fpfield<5>(_iml)
#define c2_columnindex 5
#define c3 _ml->template fpfield<6>(_iml)
#define c3_columnindex 6
#define c4 _ml->template fpfield<7>(_iml)
#define c4_columnindex 7
#define o1 _ml->template fpfield<8>(_iml)
#define o1_columnindex 8
#define o2 _ml->template fpfield<9>(_iml)
#define o2_columnindex 9
#define ek _ml->template fpfield<10>(_iml)
#define ek_columnindex 10
#define cai _ml->template fpfield<11>(_iml)
#define cai_columnindex 11
#define invc1_t _ml->template fpfield<12>(_iml)
#define invc1_t_columnindex 12
#define invc2_t _ml->template fpfield<13>(_iml)
#define invc2_t_columnindex 13
#define invc3_t _ml->template fpfield<14>(_iml)
#define invc3_t_columnindex 14
#define invo1_t _ml->template fpfield<15>(_iml)
#define invo1_t_columnindex 15
#define invo2_t _ml->template fpfield<16>(_iml)
#define invo2_t_columnindex 16
#define diro1_t _ml->template fpfield<17>(_iml)
#define diro1_t_columnindex 17
#define diro2_t _ml->template fpfield<18>(_iml)
#define diro2_t_columnindex 18
#define dirc2_t _ml->template fpfield<19>(_iml)
#define dirc2_t_columnindex 19
#define dirc3_t _ml->template fpfield<20>(_iml)
#define dirc3_t_columnindex 20
#define dirc4_t _ml->template fpfield<21>(_iml)
#define dirc4_t_columnindex 21
#define tcorr _ml->template fpfield<22>(_iml)
#define tcorr_columnindex 22
#define dirc2_t_ca _ml->template fpfield<23>(_iml)
#define dirc2_t_ca_columnindex 23
#define dirc3_t_ca _ml->template fpfield<24>(_iml)
#define dirc3_t_ca_columnindex 24
#define dirc4_t_ca _ml->template fpfield<25>(_iml)
#define dirc4_t_ca_columnindex 25
#define lcai _ml->template fpfield<26>(_iml)
#define lcai_columnindex 26
#define Dc1 _ml->template fpfield<27>(_iml)
#define Dc1_columnindex 27
#define Dc2 _ml->template fpfield<28>(_iml)
#define Dc2_columnindex 28
#define Dc3 _ml->template fpfield<29>(_iml)
#define Dc3_columnindex 29
#define Dc4 _ml->template fpfield<30>(_iml)
#define Dc4_columnindex 30
#define Do1 _ml->template fpfield<31>(_iml)
#define Do1_columnindex 31
#define Do2 _ml->template fpfield<32>(_iml)
#define Do2_columnindex 32
#define v _ml->template fpfield<33>(_iml)
#define v_columnindex 33
#define _g _ml->template fpfield<34>(_iml)
#define _g_columnindex 34
#define _ion_cai *(_ml->dptr_field<0>(_iml))
#define _p_ion_cai static_cast<neuron::container::data_handle<double>>(_ppvar[0])
#define _ion_cao *(_ml->dptr_field<1>(_iml))
#define _p_ion_cao static_cast<neuron::container::data_handle<double>>(_ppvar[1])
#define _ion_ek *(_ml->dptr_field<2>(_iml))
#define _p_ion_ek static_cast<neuron::container::data_handle<double>>(_ppvar[2])
#define _ion_ik *(_ml->dptr_field<3>(_iml))
#define _p_ion_ik static_cast<neuron::container::data_handle<double>>(_ppvar[3])
#define _ion_dikdv *(_ml->dptr_field<4>(_iml))
#define _ion_lcai *(_ml->dptr_field<5>(_iml))
#define _p_ion_lcai static_cast<neuron::container::data_handle<double>>(_ppvar[5])
#define _ion_lcao *(_ml->dptr_field<6>(_iml))
#define _p_ion_lcao static_cast<neuron::container::data_handle<double>>(_ppvar[6])
 /* Thread safe. No static _ml, _iml or _ppvar. */
 static int hoc_nrnpointerindex =  -1;
 static _nrn_mechanism_std_vector<Datum> _extcall_thread;
 static Prop* _extcall_prop;
 /* _prop_id kind of shadows _extcall_prop to allow validity checking. */
 static _nrn_non_owning_id_without_container _prop_id{};
 /* external NEURON variables */
 extern double celsius;
 /* declaration of user functions */
 static void _hoc_rates(void);
 static void _hoc_rate(void);
 static void _hoc_temper(void);
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
 {"setdata_SK2", _hoc_setdata},
 {"rates_SK2", _hoc_rates},
 {"rate_SK2", _hoc_rate},
 {"temper_SK2", _hoc_temper},
 {0, 0}
};
 
/* Direct Python call wrappers to density mechanism functions.*/
 static double _npy_rates(Prop*);
 static double _npy_rate(Prop*);
 static double _npy_temper(Prop*);
 
static NPyDirectMechFunc npy_direct_func_proc[] = {
 {"rates", _npy_rates},
 {"rate", _npy_rate},
 {"temper", _npy_temper},
 {0, 0}
};
#define temper temper_SK2
 extern double temper( _internalthreadargsprotocomma_ double , double );
 /* declare global and static user variables */
 #define gind 0
 #define _gth 0
#define Q10 Q10_SK2
 double Q10 = 3;
#define dirc4 dirc4_SK2
 double dirc4 = 80;
#define dirc3 dirc3_SK2
 double dirc3 = 160;
#define dirc2 dirc2_SK2
 double dirc2 = 200;
#define diro2 diro2_SK2
 double diro2 = 1.2;
#define diro1 diro1_SK2
 double diro1 = 0.16;
#define diff diff_SK2
 double diff = 3;
#define fac fac_SK2
 double fac = 1;
#define invo2 invo2_SK2
 double invo2 = 0.1;
#define invo1 invo1_SK2
 double invo1 = 1;
#define invc3 invc3_SK2
 double invc3 = 0.2;
#define invc2 invc2_SK2
 double invc2 = 0.08;
#define invc1 invc1_SK2
 double invc1 = 0.08;
#define ocontrt ocontrt_SK2
 double ocontrt = 0;
 /* some parameters have upper and lower limits */
 static HocParmLimits _hoc_parm_limits[] = {
 {0, 0, 0}
};
 static HocParmUnits _hoc_parm_units[] = {
 {"Q10_SK2", "1"},
 {"diff_SK2", "1"},
 {"invc1_SK2", "/ms"},
 {"invc2_SK2", "/ms"},
 {"invc3_SK2", "/ms"},
 {"invo1_SK2", "/ms"},
 {"invo2_SK2", "/ms"},
 {"diro1_SK2", "/ms"},
 {"diro2_SK2", "/ms"},
 {"dirc2_SK2", "/ms-mM"},
 {"dirc3_SK2", "/ms-mM"},
 {"dirc4_SK2", "/ms-mM"},
 {"gkbar_SK2", "mho/cm2"},
 {"gk_SK2", "mho/cm2"},
 {"ik_SK2", "mA/cm2"},
 {"acai_SK2", "mM"},
 {0, 0}
};
 static double c40 = 0;
 static double c30 = 0;
 static double c20 = 0;
 static double c10 = 0;
 static double delta_t = 0.01;
 static double o20 = 0;
 static double o10 = 0;
 /* connect global user variables to hoc */
 static DoubScal hoc_scdoub[] = {
 {"Q10_SK2", &Q10_SK2},
 {"diff_SK2", &diff_SK2},
 {"fac_SK2", &fac_SK2},
 {"ocontrt_SK2", &ocontrt_SK2},
 {"invc1_SK2", &invc1_SK2},
 {"invc2_SK2", &invc2_SK2},
 {"invc3_SK2", &invc3_SK2},
 {"invo1_SK2", &invo1_SK2},
 {"invo2_SK2", &invo2_SK2},
 {"diro1_SK2", &diro1_SK2},
 {"diro2_SK2", &diro2_SK2},
 {"dirc2_SK2", &dirc2_SK2},
 {"dirc3_SK2", &dirc3_SK2},
 {"dirc4_SK2", &dirc4_SK2},
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
"SK2",
 "gkbar_SK2",
 0,
 "gk_SK2",
 "ik_SK2",
 "acai_SK2",
 0,
 "c1_SK2",
 "c2_SK2",
 "c3_SK2",
 "c4_SK2",
 "o1_SK2",
 "o2_SK2",
 0,
 0};
 static Symbol* _ca_sym;
 static Symbol* _k_sym;
 static Symbol* _lca_sym;
 
 /* Used by NrnProperty */
 static _nrn_mechanism_std_vector<double> _parm_default{
     0.038, /* gkbar */
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
    assert(_nrn_mechanism_get_num_vars(_prop) == 35);
 	/*initialize range parameters*/
 	gkbar = _parm_default[0]; /* 0.038 */
 	 assert(_nrn_mechanism_get_num_vars(_prop) == 35);
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
 prop_ion = need_memb(_lca_sym);
 nrn_promote(prop_ion, 1, 0);
 	_ppvar[5] = _nrn_mechanism_get_param_handle(prop_ion, 1); /* lcai */
 	_ppvar[6] = _nrn_mechanism_get_param_handle(prop_ion, 2); /* lcao */
 
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

 extern "C" void _SK2_reg() {
	int _vectorized = 1;
  _initlists();
 	ion_reg("ca", -10000.);
 	ion_reg("k", -10000.);
 	ion_reg("lca", 0.0);
 	_ca_sym = hoc_lookup("ca_ion");
 	_k_sym = hoc_lookup("k_ion");
 	_lca_sym = hoc_lookup("lca_ion");
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
                                       _nrn_mechanism_field<double>{"gk"} /* 1 */,
                                       _nrn_mechanism_field<double>{"ik"} /* 2 */,
                                       _nrn_mechanism_field<double>{"acai"} /* 3 */,
                                       _nrn_mechanism_field<double>{"c1"} /* 4 */,
                                       _nrn_mechanism_field<double>{"c2"} /* 5 */,
                                       _nrn_mechanism_field<double>{"c3"} /* 6 */,
                                       _nrn_mechanism_field<double>{"c4"} /* 7 */,
                                       _nrn_mechanism_field<double>{"o1"} /* 8 */,
                                       _nrn_mechanism_field<double>{"o2"} /* 9 */,
                                       _nrn_mechanism_field<double>{"ek"} /* 10 */,
                                       _nrn_mechanism_field<double>{"cai"} /* 11 */,
                                       _nrn_mechanism_field<double>{"invc1_t"} /* 12 */,
                                       _nrn_mechanism_field<double>{"invc2_t"} /* 13 */,
                                       _nrn_mechanism_field<double>{"invc3_t"} /* 14 */,
                                       _nrn_mechanism_field<double>{"invo1_t"} /* 15 */,
                                       _nrn_mechanism_field<double>{"invo2_t"} /* 16 */,
                                       _nrn_mechanism_field<double>{"diro1_t"} /* 17 */,
                                       _nrn_mechanism_field<double>{"diro2_t"} /* 18 */,
                                       _nrn_mechanism_field<double>{"dirc2_t"} /* 19 */,
                                       _nrn_mechanism_field<double>{"dirc3_t"} /* 20 */,
                                       _nrn_mechanism_field<double>{"dirc4_t"} /* 21 */,
                                       _nrn_mechanism_field<double>{"tcorr"} /* 22 */,
                                       _nrn_mechanism_field<double>{"dirc2_t_ca"} /* 23 */,
                                       _nrn_mechanism_field<double>{"dirc3_t_ca"} /* 24 */,
                                       _nrn_mechanism_field<double>{"dirc4_t_ca"} /* 25 */,
                                       _nrn_mechanism_field<double>{"lcai"} /* 26 */,
                                       _nrn_mechanism_field<double>{"Dc1"} /* 27 */,
                                       _nrn_mechanism_field<double>{"Dc2"} /* 28 */,
                                       _nrn_mechanism_field<double>{"Dc3"} /* 29 */,
                                       _nrn_mechanism_field<double>{"Dc4"} /* 30 */,
                                       _nrn_mechanism_field<double>{"Do1"} /* 31 */,
                                       _nrn_mechanism_field<double>{"Do2"} /* 32 */,
                                       _nrn_mechanism_field<double>{"v"} /* 33 */,
                                       _nrn_mechanism_field<double>{"_g"} /* 34 */,
                                       _nrn_mechanism_field<double*>{"_ion_cai", "ca_ion"} /* 0 */,
                                       _nrn_mechanism_field<double*>{"_ion_cao", "ca_ion"} /* 1 */,
                                       _nrn_mechanism_field<double*>{"_ion_ek", "k_ion"} /* 2 */,
                                       _nrn_mechanism_field<double*>{"_ion_ik", "k_ion"} /* 3 */,
                                       _nrn_mechanism_field<double*>{"_ion_dikdv", "k_ion"} /* 4 */,
                                       _nrn_mechanism_field<double*>{"_ion_lcai", "lca_ion"} /* 5 */,
                                       _nrn_mechanism_field<double*>{"_ion_lcao", "lca_ion"} /* 6 */,
                                       _nrn_mechanism_field<int>{"_cvode_ieq", "cvodeieq"} /* 7 */);
  hoc_register_prop_size(_mechtype, 35, 8);
  hoc_register_dparam_semantics(_mechtype, 0, "ca_ion");
  hoc_register_dparam_semantics(_mechtype, 1, "ca_ion");
  hoc_register_dparam_semantics(_mechtype, 2, "k_ion");
  hoc_register_dparam_semantics(_mechtype, 3, "k_ion");
  hoc_register_dparam_semantics(_mechtype, 4, "k_ion");
  hoc_register_dparam_semantics(_mechtype, 5, "lca_ion");
  hoc_register_dparam_semantics(_mechtype, 6, "lca_ion");
  hoc_register_dparam_semantics(_mechtype, 7, "cvodeieq");
 	hoc_register_cvode(_mechtype, _ode_count, _ode_map, _ode_spec, _ode_matsol);
 	hoc_register_tolerance(_mechtype, _hoc_state_tol, &_atollist);
 
    hoc_register_var(hoc_scdoub, hoc_vdoub, hoc_intfunc);
 	ivoc_help("help ?1 SK2 /home/raven/PycharmProjects/Masters/Mod_Files/SK2.mod\n");
 hoc_register_limits(_mechtype, _hoc_parm_limits);
 hoc_register_units(_mechtype, _hoc_parm_units);
 }
static int _reset;
static const char *modelname = "SK2 multi-state model Cerebellum Golgi Cell Model";

static int error;
static int _ninits = 0;
static int _match_recurse=1;
static void _modl_cleanup(){ _match_recurse=1;}
static int rates(_internalthreadargsprotocomma_ double);
static int rate(_internalthreadargsprotocomma_ double);
 
#define _MATELM1(_row,_col) *(_nrn_thread_getelm(static_cast<SparseObj*>(_so), _row + 1, _col + 1))
 
#define _RHS1(_arg) _rhs[_arg+1]
  
#define _linmat1  1
 static int _spth1 = 1;
 static int _cvspth1 = 0;
 
static int _ode_spec1(_internalthreadargsproto_);
/*static int _ode_matsol1(_internalthreadargsproto_);*/
 static neuron::container::field_index _slist1[6], _dlist1[6]; static double *_temp1;
 static int kin (void* _so, double* _rhs, _internalthreadargsproto_);
 
static int kin (void* _so, double* _rhs, _internalthreadargsproto_)
 {int _reset=0;
 {
   double b_flux, f_flux, _term; int _i;
 {int _i; double _dt1 = 1.0/dt;
for(_i=1;_i<6;_i++){
  	_RHS1(_i) = -_dt1*(_ml->data(_iml, _slist1[_i]) - _ml->data(_iml, _dlist1[_i]));
	_MATELM1(_i, _i) = _dt1;
      
} }
 acai = ( lcai ) / diff ;
   if ( acai < cai ) {
     acai = cai ;
     }
   rates ( _threadargscomma_ acai ) ;
   /* ~ c1 <-> c2 ( dirc2_t_ca , invc1_t )*/
 f_flux =  dirc2_t_ca * c1 ;
 b_flux =  invc1_t * c2 ;
 _RHS1( 4) -= (f_flux - b_flux);
 _RHS1( 3) += (f_flux - b_flux);
 
 _term =  dirc2_t_ca ;
 _MATELM1( 4 ,4)  += _term;
 _MATELM1( 3 ,4)  -= _term;
 _term =  invc1_t ;
 _MATELM1( 4 ,3)  -= _term;
 _MATELM1( 3 ,3)  += _term;
 /*REACTION*/
  /* ~ c2 <-> c3 ( dirc3_t_ca , invc2_t )*/
 f_flux =  dirc3_t_ca * c2 ;
 b_flux =  invc2_t * c3 ;
 _RHS1( 3) -= (f_flux - b_flux);
 _RHS1( 2) += (f_flux - b_flux);
 
 _term =  dirc3_t_ca ;
 _MATELM1( 3 ,3)  += _term;
 _MATELM1( 2 ,3)  -= _term;
 _term =  invc2_t ;
 _MATELM1( 3 ,2)  -= _term;
 _MATELM1( 2 ,2)  += _term;
 /*REACTION*/
  /* ~ c3 <-> c4 ( dirc4_t_ca , invc3_t )*/
 f_flux =  dirc4_t_ca * c3 ;
 b_flux =  invc3_t * c4 ;
 _RHS1( 2) -= (f_flux - b_flux);
 _RHS1( 1) += (f_flux - b_flux);
 
 _term =  dirc4_t_ca ;
 _MATELM1( 2 ,2)  += _term;
 _MATELM1( 1 ,2)  -= _term;
 _term =  invc3_t ;
 _MATELM1( 2 ,1)  -= _term;
 _MATELM1( 1 ,1)  += _term;
 /*REACTION*/
  /* ~ c3 <-> o1 ( diro1_t , invo1_t )*/
 f_flux =  diro1_t * c3 ;
 b_flux =  invo1_t * o1 ;
 _RHS1( 2) -= (f_flux - b_flux);
 
 _term =  diro1_t ;
 _MATELM1( 2 ,2)  += _term;
 _term =  invo1_t ;
 _MATELM1( 2 ,0)  -= _term;
 /*REACTION*/
  /* ~ c4 <-> o2 ( diro2_t , invo2_t )*/
 f_flux =  diro2_t * c4 ;
 b_flux =  invo2_t * o2 ;
 _RHS1( 1) -= (f_flux - b_flux);
 _RHS1( 5) += (f_flux - b_flux);
 
 _term =  diro2_t ;
 _MATELM1( 1 ,1)  += _term;
 _MATELM1( 5 ,1)  -= _term;
 _term =  invo2_t ;
 _MATELM1( 1 ,5)  -= _term;
 _MATELM1( 5 ,5)  += _term;
 /*REACTION*/
   /* c1 + c2 + c3 + c4 + o2 + o1 = 1.0 */
 _RHS1(0) =  1.0;
 _MATELM1(0, 0) = 1;
 _RHS1(0) -= o1 ;
 _MATELM1(0, 5) = 1;
 _RHS1(0) -= o2 ;
 _MATELM1(0, 1) = 1;
 _RHS1(0) -= c4 ;
 _MATELM1(0, 2) = 1;
 _RHS1(0) -= c3 ;
 _MATELM1(0, 3) = 1;
 _RHS1(0) -= c2 ;
 _MATELM1(0, 4) = 1;
 _RHS1(0) -= c1 ;
 /*CONSERVATION*/
   } return _reset;
 }
 
double temper ( _internalthreadargsprotocomma_ double _lQ10 , double _lcelsius ) {
   double _ltemper;
 _ltemper = pow( _lQ10 , ( ( _lcelsius - 23.0 ) / 10.0 ) ) ;
   
return _ltemper;
 }
 
static void _hoc_temper(void) {
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
 _r =  temper ( _threadargscomma_ *getarg(1) , *getarg(2) );
 hoc_retpushx(_r);
}
 
static double _npy_temper(Prop* _prop) {
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
 _r =  temper ( _threadargscomma_ *getarg(1) , *getarg(2) );
 return(_r);
}
 
static int  rates ( _internalthreadargsprotocomma_ double _lc ) {
   dirc2_t_ca = dirc2_t * _lc * fac ;
   dirc3_t_ca = dirc3_t * _lc * fac ;
   dirc4_t_ca = dirc4_t * _lc * fac ;
    return 0; }
 
static void _hoc_rates(void) {
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
 
static int  rate ( _internalthreadargsprotocomma_ double _lcelsius ) {
   tcorr = temper ( _threadargscomma_ Q10 , _lcelsius ) ;
   invc1_t = invc1 * tcorr ;
   invc2_t = invc2 * tcorr ;
   invc3_t = invc3 * tcorr ;
   invo1_t = invo1 * tcorr ;
   invo2_t = invo2 * tcorr ;
   diro1_t = diro1 * tcorr ;
   diro2_t = diro2 * tcorr ;
   dirc2_t = dirc2 * tcorr ;
   dirc3_t = dirc3 * tcorr ;
   dirc4_t = dirc4 * tcorr ;
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
 
/*CVODE ode begin*/
 static int _ode_spec1(_internalthreadargsproto_) {
  int _reset=0;
  {
 double b_flux, f_flux, _term; int _i;
 {int _i; for(_i=0;_i<6;_i++) _ml->data(_iml, _dlist1[_i]) = 0.0;}
 acai = ( lcai ) / diff ;
 if ( acai < cai ) {
   acai = cai ;
   }
 rates ( _threadargscomma_ acai ) ;
 /* ~ c1 <-> c2 ( dirc2_t_ca , invc1_t )*/
 f_flux =  dirc2_t_ca * c1 ;
 b_flux =  invc1_t * c2 ;
 Dc1 -= (f_flux - b_flux);
 Dc2 += (f_flux - b_flux);
 
 /*REACTION*/
  /* ~ c2 <-> c3 ( dirc3_t_ca , invc2_t )*/
 f_flux =  dirc3_t_ca * c2 ;
 b_flux =  invc2_t * c3 ;
 Dc2 -= (f_flux - b_flux);
 Dc3 += (f_flux - b_flux);
 
 /*REACTION*/
  /* ~ c3 <-> c4 ( dirc4_t_ca , invc3_t )*/
 f_flux =  dirc4_t_ca * c3 ;
 b_flux =  invc3_t * c4 ;
 Dc3 -= (f_flux - b_flux);
 Dc4 += (f_flux - b_flux);
 
 /*REACTION*/
  /* ~ c3 <-> o1 ( diro1_t , invo1_t )*/
 f_flux =  diro1_t * c3 ;
 b_flux =  invo1_t * o1 ;
 Dc3 -= (f_flux - b_flux);
 Do1 += (f_flux - b_flux);
 
 /*REACTION*/
  /* ~ c4 <-> o2 ( diro2_t , invo2_t )*/
 f_flux =  diro2_t * c4 ;
 b_flux =  invo2_t * o2 ;
 Dc4 -= (f_flux - b_flux);
 Do2 += (f_flux - b_flux);
 
 /*REACTION*/
   /* c1 + c2 + c3 + c4 + o2 + o1 = 1.0 */
 /*CONSERVATION*/
   } return _reset;
 }
 
/*CVODE matsol*/
 static int _ode_matsol1(void* _so, double* _rhs, _internalthreadargsproto_) {int _reset=0;{
 double b_flux, f_flux, _term; int _i;
   b_flux = f_flux = 0.;
 {int _i; double _dt1 = 1.0/dt;
for(_i=0;_i<6;_i++){
  	_RHS1(_i) = _dt1*(_ml->data(_iml, _dlist1[_i]));
	_MATELM1(_i, _i) = _dt1;
      
} }
 acai = ( lcai ) / diff ;
 if ( acai < cai ) {
 acai = cai ;
 }
 rates ( _threadargscomma_ acai ) ;
 /* ~ c1 <-> c2 ( dirc2_t_ca , invc1_t )*/
 _term =  dirc2_t_ca ;
 _MATELM1( 4 ,4)  += _term;
 _MATELM1( 3 ,4)  -= _term;
 _term =  invc1_t ;
 _MATELM1( 4 ,3)  -= _term;
 _MATELM1( 3 ,3)  += _term;
 /*REACTION*/
  /* ~ c2 <-> c3 ( dirc3_t_ca , invc2_t )*/
 _term =  dirc3_t_ca ;
 _MATELM1( 3 ,3)  += _term;
 _MATELM1( 2 ,3)  -= _term;
 _term =  invc2_t ;
 _MATELM1( 3 ,2)  -= _term;
 _MATELM1( 2 ,2)  += _term;
 /*REACTION*/
  /* ~ c3 <-> c4 ( dirc4_t_ca , invc3_t )*/
 _term =  dirc4_t_ca ;
 _MATELM1( 2 ,2)  += _term;
 _MATELM1( 1 ,2)  -= _term;
 _term =  invc3_t ;
 _MATELM1( 2 ,1)  -= _term;
 _MATELM1( 1 ,1)  += _term;
 /*REACTION*/
  /* ~ c3 <-> o1 ( diro1_t , invo1_t )*/
 _term =  diro1_t ;
 _MATELM1( 2 ,2)  += _term;
 _MATELM1( 0 ,2)  -= _term;
 _term =  invo1_t ;
 _MATELM1( 2 ,0)  -= _term;
 _MATELM1( 0 ,0)  += _term;
 /*REACTION*/
  /* ~ c4 <-> o2 ( diro2_t , invo2_t )*/
 _term =  diro2_t ;
 _MATELM1( 1 ,1)  += _term;
 _MATELM1( 5 ,1)  -= _term;
 _term =  invo2_t ;
 _MATELM1( 1 ,5)  -= _term;
 _MATELM1( 5 ,5)  += _term;
 /*REACTION*/
   /* c1 + c2 + c3 + c4 + o2 + o1 = 1.0 */
 /*CONSERVATION*/
   } return _reset;
 }
 
/*CVODE end*/
 
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
  cai = _ion_cai;
  ek = _ion_ek;
  lcai = _ion_lcai;
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
 _cvode_sparse_thread(&(_thread[_cvspth1].literal_value<void*>()), 6, _dlist1, neuron::scopmath::row_view{_ml, _iml}, _ode_matsol1, _threadargs_);
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
  lcai = _ion_lcai;
 _ode_matsol_instance1(_threadargs_);
 }}
 
static void _thread_cleanup(Datum* _thread) {
   _nrn_destroy_sparseobj_thread(static_cast<SparseObj*>(_thread[_cvspth1].get<void*>()));
   _nrn_destroy_sparseobj_thread(static_cast<SparseObj*>(_thread[_spth1].get<void*>()));
 }

static void initmodel(_internalthreadargsproto_) {
  int _i; double _save;{
  c4 = c40;
  c3 = c30;
  c2 = c20;
  c1 = c10;
  o2 = o20;
  o1 = o10;
 {
   rate ( _threadargscomma_ celsius ) ;
    _ss_sparse_thread(&(_thread[_spth1].literal_value<void*>()), 6, _slist1, _dlist1, neuron::scopmath::row_view{_ml, _iml}, &t, dt, kin, _linmat1, _threadargs_);
     if (secondorder) {
    int _i;
    for (_i = 0; _i < 6; ++_i) {
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
  cai = _ion_cai;
  ek = _ion_ek;
  lcai = _ion_lcai;
 initmodel(_threadargs_);
 }
}

static double _nrn_current(_internalthreadargsprotocomma_ double _v) {
double _current=0.; v=_v;
{ {
   gk = gkbar * ( o1 + o2 ) ;
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
  cai = _ion_cai;
  ek = _ion_ek;
  lcai = _ion_lcai;
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
  cai = _ion_cai;
  ek = _ion_ek;
  lcai = _ion_lcai;
 {  sparse_thread(&(_thread[_spth1].literal_value<void*>()), 6, _slist1, _dlist1, neuron::scopmath::row_view{_ml, _iml}, &t, dt, kin, _linmat1, _threadargs_);
     if (secondorder) {
    int _i;
    for (_i = 0; _i < 6; ++_i) {
      _ml->data(_iml, _slist1[_i]) += dt*_ml->data(_iml, _dlist1[_i]);
    }}
 } }}
 dt = _dtsav;
}

static void terminal(){}

static void _initlists(){
 int _i; static int _first = 1;
  if (!_first) return;
 _slist1[0] = {o1_columnindex, 0};  _dlist1[0] = {Do1_columnindex, 0};
 _slist1[1] = {c4_columnindex, 0};  _dlist1[1] = {Dc4_columnindex, 0};
 _slist1[2] = {c3_columnindex, 0};  _dlist1[2] = {Dc3_columnindex, 0};
 _slist1[3] = {c2_columnindex, 0};  _dlist1[3] = {Dc2_columnindex, 0};
 _slist1[4] = {c1_columnindex, 0};  _dlist1[4] = {Dc1_columnindex, 0};
 _slist1[5] = {o2_columnindex, 0};  _dlist1[5] = {Do2_columnindex, 0};
_first = 0;
}

#if NMODL_TEXT
static void register_nmodl_text_and_filename(int mech_type) {
    const char* nmodl_filename = "/home/raven/PycharmProjects/Masters/Mod_Files/SK2.mod";
    const char* nmodl_file_text = 
  "TITLE SK2 multi-state model Cerebellum Golgi Cell Model\n"
  "\n"
  "COMMENT\n"
  "\n"
  "Author:Sergio Solinas, Lia Forti, Egidio DAngelo\n"
  "Based on data from: Hirschberg, Maylie, Adelman, Marrion J Gen Physiol 1998\n"
  "\n"
  "Mar 2016 - added instant L-type Calcium concentration (Beining et al (2016), \"A novel comprehensive and consistent electrophysiologcal model of dentate granule cells\")\n"
  "\n"
  "\n"
  "Published in:\n"
  "             Sergio M. Solinas, Lia Forti, Elisabetta Cesana, \n"
  "             Jonathan Mapelli, Erik De Schutter and Egidio D`Angelo (2008)\n"
  "             Computational reconstruction of pacemaking and intrinsic \n"
  "             electroresponsiveness in cerebellar golgi cells\n"
  "             Frontiers in Cellular Neuroscience 2:2\n"
  "ENDCOMMENT\n"
  "\n"
  "NEURON{\n"
  "	SUFFIX SK2\n"
  "	USEION ca READ cai\n"
  "	USEION k READ ek WRITE ik \n"
  "	USEION lca READ lcai VALENCE 0\n"
  "	RANGE gkbar, gk, ik, acai\n"
  "	GLOBAL diff, Q10, fac, invc1,invc2,invc3,invo1,invo2,diro1,diro2,dirc2,dirc3,dirc4\n"
  "}\n"
  "\n"
  "UNITS {\n"
  "	(mA) = (milliamp)\n"
  "	(mV) = (millivolt)\n"
  "	(molar) = (1/liter)\n"
  "	(mM) = (millimolar)\n"
  "}\n"
  "\n"
  "PARAMETER {\n"
  "	gkbar = 0.038 (mho/cm2)\n"
  "	Q10 = 3 (1)\n"
  "	diff = 3 (1) : diffusion factor, is also 3 in the original Solinas model\n"
  "	fac = 1 : is 1 or 0.15 in the original model , dependent on whether using high or low open probability model\n"
  "	ocontrt = 0\n"
  ": rates ca-independent\n"
  "	invc1 = 0.08  ( /ms)\n"
  "	invc2 = 0.08  ( /ms)\n"
  "	invc3 = 0.2 ( /ms)\n"
  "\n"
  "	invo1 = 1.0     ( /ms)\n"
  "	invo2 = 0.1     ( /ms)\n"
  "	diro1 = 0.16 ( /ms)\n"
  "	diro2 = 1.2    ( /ms)\n"
  "\n"
  ": rates ca-dependent\n"
  "	dirc2 = 200 ( /ms-mM )\n"
  "	dirc3 = 160 ( /ms-mM )\n"
  "	dirc4 = 80  ( /ms-mM )\n"
  "\n"
  "}\n"
  "\n"
  "ASSIGNED{\n"
  "	v	(mV)\n"
  "	ek	(mV)\n"
  "	cai (mM)\n"
  "	celsius		(degC)\n"
  "	gk	(mho/cm2)\n"
  "	ik	(mA/cm2)\n"
  "	invc1_t  ( /ms)\n"
  "	invc2_t  ( /ms)\n"
  "	invc3_t  ( /ms)\n"
  "	invo1_t  ( /ms)\n"
  "	invo2_t  ( /ms)\n"
  "	diro1_t  ( /ms)\n"
  "	diro2_t  ( /ms)\n"
  "	dirc2_t  ( /ms-mM)\n"
  "	dirc3_t  ( /ms-mM)\n"
  "	dirc4_t  ( /ms-mM)\n"
  "	tcorr	 (1)\n"
  "\n"
  "	dirc2_t_ca  ( /ms)\n"
  "	dirc3_t_ca  ( /ms)\n"
  "	dirc4_t_ca  ( /ms)\n"
  "\n"
  "	lcai		(mM)\n"
  "	acai     (mM)\n"
  "	:ilca		(mA/cm2) : instantaneous calcium current of l-type calcium channel\n"
  "	:diam	(um)\n"
  "	:VSR (um)\n"
  "}\n"
  "\n"
  "STATE {\n"
  "	c1\n"
  "	c2\n"
  "	c3\n"
  "	c4\n"
  "	o1\n"
  "	o2\n"
  "}\n"
  "\n"
  "BREAKPOINT{\n"
  "	SOLVE kin METHOD sparse\n"
  "	gk = gkbar*(o1+o2)	:(mho/cm2)\n"
  "	ik = gk*(v-ek)		:(mA/cm2)\n"
  "}\n"
  "\n"
  "INITIAL{\n"
  "	rate(celsius)\n"
  "	SOLVE kin STEADYSTATE sparse\n"
  "\n"
  "}\n"
  "\n"
  "KINETIC kin{\n"
  "	acai =  (lcai)/diff : instantaneous calcium concentration of L-type Ca channels for SK activation divided by diffusion factor (determines proximity between both channels) of SK\n"
  "\n"
  "	if (acai < cai)\n"
  "		{acai = cai}\n"
  "	rates(acai)\n"
  "\n"
  "	~c1<->c2 (dirc2_t_ca, invc1_t)\n"
  "	~c2<->c3 (dirc3_t_ca, invc2_t)\n"
  "	~c3<->c4 (dirc4_t_ca, invc3_t)\n"
  "	~c3<->o1 (diro1_t, invo1_t)\n"
  "	~c4<->o2 (diro2_t, invo2_t)\n"
  "	CONSERVE c1+c2+c3+c4+o2+o1=1\n"
  "}\n"
  "\n"
  "FUNCTION temper (Q10, celsius (degC)) {\n"
  "	temper = Q10^((celsius -23(degC)) / 10(degC))\n"
  "}\n"
  "\n"
  "PROCEDURE rates(c(mM)){\n"
  "	dirc2_t_ca = dirc2_t*c * fac\n"
  "	dirc3_t_ca = dirc3_t*c * fac\n"
  "	dirc4_t_ca = dirc4_t*c * fac\n"
  "} \n"
  "\n"
  "PROCEDURE rate (celsius(degC)) {\n"
  "	tcorr = temper (Q10,celsius)\n"
  "	invc1_t = invc1*tcorr  \n"
  "	invc2_t = invc2*tcorr\n"
  "	invc3_t = invc3*tcorr \n"
  "	invo1_t = invo1*tcorr \n"
  "	invo2_t = invo2*tcorr \n"
  "	diro1_t = diro1*tcorr \n"
  "	diro2_t = diro2*tcorr \n"
  "	dirc2_t = dirc2*tcorr\n"
  "	dirc3_t = dirc3*tcorr\n"
  "	dirc4_t = dirc4*tcorr\n"
  "}\n"
  "\n"
  "\n"
  "\n"
  "\n"
  "\n"
  "\n"
  ;
    hoc_reg_nmodl_filename(mech_type, nmodl_filename);
    hoc_reg_nmodl_text(mech_type, nmodl_file_text);
}
#endif
