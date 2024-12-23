#ifndef __DSENT_MODEL_ELECTRICAL_TIMING_NODE_H__
#define __DSENT_MODEL_ELECTRICAL_TIMING_NODE_H__

#include "util/CommonType.h"

namespace DSENT
{
    class ElectricalModel;
    
    class ElectricalTimingNode
    {
        public:
            // The starting visited number flag of all timing nodes
            static const int TIMING_NODE_INIT_VISITED_NUM;

        public:
            ElectricalTimingNode(const String& instance_name_, ElectricalModel* model_);
            virtual ~ElectricalTimingNode();

        public:

            // Calculate the delay this node contributes
            virtual double calculateDelay() const = 0;
            // Calculate the transition at this node 
            virtual double calculateTransition() const = 0;
            // get maximum of upstream drive resistance
            virtual double getMaxUpstreamRes() const;
            // get total amount of downstream load capacitance 
            virtual double getTotalDownstreamCap() const;
            // Return instance name
            const String& getInstanceName() const;            
            // get upstream timing nodes
            vector<ElectricalTimingNode*>* getUpstreamNodes() const;
            // get downstream timing nodes
            vector<ElectricalTimingNode*>* getDownstreamNodes() const;
            // Connect a downstream timing node
            void addDownstreamNode(ElectricalTimingNode* node_);
            // Return the node's parent model
            ElectricalModel* getModel();
            const ElectricalModel* getModel() const;
            // Set/get false path marker
            void setFalsePath(bool false_path_);
            bool getFalsePath() const;
            
            virtual bool isDriver() const;
            virtual bool isNet() const;
            virtual bool isLoad() const;
            
            
            //-----------------------------------------------------------------
            // Functions for delay optimization
            //-----------------------------------------------------------------
            // Return true if the instance has minimum driving strength
            virtual bool hasMinDrivingStrength() const;
            // Return true if the instance has maximum driving strength
            virtual bool hasMaxDrivingStrength() const;
            // Increase driving strength index by 1
            virtual void increaseDrivingStrength();
            // Decrease driving strength index by 1
            virtual void decreaseDrivingStrength();
            //-----------------------------------------------------------------

            //-----------------------------------------------------------------
            // Node variables for critical path delay calculations
            //-----------------------------------------------------------------
            // Critical path marker
            void setCritPath(int crit_path_);
            int getCritPath() const; 
            // Visited parity marker
            void setVisitedNum(int visited_parity_);
            int getVisitedNum() const;
            // Delay left in this path
            void setDelayLeft(double delay_left_);
            double getDelayLeft() const;
            //-----------------------------------------------------------------

        
        private:
            // Disable copy constructor
            ElectricalTimingNode(const ElectricalTimingNode& node_);

        private:
            // Name of this instance
            String m_instance_name_;
            // A pointer to the model that contains this node
            ElectricalModel* m_model_;
            // Upstream electrical nets
            vector<ElectricalTimingNode*>* m_upstream_nodes_;
            // Downstream electrical nets
            vector<ElectricalTimingNode*>* m_downstream_nodes_;
            // False path marker
            bool m_false_path_;
            // Critical path index (to next downstream node)
            int m_crit_path_;
            // Odd / even path visited (so that you don't have to clear it)
            int m_visited_num_;
            // The amount of delay left to the end of the timing path
            double m_delay_left_;
    };
    
} // namespace DSENT

#endif // __DSENT_MODEL_ELECTRICAL_TIMING_NODE_H__

