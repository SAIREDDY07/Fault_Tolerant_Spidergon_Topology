#ifndef __DSENT_MODEL_OPTICALGRAPH_OPTICALGRAPH_H__
#define __DSENT_MODEL_OPTICALGRAPH_OPTICALGRAPH_H__

#include <vector>

#include "util/CommonType.h"
#include "model/optical_graph/OpticalNode.h"

namespace DSENT
{
    class OpticalNode;
    class OpticalWavelength;
            
    class OpticalGraph
    {    
        public:
            // The visited number for the next timing run. This needs to be
            // global because several timing trees may be created to evaluate
            // a single timing path, causing problems
            static int msTreeNum;
        
        public:
            // Construct timing tree that watches over model_
            OpticalGraph(const String& instance_name_, OpticalModel* model_);
            ~OpticalGraph();
            
        public:
            // Get graph name
            const String& getInstanceName() const;
            // Perform datapath power optimization by balancing insertion loss and extinction
            // ratio with modulator/receiver and laser power, returns false if there are no
            // designs that are possible
            bool performPowerOpt(OpticalNode* node_, const WavelengthGroup& wavelengths_, unsigned int number_detectors_, double util_);
            // Recursively trace a wavelength starting from an OpticalLaser
            // source finding all lasers, modulators and detectors that a
            // wavelength group hits.
            OpticalWavelength* traceWavelength(const WavelengthGroup& wavelengths_, OpticalNode* node_);
            OpticalWavelength* traceWavelength(OpticalWavelength* wavelength_, OpticalNode* node_, OpticalLaser* laser_, OpticalModulator* modulator_, double loss_);
            // Return the model
            OpticalModel* getModel();

        private:

            // Disable the use of copy constructor
            OpticalGraph(const OpticalGraph& graph_);
        
        public:
            // Set the sequence number of the optical graph
            static void setTreeNum(int tree_num_);
            static int getTreeNum();
            
        private:
            // Name of the optical graph
            const String m_instance_name_;
            // A pointer to the model that contains this node
            OpticalModel* m_model_;

    }; // class OpticalGraph
} // namespace DSENT

#endif // __DSENT_MODEL_OPTICALGRAPH_OPTICALGRAPH_H__

