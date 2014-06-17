import math
B = 3250
Rk= 10
def main():
	for T in range( 0, 500, 1):
		Rt = math.exp(  ((B*298.15/((T/10)+273.15)) - B) / 298.15)
		print(str(Rt) + "\n")

if __name__ == "__main__":
	main()
