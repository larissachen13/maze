#!/bin/bash
# test validating command-line arguments
# wrong options
echo -en "\nINVALID OPTIONS\n"
echo -en "-----------------------------------------------------"
echo -en "\nAMStartup -f foo\n"
./AMStartup -f foo

# right options with one bad option
echo -en "\nAMStartup -n 1 -d 1 -h flume.cs.dartmouth.edu -f foo\n"
./AMStartup -n 1 -d 1 -f foo

# invalid number of avatars
echo -en "\n\nINVALID NUMBER OF AVATARS\n"
echo -en "-----------------------------------------------------"
echo -en "\nAMStartup -n nnnn -d 1 -h flume.cs.dartmouth.edu\n"
./AMStartup -n nnnn -d 1 -h flume.cs.dartmouth.edu

echo -en "\nAMStartup -n 0 -d 1 -h flume.cs.dartmouth.edu\n"
./AMStartup -n 0 -d 1 -h flume.cs.dartmouth.edu

echo -en "\nAMStartup -n 11 -d 1 -h flume.cs.dartmouth.edu\n"
./AMStartup -n 11 -d 1 -h flume.cs.dartmouth.edu


# invalid difficulty
echo -en "\n\nINVALID DIFFICULTY NUMBER\n"
echo -en "-----------------------------------------------------"
echo -en "\nAMStartup -n 1 -d nnnn -h flume.cs.dartmouth.edu\n"
./AMStartup -d nnnn -n 1 -h flume.cs.dartmouth.edu

echo -en "\nAMStartup -n 1 -d -1 -h flume.cs.dartmouth.edu\n"
./AMStartup -d -1 -n 1 -h flume.cs.dartmouth.edu

echo -en "\nAMStartup -n 1 -d 11 -h flume.cs.dartmouth.edu\n"
./AMStartup -d 11 -n 1 -h flume.cs.dartmouth.edu
