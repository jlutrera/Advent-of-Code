def product(i):
	return str(int(i) * 2024)

def split_stones(i):
	j = len(i) // 2
	n1 = str(int(i[:j]))
	n2 = str(int(i[j:]))
	return n1 + " " + n2

def calculate_stones(i):
	if i == '0':
		return '1'
	elif (len(i) % 2 == 0):
		return split_stones(i)
	else:
		return product(i)

if __name__ == "__main__":
	input = "0 89741 316108 7641 756 9 7832357 91"
	n = input.split()
	for _ in range(25):
		stones = ""
		for i in n:
			stones +=  calculate_stones(i) + " "
		#print(stones)
		n = stones.split()
	number = len(n)
	print("Result = ", number)
