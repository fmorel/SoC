#ifndef INTERPOLATION_H
#define INTERPOLATION_H

#include <systemc>
#include <stdint.h>

namespace soclib { namespace caba {

  class Interpolation: public sc_core::sc_module{
    private:
      float x,y,prevx,prevy,prevprevx,prevprevy;
      unsigned char  intensities[4];
    protected:
      SC_HAS_PROCESS(Interpolation);
    public:
      sc_core::sc_in<bool>              p_clk;
      sc_core::sc_in<bool>              p_resetn;

      sc_core::sc_in<float>             p_x;
      sc_core::sc_in<float>             p_y;

      sc_core::sc_out<unsigned char>    p_out;

      sc_core::sc_in<unsigned char>     p_buffer_in[4];
      sc_core::sc_out<int>              p_buffer_command[2];
      // constructor
      Interpolation (sc_core::sc_module_name insname);

      void interpolationTransition();
      void interpolationMoore();
  };

}}
#endif

// Local Variables:
// tab-width: 4
// c-basic-offset: 4
// c-file-offsets:((innamespace . 0)(inline-open . 0))
// indent-tabs-mode: nil
// End:

// vim: filetype=cpp:expandtab:shiftwidth=4:tabstop=4:softtabstop=4
