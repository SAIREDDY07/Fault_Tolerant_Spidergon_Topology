#ifndef __DSENT_MODEL_STD_CELLS_CELLMACROS_H__
#define __DSENT_MODEL_STD_CELLS_CELLMACROS_H__

#include "util/CommonType.h"
#include "model/std_cells/StdCell.h"

namespace DSENT
{
    class StdCell;

    // Contains cell macros that can be created within standard cells
    class CellMacros
    {
        private :
            CellMacros();
            ~CellMacros();

        public:
            //NOR2 Macro
            //Adds a NOR2 to the standard cell, normalized to some size
            static void addNor2(StdCell* cell_, const String& name_, bool sizable_, bool a1_to_zn_path_, bool a2_to_zn_path_,
                                const String& a1_net_, const String& a2_net_, const String& zn_net_);
            //Updates a NOR2 to to the standard cell, normalized to some size
            static void updateNor2(StdCell* cell_, const String& name_, double normalized_size_);            

            //NAND2 Macro
            //Adds a NAND2 to the standard cell, normalized to some size
            static void addNand2(StdCell* cell_, const String& name_, bool sizable_, bool a1_to_zn_path_, bool a2_to_zn_path_,
                                const String& a1_net_, const String& a2_net_, const String& zn_net_);
            //Updates a NAND2 to to the standard cell, normalized to some size
            static void updateNand2(StdCell* cell_, const String& name_, double normalized_size_);            
        
            //INVERTER Macro
            //Adds a inverter to the standard cell, normalized to some size
            static void addInverter(StdCell* cell_, const String& name_, bool sizable_, bool a_to_zn_path_,
                                    const String& a_net_, const String& zn_net_);
            //Updates an inverter to to the standard cell, normalized to some size
            static void updateInverter(StdCell* cell_, const String& name_, double normalized_size_);            

            //INVZ Macro
            //Adds a tristated inverter to the standard cell, normalized to some size
            static void addTristate(StdCell* cell_, const String& name_, bool sizable_, bool a_to_zn_path_, bool oe_to_zn_path_, bool oen_to_zn_path_,
                                    const String& a_net_, const String& oe_net_, const String& oen_net_, const String& zn_net_);
            //Updates an tristated inverter to to the standard cell, normalized to some size
            static void updateTristate(StdCell* cell_, const String& name_, double normalized_size_);            

            //Helper functions
            //Returns the width of NMOS transistors, given the NMOS and PMOS stacking
            static double calculateNmosWidth(const StdCell* cell_, unsigned int max_stacked_nmos_, unsigned int max_stacked_pmos_, unsigned int current_stack_nmos_);
            //Returns the width of PMOS transistors, given the NMOS and PMOS stacking
            static double calculatePmosWidth(const StdCell* cell_, unsigned int max_stacked_nmos_, unsigned int max_stacked_pmos_, unsigned int current_stack_pmos_);

    }; // class CellMacros
} // namespace DSENT

#endif // __DSENT_MODEL_STD_CELLS_CELLMACROS_H__

