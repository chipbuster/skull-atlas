import os
import numpy as np

def decode_float_list(strs):
	result = [float(i) for i in strs.split(' ')]
	result = np.reshape(result, (-1,3))
	return result

def encode_float_list(data):
	data_flat = np.reshape(data, (-1))
	return " ".join(map(str, data_flat))

def obj_parser(obj_file):
	obj = open(obj_file, 'r')
	strs = obj.readlines()

	start_idx = 9
	#line 10 is start
	v_strs = strs[start_idx:]
	end_idx = v_strs.index('\n')
	v_strs = v_strs[:end_idx]
	vertices = np.array([s[:len(s)-1].split(' ') for s in v_strs])
	vertices = vertices[:,1:].astype('float')

	f_num = int(strs[end_idx + start_idx + 1].split(' ')[1])
	start_idx = end_idx + start_idx + 4
	f_strs = strs[start_idx:start_idx + f_num]
	faces = np.array([s[:len(s)-1].split(' ') for s in f_strs])
	faces = faces[:,2:].astype('int')
	faces = faces - 1

	obj.close()

	return vertices, faces


def calculate_normal(vertices, faces):
	nvecs = [None] * np.size(vertices, axis=0)
	for f in faces:
		i1 = f[0]
		i2 = f[1]
		i3 = f[2]
		v1 = np.array(vertices[i1])
		v2 = np.array(vertices[i2])
		v3 = np.array(vertices[i3])

		v2 = v2 - v1
		v3 = v3 - v1

		cross = np.array([
			v2[1] * v3[2] - v2[2] * v3[1],
			v2[2] * v3[0] - v2[0] * v3[2],
			v2[0] * v3[1] - v2[1] * v3[0],
			])

		n = cross / np.linalg.norm(cross)
		
		if nvecs[i1] == None:
			nvecs[i1] = []
		if nvecs[i2] == None:
			nvecs[i2] = []
		if nvecs[i3] == None:
			nvecs[i3] = []

		nvecs[i1].append(n)
		nvecs[i2].append(n)
		nvecs[i3].append(n)

	normals = np.array([np.sum(i, axis=0) / len(i) for i in nvecs])

	return normals


