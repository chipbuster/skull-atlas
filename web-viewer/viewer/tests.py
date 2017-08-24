# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.test import TestCase

from models import SimilarityScore, Skull, Deformation
from utils import *
import time

# Create your tests here.
def test_model():
	path = './p3.obj'

	v, f = obj_parser(path)
	n = calculate_normal(v,f)

	s = Skull()
	s.identity = 'p3'
	s.save_vfn(v,f,n)

	start = time.time()
	res = s.decode_str_to_float_list(s.vertices)
	end = time.time()
	print res.shape
	print "time: ",end - start
	s.delete()


def test_similarity():
	dist_path = './shape_dist_output.txt'

	dist_f = open(dist_path, 'r')
	strs = dist_f.read()
	count = 0
	for sec in strs.split("?")[:-1]:
		sec = sec[1:]
		ss = sec.split('\n')
		name = ss[0]
		s_name = ss[1:-1]
		for s2_name in s_name:
			SS = SimilarityScore()
			SS.skull1_identity = name
			s2_score = int(s2_name.split(' ')[0])
			s2_identity = s2_name.split(' ')[1]
			SS.skull2_identity = s2_identity
			SS.score = s2_score
			SS.save()
			count += 1

	print count
	s1_name = 'female_caucasian_6-1_7607653_1.obj'
	print len([ss.skull1_identity + ' ' + str(ss.score) for ss in SimilarityScore.objects.filter(skull1_identity=s1_name)])

pyth = './data/map.txt'
f = open(pyth, 'r')
strs = f.read()
for line in strs.split('\n'):
	ls = line.split(' ')
	name = ls[0]
	ide = ls[1]
	print ide + ' ' + name
