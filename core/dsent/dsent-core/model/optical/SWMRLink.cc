#include "model/optical/SWMRLink.h"

#include "model/ModelGen.h"
#include "model/PortInfo.h"
#include "model/TransitionInfo.h"
#include "model/EventInfo.h"
#include "model/optical_graph/OpticalGraph.h"
#include "model/optical_graph/OpticalWaveguide.h"
#include "model/optical/RingModulator.h"
#include "model/optical/RingFilter.h"
#include "model/optical/RingDetector.h"
#include "model/optical/LaserSource.h"
#include "model/optical/ThrottledLaserSource.h"

namespace DSENT
{
    SWMRLink::SWMRLink(const String& instance_name_, const TechModel* tech_model_)
        : OpticalModel(instance_name_, tech_model_)
    {
        initParameters();
        initProperties();
    }

    SWMRLink::~SWMRLink()
    {}

    void SWMRLink::initParameters()
    {

        addParameterName("NumberBits");     
        addParameterName("CoreDataRate");
        addParameterName("LinkDataRate");

        addParameterName("NumberReaders");
        addParameterName("MaxReaders");
        addParameterName("MinReaders");

        addParameterName("LaserType");
        addParameterName("RingTuningMethod");
        addParameterName("OptimizeLoss", "TRUE");
        return;
    }

    void SWMRLink::initProperties()
    {
        addPropertyName("Length");
        addPropertyName("OptUtil", 0.5);        // default to 50% utilization (a new word 50% of the time)
        addPropertyName("ExtinctionRatio", 6);  // default properties
        addPropertyName("InsertionLoss", 2);    // default properties
        return;
    }

