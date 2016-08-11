import sys

# Print usage if not enough args
if len(sys.argv) < 3:
    print "usage: " + sys.argv[0] + " <input.ax> <output.off>"
    sys.exit()

inf = open(sys.argv[1], "r")

v_count = 0
all_lines = inf.readlines()
inf.close()

vertices = []
faces = []

line_idx = 0
while line_idx < len(all_lines):
  line = all_lines[line_idx]

  if line[0] == "#":
    line_idx += 1
    continue

  if line[0:4] == "{OFF":
    line_idx += 1
    #numbers_str = inf.readline()
    numbers = all_lines[line_idx].split()
    line_idx += 1
    shift = v_count
    # Read the vertices, and print them out.
    for i in range(int(numbers[0])):
      vertices.append(all_lines[line_idx])
      line_idx += 1
      v_count += 1

    face_line = ""
    # Now write the face, but shift its vertices by 'shift'
    for i in range(int(numbers[1])):
      f_stuff = all_lines[line_idx].split()
      line_idx += 1

      face_line += f_stuff[0] + " "
      #outf.write(f_stuff[0] + " ")
      # Shift each of the vertices' indices
      for j in range(1, int(f_stuff[0]) + 1):
        myint = int(f_stuff[j]) 
        face_line += str(myint + shift) + " "
        #outf.write(str(myint + shift) + " ")
      # Print out the color, and end the line.
      face_line += ' '.join(f_stuff[-4:]) + "\n"
      #outf.write(' '.join(f_stuff[-4:]) + "\n")

    # Read the ending bracket, ignore it
    # Increment by 2 to go to the next line
    line_idx += 1
  
    # Add it to global faces
    faces.append(face_line)

  # After the loop, increment line_idx
  line_idx += 1


outf = open(sys.argv[2], "w")
outf.write("OFF\n")
outf.write(str(len(vertices)) + " " + str(len(faces)) + " 0\n")
for v in vertices:
  outf.write(v)
for f in faces:
  outf.write(f)
outf.close()
