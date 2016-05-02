#!/bin/bash

if [ -d compHwEmu ]
then
	rm -r compHwEmu
fi

mkdir compHwEmu
mkdir compHwEmu/Demux
mkdir compHwEmu/DemuxJets
mkdir compHwEmu/DemuxSums
mkdir compHwEmu/MPSums
mkdir compHwEmu/Jets

mkdir compHwEmu/Sorts

mkdir compHwEmu/Egs
mkdir compHwEmu/DemuxEgs

mkdir compHwEmu/Taus
mkdir compHwEmu/DemuxTaus