    void SWMRLink::constructModel()
    {
        // Get parameters
        unsigned int number_bits = getParameter("NumberBits");
        double core_data_rate = getParameter("CoreDataRate");
        double link_data_rate = getParameter("LinkDataRate");
        unsigned int number_readers = getParameter("NumberReaders");
        unsigned int number_max_readers = std::min(number_readers, getParameter("MaxReaders").toUInt());
        unsigned int number_min_readers = std::min(number_max_readers, getParameter("MinReaders").toUInt());
        
        // Get directly propagated parameters
        const String& ring_tuning_method = getParameter("RingTuningMethod");

        // Calculate number of wavelengths needed
        unsigned int number_wavelengths = (double) number_bits * core_data_rate / link_data_rate;

        // Set some generated properties
        getGenProperties()->set("NumberWavelengths", number_wavelengths);

        // Create electrical ports
        createInputPort("CK");
        createInputPort("LinkCK");
        createInputPort("In", makeNetIndex(0, number_bits-1));
        createOutputPort("Out", makeNetIndex(0, number_bits-1));

        // Create Waveguides
        // Temporarily assume its all on one waveguide
        createWaveguide("LaserToMod", makeWavelengthGroup(0, number_wavelengths-1));
        for (unsigned int i = 0; i <= number_readers; ++i)
            createWaveguide("WaveguideSegment[" + (String) i + "]", makeWavelengthGroup(0, number_wavelengths-1));        

        // // Create electrical ports
        // createInputPort("CK");
        // createInputPort("In", makeNetIndex(0, number_wavelengths-1));
        // for (unsigned int i = 0; i < number_readers; ++i)
            // createOutputPort("Out" + (String) i, makeNetIndex(0, number_wavelengths-1));
        
        // Add area results
        addAreaResult(new Result("Photonic"));
        createElectricalResults();
        // Setup idle event
        getEventInfo("Idle")->setStaticTransitionInfos();
        // Create a waveguide area result
        addAreaResult(new AtomicResult("Waveguide"));
        getAreaResult("Photonic")->addSubResult(getAreaResult("Waveguide"), "Waveguide", 1.0);
        // Add results
        addNddPowerResult(new Result("Laser"));
        addNddPowerResult(new Result("RingTuning"));
        // Add event result
        // createElectricalEventResult("BroadcastFlit");
        
        for (unsigned int i = number_min_readers; i <= number_max_readers; ++i)
            createElectricalEventResult("MulticastFlit" + (String) i);

        // Create Tx, Rx backends
        // Create Tx electrical backend
        ElectricalModel* tx_backend = (ElectricalModel*) ModelGen::createModel("OpticalLinkBackendTx", "OpticalLinkBackendTx", getTechModel());
        tx_backend->setParameter("InBits", number_bits);
        tx_backend->setParameter("CoreDataRate", core_data_rate);
        tx_backend->setParameter("LinkDataRate", link_data_rate);
        tx_backend->setParameter("RingTuningMethod", ring_tuning_method);
        tx_backend->setParameter("BitDuplicate", "TRUE");
        tx_backend->construct();

        // Create Rx electrical backend
        ElectricalModel* rx_backend = (ElectricalModel*) ModelGen::createModel("OpticalLinkBackendRx", "OpticalLinkBackendRx", getTechModel());
        rx_backend->setParameter("OutBits", number_bits);
        rx_backend->setParameter("CoreDataRate", core_data_rate);
        rx_backend->setParameter("LinkDataRate", link_data_rate);
        rx_backend->setParameter("RingTuningMethod", ring_tuning_method);
        rx_backend->setParameter("BitDuplicate", "TRUE");
        rx_backend->construct();
        
        // Connect ports
        createNet("TxBackendToTx", makeNetIndex(0, number_wavelengths-1));
        createNet("RxToRxBackend", makeNetIndex(0, number_wavelengths-1));
        portConnect(tx_backend, "In", "In");
        portConnect(tx_backend, "Out", "TxBackendToTx");
        portConnect(tx_backend, "LinkCK", "LinkCK");
        portConnect(rx_backend, "In", "RxToRxBackend");
        portConnect(rx_backend, "Out", "Out");
        portConnect(rx_backend, "LinkCK", "LinkCK");

        // Add instances
        addSubInstances(tx_backend, 1.0);
        addSubInstances(rx_backend, number_readers);

        // Add electrical results
        addElectricalSubResults(tx_backend, 1.0);
        addElectricalSubResults(rx_backend, number_readers);

        // Add tuning power result
        getNddPowerResult("RingTuning")->addSubResult(tx_backend->getNddPowerResult("RingTuning"), "OpticalLinkBackendTx", 1.0);
        getNddPowerResult("RingTuning")->addSubResult(rx_backend->getNddPowerResult("RingTuning"), "OpticalLinkBackendRx", number_readers);
        
        // Add event results
        // getEventInfo("BroadcastFlit")->setTransitionInfo("LinkCK", TransitionInfo(0.0, (double) link_data_rate / (core_data_rate * 2.0), 0.0));

        // getEventResult("BroadcastFlit")->addSubResult(tx_backend->getEventResult("ProcessBits"), "OpticalLinkBackendTx", 1.0);
        // getEventResult("BroadcastFlit")->addSubResult(rx_backend->getEventResult("ProcessBits"), "OpticalLinkBackendRx", number_max_readers);
        for (unsigned int i = number_min_readers; i <= number_max_readers; ++i)
        {
            getEventInfo("MulticastFlit" + (String) i)->setTransitionInfo("LinkCK", TransitionInfo(0.0, (double) link_data_rate / (core_data_rate * 2.0), 0.0));
            getEventResult("MulticastFlit" + (String) i)->addSubResult(tx_backend->getEventResult("ProcessBits"), "OpticalLinkBackendTx", 1.0);
            getEventResult("MulticastFlit" + (String) i)->addSubResult(rx_backend->getEventResult("ProcessBits"), "OpticalLinkBackendRx",  (double) i);
        }

        buildLaser();
        buildModulator();
        buildDetectors();
        
        return;
    }
    
    void SWMRLink::updateModel()
    {
        // Get parameters
        double core_data_rate = getParameter("CoreDataRate");
        unsigned int number_readers = getParameter("NumberReaders");

        // Get properties
        double length = getProperty("Length");
        const String& extinction_ratio = getProperty("ExtinctionRatio");
        const String& insertion_loss = getProperty("InsertionLoss");
        const double opt_util = getProperty("OptUtil");
        
        // Calculate loss for each waveguide segment
        double segment_length = (double) length / number_readers;
        double segment_loss = getTechModel()->get("Waveguide->LossPerMeter").toDouble() * segment_length;
        // Set loss of each waveguide segment
        for (unsigned int i = 0; i < number_readers; ++i)
            getWaveguide("WaveguideSegment[" + (String) i + "]")->setLoss(segment_loss);
        // Calculate waveguide area
        double waveguide_area = length * getTechModel()->get("Waveguide->Pitch").toDouble();
        getAreaResult("Waveguide")->setValue(waveguide_area);
    
        // Update the laser
        Model* laser = getSubInstance("Laser");
        laser->setProperty("LaserEventTime", 1.0 / core_data_rate);
        laser->setProperty("OptUtil", opt_util);
        laser->update();
        
        // Update the modulator
        Model* modulator = getSubInstance("Modulator");
        modulator->setProperty("ExtinctionRatio", extinction_ratio);
        modulator->setProperty("InsertionLoss", insertion_loss);
        modulator->update();        
        
        // Update all receivers
        for (unsigned int i = 0; i < number_readers; ++i)
        {
            Model* detector = getSubInstance("Detector_" + (String) i);
            detector->update();
        }
                   
        // Update the backends
        Model* tx_backend = getSubInstance("OpticalLinkBackendTx");
        tx_backend->update();                   
        Model* rx_backend = getSubInstance("OpticalLinkBackendRx");
        rx_backend->update();
        
        return;
    }
    
