# -*- coding: utf-8 -*-
from __future__ import unicode_literals
import json
from django.db import models
import numpy as np

# Create your models here.
class Skull(models.Model):
	identity = models.CharField(max_length = 10, default = '')
	name = models.CharField(max_length = 20, default = '')
	skull_type = models.CharField(max_length=10, null=False, default='healthy')
	obj_path = models.CharField(max_length=100, default='')
	obj_decimated = models.CharField(max_length=100, default='')

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
	obj_path = models.CharField(max_length = 100, default='')
	obj_decimated = models.CharField(max_length = 100, default='')
