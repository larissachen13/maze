#!/bin/bash

# Test with sleep and basic maze
echo -en "\n\nTEST WITH SLEEP AND ASCII REPRESENTATION\n"
echo -en "-----------------------------------------------------"
echo -en "\nAMStartupsleep -n 2 -d 1 -h flume.cs.dartmouth.edu\n"
./AMStartupsleep -n 2 -d 2 -h flume.cs.dartmouth.edu


# Test without sleep
echo -en "\n\nTEST WITHOUT SLEEP \n"
echo -en "-----------------------------------------------------"

echo -en "\nAMStartup -n 2 -d 2 -h flume.cs.dartmouth.edu\n"
./AMStartup -n 2 -d 2 -h flume.cs.dartmouth.edu

echo -en "\nAMStartup -n 2 -d 3 -h flume.cs.dartmouth.edu\n"
./AMStartup -n 2 -d 4 -h flume.cs.dartmouth.edu
