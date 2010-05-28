#ifndef BUFFER_H
#define BUFFER_H

#include <systemc>
#include <stdint.h>
#include "utils.h"

namespace soclib { namespace caba {

        class Buffer: public sc_core::sc_module{
            private:
                int read_buffer;
                int counter;
                unsigned char buf[2][ANTE_TILE_SIZE][ANTE_TILE_SIZE];
						protected:
                SC_HAS_PROCESS(Buffer);
            public:
                sc_core::sc_in<bool>              p_clk;
                sc_core::sc_in<bool>              p_resetn;

                //Reading the buffer
                sc_core::sc_out<unsigned char>    buffer_out[4];
                sc_core::sc_in<int>               buffer_command_out[2];

                //Writing to the buffer
                sc_core::sc_in<uint32_t>           buffer_in;
                sc_core::sc_in<bool>      buffer_write_enable;
                // constructor
                Buffer (sc_core::sc_module_name insname);

								void bufferTransition();
								void bufferMealy();
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
