#!/usr/bin/env python3
"""
_ppEra_Run2_50ns_

Test for Collision Scenario implementation

"""


import unittest
import FWCore.ParameterSet.Config as cms
from Configuration.DataProcessing.GetScenario import getScenario



def writePSetFile(name, process):
    """
    _writePSetFile_

    Util to dump the process to a file

    """
    handle = open(name, 'w')
    handle.write(process.dumpPython())
    handle.close()


class ppEra_Run2_50nsScenarioTest(unittest.TestCase):
    """
    unittest for ppEra_Run2_50ns collisions scenario

    """

    def testA(self):
        """get the scenario"""
        try:
            scenario = getScenario("ppEra_Run2_50ns")
        except Exception as ex:
            msg = "Failed to get ppEra_Run2_50ns scenario\n"
            msg += str(ex)
            self.fail(msg)


    def testPromptReco(self):
        """test promptReco method"""
        scenario = getScenario("ppEra_Run2_50ns")
        try:
            process = scenario.promptReco("GLOBALTAG::ALL")
            writePSetFile("testPromptReco.py", process)
        except Exception as ex:
            msg = "Failed to create Prompt Reco configuration\n"
            msg += "for ppEra_Run2_50ns Scenario\n"
            msg += str(ex)
            self.fail(msg)


    def testExpressProcessing(self):
        """ test expressProcessing method"""
        scenario = getScenario("ppEra_Run2_50ns")
        try:
            process = scenario.expressProcessing("GLOBALTAG::ALL")
            writePSetFile("testExpressProcessing.py", process)
        except Exception as ex:
            msg = "Failed to create Express Processing configuration\n"
            msg += "for ppEra_Run2_50ns Scenario\n"
            msg += str(ex)
            self.fail(msg)


    def testAlcaSkim(self):
        """ test alcaSkim method"""
        scenario = getScenario("ppEra_Run2_50ns")
        try:
            process = scenario.alcaSkim(["MuAlCalIsolatedMu"])
            writePSetFile("testAlcaReco.py", process)
        except Exception as ex:
           msg = "Failed to create Alca Skimming configuration\n"
           msg += "for ppEra_Run2_50ns Scenario\n"
           msg += str(ex)
           self.fail(msg)


    def testDQMHarvesting(self):
        """test dqmHarvesting  method"""
        scenario = getScenario("ppEra_Run2_50ns")
        try:
            process = scenario.dqmHarvesting("dataset", 123456,
                                             "GLOBALTAG::ALL")
            writePSetFile("testDQMHarvesting.py", process)
        except Exception as ex:
            msg = "Failed to create DQM Harvesting configuration "
            msg += "for ppEra_Run2_50ns scenario:\n"
            msg += str(ex)
            self.fail(msg)


if __name__ == '__main__':
    unittest.main()