    void SWMRLink::propagateTransitionInfo()
    {
        // Get parameters
        const String& laser_type = getParameter("LaserType");
        unsigned int number_readers = getParameter("NumberReaders");

        // Propagate transition info to tx backend
        OpticalModel* tx_backend = (OpticalModel*) getSubInstance("OpticalLinkBackendTx");
        propagatePortTransitionInfo(tx_backend, "In", "In");
        propagatePortTransitionInfo(tx_backend, "LinkCK", "LinkCK");
        tx_backend->use();

        // Set transition info for the modulator
        OpticalModel* modulator = (OpticalModel*) getSubInstance("Modulator");
        propagatePortTransitionInfo(modulator, "In", tx_backend, "Out");
        modulator->use();
        
        // Modulator out transition info
        const TransitionInfo& mod_out_transitions = modulator->getOpticalOutputPort("Out")->getTransitionInfo();
        
        // Set transition info for all receivers
        for (unsigned int i = 0; i < number_readers; ++i)
        {
            OpticalModel* detector = (OpticalModel*) getSubInstance("Detector_" + (String) i);
            detector->getOpticalInputPort("In")->setTransitionInfo(mod_out_transitions);
            detector->use();
            
            // Propagate output transition info to output
            // propagatePortTransitionInfo("Out" + (String) i, detector, "Out");
        }
                
        OpticalModel* detector = (OpticalModel*) getSubInstance("Detector_0");
        // Propagate transition info to tx backend
        OpticalModel* rx_backend = (OpticalModel*) getSubInstance("OpticalLinkBackendRx");
        propagatePortTransitionInfo(rx_backend, "In", detector, "Out");
        propagatePortTransitionInfo(rx_backend, "LinkCK", "LinkCK");
        rx_backend->use();

        // Propagate output transition info to output
        propagatePortTransitionInfo("Out", rx_backend, "Out");

        // Set enable signals for the laser, if applicable
        if (laser_type == "Throttled")
        {
            // Figure out how many cycles the laser needs to be on
            double cycles = getInputPort("In")->getTransitionInfo().getFrequencyMultiplier();
        
            OpticalModel* laser = (OpticalModel*) getSubInstance("Laser");
            laser->getInputPort("LaserEnable")->setTransitionInfo(TransitionInfo(0.0, 1.0, cycles - 1.0));
            laser->use();
        }
        return;
    }

    void SWMRLink::buildLaser()
    {
        // Get parameters
        unsigned int number_wavelengths = getGenProperties()->get("NumberWavelengths");
        unsigned int number_readers = getParameter("NumberReaders");
        unsigned int number_max_readers = std::min(number_readers, getParameter("MaxReaders").toUInt());
        unsigned int number_min_readers = std::min(number_max_readers, getParameter("MinReaders").toUInt());
        const String& laser_type = getParameter("LaserType");

        // Create laser
        OpticalModel* laser = NULL;
        if (laser_type == "Throttled")
            laser = new ThrottledLaserSource("Laser", getTechModel());
        else if (laser_type == "Standard")
            laser = new LaserSource("Laser", getTechModel());        
        else
            ASSERT(false, "[Error] " + getInstanceName() + " -> Unknown laser type '" + laser_type + "'!");
        
        laser->setParameter("OutStart", 0);
        laser->setParameter("OutEnd", number_wavelengths-1);
        laser->setParameter("MaxDetectors", number_max_readers);
        laser->setParameter("MinDetectors", number_min_readers);
        laser->construct();        

        addSubInstances(laser, 1.0);
        getAreaResult("Photonic")->addSubResult(laser->getAreaResult("Photonic"), "Laser", 1.0);
        // Connect laser output port
        opticalPortConnect(laser, "Out", "LaserToMod");    

        // Without laser gating, laser is pure NDD power
        if (laser_type == "Standard")
            getNddPowerResult("Laser")->addSubResult(laser->getNddPowerResult("Laser"), "Laser", 1.0);
        // With laser power gating, laser is an event
        else
        {
            // If laser is throttled, only pay for the amount needed to reach some number of readers
            // getEventResult("BroadcastFlit")->addSubResult(laser->getEventResult("Laser" + (String) number_max_readers), "Laser", 1.0);
            for (unsigned int i = number_min_readers; i <= number_max_readers; ++i)
                getEventResult("MulticastFlit" + (String) i)->addSubResult(laser->getEventResult("Laser" + (String) i), "Laser", 1.0);
        }
        
        return;
    }
    
