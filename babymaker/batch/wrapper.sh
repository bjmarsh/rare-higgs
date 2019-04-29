#!/bin/bash

# --> Arguments <-- #
# Input
FILEID=$1
SAMPLE=$2
COPYDIR=$3
# Log input arguments
echo "[wrapper] FILEID    = " ${FILEID}
echo "[wrapper] COPYDIR   = " ${COPYDIR}

# --> Environment <-- #
# Version using cvmfs install of CMSSW
echo "[wrapper] setting env"
export CMSSWVERSION=CMSSW_10_2_5
export SCRAM_ARCH=slc6_amd64_gcc700
# Get grid environment
if [ -r "$OSGVO_CMSSW_Path"/cmsset_default.sh ]; then
    echo "sourcing environment: source $OSGVO_CMSSW_Path/cmsset_default.sh"
    source "$OSGVO_CMSSW_Path"/cmsset_default.sh
elif [ -r "$OSG_APP"/cmssoft/cms/cmsset_default.sh ]; then
    echo "sourcing environment: source $OSG_APP/cmssoft/cms/cmsset_default.sh"
    source "$OSG_APP"/cmssoft/cms/cmsset_default.sh
elif [ -r /cvmfs/cms.cern.ch/cmsset_default.sh ]; then
    echo "sourcing environment: source /cvmfs/cms.cern.ch/cmsset_default.sh"
    source /cvmfs/cms.cern.ch/cmsset_default.sh
else
    echo "ERROR! Couldn't find $OSGVO_CMSSW_Path/cmsset_default.sh or /cvmfs/cms.cern.ch/cmsset_default.sh or $OSG_APP/cmssoft/cms/cmsset_default.sh"
    exit 1
fi
# Sets up CMSSW folder, cd into it
eval `scramv1 project CMSSW $CMSSWVERSION`
# Move files to CMSSW directory
mv *.tar.xz $CMSSWVERSION
# Move to CMSSW directory
cd $CMSSWVERSION
eval `scramv1 runtime -sh`

# Log basic job info
echo "[wrapper] hostname  = " `hostname`
echo "[wrapper] date      = " `date`
echo "[wrapper] linux timestamp = " `date +%s`

# --> Input Decompression <-- #
# Log cwd
echo "[wrapper] current directory is:"
pwd
# Un-tar input tarball
echo "[wrapper] extracting input sandbox"
TARFILE=`ls *.tar.xz`
tar -xJf $TARFILE
# Log input contents
echo "[wrapper] input contents are"
ls -a

# --> Main Process <-- #
# Input file
INFILE="${SAMPLE}/merged_ntuple_${FILEID}.root"
echo "[wrapper] running: python doAll.py $INFILE" 
# Run main process
python doAll.py $INFILE

# --> Output <-- #
# Log output
echo "[wrapper] output is"
ls -lh
# Skim output
echo "[wrapper] skimming output"
python copyTree.py output.root skimmed.root -1 0 "tree" "recoMeson_nCands > 0"
# Mark output to be copied
echo "[wrapper] copying file"
OUTPUT=skimmed.root
echo "[wrapper] OUTPUT = " ${OUTPUT}

# --> Clean-up <-- #
# Copy output to destination
COPY_SRC="file://`pwd`/${OUTPUT}"
COPY_DEST="gsiftp://gftp.t2.ucsd.edu${COPYDIR}/output_${FILEID}.root"
echo "Running: env -i X509_USER_PROXY=${X509_USER_PROXY} gfal-copy -p -f -t 7200 --verbose --checksum ADLER32 ${COPY_SRC} ${COPY_DEST}"
env -i X509_USER_PROXY=${X509_USER_PROXY} gfal-copy -p -f -t 7200 --verbose --checksum ADLER32 ${COPY_SRC} ${COPY_DEST} 
COPY_STATUS=$?
if [[ $COPY_STATUS != 0 ]]; then
    echo "Removing output file because gfal-copy crashed with code $COPY_STATUS"
    env -i X509_USER_PROXY=${X509_USER_PROXY} gfal-rm --verbose ${COPY_DEST}
    REMOVE_STATUS=$?
    if [[ $REMOVE_STATUS != 0 ]]; then
        echo "Uhh, gfal-copy crashed and then the gfal-rm also crashed with code $REMOVE_STATUS"
        echo "You probably have a corrupt file sitting on hadoop now."
        exit 1
    fi
fi
# Log remaining files
pwd
ls
