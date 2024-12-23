#ifndef __DSENT_MODEL_OPTICALGRAPH_OPTICALNODE_H__
#define __DSENT_MODEL_OPTICALGRAPH_OPTICALNODE_H__

#include "model/OpticalModel.h"
#include "util/CommonType.h"

namespace DSENT
{
    class OpticalNode;

    //TODO: Change to detector
    typedef std::pair<OpticalNode*, double> DetectorEntry;
    typedef std::vector<DetectorEntry> DetectorTable;

    class OpticalNode
    {
        public:
            // The starting visited number flag of all optical nodes
            static const int OPTICAL_NODE_INIT_VISITED_NUM;
            
            // The types of optical nodes that can exist
            enum Type
            {
                WAVEGUIDE,
                LASER,
                MODULATOR,
                FILTER,
                DETECTOR
            };
    
        public:
            OpticalNode(Type type_, const String& instance_name_, OpticalModel* model_, const WavelengthGroup& wavelengths_);
            ~OpticalNode();

        public:
            // Get the type of optical node            
            Type getType() const;
            // Return instance name
            const String& getInstanceName() const;
            // Get the downstream optical nodes
            vector<OpticalNode*>* getDownstreamNodes() const;
            // Connect the downstream optical node
            void addDownstreamNode(OpticalNode* node_);
            // Return the node's parent model
            OpticalModel* getModel();
            const OpticalModel* getModel() const;                                    
            // Get wavelength groups
            WavelengthGroup getWavelengths() const;
            // Returns whether the node is expecting a set of wavelengths
            bool isExpected(const WavelengthGroup& wavelengths_) const;
            
            // Trace wavelengths, find and put all found lasers, modulators, and detectors
            //virtual void traceWavelengths(const WavelengthGroup& wavelengths_, OpticalNode* laser_,
            //    OpticalNode* modulator_, DetectorTable* detectors_, double current_loss_) const;
            
            //-----------------------------------------------------------------
            // Node variables for wavelength tracing
            //-----------------------------------------------------------------
            // Loss incurred at this optical node
            void setLoss(double loss_);
            double getLoss() const;
            // Visited number marker
            void setVisitedNum(int visited_num_);
            int getVisitedNum() const;
            //-----------------------------------------------------------------

        
        private:
            // Disable copy constructor
            OpticalNode(const OpticalNode& node_);

        private:
            // The type of optical node
            const Type m_type_;
            // Name of this instance
            String m_instance_name_;
            // A pointer to the model that contains this node
            OpticalModel* m_model_;
            // Downstream optical node
            vector<OpticalNode*>* m_downstream_nodes_;
            // Path visited count (so that you don't have to clear it)
            int m_visited_num_;
            // The amount of loss incurred at this optical node
            double m_loss_;
            // The wavelengths this optical node is supposed to see
            const WavelengthGroup m_wavelengths_;
    };
    
} // namespace DSENT

#endif // __DSENT_MODEL_OPTICALGRAPH_OPTICALNODE_H__