    void SWMRLink::buildModulator()
    {
        // Get parameters
        double link_data_rate = getParameter("LinkDataRate");
        const String& optimize_loss = getParameter("OptimizeLoss");
        unsigned int number_wavelengths = getGenProperties()->get("NumberWavelengths");
        unsigned int number_readers = getParameter("NumberReaders");
        unsigned int number_max_readers = std::min(number_readers, getParameter("MaxReaders").toUInt());
        unsigned int number_min_readers = std::min(number_max_readers, getParameter("MinReaders").toUInt());

        // Create modulator
        RingModulator* modulator = new RingModulator("Modulator", getTechModel());
        modulator->setParameter("DataRate", link_data_rate);
        modulator->setParameter("InStart", 0);
        modulator->setParameter("InEnd", number_wavelengths-1);
        modulator->setParameter("ModStart", 0);
        modulator->setParameter("ModEnd", number_wavelengths-1);
        modulator->setParameter("OptimizeLoss", optimize_loss);
        modulator->construct();
        addSubInstances(modulator, 1.0);
        getAreaResult("Photonic")->addSubResult(modulator->getAreaResult("Photonic"), "Modulator", 1.0);
        addElectricalSubResults(modulator, 1.0);

        // Connect electrical port
        portConnect(modulator, "In", "TxBackendToTx");
        // Connect modulator input, output port
        opticalPortConnect(modulator, "In", "LaserToMod");
        opticalPortConnect(modulator, "Out", "WaveguideSegment[0]");
        
        // Add modulator energy event for all broadcast events
        // getEventResult("BroadcastFlit")->addSubResult(modulator->getEventResult("Modulate"), "Modulator", 1.0);
        for (unsigned int i = number_min_readers; i <= number_max_readers; ++i)
            getEventResult("MulticastFlit" + (String) i)->addSubResult(modulator->getEventResult("Modulate"), "Modulator", 1.0);

        return;
    }
    
    void SWMRLink::buildDetectors()
    {
        // Get parameters
        double link_data_rate = getParameter("LinkDataRate");
        unsigned int number_wavelengths = getGenProperties()->get("NumberWavelengths");
        unsigned int number_readers = getParameter("NumberReaders");
        unsigned int number_max_readers = std::min(number_readers, getParameter("MaxReaders").toUInt());
        unsigned int number_min_readers = std::min(number_max_readers, getParameter("MinReaders").toUInt());

        // Create a SWMR Configuration
        for (unsigned int i = 0; i < number_readers; ++i)
        {
            String n = (String) i;
            
            // Create resonant ring detector
            RingDetector* detector = new RingDetector("Detector_" + n, getTechModel());
            detector->setParameter("DataRate", link_data_rate);
            detector->setParameter("InStart", 0);
            detector->setParameter("InEnd", number_wavelengths-1);
            detector->setParameter("DetStart", 0);
            detector->setParameter("DetEnd", number_wavelengths-1);
            detector->setParameter("DropAll", "FALSE");
            detector->setParameter("Topology", RingDetector::INTEGRATINGSENSEAMP);
            detector->construct();
            addSubInstances(detector, 1.0);
            getAreaResult("Photonic")->addSubResult(detector->getAreaResult("Photonic"), "Detector_" + n, 1.0);
            addElectricalSubResults(detector, 1.0);

            // connect to electrical port
            portConnect(detector, "Out", "RxToRxBackend");
            // connect optical input, output port           
            opticalPortConnect(detector, "In", "WaveguideSegment[" + (String) i + "]");
            opticalPortConnect(detector, "Out", "WaveguideSegment[" + (String) (i + 1) + "]");            
        }
        
        // Add an average receiver energy for all multicast events (and broadcast)
        // Result* broadcast_event = getEventResult("BroadcastFlit");
        // for (unsigned int i = 0; i < number_readers; ++i)
        // {
            // const String detector_name = "Detector_" + (String) i;
            // broadcast_event->addSubResult(getSubInstance(detector_name)->getEventResult("Receive"), detector_name, 1.0);
        // }
        for (unsigned int i = number_min_readers; i <= number_max_readers; ++i)
        {
            Result* multicast_event = getEventResult("MulticastFlit" + (String) i);
            for (unsigned int j = 0; j < number_readers; ++j)
            {
                const String detector_name = "Detector_" + (String) j;
                multicast_event->addSubResult(getSubInstance(detector_name)->getEventResult("Receive"), detector_name, (double) i / number_readers);
            }
        }

        return;
    }
    
} // namespace DSENT

