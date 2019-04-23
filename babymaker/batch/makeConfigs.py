import glob, os, subprocess
from textwrap import dedent

def MakeConfigs(sample, project, test=False, verbose=True):

    # Check for valid sample path
    if not os.path.isdir(sample):
        if verbose: print("WARNING: Skipping {} -- directory does not exist.".format(sample))
        return

    # Get sample files
    files = glob.glob("{}/*.root".format(sample))
    # Check for non-zero input .root files
    if len(files) == 0:
        if verbose: print("WARNING: Skipping {} -- contains 0 .root files.".format(sample))
        return

    # Get sample name
    if sample[-1] == "/":
        sample = sample[:-1]
    splitSample = (sample.split("/")[-1]).split("_")
    name = splitSample[0]
    if "Tune" not in splitSample[1]:
        name += "_"+splitSample[1]

    # Get output directory
    user = os.environ["USER"]
    outdir = "/hadoop/cms/store/user/{0}/{1}/{2}".format(user, project, ("test_" if test else "")+name)

    # Create config directory if missing
    if not os.path.isdir("configs"): os.mkdir("configs")

    # Write config file
    outFile = "configs/{0}config_{1}.cmd".format("test_" if test else "", name)
    with open(outFile, 'w') as fout:
        # Get credentials
        x509file = subprocess.check_output(["find","/tmp/", "-maxdepth", "1", "-type", "f", "-user", user, "-regex", "^.*x509.*$"])
        os.system("mkdir -p /data/tmp/{0}/condor_job_logs/{1}/".format(user, project))
        os.system("mkdir -p /data/tmp/{0}/condor_submit_logs/{1}/".format(user, project))
        # Write config header
        fout.write(dedent("""
        universe=Vanilla
        when_to_transfer_output = ON_EXIT
        transfer_input_files=wrapper.sh, package.tar.xz
        +DESIRED_Sites="T2_US_UCSD"
        +remote_DESIRED_Sites="T2_US_UCSD"
        +Owner = undefined
        log=/data/tmp/{0}/condor_submit_logs/{1}/condor_12_01_16.log
        output=/data/tmp/{0}/condor_job_logs/{1}/1e.$(Cluster).$(Process).out
        error =/data/tmp/{0}/condor_job_logs/{1}/1e.$(Cluster).$(Process).err
        notification=Never
        x509userproxy={2}

        """.format(user, project, x509file)))
        # Write executable calls
        jobid = 0
        for f in files:
            jobid = (f.split("_")[-1]).split(".")[0]
            fout.write("executable=wrapper.sh\n")
            fout.write("transfer_executable=True\n")
            fout.write("arguments={0} {1} {2}\n".format(jobid, sample, outdir))
            fout.write("queue\n\n")
            if test: return

    return

if __name__ == "__main__":
    project = "rare-higgs"
    MakeConfigs("/hadoop/cms/store/group/snt/run2_mc2018_private/WH_HtoRhoGammaPhiGamma_privateMC_102x_MINIAOD_v1", project, test=False)
    MakeConfigs("/hadoop/cms/store/group/snt/run2_mc2018/WGToLNuG_TuneCP5_13TeV-madgraphMLM-pythia8_RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1_MINIAODSIM_CMS4_V10-02-04", project, test=False)
    MakeConfigs("/hadoop/cms/store/group/snt/run2_mc2018/TTGamma_SingleLeptFromT_TuneCP5_13TeV_madgraph_pythia8_RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15_ext1-v2_MINIAODSIM_CMS4_V10-02-04", project, test=False)
    MakeConfigs("/hadoop/cms/store/group/snt/run2_mc2018/TTGamma_SingleLeptFromTbar_TuneCP5_13TeV_madgraph_pythia8_RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15_ext1-v2_MINIAODSIM_CMS4_V10-02-04", project, test=False)
    MakeConfigs("/hadoop/cms/store/group/snt/run2_mc2018/WJetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8_RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v2_MINIAODSIM_CMS4_V10-02-04", project, test=False)
    MakeConfigs("/hadoop/cms/store/group/snt/run2_mc2018/TTJets_SingleLeptFromT_TuneCP5_13TeV-madgraphMLM-pythia8_RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1_MINIAODSIM_CMS4_V10-02-04", project, test=False)
    MakeConfigs("/hadoop/cms/store/group/snt/run2_mc2018/TTJets_SingleLeptFromTbar_TuneCP5_13TeV-madgraphMLM-pythia8_RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1_MINIAODSIM_CMS4_V10-02-04", project, test=False)