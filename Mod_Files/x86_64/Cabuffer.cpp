/* Created by Language version: 7.7.0 */
/* NOT VECTORIZED */
#define NRN_VECTORIZED 0
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
#include <vector>
using std::size_t;
static auto& std_cerr_stream = std::cerr;
static constexpr auto number_of_datum_variables = 21;
static constexpr auto number_of_floating_point_variables = 16;
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
 
#define nrn_init _nrn_init__Cabuffer
#define _nrn_initial _nrn_initial__Cabuffer
#define nrn_cur _nrn_cur__Cabuffer
#define _nrn_current _nrn_current__Cabuffer
#define nrn_jacob _nrn_jacob__Cabuffer
#define nrn_state _nrn_state__Cabuffer
#define _net_receive _net_receive__Cabuffer 
#define state state__Cabuffer 
 
#define _threadargscomma_ /**/
#define _threadargsprotocomma_ /**/
#define _internalthreadargsprotocomma_ /**/
#define _threadargs_ /**/
#define _threadargsproto_ /**/
#define _internalthreadargsproto_ /**/
 	/*SUPPRESS 761*/
	/*SUPPRESS 762*/
	/*SUPPRESS 763*/
	/*SUPPRESS 765*/
	 extern double *hoc_getarg(int);
 
#define t nrn_threads->_t
#define dt nrn_threads->_dt
#define tau _ml->template fpfield<0>(_iml)
#define tau_columnindex 0
#define brat _ml->template fpfield<1>(_iml)
#define brat_columnindex 1
#define ica _ml->template fpfield<2>(_iml)
#define ica_columnindex 2
#define VSR _ml->template fpfield<3>(_iml)
#define VSR_columnindex 3
#define ncai _ml->template fpfield<4>(_iml)
#define ncai_columnindex 4
#define inca _ml->template fpfield<5>(_iml)
#define inca_columnindex 5
#define lcai _ml->template fpfield<6>(_iml)
#define lcai_columnindex 6
#define ilca _ml->template fpfield<7>(_iml)
#define ilca_columnindex 7
#define tcai _ml->template fpfield<8>(_iml)
#define tcai_columnindex 8
#define itca _ml->template fpfield<9>(_iml)
#define itca_columnindex 9
#define B _ml->template fpfield<10>(_iml)
#define B_columnindex 10
#define cai _ml->template fpfield<11>(_iml)
#define cai_columnindex 11
#define Dcai _ml->template fpfield<12>(_iml)
#define Dcai_columnindex 12
#define cao _ml->template fpfield<13>(_iml)
#define cao_columnindex 13
#define Dcao _ml->template fpfield<14>(_iml)
#define Dcao_columnindex 14
#define _g _ml->template fpfield<15>(_iml)
#define _g_columnindex 15
#define _ion_ica *(_ml->dptr_field<0>(_iml))
#define _p_ion_ica static_cast<neuron::container::data_handle<double>>(_ppvar[0])
#define _ion_cai *(_ml->dptr_field<1>(_iml))
#define _p_ion_cai static_cast<neuron::container::data_handle<double>>(_ppvar[1])
#define _ion_cao *(_ml->dptr_field<2>(_iml))
#define _p_ion_cao static_cast<neuron::container::data_handle<double>>(_ppvar[2])
#define _ion_ca_erev *_ml->dptr_field<3>(_iml)
#define _style_ca	*_ppvar[4].get<int*>()
#define _ion_inca *(_ml->dptr_field<5>(_iml))
#define _p_ion_inca static_cast<neuron::container::data_handle<double>>(_ppvar[5])
#define _ion_ncao *(_ml->dptr_field<6>(_iml))
#define _p_ion_ncao static_cast<neuron::container::data_handle<double>>(_ppvar[6])
#define _ion_ncai *(_ml->dptr_field<7>(_iml))
#define _p_ion_ncai static_cast<neuron::container::data_handle<double>>(_ppvar[7])
#define _ion_nca_erev *_ml->dptr_field<8>(_iml)
#define _style_nca	*_ppvar[9].get<int*>()
#define _ion_ilca *(_ml->dptr_field<10>(_iml))
#define _p_ion_ilca static_cast<neuron::container::data_handle<double>>(_ppvar[10])
#define _ion_lcao *(_ml->dptr_field<11>(_iml))
#define _p_ion_lcao static_cast<neuron::container::data_handle<double>>(_ppvar[11])
#define _ion_lcai *(_ml->dptr_field<12>(_iml))
#define _p_ion_lcai static_cast<neuron::container::data_handle<double>>(_ppvar[12])
#define _ion_lca_erev *_ml->dptr_field<13>(_iml)
#define _style_lca	*_ppvar[14].get<int*>()
#define _ion_itca *(_ml->dptr_field<15>(_iml))
#define _p_ion_itca static_cast<neuron::container::data_handle<double>>(_ppvar[15])
#define _ion_tcao *(_ml->dptr_field<16>(_iml))
#define _p_ion_tcao static_cast<neuron::container::data_handle<double>>(_ppvar[16])
#define _ion_tcai *(_ml->dptr_field<17>(_iml))
#define _p_ion_tcai static_cast<neuron::container::data_handle<double>>(_ppvar[17])
#define _ion_tca_erev *_ml->dptr_field<18>(_iml)
#define _style_tca	*_ppvar[19].get<int*>()
#define diam	(*(_ml->dptr_field<20>(_iml)))
 static _nrn_mechanism_cache_instance _ml_real{nullptr};
