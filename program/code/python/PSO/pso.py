__author__ = 'liajixuan'
import random
import copy
import numpy
birds=5#int(raw_input("Enter count of bird: "))
xcount=2#int(raw_input("Enter count of x: "))
pos=numpy.array([[random.randrange(1,100) for i in xrange(xcount)] for j in xrange(birds)],dtype=float)
speed=numpy.array([[random.randrange(1,100) for i in xrange(xcount)] for j in xrange(birds)],dtype=float)
bestpos=copy.deepcopy(pos)
print pos
print numpy.min(pos)
#print speed
#print bestpos
#birdsbestpos=[]
w=0.8
c1=2
c2=2
r1=0.2
r2=0.3
#pos=numpy.empty()
#for i in range(birds):
#    pos.append([])
#    speed.append([])
#    bestpos.append([])
def CalDis(list):
    dis=0.0
    for i in list:
        dis+=i**2
    return dis
def FindBirdsMostPos():
    best=CalDis(bestpos[0])
    index=0
    for i in xrange(birds):
        temp=CalDis(bestpos[i])
        if temp<best:
            best=temp
            index=i
    return bestpos[index]
birdsbestpos=FindBirdsMostPos()
# def NumMulVec(num,list):
#     for i in xrange(len(list)):
#         list[i]*=num
#     return list
# def VecSubVec(list1,list2):
#     for i in xrange(len(list1)):
#         list1[i]-=list2[i]
#     return list1
# def VecAddVec(list1,list2):
#     for i in xrange(len(list1)):
#         list1[i]+=list2[i]
#     return list1
def UpdateSpeed():
    global speed,pos,birdsbestpos
    for i in xrange(birds):
        temp1=speed[i]*0.8
        temp2=bestpos[i]-pos[i]
        temp2=temp2*(c1*r1)
        temp1=temp1+temp2
        temp2=birdsbestpos-pos[i]
        temp2=temp2*(c2*r2)
        speed[i]=temp1+temp2
def UpdatePos():
    global bestpos,birdsbestpos,pos
    for i in xrange(birds):
        pos[i]=pos[i]+speed[i]
        if CalDis(pos[i])<CalDis(bestpos[i]):
            bestpos[i]=copy.deepcopy(pos[i])
    birdsbestpos=FindBirdsMostPos()
for i in xrange(100):
    print CalDis(birdsbestpos)
    print "pos is ",pos
    print "speed is ",speed
    print "bestpos is",bestpos
    print "birdbestpos is",birdsbestpos
    UpdateSpeed()
    UpdatePos()

raw_input()