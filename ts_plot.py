#!/usr/bin/env python3
import numpy as np
import pandas as pd
import matplotlib as mpl
mpl.use('Agg')
import matplotlib.pyplot as plt

def plotter(ax,data,label,color,alpha,column,mask=False):
    length=np.array(list(map(float,data['N'])))
    col_data=np.array(list(map(float,data[column])))
    exit_status=np.array(list(map(float,data['exit_status'])))
    if mask:
        mask=(length>=14)&(length<=14)
    else:
        mask=length>=-1
    ax.plot(length[mask],col_data[mask],linestyle='--',linewidth=0.5,color=color,zorder=0)
    neomask=mask&(exit_status==0)
    if label:
        ax.scatter(length[neomask],col_data[neomask],marker='+',label=label,color=color,alpha=alpha,zorder=1)
    else:
        ax.scatter(length[neomask],col_data[neomask],marker='+',color=color,alpha=alpha,zorder=1)
    if np.sum(exit_status)>0:
        neomask=mask&(exit_status==1)
        ax.scatter(length[neomask],col_data[neomask],marker='x',color=color,alpha=alpha,zorder=1)

def runner(data,column):
    tvars=['int8_t','uint8_t','int16_t','uint16_t','int32_t','uint32_t','int64_t','uint64_t']
    cvars=['int32_t','uint32_t','float','int64_t','uint64_t','double','long double']
    alpha=0.0
    dalpha=1.0/(len(tvars)*len(cvars)+1)
    inset=plt.axes().inset_axes([0.3,0.25,0.05,0.7])
    for tvar in tvars:
        for cvar in cvars:
            tvar_filter=data['tvar'].str.match(tvar)
            cvar_filter=data['cvar'].str.match(cvar)
            gcc_filter=data['compiler'].str.match('g\+\+')
            gcc_selection=data[gcc_filter & tvar_filter & cvar_filter]
            alpha=alpha+dalpha
            plotter(plt,gcc_selection,'g++<'+tvar+','+cvar+'>','blue',alpha,column)
            #plotter(inset,gcc_selection,False,'blue',alpha,column,mask=True)
    alpha=0.0
    for tvar in tvars:
        for cvar in cvars:
            tvar_filter=data['tvar'].str.match(tvar)
            cvar_filter=data['cvar'].str.match(cvar)
            clang_filter=data['compiler'].str.match('clang\+\+')
            clang_selection=data[clang_filter & tvar_filter & cvar_filter]
            alpha=alpha+dalpha
            plotter(plt,clang_selection,'clang++<'+tvar+','+cvar+'>','green',alpha,column)
            plotter(inset,clang_selection,False,'green',alpha,column,mask=True)
    plt.xlim((0,16))
    inset.autoscale(axis='y',tight=True)
    inset.set_xticks([14])
    if(column=='compile_time'):
        plt.yscale('log')
        plt.ylim((0.1,None))
    plt.legend(loc='best',prop={'size':1.5})
    plt.tight_layout()
    plt.savefig('ts_'+column+'.pdf')

data=pd.read_csv('ts_compile.ls',quotechar='"',delimiter=';',names=['compiler','N','tvar','cvar','compile_time','memory_use','exit_status'])

runner(data,'compile_time')
plt.clf()
runner(data,'memory_use')
