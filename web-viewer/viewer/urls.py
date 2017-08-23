from django.conf.urls import url

from . import views

urlpatterns = [
  url(r'^$', views.index, name='index'),
  url(r'^skull$', views.skull, name='skull'),
  url(r'^deform$', views.deform, name='deform')
]
