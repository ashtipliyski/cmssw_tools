#!/bin/bash

# Script collects tx and rx files from MP and Demux MP7 boards. The data for the
# MP boards is usually merged, which makes it necessary to transfer 4 files,
# tx/rx for MP and tx/rx for demux.
# 
# The script receives a parameter with the path of the folder which contains the
# relevant buffer files. The expected folder structure is
# 
# /good/
# /good/demux/
# /good/demux/rx_summary.txt
# /good/demux/tx_summary.txt
# /good/mp0/ <-- contains both rx/tx files
# ... (separate folder for each mp board) ...
# /good/mp8/ <-- contains both rx/tx files
# /merge/rx_sumamry.txt 
# /merge/tx_summary.txt
# 
#

if [ -z $1 ]
then
	echo "Please supply an argument with the path of the folder containing the buffer files"
	exit
fi

if [ ! -d $1 ]
then
	echo "Path provided does not point to a valid directory. Check your input and try again."
	exit
fi

cp $1/good/demux/rx_summary.txt ./demux_rx_summary.txt
cp $1/good/demux/tx_summary.txt demux_tx_summary.txt
cp $1/merge/rx_summary.txt mp_rx_summary.txt
cp $1/merge/tx_summary.txt mp_tx_summary.txt
