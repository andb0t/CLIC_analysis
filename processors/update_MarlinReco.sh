#!/bin/bash

echo "Update MarlinReco ..."
cp IsolatedLeptonFinder/include/*.h ../../MarlinReco/Analysis/IsolatedLeptonFinder/include
cp IsolatedLeptonFinder/src/*.cc ../../MarlinReco/Analysis/IsolatedLeptonFinder/src
cp IsolatedLeptonFinder/README ../../MarlinReco/Analysis/IsolatedLeptonFinder
