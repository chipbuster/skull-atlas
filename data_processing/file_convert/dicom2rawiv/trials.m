close all;
clear;
clc;

filename = '00000001';
info = dicominfo(filename);
q = info.InstanceNumber;
p = isfield(info, 'InstanceNumber');