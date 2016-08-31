import json
import sqlite3
import sys
import os
import csv

dbfile = sys.argv[1]
datapath = sys.argv[2]
simscorefile = sys.argv[3]

conn = sqlite3.connect(dbfile)
c = conn.cursor()

c.execute("""CREATE TABLE studies
          (PatientID text, StudyDescription text, StudyDate text,
           PatientAge integer, StudyTime numeric, PatientSex text)""")

c.execute("""CREATE TABLE series
          (PatientID text, SliceThickness real, SeriesDescription text,
           CVCSeriesId text)""")

c.execute("""CREATE TABLE simscores
          (PatientID1 text, CVCSeriesId1 text, PatientID2 text, CVCSeriesId2 text, score real)""")

## Search for all JSON files in the datapath provided by the user.
## If the JSON has 'study' in the name, add its data to studies
## If the JSON has 'series' in the name, add its data to series
for (root,_,filelist) in os.walk(datapath):
    for fname in filelist:
        if "json" in fname:
            jsonfile = os.path.join(root,fname)

            if 'study' in jsonfile:
                with open(jsonfile,'r') as data_file:
                    data = json.load(data_file)

                    c.execute("""INSERT INTO studies VALUES (?,?,?,?,?,?)""",
                    (data['PatientID'], data['StudyDescription'], data['StudyDate'],
                     data['PatientAge'], data['StudyTime'], data['PatientSex'] ))

            if 'series' in jsonfile:
                with open(jsonfile,'r') as data_file:
                    data = json.load(data_file)

                    c.execute("""INSERT INTO series VALUES (?,?,?,?)""",
                    (data['PatientID'], data['SliceThickness'], data['SeriesDescription'],
                     data['CVCSeriesId'] ))

conn.commit()

simScores = []

with open(simscorefile, 'r') as simfile:
    r = csv.reader(simfile)
    for row in r:
        simScores.append(row)

# First, create a lookup table matching row/col numbers to patient-id / series number
# This assumes filenames are stored as strings in the leftmost column

indexPatientLookup = [None] * len(simScores)

for i in range(len(simScores)):
    nameString = simScores[i][0]
    nameComponents = nameString.split('/')

    # This part is fragile and depends pretty heavily on the format of the string staying the same
    patientID = nameComponents[3]
    CVCseriesID = nameComponents[-1].split('.')[0]

    indexPatientLookup[i] = (patientID,CVCseriesID)

## Now that we have the lookups between index and patient, delete the first column

simScores = [ a[1:] for a in simScores ]

# Insert the data into the database

for i in range(len(simScores)):
    (pid1, sid1) = indexPatientLookup[i] #Get first patient ID and series ID

    for j in range(len(simScores)):
        (pid2, sid2) = indexPatientLookup[j]

        c.execute("""INSERT INTO simscores VALUES (?,?,?,?,?)""",
        (pid1,sid1,pid2,sid2,simScores[i][j]))

conn.commit()
conn.close()
