from sys import exit
import argparse
import logging


def parse_options_and_init_log(loglevel=logging.INFO):
    """
    Adds often used options to the OptionParser...
    """
    parser = argparse.ArgumentParser(description="L1 Analysis Framework macro", formatter_class=argparse.ArgumentDefaultsHelpFormatter)
    parser.add_argument("-f", "--fname", dest="fname", default="", type=str, help="A root file containing L1Ntuples.")
    parser.add_argument("-l", "--flist", dest="flist", default="", type=str, help="A txt file containing list of L1Ntuple files, one file per line.")
    parser.add_argument("-n", "--nevents", dest="nevents", default=-1, type=int, help="Number of events to run, -1 for all [default: %default]")
    parser.add_argument("-s", "--start", dest="start_event", default=0, type=int, help="At which event should processing start [default: %default]")

    opts, unknown = parser.parse_known_args()

    return opts, parser
