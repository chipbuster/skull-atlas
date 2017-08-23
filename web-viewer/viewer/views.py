# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.shortcuts import render

# Create your views here.
import os
from django.http import HttpResponse
from django.http import JsonResponse
from django.template import loader

def index(request):
  template = loader.get_template('viewer/index.html')
  context = {}
  return HttpResponse(template.render(context, request))

def skull(request):
  name = request.GET['name']
  resp = ''
  with open(os.path.join('data', name + '.obj')) as f:
    resp = f.read()
  return HttpResponse(resp)

def deform(request):
  frame = request.GET['frame']
  frame = int(frame)
  obj = ''
  with open(os.path.join('data', str(frame) + '.obj')) as f:
    obj = f.read()
  if frame < 50:
    return JsonResponse({'frame': frame + 1, 'obj': obj})
  else:
    return JsonResponse({'frame': 0, 'obj': obj})