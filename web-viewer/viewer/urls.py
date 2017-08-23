from django.conf.urls import url

from . import views

urlpatterns = [
  	url(r'^deform/$', views.skull_deform_frame, name='deform'),
  	url(r'^similar/$', views.similar_skull, ),
  	url(r'^skull_detail/$', views.skull_detail, name=''),
  	url(r'^$', views.show_skull, name='skulls'),
]
