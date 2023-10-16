with open("res/lowpolycar.obj", "rt") as f:
    lines = f.readlines()
    verts = [line.rstrip('\n').removeprefix('v ') for line in lines if line.startswith('v')]
    vtpls = [(float(j[0]), float(j[1]), float(j[2])) for j in (i.split(' ') for i in verts)]
    pairs = [line.rstrip('\n').removeprefix('l ') for line in lines if line.startswith('l')]
    ptpls = [(int(j[0]), int(j[1])) for j in (i.split(' ') for i in pairs)]

with open('cout.txt', "wt") as o:
    l = len(vtpls)
    l2 = len(ptpls)
    o.write(f'double vehiclepoints[{l}][3] = ' + '{\n')
    for tpl in vtpls:
        o.write('    ' + str(tpl).replace('(', '{').replace(')', '}') + ',\n')
    o.write('};\n\n')
    o.write(f'int vehiclepairs[{l2}][2] = ' + '{\n')
    for tpl in ptpls:
        o.write('    ' + str(tpl).replace('(', '{').replace(')', '}') + ',\n')
    o.write('};')