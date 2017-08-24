# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.shortcuts import render

# Create your views here.
from django.http import HttpResponse, JsonResponse
from django.template import loader
from .models import Skull, SimilarityScore, Deformation
import viewer.utils
import math
import json


def index(request):
    template = loader.get_template('viewer/index.html')
    context = {}
    return HttpResponse(template.render(context, request))

def wall(request):
    template = loader.get_template('viewer/wall.html')
    context = {}
    return HttpResponse(template.render(context, request))

def detail(request):
    template = loader.get_template('viewer/detail.html')
    id = request.GET['skull']
    skull = Skull.objects.get(identity=id)
    skull_type = skull.skull_type
    similar_skulls = []
    
    data = {}        
    if skull_type == 'injured':
        if id == 'q1':
            similar_skulls = ['p49', 'p74', 'p38', 'p39', 'p36']
        elif id == 'q2':
            similar_skulls = ['p37', 'p53', 'p51', 'p88', 'p62']
        else:
            similar_skulls = ['p69', 'p93', 'p5', 'p75', 'p34']

    data = {'id': id, 'type': skull_type, 'results': similar_skulls}
    context = {'data': json.dumps(data)}
    return HttpResponse(template.render(context, request))

def skulls(request):
    id = request.GET['skull']
    dec = int(request.GET['dec'])
    skull = Skull.objects.get(identity=id)
    # print('HAHA', id, dec, skull.obj_path_decimated)
    filename = skull.obj_path
    if dec == 1:
        filename = skull.obj_decimated
    
    with open(filename, 'r') as f:
        obj = f.read()
        return HttpResponse(obj)


def show_skulls(request):
    skull_each_page = 8
    if request.method == 'GET':
        page = int(request.GET.get('page', '1'))
        skull_type = request.GET.get('type', 'healthy')

        skulls = Skull.objects.filter(skull_type=skull_type)
        total_skull_num = len(skulls)

        start_idx = (page - 1) * skull_each_page
        end_idx = page * skull_each_page
        max_page = ceil(float(total_skull_num) / skull_each_page)

        if end_idx > total_skull_num:
            end_idx = total_skull_num

        result_skulls = skulls[start_idx:end_idx]

        result = []
        for s in result_skulls:
            res = {
                'name': s.name,
                'identity': s.identity,
            }
            dec_f = open(s.obj_path_decimated, 'r')
            res['obj_decimated'] = dec_f.read()
            dec_f.close()
            result.append(res)

        context = {
            'status': 1,
            'num': len(result),
            'max_page': max_page,
            'skulls': result,
        }
    else:
        print("show_skull: request method should be get")
        context = {
            'status': 0
        }
    return JsonResponse(context)

def skull_detail(request):
    template = loader.get_template('viewer/index.html')
    if request.method == 'GET':
        skull_identity = request.GET.get('skull', '-1')
        skull = Skull.objects.get(identity=skull_identity)
        # vertices = decode_float_list(skull.vertices)
        # faces = decode_float_list(skull.faces)
        # normals = decode_float_list(skull.normals)
        skull_obj = skull.obj_path

        obj = open(skull_obj, 'r')

        similar_skulls = []
        if skull.skull_type == 'injured':
            result = SimilarityScore.get_similar_sort(skull_identity, 5)
            for r in result:
                similar_id = r.skull2_identity
                skull2 = Skull.objects.get(identity=similar_id)
                similar_skulls.append({
                    'identity': skull2.identity,
                    'name': skull2.name,
                    'type': skull2.skull_type,
                    'obj_decimated': skull2.obj_decimated,
                })

        context = {
            'status': 1,
            'identity': skull.identity,
            'name': skull.name,
            'type': skull.skull_type,
            'obj': obj.read(),
            'similar_skulls': similar_skulls,
        }
        obj.close()

    else:
        print("skull_detail: request method should be get")
        context = {
            'status': 0
        }

    return HttpResponse(context)


def skull_deform_frame(request):

    if request.method == 'GET':
        skull1_identity = request.GET.get('skull1', '-1')
        skull2_identity = request.GET.get('skull2', '-1')
        frame_num = int(request.GET.get('frame', '-1'))
        is_dec = int(request.GET.get('dec'))
        skull1 = Skull.objects.get(identity=skull1_identity)
        skull2 = Skull.objects.get(identity=skull2_identity)

        try:
            if is_dec == 1:
                deform = Deformation.objects.get(
                    skull1_identity=skull1_identity, skull2_identity=skull2_identity, frame_num=frame_num)
                max_frame = len(Deformation.objects.filter(
                    skull1_identity=skull1_identity, skull2_identity=skull2_identity))
                deform_obj = open(deform.obj_decimated, 'r')
                context = {
                    'status': 1,
                    'obj_decimated': deform_obj.read(),
                    'skull1_name': skull1.name,
                    'skull2_name': skull2.name,
                    'frame': frame_num + 1,
                    'max_frame': max_frame,
                }
                deform_obj.close()

            else:
                deform = Deformation.objects.get(
                    skull1_identity=skull1_identity, skull2_identity=skull2_identity, frame_num=frame_num)
                deform_obj = open(deform.obj_path, 'r')
                context = {
                    'status': 1,
                    'obj': deform_obj.read(),
                    'skull1_name': skull1.name,
                    'skull2_name': skull2.name,
                }
                deform_obj.close()
        except:
            context = {
                'status': 1,
                'skull1_name': skull1.name,
                'skull2_name': skull2.name,
                'frame': 0,
            }

    else:
        print("skull_deform_frame: request method should be get")
        context = {
            'status': 0
        }

    return JsonResponse(context)


def similar_skull(request):
    if request.method == 'GET':

        skull1_identity = request.GET.get('skull1', '-1')
        skull2_identity = request.GET.get('skull2', '-1')

        skull1 = Skull.objects.get(identity=skull1_identity)
        skull2 = Skull.objects.get(identity=skull2_identity)
        skull1_obj = open(skull1.obj_path, 'r')
        skull2_obj = open(skull2.obj_path, 'r')

        context = {
            'status': 1,
            'skull1_obj': skull1_obj.read(),
            'skull1_name':	skull1.name,
            'skull2_obj': skull2_obj.read(),
            'skull2_name': skull2.name,
        }

        skull1_obj.close()
        skull2_obj.close()

    else:
        print("similar_skull: request method should be get")
        context = {
            'status': 0
        }
    return JsonResponse(context)
