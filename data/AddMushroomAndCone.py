import sqlite3

class Point:
    def __init__(self,x,y,z):
        self.x=x
        self.y=y
        self.z=z

    def __str__(self):
        return ("(%f,%f,%f)" % (self.x,self.y,self.z))


class ObjPosition:
    def __init__(self,pos,forward,right):
        self.pos = pos
        self.forward = forward
        self.right = right

    def __str__(self):
        return "Pos:%s\nForward:%s\nright:%s\n"%(str(self.pos),str(self.forward),str(self.right))
        

PosFile = open('RoadPositions.txt')

ObjPoses=[]
ARef=[]
for line in PosFile.readlines():
    if line!='\n':
        Nums = line.split()
        AVector = Point(float(Nums[0]),float(Nums[1]),float(Nums[2]))
        ARef.append(AVector)

        if len(ARef)==3:
            AObj=ObjPosition(ARef[0],ARef[1],ARef[2])
            ObjPoses.append(AObj)
            ARef=[]

# start write database
conn = sqlite3.connect('./carworld.db')
curs = conn.cursor()
# judge if the table exist
try:
    curs.execute('drop table CollideObjPosition')
except:
    pass
curs.execute('create table CollideObjPosition (tag text,x float, y float, z float, forwardx float,forwardy float, forwardz float, rightx float, righty float, rightz float)')

count=0
for obj in ObjPoses:
    count=count+1
    sql = 'insert into CollideObjPosition values(%s, %f,%f,%f, %f,%f,%f, %f,%f,%f)'
    if count%2==0:
        curs.execute(sql %('"cone"',obj.pos.x,obj.pos.y,obj.pos.z,obj.forward.x,obj.forward.y,obj.forward.z,obj.right.x,obj.right.y,obj.right.z))
    else:
        curs.execute(sql %('"mushroom"',obj.pos.x,obj.pos.y,obj.pos.z,obj.forward.x,obj.forward.y,obj.forward.z,obj.right.x,obj.right.y,obj.right.z))

conn.commit()
