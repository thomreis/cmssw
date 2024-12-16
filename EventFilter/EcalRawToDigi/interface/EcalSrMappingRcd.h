#ifndef EventFilter_EcalRawToDigi_EcalSrMappingRcd_h

#include "FWCore/Framework/interface/DependentRecordImplementation.h"
#include "Geometry/EcalMapping/interface/EcalMappingRcd.h"

class EcalSrMappingRcd
    : public edm::eventsetup::DependentRecordImplementation<EcalSrMappingRcd, edm::mpl::Vector<EcalMappingRcd>> {};
#endif
