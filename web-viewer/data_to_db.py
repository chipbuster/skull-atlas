#!/usr/bin/env python
#coding:utf-8
import django 
import os
from PIL import Image
os.environ.setdefault("DJANGO_SETTINGS_MODULE", "skullviewer.settings")
if django.VERSION >= (1, 7):#自动判断版本
	django.setup()

from viewer.models import Skull, Deformation, SimilarityScore


models_path = './data/models/'
deformation_path = './data/deformation/'
thumbnails_path = './data/thumbnails/'
decimated_path = './data/decimated/'
similarity_path = './data/similarity.txt'
map_path = './data/map.txt'
 
def load_similarity():
	f = open(similarity_path, 'r')
	strs = f.read()

	for sec in strs.split("?")[:-1]:
		sec = sec[1:]
		substrings = sec.split('\n')
		s1_id = substrings[0]
		s2_strings = substrings[1:-1]
		for s2_string in s2_strings:
			ss = s2_string.split(' ')
			s2_score = int(ss[0])
			s2_id = ss[1]

			SimilarityScore.objects.get_or_create(skull1_identity=s1_id,skull2_identity=s2_id, score=s2_score)

	f.close()

	print("similarity records: ", len(SimilarityScore.objects.all()))

def load_skull():
	for model in os.listdir(models_path):
		if len(model) < 5 or model[-4:] != '.obj':
			continue
		model_id = model[:-4]
		skull_type = model_id[0]
		if skull_type == 'p':
			skull_type = 'healthy'
		elif skull_type == 'q':
			skull_type = 'injured'
		else:
			skull_type = 'healthy'
			print("no such type in load_skull")
		skull = Skull.objects.get_or_create(identity=model_id, skull_type = skull_type,
			obj_path=models_path + model)
		print("model loaded: ", model_id)

	for dec in os.listdir(decimated_path):
		if len(dec) < 5 or dec[-4:] != '.obj':
			continue
		model_id = model[:-4]
		try:
			skull = Skull.objects.get(identity=model_id)
			skull.obj_decimated = decimated_path + dec
			skull.save()
		except:
			print("decimated fails: ", model_id)

	f = open(map_path, 'r')
	strs = f.read()
	f.close()
	for line in strs.split('\n'):
		ls = line.split(' ')
		model_id = ls[1][:-4]
		name = ls[0][:-4]
		try:
			skull = Skull.objects.get(identity=model_id)
			skull.name = name
			skull.save()
		except:
			print("name map fails: ", model_id)

def load_deformation():
	for deform in os.listdir(deformation_path):
		if len(deform) < 5 or deform[-4:] != '.obj':
			continue
		file_name = deform[:-4]
		parts = file_name.split('_')
		skull1_identity = parts[0]
		skull2_identity = parts[1]
		frame = int(parts[2])
		if len(parts) > 3 and parts[3] == 'dec':
			deform = Deformation.objects.get_or_create(skull1_identity = skull1_identity, skull2_identity=skull2_identity, frame_num = frame, obj_decimated = deformation_path + deform)


def clear_data():
	Skull.objects.all().delete()
	print("skull deleted")
	SimilarityScore.objects.all().delete()
	print("similarity deleted")
	Deformation.objects.all().delete()
	print("deformation deleted")

def main():
	clear_data()
	load_similarity()
	load_skull()
	load_deformation()
 
if __name__ == "__main__":
	main()
	print('Done!')