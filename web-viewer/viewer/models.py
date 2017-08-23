# -*- coding: utf-8 -*-
from __future__ import unicode_literals
import json
from django.db import models
# from .utils import encode_float_list, decode_float_list, obj_parser, calculate_normal
import numpy as np

# Create your models here.
class Skull(models.Model):
	identity = models.CharField(max_length = 10, default = '')
	name = models.CharField(max_length = 20, default = '')
	# vertices = models.TextField()
	# faces = models.TextField()
	# normals = models.TextField()
	skull_type = models.CharField(max_length=10, null=False, default='healthy')
	obj_path = models.CharField(max_length=100, default='')
	obj_path_decimated = models.CharField(max_length=100, default='')
	thumbnail = models.ImageField(upload_to = './data/thumbnails/', default='./data/thumbnails/p3.png')

	# def load_from_obj(self, obj_file):
	# 	v, f = obj_parser(obj_file)
	# 	n = calculate_normal(v, f)
	# 	self.save_vfn(v,f,n)

	# def get_vfn(self):
	# 	v = decode_float_list(self.vertices)
	# 	f = decode_float_list(self.faces)
	# 	n = decode_float_list(self.normals)
	# 	return v, f, n

	# def save_vfn(self, v, f, n):
	# 	v_text = encode_float_list(v)
	# 	f_text = encode_float_list(f)
	# 	n_text = encode_float_list(n)
	# 	self.vertices = v_text
	# 	self.faces = f_text
	# 	self.normals = n_text
	# 	self.save()

	# def decode_str_to_float_list(self, strs):
	# 	result = [float(i) for i in strs.split(' ')]
	# 	result = np.reshape(result, (-1,3))
	# 	return result

	# def get_image_path(self):
	# 	return os.path.join('thumbnails', self.identity)

class SimilarityScore(models.Model):
	skull1_identity = models.CharField(max_length = 20, default = '')
	skull2_identity = models.CharField(max_length = 20, default = '')
	score = models.DecimalField(max_digits=5, decimal_places=2, null=False)

	@staticmethod
	def get_similar_sort(identity, num):
		scores = SimilarityScore.objects.filter(skull1_identity = identity)
		length = len(scores)

		if length < num:
			num = length

		result = sorted(scores,key = lambda x:x.score)
		result = result[:num]

		return result


class Deformation(models.Model):
	skull1_identity = models.CharField(max_length = 5, default = '')
	skull2_identity = models.CharField(max_length = 5, default = '')
	frame_num = models.IntegerField(default=0)
	# vertices = models.TextField(null=False)
	# normals = models.TextField(default='')
	obj_path = models.CharField(max_length = 100, default='')
	obj_path_decimated = models.CharField(max_length = 100, default='')

	# def get_vn(self):
	# 	v = decode_float_list(self.vertices)
	# 	n = decode_float_list(self.normals)
	# 	return v, n

	# def save_vn(self, v, n):
	# 	v_text = encode_float_list(v)
	# 	n_text = encode_float_list(n)
	# 	self.vertices = v_text
	# 	self.normals = n_text
	# 	self.save()
