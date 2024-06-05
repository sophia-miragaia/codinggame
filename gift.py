import sys
import math

n = int(input())
c = int(input())

Budgets = []
for i in range(n):
	b = int(input())
	Budgets.append(b)

if sum(Budgets) < c:
	print("IMPOSSIBLE")
else:
	Pays = Budgets
	solved = False
	while not solved:
		Pays[Pays.index(max(Pays))] -= 1
		if sum(Pays) == c:
			solved = True

	Pays.sort()
	for p in Pays:
		print(p)