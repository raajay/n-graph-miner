n-graph-miner
=============

Source code for frequent episodes discovery proposed in the paper:
"Discovering injective episodes with general partial orders",
Avinash Achar, Srivatsan Laxman, Raajay Viswanathan, P. S. Sastry
Published in Journal of Data Mining and Knowledge Discovery
Volume 25 Issue 1, July 2012 Pages 67 - 108

The code is release under GNU GPL V3 license. Please refer to 
LICENSE file for details.

COMPILING
=========

Compiling the source files using the given Makefile produces a binary 
that can be used to mine frequent partial orders from a given data stream. 
Refer to the paper for the working of the algorithm and the description of 
terminology and parameters.

USAGE : ./bin/n-graph-miner <datastream> <frequency threshold> <entropy threshold> <expiry time> <freq episodes output>

Description of the data stream : 
-------------------------------

The stream of events to be mined is given as input to the 'n-graph-miner' as a 
text file with the following format:

  1. The event types are integer coded. Starting from 1 to maximum number of 
  participating events.
  2. Each line in the text file represents the occurrence of an event. 
  The format for a line is <event type>,<time of occurrence of event>
  3. All the event occurrences in the input file should be ordered with 
  respect to time. 
  4. For example: Consider a event stream with 5 different event types. 
  A sample stream will be of the following form
     
     1,0.003
     2,0.005
     4,0.010
     3,0.010
     1,0.015
     5,0.020
    
     Here, the time information is in seconds. 
     Note that the 'binary' handles time with millisecond precision. Suitable 
     pre-processing is required for data that have different precision
     level. A sample stream "stream.txt" is provided.

Description of the output file containing frequent episodes:
------------------------------------------------------------

  The frequent episodes obtained at different levels of mining are
  dumped in a file (given as the fifth parameter to the binary).
  (A sample file 'freqEpisodes' is provided)

  Any line in the episodes output file looks like this:

  6 11 12 13 14 16 .  : 353 : 0.492783  :. 11>12,11>13,11>14,12>14,

  Here, [6 11 12 13 14 16] represents the set of events of the partial order.
  [11>12, 11>13, 11>14, 12>14] represents the partial order on the set of these events. 
  This set of edges is transitively closed. 
  Together the above 2 sets determine the partial order. 
  '353', and '0.492783' represents the frequency count and bi-directional evidence respectively.


Parameters to be used:
----------------------

Algorithm parameters like frequency threshold, entropy threshold and
expiry time are passed as command line arguments to the binary.
Other parameters are set in a text file : 'inputfile'

The input file looks like this:

maxLengthSize: 10 
numMaxPaths: 1000
maxEvents: 400
maxSize: 10
episodeType: general


1. maxLengthSize and numMaxPaths represents 'L_max' and 'N_max' respectively. 
(Refer paper for meaning of the terms.)
2. maxEvents represents the maximum integer coded event type. 
If all integers from 1 to maxEvents are used then it represent the total number of participation events.
3. maxSize represents the maximum level of mining required. 
If maxSize = 6, the algorithm returns frequent episodes up to size 6.
4. episodeType: This can take values [serial|parallel|general]. 
These represent the type of patterns that we are looking for.

A sample file "inputfile" is provided.

------------------------------------------------------------------------------

Contact : Raajay (raajay.v@gmail.com) for further details or
clarifications, if needed.
