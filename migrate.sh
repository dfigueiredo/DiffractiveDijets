#/bin/sh

##
## Migrate CVS package to Git
##
## Author: dmf@cern.ch
##
## Program does not have defense in case you put wrong information.
## Private use. SHELL: bash
##

## Default variables, during program execution you can change it.
default=dfigueiredo #Git username
defaultcvs=ForwardAnalysis #Example: /afs/cern.ch/project/cvs/reps/CMSSW/UserCode/$defaultcvs

# @(#) s1
set -o nounset
echo

## Debug, Bash version
debug=":"
debug="echo"

echo "(Versions displayed with local utility \"version\")"
version >/dev/null 2>&1 && version bash
echo
echo "------------------"
echo "Migrate CVS to Git"
echo "------------------"
echo
read -p "Insert Git username (default is $default): " -e t1
if [ -n "$t1" ]
then
  git_user="$t1"
else
  git_user="$default"
fi
echo "Git user is $git_user"
echo 
read -p "Insert CVS package to be migrated (default is $defaultcvs): " -e t2
if [ -n "$t2" ]
then
  git_cvs="$t2"
else
  git_cvs="$defaultcvs"
fi
echo "CVS package to be migrated is $git_cvs"
echo
clear
echo "Git username password will be requested..."
sleep 2

curl -u $git_user https://api.github.com/user/repos -d '{"name":"'${git_cvs}'"}'

cvs2git --blobfile=git-blob.dat --dumpfile=git-dump.dat \
        /afs/cern.ch/project/cvs/reps/CMSSW/UserCode/$git_cvs \
        --symbol-transform="(.*)/:\1-" \
        --use-external-blob-generator \
        --fallback-encoding "UTF8" \
        --username $git_user

MY_REMOTE=git@github.com:$git_user/$git_cvs.git

git init Migrated
cd Migrated
git remote add origin $MY_REMOTE
cat ../git-blob.dat ../git-dump.dat | git fast-import
git config http.postBuffer 524288000

clear
echo "SSH git key password will be requested twice..."
sleep 2

git push origin master
git push --mirror -u origin

echo 
echo "End!"
echo 
