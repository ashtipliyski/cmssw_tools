#!/bin/bash

if [ "$1" = "--help" ]
then
	echo "helpful stuff"
	echo "-------------"
	echo "unpack_crab_logs.sh [no-events] [--discart-postjob] [--discart-stderr]"
	exit
else
	no_logs=$1
fi

for i in `seq 1 $no_logs`
do
	current_file=cmsRun_$i.log.tar.gz;

	if [ ! -f $current_file ]
	then
		echo $current_file does not exist, skipping.
	else
		echo Extracting $current_file

		# this will generate several three files:
		# - cmsRun-stderr-$i.log (job log from stderr)
		# - cmsRun-stdout-$i.log (job log from stdout)
		# - FrameworkJobReport-$i.xml 
		tar xzf $current_file

		if [ "$2" == "--discart-postjob" ]
		then
			rm FrameworkJobReport-$i.xml
		fi
		
		if [ "$3" == "--discart-stderr" ]
		then
			rm cmsRun-stderr-$i.log
		fi
	fi

	
done
echo $no_logs to process
