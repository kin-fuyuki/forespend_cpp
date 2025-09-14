print("g Grid_Grid")
face=512
#side z
while face!=-511:
	print("v -512.0 0.0 "+str(face))
	print("v -512.0 1.0 "+str(face))
	print("v  512.0 0.0 "+str(face))
	print("v  512.0 1.0 "+str(face))
	face-=1
#side x
face=512
while face!=-511:
	print("v "+str(face)+" 0.0 -512.0")
	print("v "+str(face)+" 1.0 -512.0")
	print("v "+str(face)+" 0.0  512.0")
	print("v "+str(face)+" 1.0  512.0")
	face-=1

print("s0")