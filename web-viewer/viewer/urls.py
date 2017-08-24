from django.conf.urls import url

from . import views

urlpatterns = [
  	url(r'^deform/$', views.skull_deform_frame, name='deformation'),
  	url(r'^similar/$', views.similar_skull, name='similarity'),
  	url(r'^skull_detail/$', views.skull_detail, name='skull_detail'),
  	# url(r'^skulls/$', views.show_skulls, name='skulls'),
    url(r'^skulls$', views.skulls, name='skulls'),
    url(r'^wall/$', views.wall, name='wall'),
  	url(r'^$', views.index, name='index'),
]
