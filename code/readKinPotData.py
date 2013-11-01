import matplotlib.pyplot as plt
import numpy as np

def read():

	inFile = open("../data/conservations/energy/Earth.dat",'r')

	global _t; _t = []
	global _x; _x = []
	global _y; _y = []
	global _z; _z = []

	for line in inFile:
		columns = line.split()
		_t.append(float(columns[0]))
		_x.append(float(columns[1]))
		_y.append(float(columns[2]))
		_z.append(float(columns[3]))

	inFile.close()

read()
fig = plt.figure()
ax = fig.add_subplot(111)
ax.plot(_t,_x, label='$E_k$')
ax.hold('on')
ax.plot(_t,_y, label='$E_p$')
ax.plot(_t,_z, '--', label='$E$')
fig.suptitle('Kinetic, potential and total energy of the Earth during its orbit.')
ax.set_title('$T = 100, \\ dt = 0.1$')
ax.set_xlabel('$t \\ \mathrm{[year]}$', fontsize='14')
ax.set_ylabel('$\mathrm{Energy} \\ \mathrm{[GM_{Sun} \\ AU^2 year^{-2}]}$',
		fontsize='14')
ax.legend(loc='best')
ax.grid('on')
plt.show()
