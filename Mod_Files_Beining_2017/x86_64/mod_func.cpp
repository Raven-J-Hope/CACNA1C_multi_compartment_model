#include <stdio.h>
#include "hocdec.h"
extern int nrnmpi_myid;
extern int nrn_nobanner_;

extern "C" void _BK_reg(void);
extern "C" void _Cabuffer_reg(void);
extern "C" void _CadepK_reg(void);
extern "C" void _Caold_reg(void);
extern "C" void _Cav12_reg(void);
extern "C" void _Cav13_reg(void);
extern "C" void _Cav22_reg(void);
extern "C" void _Cav32_reg(void);
extern "C" void _HCN_reg(void);
extern "C" void _ichan3_reg(void);
extern "C" void _Kir21_reg(void);
extern "C" void _Kv11_reg(void);
extern "C" void _Kv14_reg(void);
extern "C" void _Kv21_reg(void);
extern "C" void _Kv33_reg(void);
extern "C" void _Kv34_reg(void);
extern "C" void _Kv42b_reg(void);
extern "C" void _Kv42_reg(void);
extern "C" void _Kv723_reg(void);
extern "C" void _na8st_reg(void);
extern "C" void _SK2_reg(void);

extern "C" void modl_reg() {
  if (!nrn_nobanner_) if (nrnmpi_myid < 1) {
    fprintf(stderr, "Additional mechanisms from files\n");
    fprintf(stderr, " \"BK.mod\"");
    fprintf(stderr, " \"Cabuffer.mod\"");
    fprintf(stderr, " \"CadepK.mod\"");
    fprintf(stderr, " \"Caold.mod\"");
    fprintf(stderr, " \"Cav12.mod\"");
    fprintf(stderr, " \"Cav13.mod\"");
    fprintf(stderr, " \"Cav22.mod\"");
    fprintf(stderr, " \"Cav32.mod\"");
    fprintf(stderr, " \"HCN.mod\"");
    fprintf(stderr, " \"ichan3.mod\"");
    fprintf(stderr, " \"Kir21.mod\"");
    fprintf(stderr, " \"Kv11.mod\"");
    fprintf(stderr, " \"Kv14.mod\"");
    fprintf(stderr, " \"Kv21.mod\"");
    fprintf(stderr, " \"Kv33.mod\"");
    fprintf(stderr, " \"Kv34.mod\"");
    fprintf(stderr, " \"Kv42b.mod\"");
    fprintf(stderr, " \"Kv42.mod\"");
    fprintf(stderr, " \"Kv723.mod\"");
    fprintf(stderr, " \"na8st.mod\"");
    fprintf(stderr, " \"SK2.mod\"");
    fprintf(stderr, "\n");
  }
  _BK_reg();
  _Cabuffer_reg();
  _CadepK_reg();
  _Caold_reg();
  _Cav12_reg();
  _Cav13_reg();
  _Cav22_reg();
  _Cav32_reg();
  _HCN_reg();
  _ichan3_reg();
  _Kir21_reg();
  _Kv11_reg();
  _Kv14_reg();
  _Kv21_reg();
  _Kv33_reg();
  _Kv34_reg();
  _Kv42b_reg();
  _Kv42_reg();
  _Kv723_reg();
  _na8st_reg();
  _SK2_reg();
}
