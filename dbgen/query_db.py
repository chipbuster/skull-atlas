import json
import sqlite3
import sys
import os
import csv
import cursesmenu as cm
import subprocess

def add_trailing_slash(path):
    """If path does not have a trailing slash, append one"""

    if path[-1] != "/":
        path.append("/")

    return path

def get_filepath_from_ids(id_tuple, basepath):
    """Gets the path to a file with the given (patientID, seriesID) tuple from the base path"""
    (pid,series) = id_tuple

    for (root,dirs,files) in os.walk(basepath):
        if pid in dirs:
            path1 = os.path.join(root,pid)
            for (root2, dirs2, files2) in os.walk(path1):
                if series + ".obj" in files2:
                    return os.path.join(path1,root2,series+".obj")

    return None


dbfile = sys.argv[1]
datapath = sys.argv[2]
scriptpath = sys.argv[3]

conn = sqlite3.connect(dbfile)
c = conn.cursor()

patientIDList_Raw = c.execute("""SELECT patientID from studies""")

patientIDList = [ str(id) for (id, ) in patientIDList_Raw ]
patientIDList = list(set(patientIDList)) #Fast and dirty way to remove dupes

menu = cm.CursesMenu("Skull Atlas", "Please select the patient to query")

# selectionmenu return the index of the choice. Use it to get the patientID string
selectionNum = cm.SelectionMenu.get_selection(patientIDList,
                                              title="Pick a patient to query")
patient = patientIDList[selectionNum]

# Find all series associated with this patient and have the user select one
serieslist_Raw = c.execute("""SELECT CVCSeriesID FROM series WHERE PatientID=?""",(patient,))
serieslist = [ str(series) for (series,) in serieslist_Raw ]

# If no series, something has gone wrong
if not serieslist:
    raise ValueError("Patient " + patient + " has no associated CT Scans!")

print(chr(27) + "[2J")
selectionNum = cm.SelectionMenu.get_selection(serieslist,
                                              title="We found the following series for patient %s"%(patient),
                                              subtitle="Which one do you want to query for similarity?")

patientSeries = serieslist[selectionNum]

## Generate top 10 matches, then filter them based off of patientID
matches = c.execute("""SELECT PatientID2,CVCSeriesID2,score FROM simscores WHERE
                         PatientID1=? AND CVCSeriesId1=? ORDER BY score ASC LIMIT 10""",(patient,patientSeries))

# Filter out all matches with the same patient and take top 5
matches = [ (pid,sid,score) for (pid,sid,score) in matches if pid != patient ]
matches = matches[0:5]

matchstrings = []
# Generate strings to use in interface
for match in matches:
    (pid,sid,score) = match
    mstr = ""
    mstr += "PatientID: " + pid
    mstr += "   SeriesID: " + sid
    mstr += "   Match Score: " + str(score)
    matchstrings.append(mstr)



selectionNum = cm.SelectionMenu.get_selection(matchstrings,
                                              title="We found the following matches for patient " + patient,
                                              subtitle="Which one do you want to visualize?")

(targetpid,targetsid,matchscore) = matches[selectionNum]

patientObj = get_filepath_from_ids((patient,patientSeries), datapath)
targetObj = get_filepath_from_ids((targetpid,targetsid), datapath)

if patientObj is None or targetObj is None:
    if patientObj is None:
        raise FileNotFoundError("Could not find the file for patient " + patient
                                +", series " + patientSeries + " in path" + datapath)
    if targetObj is None:
        raise FileNotFoundError("Could not find the file for patient " + targetpid
                                +", series " + targetsid + " in path" + datapath)

print("Please wait, preparing meshes for viewing...")

#Prepare commands for editing meshes

obj2off = add_trailing_slash(scriptpath) + "obj2off.sh"
offcomb = add_trailing_slash(scriptpath) + "combineOff.pl"
aligntool = "/mnt/spinny/CVC/skull-atlas/libicp/build/icp"
viewer  = "meshlab"
patientMesh = "patient.off"
targetMesh = "target.off"
alignedTargetMesh = "target_aligned.off"
finalMesh = "combined.off"

if not os.path.isfile(aligntool):
    raise ValueError("Please set the aligntool to the libicp binary")

print("-- Converting patient meshes to OFF format")

# Change both meshes from obj to off meshes, storing in current dir
p1 = subprocess.call([obj2off, patientObj, patientMesh])
p2 = subprocess.call([obj2off, targetObj, targetMesh])

print ("-- Aligning meshes with libICP (you may see some output on the screen at this point)")

# Use libICP to align target skull to patient skull
p3 = subprocess.call([aligntool, patientMesh, targetMesh, "-1", alignedTargetMesh])

print("-- Joining aligned skull meshes")
# Join the aligned target mesh with the patient mesh and color
p4 = subprocess.call([offcomb, patientMesh, alignedTargetMesh, finalMesh])


print("-- Opening viewer...")
# View the final joined mesh
p5 = subprocess.Popen([viewer,finalMesh])
