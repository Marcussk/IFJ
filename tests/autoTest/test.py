#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import os, re, subprocess, shutil
import json, glob, ntpath, sys
from jinja2 import Template
from multiprocessing.dummy import Pool  # use threads

"""
Testing script by group 76

Autors:
    script : Michal Orsák, xorsak01 
    samples: Marek  Beňo , xbenom01

Why use rather this:
- Who else checks all tests with valgrind?
- Who else writes test for the test?
- Who else runs ~16 tests in parallel?
- Who else does it for free?
- Who else have web and shell interface?
- Who else creates html page with online diffs editors for every failed tests?
- What the hell, this is not enough for you ? Better try it by yourself :-D 

Usage:
- Setup your src folder and binary name, this scripts automatic builds your project, so you don't have to
- Just run python3 test.py or run this file directly if you know what chmod +x does
- Clean rule is recommended in makefile
- If you add new sample ("sample(\d*)\.pas") the reference file is AUTOMATICALY GENERATED and you need only CHECK the result
- FIRST ROW OF FILE can contains anything but is used as a description, we are recommending useful message there
- If you wont write YOUR OWN TEST REFERENCE, just copy another and change values, it's simple jonson, nothing special
- If you does not understand anything please make link yourself on http://cs.lmgtfy.com/ thank you. 

Performance:
On i7-4710qm this tests runs <5s with valgrind on (yes turn valgrind and it will be ~10x faster)
and  <1s with valgrind off if you having trouble with the performance
your implementation is probably very slow or you are using too many threads, experiment with MAX_THREADS
or you just written too long test

Comrade hint:
- Check new reference results if you don't wont to die.
- http://sd.keepcalm-o-matic.co.uk/i/keep-calm-and-work-harder-comrade.png

"""
 
BIN_NAME = "ifj.run"
SRC_FOLDER = "../../src"
SAMPLES_FOLDER = "../../samples"
SAMPLES_RES_FOLDER = "results/"
ORIGINAL_CWD = os.getcwd()
REPORT_TEMPLATE = "test_report_template.html"
REPORT_FILE = "results.html"
MAX_THREADS = 16
CHECK_VALGRIND = True  # Consumes MOST OF TIME


findValgrindErr = re.compile("==\d*== ERROR SUMMARY: (\d)")
firstCommentRegex = re.compile(".*{([^}]*)}.*", re.MULTILINE | re.DOTALL)


def makeBinary():
    os.chdir(SRC_FOLDER)    
    subprocess.call(["make", "clean"], shell=True)
    subprocess.call("make", shell=True)
    if not os.path.isfile(BIN_NAME):
        return False 
    shutil.copy2(BIN_NAME, os.path.join(ORIGINAL_CWD, BIN_NAME))
    os.chdir(ORIGINAL_CWD)
    return True

def listTests():
    files = glob.glob(SAMPLES_FOLDER + "/sample*.pas")
    files = sorted(files, key=lambda name: int(re.search(".*sample([0-9]*)\.pas", name).group(1)))
    errTests = glob.glob(SAMPLES_FOLDER + "/errsample*.pas")
    errTests = sorted(errTests, key=lambda name: int(re.search(".*sample([0-9]*)\.pas", name).group(1)))
    return files + errTests
    
def getTestResPath(testFileName):
    return os.path.join(SAMPLES_RES_FOLDER, ntpath.basename(testFileName).replace(".pas", ".json"))

def loadTestResults(testFileName):
    name = getTestResPath()
    with open(os.path.join(SAMPLES_RES_FOLDER, name)) as f:
        data = json.load(f)
        return data

def encodingFix(result):
    if result["stdout"] != None:
        result["stdout"] = result["stdout"].decode("utf-8")
    if result["stderr"] != None:
        result["stderr"] = result["stderr"].decode("utf-8")

def createTest(testFileName):
    resultFileName = getTestResPath(testFileName)
    result = { "stdin": "" }
    p = subprocess.Popen(["./" + BIN_NAME, testFileName], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    result["stdout"], result["stderr"] = p.communicate()
    result["returns"] = p.returncode
    encodingFix(result)
    with open(resultFileName, "w") as f:
        json.dump(result, f)

def performTest(sampleFile, resultFileName):
    prompt = "sampleFile : %s " % (sampleFile)
    result = {}
    with open(sampleFile) as sf:
        ex = sf.read()
        comment = firstCommentRegex.search(ex)
        if comment:
            result["descr"] = comment.group(1)
                   
    with open(resultFileName) as f:
        resultRef = json.load(f)

    p = subprocess.Popen(["./" + BIN_NAME, sampleFile], stdout=subprocess.PIPE, stderr=subprocess.PIPE, stdin=subprocess.PIPE)
    result["stdout"], result["stderr"] = p.communicate(input=resultRef["stdin"])
    result["returns"] = p.returncode
    
    if CHECK_VALGRIND:
        pValgrind = subprocess.Popen(["valgrind" , "./" + BIN_NAME, sampleFile], stdout=subprocess.PIPE, stderr=subprocess.PIPE, stdin=subprocess.PIPE)
        valgrindOut = pValgrind.communicate(input=resultRef["stdin"])[1]
        result["valgrindErrors"] = int(findValgrindErr.search(valgrindOut.decode("utf-8")).group(1))
    else:
        result["valgrindErrors"] = -1
    
    encodingFix(result)  

    if resultRef["stdout"] != result["stdout"] or resultRef["returns"] != result["returns"] or result["valgrindErrors"] > 0:
       print(prompt + "[ \033[0;31mFAILED\033[0m ] for more see %s" % REPORT_FILE)
       return { "result" : result, "sampleFile": sampleFile, "resultFile":resultFileName, "resultRef": resultRef }
    else:
       print(prompt + "\033[92mPASSED\033[0m")
        
        
def renderErrors(errors):
    with open(REPORT_TEMPLATE) as f:
        template = Template(f.read())

    with open(REPORT_FILE, "w") as outF:
        errorsDump = json.dumps(errors)
        outF.write(template.render(results=errorsDump))

def evalTest(testFileName):
    resultFileName = getTestResPath(testFileName)
    if not os.path.isfile(resultFileName):
        print("mising results for %s , creating new" % testFileName)
        createTest(testFileName)
    else:
        return performTest(testFileName, resultFileName)

if __name__ == "__main__":
    if os.path.isfile(REPORT_FILE):
        os.remove(REPORT_FILE)
    if not makeBinary():
        print("cannot even compile %s" % BIN_NAME)
    else:
       pool = Pool(MAX_THREADS)  # no more than 20 concurrent connections
       testFiles = listTests()
       errors = pool.map(evalTest, testFiles)
       errors = list(filter(lambda x: x, errors))  # filter empty results
    renderErrors(errors)