static _nrn_mechanism_cache_range *_ml{&_ml_real};
static size_t _iml{0};
static Datum *_ppvar;
 static int hoc_nrnpointerindex =  -1;
 static Prop* _extcall_prop;
 /* _prop_id kind of shadows _extcall_prop to allow validity checking. */
 static _nrn_non_owning_id_without_container _prop_id{};
 /* external NEURON variables */
 /* declaration of user functions */
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
 {"setdata_Cabuffer", _hoc_setdata},
 {0, 0}
};
 
/* Direct Python call wrappers to density mechanism functions.*/
 
static NPyDirectMechFunc npy_direct_func_proc[] = {
 {0, 0}
};
 /* declare global and static user variables */
 #define gind 0
 #define _gth 0
#define Fa Fa_Cabuffer
 double Fa = 96485.3;
#define cai0 cai0_Cabuffer
 double cai0 = 0;
#define cao0 cao0_Cabuffer
 double cao0 = 0;
#define depth depth_Cabuffer
 double depth = 0.2;
 /* some parameters have upper and lower limits */
 static HocParmLimits _hoc_parm_limits[] = {
 {0, 0, 0}
};
 static HocParmUnits _hoc_parm_units[] = {
 {"depth_Cabuffer", "um"},
 {"cai0_Cabuffer", "mM"},
 {"cao0_Cabuffer", "mM"},
 {"Fa_Cabuffer", "coulomb"},
 {"tau_Cabuffer", "ms"},
 {0, 0}
};
 static double delta_t = 0.01;
 static double v = 0;
 /* connect global user variables to hoc */
 static DoubScal hoc_scdoub[] = {
 {"depth_Cabuffer", &depth_Cabuffer},
 {"cai0_Cabuffer", &cai0_Cabuffer},
 {"cao0_Cabuffer", &cao0_Cabuffer},
 {"Fa_Cabuffer", &Fa_Cabuffer},
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
 neuron::legacy::set_globals_from_prop(_prop, _ml_real, _ml, _iml);
_ppvar = _nrn_mechanism_access_dparam(_prop);
 Node * _node = _nrn_mechanism_access_node(_prop);
v = _nrn_mechanism_access_voltage(_node);
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
 
#define _cvode_ieq _ppvar[21].literal_value<int>()
 static void _ode_synonym(_nrn_model_sorted_token const&, NrnThread&, Memb_list&, int);
 static void _ode_matsol_instance1(_internalthreadargsproto_);
 /* connect range variables in _p that hoc is supposed to know about */
 static const char *_mechanism[] = {
 "7.7.0",
"Cabuffer",
 "tau_Cabuffer",
 "brat_Cabuffer",
 0,
 0,
 0,
 0};
 static Symbol* _morphology_sym;
 static Symbol* _ca_sym;
 static Symbol* _nca_sym;
 static Symbol* _lca_sym;
 static Symbol* _tca_sym;
 
 /* Used by NrnProperty */
 static _nrn_mechanism_std_vector<double> _parm_default{
     9, /* tau */
     1, /* brat */
 }; 
 
 
extern Prop* need_memb(Symbol*);
static void nrn_alloc(Prop* _prop) {
  Prop *prop_ion{};
  Datum *_ppvar{};
   _ppvar = nrn_prop_datum_alloc(_mechtype, 22, _prop);
    _nrn_mechanism_access_dparam(_prop) = _ppvar;
     _nrn_mechanism_cache_instance _ml_real{_prop};
    auto* const _ml = &_ml_real;
    size_t const _iml{};
    assert(_nrn_mechanism_get_num_vars(_prop) == 16);
 	/*initialize range parameters*/
 	tau = _parm_default[0]; /* 9 */
 	brat = _parm_default[1]; /* 1 */
 	 assert(_nrn_mechanism_get_num_vars(_prop) == 16);
 	_nrn_mechanism_access_dparam(_prop) = _ppvar;
 	/*connect ionic variables to this model*/
 prop_ion = need_memb(_morphology_sym);
 	_ppvar[20] = _nrn_mechanism_get_param_handle(prop_ion, 0); /* diam */
 prop_ion = need_memb(_ca_sym);
 nrn_check_conc_write(_prop, prop_ion, 1);
 nrn_check_conc_write(_prop, prop_ion, 0);
 nrn_promote(prop_ion, 3, 0);
 	_ppvar[0] = _nrn_mechanism_get_param_handle(prop_ion, 3); /* ica */
 	_ppvar[1] = _nrn_mechanism_get_param_handle(prop_ion, 1); /* cai */
 	_ppvar[2] = _nrn_mechanism_get_param_handle(prop_ion, 2); /* cao */
 	_ppvar[3] = _nrn_mechanism_get_param_handle(prop_ion, 0); // erev ca
 	_ppvar[4] = {neuron::container::do_not_search, &(_nrn_mechanism_access_dparam(prop_ion)[0].literal_value<int>())}; /* iontype for ca */
 prop_ion = need_memb(_nca_sym);
 nrn_check_conc_write(_prop, prop_ion, 1);
 nrn_promote(prop_ion, 3, 0);
 	_ppvar[5] = _nrn_mechanism_get_param_handle(prop_ion, 3); /* inca */
 	_ppvar[6] = _nrn_mechanism_get_param_handle(prop_ion, 2); /* ncao */
 	_ppvar[7] = _nrn_mechanism_get_param_handle(prop_ion, 1); /* ncai */
 	_ppvar[8] = _nrn_mechanism_get_param_handle(prop_ion, 0); // erev nca
 	_ppvar[9] = {neuron::container::do_not_search, &(_nrn_mechanism_access_dparam(prop_ion)[0].literal_value<int>())}; /* iontype for nca */
 prop_ion = need_memb(_lca_sym);
 nrn_check_conc_write(_prop, prop_ion, 1);
 nrn_promote(prop_ion, 3, 0);
 	_ppvar[10] = _nrn_mechanism_get_param_handle(prop_ion, 3); /* ilca */
 	_ppvar[11] = _nrn_mechanism_get_param_handle(prop_ion, 2); /* lcao */
 	_ppvar[12] = _nrn_mechanism_get_param_handle(prop_ion, 1); /* lcai */
 	_ppvar[13] = _nrn_mechanism_get_param_handle(prop_ion, 0); // erev lca
 	_ppvar[14] = {neuron::container::do_not_search, &(_nrn_mechanism_access_dparam(prop_ion)[0].literal_value<int>())}; /* iontype for lca */
 prop_ion = need_memb(_tca_sym);
 nrn_check_conc_write(_prop, prop_ion, 1);
 nrn_promote(prop_ion, 3, 0);
 	_ppvar[15] = _nrn_mechanism_get_param_handle(prop_ion, 3); /* itca */
 	_ppvar[16] = _nrn_mechanism_get_param_handle(prop_ion, 2); /* tcao */
 	_ppvar[17] = _nrn_mechanism_get_param_handle(prop_ion, 1); /* tcai */
 	_ppvar[18] = _nrn_mechanism_get_param_handle(prop_ion, 0); // erev tca
 	_ppvar[19] = {neuron::container::do_not_search, &(_nrn_mechanism_access_dparam(prop_ion)[0].literal_value<int>())}; /* iontype for tca */
 
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

 extern "C" void _Cabuffer_reg() {
	int _vectorized = 0;
  _initlists();
 	ion_reg("ca", -10000.);
 	ion_reg("nca", 0.0);
 	ion_reg("lca", 0.0);
 	ion_reg("tca", 0.0);
 	_morphology_sym = hoc_lookup("morphology");
 	_ca_sym = hoc_lookup("ca_ion");
 	_nca_sym = hoc_lookup("nca_ion");
 	_lca_sym = hoc_lookup("lca_ion");
 	_tca_sym = hoc_lookup("tca_ion");
 	register_mech(_mechanism, nrn_alloc,nrn_cur, nrn_jacob, nrn_state, nrn_init, hoc_nrnpointerindex, 0);
 _mechtype = nrn_get_mechtype(_mechanism[1]);
 hoc_register_parm_default(_mechtype, &_parm_default);
         hoc_register_npy_direct(_mechtype, npy_direct_func_proc);
     _nrn_setdata_reg(_mechtype, _setdata);
 #if NMODL_TEXT
  register_nmodl_text_and_filename(_mechtype);
#endif
   _nrn_mechanism_register_data_fields(_mechtype,
                                       _nrn_mechanism_field<double>{"tau"} /* 0 */,
                                       _nrn_mechanism_field<double>{"brat"} /* 1 */,
                                       _nrn_mechanism_field<double>{"ica"} /* 2 */,
                                       _nrn_mechanism_field<double>{"VSR"} /* 3 */,
                                       _nrn_mechanism_field<double>{"ncai"} /* 4 */,
                                       _nrn_mechanism_field<double>{"inca"} /* 5 */,
                                       _nrn_mechanism_field<double>{"lcai"} /* 6 */,
                                       _nrn_mechanism_field<double>{"ilca"} /* 7 */,
                                       _nrn_mechanism_field<double>{"tcai"} /* 8 */,
                                       _nrn_mechanism_field<double>{"itca"} /* 9 */,
                                       _nrn_mechanism_field<double>{"B"} /* 10 */,
                                       _nrn_mechanism_field<double>{"cai"} /* 11 */,
                                       _nrn_mechanism_field<double>{"Dcai"} /* 12 */,
                                       _nrn_mechanism_field<double>{"cao"} /* 13 */,
                                       _nrn_mechanism_field<double>{"Dcao"} /* 14 */,
                                       _nrn_mechanism_field<double>{"_g"} /* 15 */,
                                       _nrn_mechanism_field<double*>{"_ion_ica", "ca_ion"} /* 0 */,
                                       _nrn_mechanism_field<double*>{"_ion_cai", "ca_ion"} /* 1 */,
                                       _nrn_mechanism_field<double*>{"_ion_cao", "ca_ion"} /* 2 */,
                                       _nrn_mechanism_field<double*>{"_ion_ca_erev", "ca_ion"} /* 3 */,
                                       _nrn_mechanism_field<int*>{"_style_ca", "#ca_ion"} /* 4 */,
                                       _nrn_mechanism_field<double*>{"_ion_inca", "nca_ion"} /* 5 */,
                                       _nrn_mechanism_field<double*>{"_ion_ncao", "nca_ion"} /* 6 */,
                                       _nrn_mechanism_field<double*>{"_ion_ncai", "nca_ion"} /* 7 */,
                                       _nrn_mechanism_field<double*>{"_ion_nca_erev", "nca_ion"} /* 8 */,
                                       _nrn_mechanism_field<int*>{"_style_nca", "#nca_ion"} /* 9 */,
                                       _nrn_mechanism_field<double*>{"_ion_ilca", "lca_ion"} /* 10 */,
                                       _nrn_mechanism_field<double*>{"_ion_lcao", "lca_ion"} /* 11 */,
                                       _nrn_mechanism_field<double*>{"_ion_lcai", "lca_ion"} /* 12 */,
                                       _nrn_mechanism_field<double*>{"_ion_lca_erev", "lca_ion"} /* 13 */,
                                       _nrn_mechanism_field<int*>{"_style_lca", "#lca_ion"} /* 14 */,
                                       _nrn_mechanism_field<double*>{"_ion_itca", "tca_ion"} /* 15 */,
                                       _nrn_mechanism_field<double*>{"_ion_tcao", "tca_ion"} /* 16 */,
                                       _nrn_mechanism_field<double*>{"_ion_tcai", "tca_ion"} /* 17 */,
                                       _nrn_mechanism_field<double*>{"_ion_tca_erev", "tca_ion"} /* 18 */,
                                       _nrn_mechanism_field<int*>{"_style_tca", "#tca_ion"} /* 19 */,
                                       _nrn_mechanism_field<double*>{"diam", "diam"} /* 20 */,
                                       _nrn_mechanism_field<int>{"_cvode_ieq", "cvodeieq"} /* 21 */);
  hoc_register_prop_size(_mechtype, 16, 22);
  hoc_register_dparam_semantics(_mechtype, 0, "ca_ion");
  hoc_register_dparam_semantics(_mechtype, 1, "ca_ion");
  hoc_register_dparam_semantics(_mechtype, 2, "ca_ion");
  hoc_register_dparam_semantics(_mechtype, 3, "ca_ion");
  hoc_register_dparam_semantics(_mechtype, 4, "#ca_ion");
  hoc_register_dparam_semantics(_mechtype, 5, "nca_ion");
  hoc_register_dparam_semantics(_mechtype, 6, "nca_ion");
  hoc_register_dparam_semantics(_mechtype, 7, "nca_ion");
  hoc_register_dparam_semantics(_mechtype, 8, "nca_ion");
  hoc_register_dparam_semantics(_mechtype, 9, "#nca_ion");
  hoc_register_dparam_semantics(_mechtype, 10, "lca_ion");
  hoc_register_dparam_semantics(_mechtype, 11, "lca_ion");
  hoc_register_dparam_semantics(_mechtype, 12, "lca_ion");
  hoc_register_dparam_semantics(_mechtype, 13, "lca_ion");
  hoc_register_dparam_semantics(_mechtype, 14, "#lca_ion");
  hoc_register_dparam_semantics(_mechtype, 15, "tca_ion");
  hoc_register_dparam_semantics(_mechtype, 16, "tca_ion");
  hoc_register_dparam_semantics(_mechtype, 17, "tca_ion");
  hoc_register_dparam_semantics(_mechtype, 18, "tca_ion");
  hoc_register_dparam_semantics(_mechtype, 19, "#tca_ion");
  hoc_register_dparam_semantics(_mechtype, 21, "cvodeieq");
  hoc_register_dparam_semantics(_mechtype, 20, "diam");
 	nrn_writes_conc(_mechtype, 0);
 	hoc_register_cvode(_mechtype, _ode_count, _ode_map, _ode_spec, _ode_matsol);
 	hoc_register_tolerance(_mechtype, _hoc_state_tol, &_atollist);
 	hoc_register_synonym(_mechtype, _ode_synonym);
 
    hoc_register_var(hoc_scdoub, hoc_vdoub, hoc_intfunc);
 	ivoc_help("help ?1 Cabuffer /home/raven/PycharmProjects/Masters_model/Mod_Files_Beining_2017/Cabuffer.mod\n");
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
 static neuron::container::field_index _slist1[2], _dlist1[2];
 static int state(_internalthreadargsproto_);
 
/*CVODE*/
 static int _ode_spec1 () {_reset=0;
 {
   ncai = - inca * B ;
   lcai = - ilca * B ;
   tcai = - itca * B ;
   Dcai = - ica * B / brat - ( cai - cai0 ) / tau ;
   Dcao = 0.0 ;
   }
 return _reset;
}
 static int _ode_matsol1 () {
 ncai = - inca * B ;
 lcai = - ilca * B ;
 tcai = - itca * B ;
 Dcai = Dcai  / (1. - dt*( ( - ( ( 1.0 ) ) / tau ) )) ;
 Dcao = Dcao  / (1. - dt*( 0.0 )) ;
  return 0;
}
 /*END CVODE*/
 static int state () {_reset=0;
 {
   ncai = - inca * B ;
   lcai = - ilca * B ;
   tcai = - itca * B ;
    cai = cai + (1. - exp(dt*(( - ( ( 1.0 ) ) / tau ))))*(- ( ( ( - ica )*( B ) ) / brat - ( ( ( - cai0 ) ) ) / tau ) / ( ( - ( ( 1.0 ) ) / tau ) ) - cai) ;
    cao = cao - dt*(- ( 0.0 ) ) ;
   }
  return 0;
}
 
static int _ode_count(int _type){ return 2;}
 
static void _ode_spec(_nrn_model_sorted_token const& _sorted_token, NrnThread* _nt, Memb_list* _ml_arg, int _type) {
      Node* _nd{};
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
  cai = _ion_cai;
  cao = _ion_cao;
  cai = _ion_cai;
  cao = _ion_cao;
  inca = _ion_inca;
  ncai = _ion_ncai;
  ilca = _ion_ilca;
  lcai = _ion_lcai;
  itca = _ion_itca;
  tcai = _ion_tcai;
     _ode_spec1 ();
  _ion_cai = cai;
  _ion_cao = cao;
  _ion_ncai = ncai;
  _ion_lcai = lcai;
  _ion_tcai = tcai;
 }}
 
static void _ode_map(Prop* _prop, int _ieq, neuron::container::data_handle<double>* _pv, neuron::container::data_handle<double>* _pvdot, double* _atol, int _type) { 
  _ppvar = _nrn_mechanism_access_dparam(_prop);
  _cvode_ieq = _ieq;
  for (int _i=0; _i < 2; ++_i) {
    _pv[_i] = _nrn_mechanism_get_param_handle(_prop, _slist1[_i]);
    _pvdot[_i] = _nrn_mechanism_get_param_handle(_prop, _dlist1[_i]);
    _cvode_abstol(_atollist, _atol, _i);
  }
 	_pv[0] = _p_ion_cai;
 	_pv[1] = _p_ion_cao;
 }
 static void _ode_synonym(_nrn_model_sorted_token const& _sorted_token, NrnThread& _nt, Memb_list& _ml_arg, int _type) {
 _nrn_mechanism_cache_range _lmr{_sorted_token, _nt, _ml_arg, _type};
auto* const _ml = &_lmr;
auto const _cnt = _ml_arg._nodecount;
for (int _iml = 0; _iml < _cnt; ++_iml) {
  Datum* _ppvar = _ml_arg._pdata[_iml];
 _ion_ncai =  - inca * B ;
 _ion_lcai =  - ilca * B ;
 _ion_tcai =  - itca * B ;
   }
}
 
static void _ode_matsol_instance1(_internalthreadargsproto_) {
 _ode_matsol1 ();
 }
 
static void _ode_matsol(_nrn_model_sorted_token const& _sorted_token, NrnThread* _nt, Memb_list* _ml_arg, int _type) {
      Node* _nd{};
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
  cai = _ion_cai;
  cao = _ion_cao;
  cai = _ion_cai;
  cao = _ion_cao;
  inca = _ion_inca;
  ncai = _ion_ncai;
  ilca = _ion_ilca;
  lcai = _ion_lcai;
  itca = _ion_itca;
  tcai = _ion_tcai;
 _ode_matsol_instance1(_threadargs_);
 }}

static void initmodel() {
  int _i; double _save;_ninits++;
 _save = t;
 t = 0.0;
{
 {
   if ( 2.0 * depth >= diam ) {
     VSR = 0.25 * diam ;
     }
   else {
     VSR = depth * ( 1.0 - depth / diam ) ;
     }
   B = ( 1e4 ) / ( 2.0 * Fa * VSR ) ;
   cao0 = cao ;
   cai0 = cai ;
   }
  _sav_indep = t; t = _save;

}
}

static void nrn_init(_nrn_model_sorted_token const& _sorted_token, NrnThread* _nt, Memb_list* _ml_arg, int _type){
Node *_nd; double _v; int* _ni; int _cntml;
_nrn_mechanism_cache_range _lmr{_sorted_token, *_nt, *_ml_arg, _type};
auto* const _vec_v = _nt->node_voltage_storage();
_ml = &_lmr;
_ni = _ml_arg->_nodeindices;
_cntml = _ml_arg->_nodecount;
for (_iml = 0; _iml < _cntml; ++_iml) {
 _ppvar = _ml_arg->_pdata[_iml];
   _v = _vec_v[_ni[_iml]];
 v = _v;
  ica = _ion_ica;
  cai = _ion_cai;
  cao = _ion_cao;
  cai = _ion_cai;
  cao = _ion_cao;
  inca = _ion_inca;
  ncai = _ion_ncai;
  ilca = _ion_ilca;
  lcai = _ion_lcai;
  itca = _ion_itca;
  tcai = _ion_tcai;
 initmodel();
  _ion_cai = cai;
  _ion_cao = cao;
  nrn_wrote_conc(_ca_sym, _ion_ca_erev, _ion_cai, _ion_cao, _style_ca);
  _ion_ncai = ncai;
  nrn_wrote_conc(_nca_sym, _ion_nca_erev, _ion_ncai, _ion_ncao, _style_nca);
  _ion_lcai = lcai;
  nrn_wrote_conc(_lca_sym, _ion_lca_erev, _ion_lcai, _ion_lcao, _style_lca);
  _ion_tcai = tcai;
  nrn_wrote_conc(_tca_sym, _ion_tca_erev, _ion_tcai, _ion_tcao, _style_tca);
}}

static double _nrn_current(double _v){double _current=0.;v=_v;{
} return _current;
}

static void nrn_cur(_nrn_model_sorted_token const& _sorted_token, NrnThread* _nt, Memb_list* _ml_arg, int _type){
_nrn_mechanism_cache_range _lmr{_sorted_token, *_nt, *_ml_arg, _type};
auto const _vec_rhs = _nt->node_rhs_storage();
auto const _vec_sav_rhs = _nt->node_sav_rhs_storage();
auto const _vec_v = _nt->node_voltage_storage();
Node *_nd; int* _ni; double _rhs, _v; int _cntml;
_ml = &_lmr;
_ni = _ml_arg->_nodeindices;
_cntml = _ml_arg->_nodecount;
for (_iml = 0; _iml < _cntml; ++_iml) {
 _ppvar = _ml_arg->_pdata[_iml];
   _v = _vec_v[_ni[_iml]];
 
}}

static void nrn_jacob(_nrn_model_sorted_token const& _sorted_token, NrnThread* _nt, Memb_list* _ml_arg, int _type) {
_nrn_mechanism_cache_range _lmr{_sorted_token, *_nt, *_ml_arg, _type};
auto const _vec_d = _nt->node_d_storage();
auto const _vec_sav_d = _nt->node_sav_d_storage();
auto* const _ml = &_lmr;
Node *_nd; int* _ni; int _iml, _cntml;
_ni = _ml_arg->_nodeindices;
_cntml = _ml_arg->_nodecount;
for (_iml = 0; _iml < _cntml; ++_iml) {
  _vec_d[_ni[_iml]] += _g;
 
}}

static void nrn_state(_nrn_model_sorted_token const& _sorted_token, NrnThread* _nt, Memb_list* _ml_arg, int _type){
Node *_nd; double _v = 0.0; int* _ni; int _cntml;
_nrn_mechanism_cache_range _lmr{_sorted_token, *_nt, *_ml_arg, _type};
auto* const _vec_v = _nt->node_voltage_storage();
_ml = &_lmr;
_ni = _ml_arg->_nodeindices;
_cntml = _ml_arg->_nodecount;
for (_iml = 0; _iml < _cntml; ++_iml) {
 _ppvar = _ml_arg->_pdata[_iml];
 _nd = _ml_arg->_nodelist[_iml];
   _v = _vec_v[_ni[_iml]];
 v=_v;
{
  ica = _ion_ica;
  cai = _ion_cai;
  cao = _ion_cao;
  cai = _ion_cai;
  cao = _ion_cao;
  inca = _ion_inca;
  ncai = _ion_ncai;
  ilca = _ion_ilca;
  lcai = _ion_lcai;
  itca = _ion_itca;
  tcai = _ion_tcai;
 { error =  state();
 if(error){
  std_cerr_stream << "at line 48 in file Cabuffer.mod:\nBREAKPOINT {\n";
  std_cerr_stream << _ml << ' ' << _iml << '\n';
  abort_run(error);
}
 } {
   }
  _ion_cai = cai;
  _ion_cao = cao;
  _ion_ncai = ncai;
  _ion_lcai = lcai;
  _ion_tcai = tcai;
}}

}

static void terminal(){}

static void _initlists() {
 int _i; static int _first = 1;
  if (!_first) return;
 _slist1[0] = {cai_columnindex, 0};  _dlist1[0] = {Dcai_columnindex, 0};
 _slist1[1] = {cao_columnindex, 0};  _dlist1[1] = {Dcao_columnindex, 0};
_first = 0;
}

#if NMODL_TEXT
static void register_nmodl_text_and_filename(int mech_type) {
    const char* nmodl_filename = "/home/raven/PycharmProjects/Masters_model/Mod_Files_Beining_2017/Cabuffer.mod";
    const char* nmodl_file_text = 
  ": Calcium buffer shell model with instant Calcium handling for clustered channels by Beining et al (2016), \"A novel comprehensive and consistent electrophysiologcal model of dentate granule cells\"\n"
  "\n"
  "NEURON {\n"
  "	SUFFIX Cabuffer\n"
  "	USEION ca READ ica,cai,cao WRITE cai, cao\n"
  "	USEION nca READ inca WRITE ncai VALENCE 0\n"
  "	USEION lca READ ilca WRITE lcai VALENCE 0\n"
  "	USEION tca READ itca WRITE tcai VALENCE 0\n"
  "	GLOBAL depth,cao0,cai0\n"
  "	RANGE cai,cao,ncai,lcai,brat, tau\n"
  "}\n"
  "\n"
  "UNITS {\n"
  "	(molar) = (1/liter)\n"
  "	(mM) = (millimolar)\n"
  "	(mV) = (millivolt)\n"
  "	(mA) = (milliamp)\n"
  "	(S) = (siemens)\n"
  "	(um) = (micrometer)\n"
  "}\n"
  "\n"
  "PARAMETER {\n"
  "	tau = 9				(ms)\n"
  "	depth = .2 		(um)\n"
  "	cai0  				(mM)	\n"
  "	cao0   				(mM)	\n"
  "	Fa = 96485.3365 (coulomb)\n"
  "	brat = 1  : binding ratio by buffer\n"
  "}\n"
  "\n"
  "ASSIGNED {\n"
  "	ica		(mA/cm2)\n"
  "	diam	(um)\n"
  "	VSR (um)\n"
  "	ncai		(mM)\n"
  "	inca		(mA/cm2) : instantaneous calcium current of n-type calcium channel\n"
  "	lcai		(mM)\n"
  "	ilca		(mA/cm2) : instantaneous calcium current of l-type calcium channel\n"
  "	tcai		(mM)\n"
  "	itca		(mA/cm2) : instantaneous calcium current of t-type calcium channel\n"
  "	B 			(mM*cm2/mA)\n"
  "}\n"
  "\n"
  "STATE { \n"
  "cai (mM) 		<1e-5> \n"
  "cao (mM)}\n"
  "\n"
  "BREAKPOINT {\n"
  "	SOLVE state METHOD cnexp\n"
  "}\n"
  "\n"
  "DERIVATIVE state {	: exact when v held constant; integrates over dt step\n"
  "	ncai = - inca * B  : instantaneous calcium concentration of N-type Ca channels for BK activation times sensitivity factor of BK\n"
  "	lcai = - ilca * B : instantaneous calcium concentration of N-type Ca channels for BK activation times sensitivity factor of BK\n"
  "	tcai = - itca * B  : instantaneous calcium concentration of N-type Ca channels for BK activation times sensitivity factor of BK\n"
  "	cai' = -ica * B / brat -(cai-cai0)/tau	:(1e4)/(2*Fa*0.25*diam)	 *1e4 is for correction from um to cm to result in /cm3.. *1000 for /liter solves with /1000 for /ms ..  ja 1e4 es stimmt wirklich..june 2016\n"
  "	cao' = 0\n"
  "}\n"
  "\n"
  "INITIAL {\n"
  "	if (2*depth >= diam) {\n"
  "		VSR = 0.25*diam : if diameter gets less than double the depth, the surface to volume ratio (here volume to surface ratio VSR) cannot be less than 1/(0.25diam) (instead of diam/(d*(diam-d)) )\n"
  "	}else{\n"
  "		VSR = depth*(1-depth/diam)\n"
  "	}\n"
  "	B = (1e4)/(2*Fa*VSR)\n"
  "	cao0 	= 		cao\n"
  "	cai0 	= 		cai\n"
  "}\n"
  ;
    hoc_reg_nmodl_filename(mech_type, nmodl_filename);
    hoc_reg_nmodl_text(mech_type, nmodl_file_text);
}
#endif
